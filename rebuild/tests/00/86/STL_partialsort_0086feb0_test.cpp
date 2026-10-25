// Behaviour test for 0x0086FEB0: partial sort of 12-byte {signed key, 2 words} entries (smallest middle-first, in order)
#include <cstdio>

struct Entry { long key; long a; long b; };

static void PushHeap(Entry* first, int hole, int top, Entry val)
{
    int parent = (hole - 1) / 2;
    while (hole > top && first[parent].key < val.key) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}

static void AdjustHeap(Entry* first, int hole, int len, Entry val)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (first[second].key < first[second - 1].key) second--;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val);
}

static void MakeHeap(Entry* first, Entry* last)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, first[parent]);
        if (parent == 0) return;
        parent--;
    }
}

static void SortHeap(Entry* first, Entry* last)
{
    while (last - first > 1) {
        --last;
        Entry v = *last;
        *last = *first;
        AdjustHeap(first, 0, (int)(last - first), v);
    }
}

static void PartialSort(Entry* first, Entry* middle, Entry* last)
{
    MakeHeap(first, middle);
    for (Entry* i = middle; i < last; ++i)
        if (i->key < first->key) {
            Entry v = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), v);
        }
    SortHeap(first, middle);
}

int main()
{
    long keys[12] = { 30, -10, 70, 20, 90, 0, -50, 80, 40, 60, 10, -20 };
    Entry e[12];
    for (int i = 0; i < 12; ++i) { e[i].key = keys[i]; e[i].a = keys[i] * 3; e[i].b = i; }
    PartialSort(e, e + 5, e + 12);
    long want[5] = { -50, -20, -10, 0, 10 };
    for (int i = 0; i < 5; ++i)
        if (e[i].key != want[i] || e[i].a != want[i] * 3) { std::printf("0086feb0_TEST FAIL\n"); return 1; }
    for (int i = 5; i < 12; ++i)
        if (e[i].key < 10 || e[i].a != e[i].key * 3) { std::printf("0086feb0_TEST FAIL\n"); return 1; }
    std::printf("0086feb0_TEST PASS\n");
    return 0;
}