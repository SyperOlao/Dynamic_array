//
// Created by SyperOlao on 03.11.2025.
//
#pragma once

#include <cassert>
#include <type_traits>

constexpr std::conditional_t<!std::is_nothrow_move_constructible_v<T> && std::is_copy_constructible_v<T>, const T&, T&&> //
move_if_noexcept(T& t) noexcept
{
    return std::move(t);
}

template<typename T>
Array<T>::Array() : data_(nullptr) {
    capacity_ = AppConsts::kMinCapacity;
    data_ = allocate_new_data(capacity_);
}

template<typename T>
::Array<T>::Array(const int capacity) : data_(nullptr), capacity_(capacity > 0 ? capacity : 0) {
    if (capacity_ > 0) {
        data_ = allocate_new_data(capacity_);
    } else {
        capacity_ = AppConsts::kMinCapacity;
        data_ = allocate_new_data(capacity_);
    }
}

template<typename T>
T* Array<T>::allocate_new_data(const int capacity) {
    if (capacity <= 0) return nullptr;
    void* raw = std::malloc(static_cast<size_t>(capacity) * sizeof(T));
    if (!raw) throw std::bad_alloc();
    return static_cast<T*>(raw);
}

template<typename T>
void Array<T>::grow_if_needed_for_insert() {
    if (size_ < capacity_) return;

    const int new_capacity = capacity_ > 0 ? 2 * capacity_ : AppConsts::kMinCapacity;
    T* new_data = allocate_new_data(new_capacity);

    int i = 0;
    try {
        for (; i < size_; ++i) {
            ::new (static_cast<void*>(new_data + i)) T(move_if_noexcept(data_[i]));
        }
    } catch (...) {
        while (i-- > 0) {
            (new_data[i].~T();
        }
        std::free(new_data);
        throw;
    }

    for (int j = 0; j < size_; ++j) {
        data_[j].~T();
    }
    std::free(data_);

    data_ = new_data;
    capacity_ = new_capacity;
}



template<typename T>
const T &Array<T>::Iterator::get() const {
    assert(owner_ && "Iterator: owner_ is null");
    assert(idx_ >= 0 && idx_ < owner_->size() && "Iterator: index out of bounds");
    return owner_->data_[idx_];
}

template<typename T>
void Array<T>::Iterator::set(const T &value) {
    assert(owner_ && idx_ >= 0 && idx_ < owner_->size_);
    owner_->data_[idx_] = value;
}

template<typename T>
void Array<T>::Iterator::next() {
    idx_ += step_;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
    return idx_ != end_;
}

template<typename T>
const T &Array<T>::ConstIterator::get() const {
    assert(owner_ && "Iterator: owner_ is null");
    assert(idx_ >= 0 && idx_ < owner_->size() && "Iterator: index out of bounds");
    return owner_->data_[idx_];
}

template<typename T>
void Array<T>::ConstIterator::next() {
    idx_ += step_;
}

template<typename T>
bool Array<T>::ConstIterator::hasNext() const {
    return idx_ != end_;
}


template<typename T>
Array<T>::~Array() {
    for (int i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    std::free(data_);
}

template<typename T>
int Array<T>::insert(const T &value) {
    grow_if_needed_for_insert();
    ::new (static_cast<void*>(data_ + size_)) T(value);
    return size_++;
}

template<typename T>
int Array<T>::insert(T &&value) {
    grow_if_needed_for_insert();
    ::new (static_cast<void*>(data_ + size_)) T(std::move(value));
    return size_++;
}
// размещающий new
template<typename T>
int Array<T>::insert(int index, const T &value) {
    assert(index >= 0 && index <= size_ && "insert index out of range");
    grow_if_needed_for_insert();

    int i = size_;
    try {
        for (; i > index; --i) {
            ::new (static_cast<void*>(data_ + i)) T(move_if_noexcept(data_[i - 1]));
            data_[i - 1].~T();
        }
        ::new (static_cast<void*>(data_ + index)) T(value);
    }
    catch (...) {
        for (int j = i; j < size_; ++j) {
             data_[j].~T();
        }
        throw;
    }

    ++size_;
    return index;
}

template<typename T>
void Array<T>::remove(int index) {
    assert(index >= 0 && index < size_ && "remove: index out of range");
    if (size_ == 0) return;

    const int last = size_ - 1;

    data_[index].~T();

    int i = index;
    try {
        for (; i < last; ++i) {
            ::new (static_cast<void*>(data_ + i)) T(move_if_noexcept(data_[i + 1]));
            data_[i + 1].~T();
        }
    } catch (...) {
        throw;
    }

    --size_;
}

template<typename T>
const T &Array<T>::operator[](int index) const {
    assert(index >= 0 && index < size_ && "Array: index out of range");
    return data_[index];
}

template<typename T>
T &Array<T>::operator[](int index) {
    assert(index >= 0 && index < size_ && "Array: index out of range");
    return data_[index];
}

template<typename T>
int Array<T>::size() const {
    return size_;
}

template<typename T>
Array<T>::Array(const Array<T> &other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ <= 0) {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        return;
    }

    data_ = allocate_new_data(capacity_);

    int i = 0;
    try {
        for (; i < size_; ++i) {
            ::new (static_cast<void*>(data_ + i)) T(other.data_[i]);
        }
    } catch (...) {
        while (i-- > 0) {
            data_[i].~T();
        }
        std::free(data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
    }
}

template<typename T>
Array<T>::Array(Array<T> &&other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
Array<T> &Array<T>::operator=(const Array<T> &other) {
    if (this == &other) return *this;

    Array<T> tmp(other);
    using std::swap;
    swap(data_, tmp.data_);
    swap(size_, tmp.size_);
    swap(capacity_, tmp.capacity_);
    return *this;
}

template<typename T>
Array<T> &Array<T>::operator=(Array<T> &&other) noexcept {
    if (this == &other) return *this;

    for (int i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    std::free(data_);

    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}