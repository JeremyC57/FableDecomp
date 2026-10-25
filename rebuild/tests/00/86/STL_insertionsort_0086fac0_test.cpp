// Behaviour test for 0x0086FAC0 std::__insertion_sort over 12-byte pair<long, {long, long}> records (inlined __linear_insert).
#include <cstdio>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct SPayload8 { long m_value0; long m_value1; };
struct SLongKeyedRecord {
    long first;
    SPayload8 second;
    SLongKeyedRecord() { }
    SLongKeyedRecord(const SLongKeyedRecord& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess {
    void* m_context;
    bool operator()(const SLongKeyedRecord& a, const SLongKeyedRecord& b) const { return a.first < b.first; }
};

static int g_backward = 0, g_unguarded = 0;
static SLongKeyedRecord* __copy_backward(SLongKeyedRecord* first, SLongKeyedRecord* last, SLongKeyedRecord* result,
                                         const random_access_iterator_tag&, int*)
{
    ++g_backward;
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
// stand-in for the out-of-line 0x0086F090: the real unguarded linear insert (shift while value < prev)
static void __unguarded_linear_insert(SLongKeyedRecord* last, SLongKeyedRecord value, CKeyPairCompareLess comp)
{
    ++g_unguarded;
    SLongKeyedRecord* next = last - 1;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}
inline SLongKeyedRecord* copy_backward(SLongKeyedRecord* first, SLongKeyedRecord* last, SLongKeyedRecord* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline void __linear_insert(SLongKeyedRecord* first, SLongKeyedRecord* last, SLongKeyedRecord value, CKeyPairCompareLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}
static void __insertion_sort(SLongKeyedRecord* first, SLongKeyedRecord* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SLongKeyedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    long keys[6] = { 5, -3, 9, 0, -7, 9 };
    SLongKeyedRecord a[6];
    for (int k = 0; k < 6; ++k) { a[k].first = keys[k]; a[k].second.m_value0 = 100 + k; a[k].second.m_value1 = 200 + k; }
    CKeyPairCompareLess comp = { 0 };
    __insertion_sort(a, a + 6, comp);
    long want[6] = { -7, -3, 0, 5, 9, 9 };
    long src[6] = { 4, 1, 3, 0, 2, 5 };   // stable: the two 9s keep their order
    for (int k = 0; k < 6; ++k)
        if (a[k].first != want[k] || a[k].second.m_value0 != 100 + src[k] || a[k].second.m_value1 != 200 + src[k]) {
            std::printf("0086fac0_TEST FAIL\n"); return 1;
        }
    // -3 and -7 go to the front through copy_backward, the others through the unguarded insert
    if (g_backward != 2 || g_unguarded != 3) { std::printf("0086fac0_TEST FAIL\n"); return 1; }
    std::printf("0086fac0_TEST PASS\n");
    return 0;
}