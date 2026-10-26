// behaviour test for 0x0048D520 std::deque<C2DVector>::~deque(): every element from start to finish is destroyed once, in order, across node boundaries; then each node buffer from the start node to the finish node and the map are freed; a deque without a map frees nothing
#include <cstdio>
static int gDestroyed[16]; static int gNDestroyed = 0; static int gFreed = 0;
struct Elem { int Id; };
struct Deque { Elem** Map; Elem** StartNode; Elem* StartCur; Elem** FinishNode; Elem* FinishCur; int PerNode; };
static void DestroyDeque(Deque& d)                            // the function under test
{
    Elem** node = d.StartNode; Elem* cur = d.StartCur;
    while (!(node == d.FinishNode && cur == d.FinishCur)) {
        gDestroyed[gNDestroyed++] = cur->Id;
        ++cur;
        if (cur == *node + d.PerNode) { ++node; cur = *node; }
    }
    if (d.Map) {
        for (Elem** n = d.StartNode; n < d.FinishNode + 1; ++n) if (*n) ++gFreed;
        ++gFreed;
    }
}
int main()
{
    int fail = 0;
    Elem a[2] = { { 1 }, { 2 } }, b[2] = { { 3 }, { 4 } }, c[2] = { { 5 }, { 6 } };
    Elem* map[3] = { a, b, c };
    Deque d; d.Map = map; d.StartNode = map; d.StartCur = a + 1; d.FinishNode = map + 2; d.FinishCur = c + 1; d.PerNode = 2;
    DestroyDeque(d);
    if (gNDestroyed != 4 || gDestroyed[0] != 2 || gDestroyed[1] != 3 || gDestroyed[2] != 4 || gDestroyed[3] != 5) fail |= 1;
    if (gFreed != 4) fail |= 2;
    Deque e; e.Map = 0; e.StartNode = 0; e.StartCur = 0; e.FinishNode = 0; e.FinishCur = 0; e.PerNode = 2;
    DestroyDeque(e);
    if (gNDestroyed != 4 || gFreed != 4) fail |= 4;
    if (fail) { std::printf("0048d520_TEST FAIL %d\n", fail); return 1; }
    std::printf("0048d520_TEST PASS\n");
    return 0;
}
