// Behaviour test for 0x007588A0 std::__unguarded_insertion_sort_aux over the 0x2C-byte wound-morph entries
#include <cstdio>

enum EBodyLocation { LOC0 = 0 };
struct CWoundMorphsEntry { long Words[10]; };
template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};
typedef pair<int, CWoundMorphsEntry> TWoundPair;
struct CKeyPairCompareLess {
    bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; }
};

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

void __unguarded_insertion_sort_aux(TWoundPair* first, TWoundPair* last, TWoundPair*, CKeyPairCompareLess comp)
{
    for (TWoundPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, TWoundPair(*i), comp);
}

int main()
{
    // element 0 is the guard (smallest key); sort [1, 8)
    int keys[8] = { -100, 5, -3, 9, 5, 0, -50, 2 };
    TWoundPair a[8];
    for (int i = 0; i < 8; ++i) { a[i].first = keys[i]; for (int k = 0; k < 10; ++k) a[i].second.Words[k] = keys[i] * 10 + k; }
    __unguarded_insertion_sort_aux(a + 1, a + 8, (TWoundPair*)0, CKeyPairCompareLess());
    int want[8] = { -100, -50, -3, 0, 2, 5, 5, 9 };
    int ok = 1;
    for (int i = 0; i < 8; ++i) {
        if (a[i].first != want[i]) ok = 0;
        for (int k = 0; k < 10; ++k) if (a[i].second.Words[k] != want[i] * 10 + k) ok = 0;
    }
    if (!ok) { std::printf("007588a0_TEST FAIL\n"); return 1; }
    std::printf("007588a0_TEST PASS\n");
    return 0;
}