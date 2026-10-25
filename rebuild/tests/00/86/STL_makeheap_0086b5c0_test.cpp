// Behaviour test for 0x0086B5C0: make_heap of 16-byte {signed key, 3 words} entries (max-heap by key)
#include <cstdio>

struct Entry { long key; long a; long b; long c; };

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

int main()
{
    long keys[10] = { 3, -1, 7, 2, 9, 0, -5, 8, 4, 6 };
    Entry e[10];
    for (int i = 0; i < 10; ++i) { e[i].key = keys[i]; e[i].a = keys[i] * 3; e[i].b = i; e[i].c = -keys[i]; }
    MakeHeap(e, e + 10);
    if (e[0].key != 9) { std::printf("0086b5c0_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 10; ++i)
        if (e[(i - 1) / 2].key < e[i].key || e[i].a != e[i].key * 3 || e[i].c != -e[i].key) { std::printf("0086b5c0_TEST FAIL\n"); return 1; }
    Entry one[1]; one[0].key = 4; MakeHeap(one, one + 1);
    if (one[0].key != 4) { std::printf("0086b5c0_TEST FAIL\n"); return 1; }
    std::printf("0086b5c0_TEST PASS\n");
    return 0;
}