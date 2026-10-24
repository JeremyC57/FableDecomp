// behaviour test for 0x0097adf0 _Push_heap (CThing* elements)
#include <cstdio>
struct CThing { int size; };
struct CCmp { float OriginX, OriginY, DirectionX, DirectionY; bool operator()(CThing* a, CThing* b) const { return (a->size - OriginX) * (a->size - OriginX) > (b->size - OriginX) * (b->size - OriginX); } };
static void _Push_heap(CThing** first, long hole, long top, CThing* value, CCmp comp)
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
    CCmp c; c.OriginX = 3.0f; c.OriginY = 0; c.DirectionX = 1; c.DirectionY = 0;
    static const int sizes[7] = { 5, 1, 8, 3, 9, 2, 4 };
    static CThing objs[7];
    CThing* heap[7];
    for (int i = 0; i < 7; ++i) { objs[i].size = sizes[i]; _Push_heap(heap, i, 0, &objs[i], c);
        for (int k = 1; k <= i; ++k) if (c(heap[(k - 1) / 2], heap[k])) { std::printf("0097adf0_TEST FAIL\n"); return 1; } }
    if (heap[0] != &objs[3]) { std::printf("0097adf0_TEST FAIL\n"); return 1; }
    // hole == top: the value is stored without a comparison
    CThing* keep = heap[0];
    _Push_heap(heap, 0, 0, heap[6], c);
    if (heap[0] != heap[6]) { std::printf("0097adf0_TEST FAIL\n"); return 1; }
    heap[0] = keep;
    std::printf("0097adf0_TEST PASS\n");
    return 0;
}