// Behaviour test for 0x00A33800 _STL::__linear_insert over pair<unsigned long, C3DDummyObject>: a value below the first
// key shifts [first, last) up one slot and goes first; otherwise the unguarded insert walks it down from last. Working
// stand-ins for __copy_backward and __unguarded_linear_insert.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace _STL {

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject { CMatrix3x4 Transform; long BoneIndex; };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }                                         // test-only: lets the test build an array
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned long, C3DDummyObject> TEntry;

struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

static int g_backward = 0, g_unguarded = 0;
TEntry* __fastcall __copy_backward(TEntry* first, TEntry* last, TEntry* result, const random_access_iterator_tag& tag, int* distance)
{
    ++g_backward;
    while (last != first) *--result = *--last;
    return result;
}
void __fastcall __unguarded_linear_insert(TEntry* last, TEntry val, CKeyPairCompareLess comp)
{
    ++g_unguarded;
    TEntry* next = last - 1;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

inline TEntry* copy_backward(TEntry* first, TEntry* last, TEntry* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(TEntry* first, TEntry* last, TEntry val, CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}

int main()
{
    using namespace _STL;
    TEntry a[5];
    unsigned long keys[5] = { 10, 20, 30, 5, 25 };
    for (int i = 0; i < 5; ++i) { a[i].first = keys[i]; a[i].second.BoneIndex = (long)keys[i] * 2; }
    CKeyPairCompareLess comp;
    __linear_insert(a, a + 3, a[3], comp);             // 5 < 10: shift and put first
    __linear_insert(a, a + 4, a[4], comp);             // 25: unguarded insert
    unsigned long want[5] = { 5, 10, 20, 25, 30 };
    int ok = g_backward == 1 && g_unguarded == 1;
    for (int i = 0; i < 5; ++i) ok = ok && a[i].first == want[i] && a[i].second.BoneIndex == (long)want[i] * 2;
    if (!ok) { std::printf("00a33800_TEST FAIL\n"); return 1; }
    std::printf("00a33800_TEST PASS\n");
    return 0;
}