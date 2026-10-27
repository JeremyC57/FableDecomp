// Behaviour test for 0x0086F9A0 std::__pop_heap_aux over pair<EExpressionType, CExpressionSet::CEntry>: the heap top moves
// to the last slot and [first, last - 1) is re-heaped with the old last element.
#include <cstdio>

struct Entry { int first; long payload; };

static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }
static void PushHeap(Entry* f, int hole, int top, Entry v)
{
    int parent = (hole - 1) / 2;
    while (hole > top && Less(f[parent], v)) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = v;
}
static void AdjustHeap(Entry* f, int hole, int len, Entry v)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) { if (Less(f[second], f[second - 1])) second--; f[hole] = f[second]; hole = second; second = 2 * (second + 1); }
    if (second == len) { f[hole] = f[second - 1]; hole = second - 1; }
    PushHeap(f, hole, top, v);
}
static void PopHeapAux(Entry* first, Entry* last)
{
    Entry val = *(last - 1);
    *(last - 1) = *first;
    AdjustHeap(first, 0, int((last - 1) - first), val);
}

int main()
{
    Entry h[6] = { { 10, 100 }, { 6, 60 }, { 9, 90 }, { 2, 20 }, { 5, 50 }, { 4, 40 } };
    PopHeapAux(h, h + 6);
    if (h[5].first != 10 || h[5].payload != 100) { std::printf("0086f9a0_TEST FAIL\n"); return 1; }
    if (h[0].first != 9) { std::printf("0086f9a0_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 5; ++i)
        if (h[(i - 1) / 2].first < h[i].first) { std::printf("0086f9a0_TEST FAIL\n"); return 1; }
    PopHeapAux(h, h + 5);
    if (h[4].first != 9 || h[0].first != 6) { std::printf("0086f9a0_TEST FAIL\n"); return 1; }
    std::printf("0086f9a0_TEST PASS\n");
    return 0;
}