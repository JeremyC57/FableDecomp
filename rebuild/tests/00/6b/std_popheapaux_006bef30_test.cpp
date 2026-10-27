// Behaviour test for std::__pop_heap_aux<pair<long, CRegisteredMusicEntry>*> (0x006BEF30): moves the max key to last-1 and re-heaps the rest.
#include <cstdio>
struct Entry { int key; int payload; };
static void adjust_heap(Entry* first, int hole, int len, Entry val)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (first[child].key < first[child - 1].key) --child;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && first[parent].key < val.key) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void pop_heap_aux(Entry* first, Entry* last)
{
    Entry val = *(last - 1);                 // the copy built in the argument area
    *(last - 1) = *first;
    adjust_heap(first, 0, (int)(last - 1 - first), val);
}
int main()
{
    Entry h[5] = { {9, 90}, {7, 70}, {8, 80}, {3, 30}, {5, 50} };   // a max-heap
    pop_heap_aux(h, h + 5);
    if (h[4].key != 9 || h[4].payload != 90 || h[0].key != 8) { std::printf("006bef30_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 4; ++i) if (h[(i - 1) / 2].key < h[i].key) { std::printf("006bef30_TEST FAIL\n"); return 1; }
    pop_heap_aux(h, h + 4);
    if (h[3].key != 8 || h[0].key != 7) { std::printf("006bef30_TEST FAIL\n"); return 1; }
    std::printf("006bef30_TEST PASS\n");
    return 0;
}