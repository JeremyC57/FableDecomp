// Behaviour test for 0x00517500 __insertion_sort<CThingBuilding**, CIsHouseLargerThan>: the range ends up ordered
// largest house first, equal houses keep their input order (insertion sort is stable), both the memmove-to-front and
// the unguarded walk-back paths run, and empty / one-element ranges are untouched.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThingBuilding { public: long HouseSize; };
class CIsHouseLargerThan {
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const { return a->HouseSize > b->HouseSize; }
};
inline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (int)((const char*)last - (const char*)first);
    return (num > 0) ? std::memmove((char*)result - num, first, num) : ((char*)result - num);
}
template <class _Tp> inline _Tp** copy_backward(_Tp** first, _Tp** last, _Tp** result)
{
    return (_Tp**)__copy_trivial_backward(first, last, result);
}
static int g_unguarded;
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter last, _Tp val, _Compare comp)
{
    ++g_unguarded;
    _RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter first, _RandomAccessIter last, _Tp val, _Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter first, _RandomAccessIter last, _Compare comp)
{
    if (first == last) return;
    for (_RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    static const long sizes[10] = { 3, 9, 1, 4, 4, 12, 0, 4, 2, 9 };
    CThingBuilding b[10];
    CThingBuilding* a[10];
    for (int i = 0; i < 10; ++i) { b[i].HouseSize = sizes[i]; a[i] = &b[i]; }
    CIsHouseLargerThan comp;
    __insertion_sort(a, a, comp);
    __insertion_sort(a, a + 1, comp);
    int ok = g_unguarded == 0;
    for (int i = 0; i < 10; ++i) ok &= a[i] == &b[i];
    __insertion_sort(a, a + 10, comp);
    // expected: 12(b5) 9(b1) 9(b9) 4(b3) 4(b4) 4(b7) 3(b0) 2(b8) 1(b2) 0(b6)
    static const int want[10] = { 5, 1, 9, 3, 4, 7, 0, 8, 2, 6 };
    for (int i = 0; i < 10; ++i) ok &= a[i] == &b[want[i]];
    ok &= g_unguarded > 0;
    if (!ok) { std::printf("00517500_TEST FAIL\n"); return 1; }
    std::printf("00517500_TEST PASS\n");
    return 0;
}