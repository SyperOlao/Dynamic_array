//
// Created by SyperOlao on 04.11.2025.
//

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../core/Array.h"

class ArrayIntTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ArrayIntTest);
        CPPUNIT_TEST(test_push_back_and_index);
        CPPUNIT_TEST(test_insert_at_middle_shifts_tail);
        CPPUNIT_TEST(test_remove_shifts_left_and_destroys_last);
        CPPUNIT_TEST(test_iterator_forward);
        CPPUNIT_TEST(test_iterator_reverse);
        CPPUNIT_TEST(test_reallocation_keeps_values);
    CPPUNIT_TEST_SUITE_END();

public:
    void test_push_back_and_index() {
        Array<int> a;
        for (int i = 0; i < 5; ++i) a.insert(i);
        CPPUNIT_ASSERT_EQUAL(5, a.size());
        for (int i = 0; i < a.size(); ++i) {
            CPPUNIT_ASSERT_EQUAL(i, a[i]);
        }
    }

    void test_insert_at_middle_shifts_tail() {
        Array<int> a;
        for (int i = 0; i < 4; ++i) a.insert(i);   // [0,1,2,3]
        int idx = a.insert(2, 99);                // -> [0,1,99,2,3]
        CPPUNIT_ASSERT_EQUAL(2, idx);
        CPPUNIT_ASSERT_EQUAL(5, a.size());
        CPPUNIT_ASSERT_EQUAL(0, a[0]);
        CPPUNIT_ASSERT_EQUAL(1, a[1]);
        CPPUNIT_ASSERT_EQUAL(99, a[2]);
        CPPUNIT_ASSERT_EQUAL(2, a[3]);
        CPPUNIT_ASSERT_EQUAL(3, a[4]);
    }

    void test_remove_shifts_left_and_destroys_last() {
        struct Tracked {
            static inline int alive = 0;
            int v{};
            Tracked(int x=0): v(x){ ++alive; }
            Tracked(const Tracked& o): v(o.v){ ++alive; }
            Tracked(Tracked&& o) noexcept: v(o.v){ ++alive; }
            Tracked& operator=(const Tracked&) = default;
            Tracked& operator=(Tracked&&) noexcept = default;
            ~Tracked(){ --alive; }
        };

        {
            Array<Tracked> a;
            for (int i=0;i<3;++i) a.insert(Tracked{i});
            CPPUNIT_ASSERT_EQUAL(3, Tracked::alive);
            a.remove(1);
            CPPUNIT_ASSERT_EQUAL(2, a.size());
            CPPUNIT_ASSERT_EQUAL(0, a[0].v);
            CPPUNIT_ASSERT_EQUAL(2, a[1].v);
            CPPUNIT_ASSERT_EQUAL(2, Tracked::alive);
        }
        CPPUNIT_ASSERT_EQUAL(0, Tracked::alive);
    }

    void test_iterator_forward() {
        Array<int> a;
        for (int i = 1; i <= 4; ++i) a.insert(i);
        auto it = a.iterator();
        int sum = 0;
        for (; it.hasNext(); it.next()) sum += it.get();
        CPPUNIT_ASSERT_EQUAL(1+2+3+4, sum);
    }

    void test_iterator_reverse() {
        Array<int> a;
        for (int i = 1; i <= 3; ++i) a.insert(i);
        auto it = a.reverseIterator();
        int seq[3]; int k=0;
        for (; it.hasNext(); it.next()) seq[k++] = it.get();
        CPPUNIT_ASSERT_EQUAL(3, k);
        CPPUNIT_ASSERT_EQUAL(3, seq[0]);
        CPPUNIT_ASSERT_EQUAL(2, seq[1]);
        CPPUNIT_ASSERT_EQUAL(1, seq[2]);
    }

    void test_reallocation_keeps_values() {
        Array<int> a;
        for (int i=0;i<100;++i) a.insert(i);
        CPPUNIT_ASSERT_EQUAL(100, a.size());
        for (int i=0;i<100;++i) CPPUNIT_ASSERT_EQUAL(i, a[i]);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(ArrayIntTest);

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    bool ok = runner.run("", false);
    return ok ? 0 : 1;
}
