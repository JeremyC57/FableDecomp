// behaviour test for 0x00470d77 (STLport unguarded insertion sort of pair<unsigned long, CSoundPair> by key)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
struct CSoundPair {                             // 8 bytes
    long FirstSound;                            // +0x00
    long LastSound;                             // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}

template <class K, class V>
struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
typedef pair<unsigned long, CSoundPair> CSoundEntry;
template void __fastcall __unguarded_insertion_sort_aux(CSoundEntry*, CSoundEntry*, CSoundEntry*,
                                                        CKeyPairCompareLess<unsigned long, CSoundPair>);
}

int main()
{
    static double raw[16];
    std::CSoundEntry* a = (std::CSoundEntry*)raw;
    unsigned long keys[6] = { 0, 9, 3, 0xFFFFFFF0u, 3, 1 };
    for (int i = 0; i < 6; ++i) { a[i].first = keys[i]; a[i].second.FirstSound = i; a[i].second.LastSound = 100 + i; }
    CKeyPairCompareLess<unsigned long, CSoundPair> comp;
    std::__unguarded_insertion_sort_aux(a + 1, a + 6, (std::CSoundEntry*)0, comp);
    std::__unguarded_insertion_sort_aux(a + 6, a + 6, (std::CSoundEntry*)0, comp);   // empty range
    // sorted by key (unsigned), equal keys keep their order (3 from slot 2 before 3 from slot 4), payload moves with key
    int ok = a[0].first == 0 && a[1].first == 1 && a[1].second.FirstSound == 5 && a[2].first == 3 && a[2].second.FirstSound == 2
          && a[3].first == 3 && a[3].second.FirstSound == 4 && a[4].first == 9 && a[4].second.LastSound == 101
          && a[5].first == 0xFFFFFFF0u && a[5].second.FirstSound == 3;
    if (!ok) { printf("00470d77_TEST FAIL\n"); return 1; }
    printf("00470d77_TEST PASS\n");
    return 0;
}