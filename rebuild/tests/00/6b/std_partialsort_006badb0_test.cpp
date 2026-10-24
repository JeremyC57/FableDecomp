// Behaviour test for 0x006BADB0 std::__partial_sort(pair<unsigned long, CCountedPointer<CFaction> >*, .., comp).
#include <cstdio>

struct Entry { unsigned int first; int tag; };
struct KeyLess { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

// SGI heap helpers (stand-ins for the out-of-line 0x006BA230 / 0x006BA180 / 0x006BA830)
template <class C> static void PushHeap(Entry* first, int hole, int top, Entry value, C comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
template <class C> static void AdjustHeap(Entry* first, int hole, int len, Entry value, C comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) --second;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, value, comp);
}
template <class C> static void MakeHeap(Entry* first, Entry* last, C comp)
{
    int len = (int)(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; --parent) { AdjustHeap(first, parent, len, first[parent], comp); if (parent == 0) break; }
}
template <class C> static void PopHeap(Entry* first, Entry* last, Entry* result, Entry value, C comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), value, comp);
}
template <class C> static void SortHeap(Entry* first, Entry* last, C comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}

template <class C> static void PartialSort(Entry* first, Entry* middle, Entry* last, Entry*, C comp)
{
    MakeHeap(first, middle, comp);
    for (Entry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, Entry(*i), comp);
    SortHeap(first, middle, comp);
}

int main()
{
    unsigned int keys[9] = { 40, 7, 0x90000000u, 15, 3, 22, 1, 99, 8 };
    Entry v[9];
    for (int i = 0; i < 9; ++i) { v[i].first = keys[i]; v[i].tag = i; }
    PartialSort(v, v + 4, v + 9, (Entry*)0, KeyLess());
    int ok = v[0].first == 1 && v[1].first == 3 && v[2].first == 7 && v[3].first == 8;
    unsigned int sum = 0;
    for (int i = 0; i < 9; ++i) sum += v[i].first;
    unsigned int expect = 0;
    for (int i = 0; i < 9; ++i) expect += keys[i];
    if (sum != expect) ok = 0;                          // a permutation
    for (int i = 4; ok && i < 9; ++i) if (v[i].first < 8) ok = 0;
    if (!ok) { std::printf("006badb0_TEST FAIL\n"); return 1; }
    std::printf("006badb0_TEST PASS\n");
    return 0;
}