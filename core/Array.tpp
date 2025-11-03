//
// Created by SyperOlao on 03.11.2025.
//
#pragma once

#include <cassert>


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
        data_ = allocate_new_data(AppConsts::kMinCapacity);
    }
}

template<typename T>
void Array<T>::grow_if_needed_for_insert() {
    if (size_ < capacity_) return;
    const int new_capacity = capacity_ > 0 ? 2 * capacity_ : AppConsts::kMinCapacity;
    T *new_data = allocate_new_data(new_capacity);
    int i = 0;
    try {
        for (; i < size_; i++) {
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::construct_at(new_data + i, std::move_if_noexcept(data_[i]));
            } else {
                std::construct_at(new_data + i, data_[i]);
            }
        }
    } catch (...) {
        while (i-- > 0) std::destroy_at(new_data + i);
        std::free(new_data);
        throw;
    }

    for (int j = 0; j < size_; ++j) {
        std::destroy_at(data_ + j);
    }
    std::free(data_);

    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T>
T *Array<T>::allocate_new_data(const int capacity) {
    if (capacity <= 0) return nullptr;
    void* raw = std::malloc(static_cast<size_t>(capacity) * sizeof(T));
    if (!raw) throw std::bad_alloc();
    return static_cast<T*>(raw);
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
        std::destroy_at(data_ + i);
    }
    std::free(data_);
}

template<typename T>
int Array<T>::insert(const T &value) {
    grow_if_needed_for_insert();
    std::construct_at(data_ + size_, value);
    return size_++;
}

template<typename T>
int Array<T>::insert(T&& value) {
    grow_if_needed_for_insert();
    std::construct_at(data_ + size_, std::move(value));
    return size_++;
}

template<typename T>
int Array<T>::insert(int index, const T &value) {
    assert(index >= 0 && index <= size_ && "insert index out of range");
    grow_if_needed_for_insert();

    try {
        if (index < size_) {
            std::construct_at(data_ + size_,
                            std::move_if_noexcept(data_[size_ - 1]));
            for (int i = size_ - 1; i > index; --i) {
                data_[i] = std::move_if_noexcept(data_[i - 1]);
            }
            data_[index] = value;
        }
        else {
            std::construct_at(data_ + size_, value);
        }

    }catch (...) {
        if (index < size_) {
            std::destroy_at(data_ + size_);
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
    for (int i = index; i < last; ++i) {
        if constexpr (std::is_nothrow_move_assignable_v<T> || !std::is_copy_assignable_v<T>) {
            data_[i] = std::move_if_noexcept(data_[i + 1]);
        } else {
            data_[i] = data_[i + 1];
        }
    }
    std::destroy_at(data_ + last);
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
