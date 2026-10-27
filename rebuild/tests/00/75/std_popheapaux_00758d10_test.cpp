// Behaviour test for 0x00758D10: STLport __pop_heap_aux over (key, 40-byte entry) pairs: root moves to the back, the rest stays a heap
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SEntry { long Key; long Words[10]; };
static bool Less(const SEntry& a, const SEntry& b) { return a.Key < b.Key; }
static void PushHeap(SEntry* f, int hole, int top, SEntry v)
{
    int parent = (hole - 1) / 2;
    while (hole > top && Less(f[parent], v)) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = v;
}
static void AdjustHeap(SEntry* f, int hole, int len, SEntry v)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (Less(f[child], f[child - 1])) child--;
        f[hole] = f[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { f[hole] = f[child - 1]; hole = child - 1; }
    PushHeap(f, hole, top, v);
}
static void PopHeapAux(SEntry* first, SEntry* last)
{
    SEntry val = *(last - 1);
    *(last - 1) = *first;
    AdjustHeap(first, 0, (int)((last - 1) - first), val);
}
int main()
{
    SEntry h[8];
    long keys[8] = { 9, 7, 8, 3, 6, 5, 4, 1 };          // a max-heap
    for (int i = 0; i < 8; ++i) { h[i].Key = keys[i]; for (int w = 0; w < 10; ++w) h[i].Words[w] = keys[i] * 100 + w; }
    PopHeapAux(h, h + 8);
    CHECK(h[7].Key == 9 && h[7].Words[3] == 903);
    for (int i = 1; i < 7; ++i) CHECK(!Less(h[(i - 1) / 2], h[i]));
    CHECK(h[0].Key == 8);
    for (int i = 0; i < 7; ++i) CHECK(h[i].Words[0] == h[i].Key * 100);
    if (!g_fail) printf("00758d10_TEST PASS\n");
    return g_fail;
}