// Behaviour test for STLport __final_insertion_sort over pair<unsigned long, CSoundPair> (0x00470D36).
#include <cstdio>

class CSoundPair { public: unsigned int First, Second; };
template <class A, class B> struct pair { A first; B second; };
typedef pair<unsigned int, CSoundPair> Elem;

struct CKeyPairCompareLess {
    bool operator()(const Elem& a, const Elem& b) const { return a.first < b.first; }
};

static int g_insertion = 0, g_unguarded = 0;

template <class It, class T, class C> void __unguarded_linear_insert(It last, T val, C comp)
{
    It next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class It, class C> void __insertion_sort(It first, It last, C comp)
{
    ++g_insertion;
    if (first == last) return;
    for (It i = first + 1; i != last; ++i) {
        Elem val = *i;
        if (comp(val, *first)) {
            for (It j = i; j != first; --j) *j = *(j - 1);
            *first = val;
        }
        else
            __unguarded_linear_insert(i, val, comp);
    }
}
template <class It, class T, class C> void __unguarded_insertion_sort_aux(It first, It last, T*, C comp)
{
    ++g_unguarded;
    for (It i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);
}
template <class It, class C> void __unguarded_insertion_sort(It first, It last, C comp)
{
    __unguarded_insertion_sort_aux(first, last, (Elem*)0, comp);
}
template <class It, class C> void __final_insertion_sort(It first, It last, C comp)
{
    if (last - first > 16) {
        __insertion_sort(first, first + 16, comp);
        __unguarded_insertion_sort(first + 16, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

int main()
{
    // the real sort leaves every element within 16 of its place before this pass: emulate with blocks
    Elem a[40];
    for (int i = 0; i < 40; ++i) { a[i].first = (unsigned int)((i / 8) * 8 + (7 - i % 8)); a[i].second.First = i; a[i].second.Second = 0; }
    __final_insertion_sort(a, a + 40, CKeyPairCompareLess());
    for (int i = 0; i < 40; ++i) if (a[i].first != (unsigned int)i) { std::printf("00470d36_TEST FAIL\n"); return 1; }
    if (g_insertion != 1 || g_unguarded != 1) { std::printf("00470d36_TEST FAIL\n"); return 1; }
    Elem b[5];
    for (int i = 0; i < 5; ++i) { b[i].first = (unsigned int)(5 - i); }
    __final_insertion_sort(b, b + 5, CKeyPairCompareLess());
    for (int i = 0; i < 5; ++i) if (b[i].first != (unsigned int)(i + 1)) { std::printf("00470d36_TEST FAIL\n"); return 1; }
    if (g_insertion != 2 || g_unguarded != 1) { std::printf("00470d36_TEST FAIL\n"); return 1; }
    std::printf("00470d36_TEST PASS\n");
    return 0;
}