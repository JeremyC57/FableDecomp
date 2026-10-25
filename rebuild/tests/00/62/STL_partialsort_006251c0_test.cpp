// Behaviour test for 0x006251C0: STLport __partial_sort (make_heap, pop smaller later elements in, sort_heap)
#include <cstdio>

struct Quest { int key; int tag; };
struct Less { bool operator()(const Quest& a, const Quest& b) const { return a.key < b.key; } };

static void AdjustHeap(Quest* first, int hole, int len, Quest v, Less comp)
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
    while (hole > top && comp(first[parent], v)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = v;
}
static void MakeHeap(Quest* first, Quest* last, Less comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) { AdjustHeap(first, parent, len, first[parent], comp); if (parent == 0) return; parent--; }
}
static void PopHeap(Quest* first, Quest* last, Quest* result, Quest v, Less comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), v, comp);
}
static void SortHeap(Quest* first, Quest* last, Less comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}
static void PartialSort(Quest* first, Quest* middle, Quest* last, Less comp)
{
    MakeHeap(first, middle, comp);
    for (Quest* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, Quest(*i), comp);
    SortHeap(first, middle, comp);
}

int main()
{
    int keys[12] = { 9, 3, 7, 1, 12, 5, 11, 2, 8, 6, 10, 4 };
    Quest q[12];
    for (int i = 0; i < 12; ++i) { q[i].key = keys[i]; q[i].tag = i; }
    PartialSort(q, q + 5, q + 12, Less());
    for (int i = 0; i < 5; ++i)
        if (q[i].key != i + 1) { std::printf("006251c0_TEST FAIL\n"); return 1; }
    int sum = 0;
    for (int i = 0; i < 12; ++i) sum += q[i].key;
    if (sum != 78) { std::printf("006251c0_TEST FAIL\n"); return 1; }
    std::printf("006251c0_TEST PASS\n");
    return 0;
}