// Behaviour test for 0x00A73330 std::__partial_sort over (long key, record) pairs: after the call [first, middle)
// holds the smallest keys in ascending order and the whole range is a permutation of the input.
#include <cstdio>

struct SRec { long key; int tag; };
struct CLess { bool operator()(const SRec& a, const SRec& b) const { return a.key < b.key; } };

static void AdjustHeap(SRec* first, int hole, int len, SRec val, CLess comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = val;
}
static void MakeHeap(SRec* first, SRec* last, CLess comp)
{
    int len = (int)(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; parent--) {
        AdjustHeap(first, parent, len, first[parent], comp);
        if (parent == 0) return;
    }
}
static void PopHeap(SRec* first, SRec* last, SRec* result, SRec val, CLess comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val, comp);
}
static void SortHeap(SRec* first, SRec* last, CLess comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}
// the function under test (same shape as the source)
static void PartialSort(SRec* first, SRec* middle, SRec* last, SRec*, CLess comp)
{
    MakeHeap(first, middle, comp);
    for (SRec* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, SRec(*i), comp);
    SortHeap(first, middle, comp);
}

int main()
{
    long keys[12] = { 9, -3, 7, 7, 100, 0, -50, 12, 5, 4, 3, -3 };
    SRec v[12];
    long sum = 0;
    for (int i = 0; i < 12; ++i) { v[i].key = keys[i]; v[i].tag = i; sum += keys[i]; }
    PartialSort(v, v + 5, v + 12, (SRec*)0, CLess());
    long expect[5] = { -50, -3, -3, 0, 3 };
    for (int i = 0; i < 5; ++i)
        if (v[i].key != expect[i]) { std::printf("00a73330_TEST FAIL\n"); return 1; }
    long sum2 = 0;
    for (int i = 0; i < 12; ++i) sum2 += v[i].key;
    for (int i = 5; i < 12; ++i)
        if (v[i].key < 3) { std::printf("00a73330_TEST FAIL\n"); return 1; }
    if (sum2 != sum) { std::printf("00a73330_TEST FAIL\n"); return 1; }
    std::printf("00a73330_TEST PASS\n");
    return 0;
}