//
// Created by SyperOlao on 04.11.2025.
//

#include <gtest/gtest.h>
#include "../core/Array.h"

struct Tracked {
    static inline int alive = 0;
    int v{};

    Tracked(int x = 0) : v(x) { ++alive; }
    Tracked(const Tracked& o) : v(o.v) { ++alive; }
    Tracked(Tracked&& o) noexcept : v(o.v) { ++alive; }
    Tracked& operator=(const Tracked&) = default;
    Tracked& operator=(Tracked&&) noexcept = default;
    ~Tracked() { --alive; }
};

TEST(ArrayIntTest, PushBackAndIndex) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i);
    }

    EXPECT_EQ(5, a.size());
    for (int i = 0; i < a.size(); ++i) {
        EXPECT_EQ(i, a[i]);
    }
}

TEST(ArrayIntTest, InsertAtMiddleShiftsTail) {
    Array<int> a;
    for (int i = 0; i < 4; ++i) {
        a.insert(i);
    }

    int idx = a.insert(2, 99);
    EXPECT_EQ(2, idx);
    EXPECT_EQ(5, a.size());
    EXPECT_EQ(0, a[0]);
    EXPECT_EQ(1, a[1]);
    EXPECT_EQ(99, a[2]);
    EXPECT_EQ(2, a[3]);
    EXPECT_EQ(3, a[4]);
}

TEST(ArrayIntTest, RemoveShiftsLeftAndDestroysLast) {
    {
        Array<Tracked> a;
        for (int i = 0; i < 3; ++i) {
            a.insert(Tracked{i});
        }
        EXPECT_EQ(3, Tracked::alive);

        a.remove(1);

        EXPECT_EQ(2, a.size());
        EXPECT_EQ(0, a[0].v);
        EXPECT_EQ(2, a[1].v);
        EXPECT_EQ(2, Tracked::alive);
    }

    EXPECT_EQ(0, Tracked::alive);
}

TEST(ArrayIntTest, IteratorForward) {
    Array<int> a;
    for (int i = 1; i <= 4; ++i) {
        a.insert(i);
    }

    auto it = a.iterator();
    int sum = 0;
    for (; it.hasNext(); it.next()) {
        sum += it.get();
    }

    EXPECT_EQ(1 + 2 + 3 + 4, sum);
}

TEST(ArrayIntTest, IteratorReverse) {
    Array<int> a;
    for (int i = 1; i <= 3; ++i) {
        a.insert(i);
    }

    auto it = a.reverseIterator();
    int seq[3];
    int k = 0;
    for (; it.hasNext(); it.next()) {
        seq[k++] = it.get();
    }

    EXPECT_EQ(3, k);
    EXPECT_EQ(3, seq[0]);
    EXPECT_EQ(2, seq[1]);
    EXPECT_EQ(1, seq[2]);
}

TEST(ArrayIntTest, ReallocationKeepsValues) {
    Array<int> a;
    for (int i = 0; i < 100; ++i) {
        a.insert(i);
    }

    EXPECT_EQ(100, a.size());
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(i, a[i]);
    }
}
