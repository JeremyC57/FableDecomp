// Behaviour test for 0x00ADBE70 _STL::__insertion_sort over 12-byte POD records keyed by a signed long at +4 (inlined __linear_insert).
#include <cstdio>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct SLongKeyAt4Record { long m_value0; long m_key; long m_value2; };
struct CKeyAt4Less {
    void* m_context;
    bool operator()(const SLongKeyAt4Record& a, const SLongKeyAt4Record& b) const { return a.m_key < b.m_key; }
};

static int g_backward = 0, g_unguarded = 0;
static SLongKeyAt4Record* __copy_backward(SLongKeyAt4Record* first, SLongKeyAt4Record* last, SLongKeyAt4Record* result, const random_access_iterator_tag&, int*)
{
    ++g_backward;
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
static void __unguarded_linear_insert(SLongKeyAt4Record* last, SLongKeyAt4Record value, CKeyAt4Less comp)
{
    ++g_unguarded;
    SLongKeyAt4Record* next = last - 1;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}
inline SLongKeyAt4Record* copy_backward(SLongKeyAt4Record* first, SLongKeyAt4Record* last, SLongKeyAt4Record* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline void __linear_insert(SLongKeyAt4Record* first, SLongKeyAt4Record* last, SLongKeyAt4Record value, CKeyAt4Less comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}
static void __insertion_sort(SLongKeyAt4Record* first, SLongKeyAt4Record* last, CKeyAt4Less comp)
{
    if (first == last)
        return;
    for (SLongKeyAt4Record* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SLongKeyAt4Record a[6];
    long keys[6] = { 5, -3, 8, 5, -10, 0 };
    for (int k = 0; k < 6; ++k) { a[k].m_value0 = k; a[k].m_key = keys[k]; a[k].m_value2 = 100 + k; }
    CKeyAt4Less comp = { 0 };
    __insertion_sort(a, a + 6, comp);
    long sorted[6] = { -10, -3, 0, 5, 5, 8 };
    long origin[6] = { 4, 1, 5, 0, 3, 2 };
    for (int k = 0; k < 6; ++k)
        if (a[k].m_key != sorted[k] || a[k].m_value0 != origin[k] || a[k].m_value2 != 100 + origin[k]) { std::printf("00adbe70_TEST FAIL\n"); return 1; }
    if (g_backward != 2 || g_unguarded != 3) { std::printf("00adbe70_TEST FAIL\n"); return 1; }
    std::printf("00adbe70_TEST PASS\n");
    return 0;
}