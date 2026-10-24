// Behaviour test for 004d0b50: out-of-line __pop_heap for a 28-byte keyed record -- heap-sort seven keyed elements through the row's function (with the
// SGI __adjust_heap/__push_heap written out here) and check ascending keys with each payload still beside its key.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#endif
static const char* const g_names[10] = { "k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9" };
#define NUM(s) ((long)((s).c_str()[1] - '0'))
template <class T, class C> void push_impl(T* first, long holeIndex, long topIndex, T value, C comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}
template <class T, class C> void adjust_impl(T* first, long holeIndex, long len, T value, C comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    push_impl(first, holeIndex, topIndex, value, comp);
}
struct SRecord24 {                                     // 24-byte record, user copy constructor through the implicit operator=
    long m_0, m_4, m_8, m_c, m_10, m_14;
    SRecord24() { }
    SRecord24(const SRecord24& other) { *this = other; }
};

struct SKeyedRecord24 {                                // std::pair<key, SRecord24>, 28 bytes
    long      first;                                   // +0x00
    SRecord24 second;                                  // +0x04
    SKeyedRecord24() { }
    SKeyedRecord24(const SKeyedRecord24& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // empty, passed by value
    CKeyPairCompareLess() { }
    bool operator()(const SKeyedRecord24& a, const SKeyedRecord24& b) const { return a.first < b.first; }
};

void __adjust_heap(SKeyedRecord24* first, long holeIndex, long len, SKeyedRecord24 value,
                              CKeyPairCompareLess comp) { adjust_impl(first, holeIndex, len, value, comp); }   // 0x004D0660

void __pop_heap(SKeyedRecord24* first, SKeyedRecord24* last, SKeyedRecord24* result, SKeyedRecord24 value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

#define KEY(e) ((long)(e).first)
int main()
{
    {
        static const int keys[7] = { 5, 9, 1, 7, 3, 8, 2 };
        static const int sorted[7] = { 1, 2, 3, 5, 7, 8, 9 };
        SKeyedRecord24 a[7];
        for (int i = 0; i < 7; ++i) {
            SKeyedRecord24& e = a[i];
            int k = keys[i];
            e.first = k;
            e.second.m_14 = k * 10;
        }
        CKeyPairCompareLess comp;
        for (int i = 1; i < 7; ++i) {
            SKeyedRecord24 v(a[i]);
            push_impl(a, (long)i, 0L, v, comp);
        }
        for (int len = 7; len > 1; --len)
            __pop_heap(a, a + len - 1, a + len - 1, SKeyedRecord24(a[len - 1]), comp, (long*)0);
        for (int i = 0; i < 7; ++i) {
            const SKeyedRecord24& e = a[i];
            if (KEY(e) != sorted[i] || !(e.second.m_14 == KEY(e) * 10)) { std::printf("004d0b50_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("004d0b50_TEST PASS\n");
    return 0;
}