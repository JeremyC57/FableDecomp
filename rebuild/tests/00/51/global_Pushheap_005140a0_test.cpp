// behaviour test for 0x005140a0 _Push_heap (CThingBuilding* elements)
#include <cstdio>
class CThingBuilding { public: int size; };
class CIsHouseLargerThan { public: char m_tag; bool operator()(CThingBuilding* a, CThingBuilding* b) const { return a->size > b->size; } };
static void _Push_heap(CThingBuilding** first, long hole, long top, CThingBuilding* value, CIsHouseLargerThan comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    CIsHouseLargerThan c; c.m_tag = 0;
    static const int sizes[7] = { 5, 1, 8, 3, 9, 2, 4 };
    static CThingBuilding objs[7];
    CThingBuilding* heap[7];
    for (int i = 0; i < 7; ++i) { objs[i].size = sizes[i]; _Push_heap(heap, i, 0, &objs[i], c);
        for (int k = 1; k <= i; ++k) if (c(heap[(k - 1) / 2], heap[k])) { std::printf("005140a0_TEST FAIL\n"); return 1; } }
    if (heap[0] != &objs[1]) { std::printf("005140a0_TEST FAIL\n"); return 1; }
    // hole == top: the value is stored without a comparison
    CThingBuilding* keep = heap[0];
    _Push_heap(heap, 0, 0, heap[6], c);
    if (heap[0] != heap[6]) { std::printf("005140a0_TEST FAIL\n"); return 1; }
    heap[0] = keep;
    std::printf("005140a0_TEST PASS\n");
    return 0;
}