// Behaviour test for 0x0073D720 std::__unguarded_insertion_sort_aux over the mode-registry pairs (unsigned key)
#include <cstdio>
struct CRegisteredModeInfo { int tag; };

template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<unsigned int, CRegisteredModeInfo> TModePair;   // unsigned int: the 32-bit unsigned key

struct TModeLess {
    bool operator()(const TModePair& a, const TModePair& b) const { return a.first < b.first; }
};

static void Fill(TModePair* p, const unsigned int* keys, int n)
{
    for (int i = 0; i < n; ++i) { p[i].first = keys[i]; p[i].second.tag = i; }
}

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

void __unguarded_insertion_sort_aux(TModePair* first, TModePair* last, TModePair*, TModeLess comp)
{
    for (TModePair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, TModePair(*i), comp);
}

int main()
{
    // p[0] is the guard (smallest key); 0x90000000 must sort ABOVE 5 (unsigned compare)
    TModePair p[8];
    unsigned int keys[8] = { 0u, 7u, 0x90000000u, 3u, 7u, 1u, 0xFFFFFFFFu, 5u };
    Fill(p, keys, 8);
    __unguarded_insertion_sort_aux(p + 1, p + 8, (TModePair*)0, TModeLess());
    unsigned int want[8] = { 0u, 1u, 3u, 5u, 7u, 7u, 0x90000000u, 0xFFFFFFFFu };
    int ok = 1;
    for (int i = 0; i < 8; ++i) if (p[i].first != want[i]) ok = 0;
    if (p[4].second.tag != 1 || p[5].second.tag != 4) ok = 0;      // equal keys keep their order, values travel along
    if (p[0].second.tag != 0) ok = 0;
    __unguarded_insertion_sort_aux(p + 2, p + 2, (TModePair*)0, TModeLess());   // empty range
    for (int i = 0; i < 8; ++i) if (p[i].first != want[i]) ok = 0;
    if (!ok) { std::printf("0073d720_TEST FAIL\n"); return 1; }
    std::printf("0073d720_TEST PASS\n");
    return 0;
}