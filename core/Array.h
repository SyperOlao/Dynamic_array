//
// Created by SyperOlao on 03.11.2025.
//

#ifndef DYNAMICARRAY_ARRAY_H
#define DYNAMICARRAY_ARRAY_H
#include "Consts.h"
#include <new>
#include <utility>
#include <cstdlib>


template<typename T>
class Array final {
    T *data_ = nullptr;
    int size_ = 0;
    int capacity_ = 0;

    void grow_if_needed_for_insert();

    static T *allocate_new_data(int capacity = 0);

    class Iterator {
    public:
        const T &get() const;

        void set(const T &value);

        void next();

        [[nodiscard]] bool hasNext() const;

    private:
        Array *owner_ = nullptr;
        int idx_ = 0;
        int step_ = +1;
        int end_ = 0;

        friend class Array;

        Iterator(Array *o, const int start, const int step, const int end)
            : owner_(o), idx_(start), step_(step), end_(end) {
        }
    };

    class ConstIterator {
    public:
        const T &get() const;

        void next();

        [[nodiscard]] bool hasNext() const;

    private:
        const Array *owner_ = nullptr;
        int idx_ = 0;
        int step_ = +1;
        int end_ = 0;
        friend class Array;

        ConstIterator(const Array *owner, const int start, const int step, const int end)
            : owner_(owner), idx_(start), step_(step), end_(end) {
        }
    };

public:
    Array();

    Array(const Array& other);
    Array(Array<T>&& other) noexcept;

    Array<T>& operator=(const Array<T>& other);
    Array<T>& operator=(Array<T>&& other) noexcept;

    explicit Array(int capacity);

    ~Array();

    int insert(const T &value);

    int insert(T &&value);

    int insert(int index, const T &value);

    void remove(int index);

    const T &operator[](int index) const;

    T &operator[](int index);

    [[nodiscard]] int size() const;

    Iterator iterator() { return Iterator(this, 0, +1, size_); }
    ConstIterator iterator() const { return ConstIterator(this, 0, +1, size_); }

    Iterator reverseIterator() { return Iterator(this, size_ - 1, -1, -1); }
};

#include "Array.tpp"
#endif //DYNAMICARRAY_ARRAY_H
