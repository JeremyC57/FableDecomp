// behaviour test for 0x00932370 _Push_heap (CThing* elements)
#include <cstdio>
class CThing { public: int size; };
static bool LowerThan(CThing* a, CThing* b) { return a->size < b->size; }
typedef bool (*TCmp)(CThing*, CThing*);
static void _Push_heap(CThing** first, long hole, long top, CThing* value, TCmp comp)
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
    TCmp c = LowerThan;
    static const int sizes[7] = { 5, 1, 8, 3, 9, 2, 4 };
    static CThing objs[7];
    CThing* heap[7];
    for (int i = 0; i < 7; ++i) { objs[i].size = sizes[i]; _Push_heap(heap, i, 0, &objs[i], c);
        for (int k = 1; k <= i; ++k) if (c(heap[(k - 1) / 2], heap[k])) { std::printf("00932370_TEST FAIL\n"); return 1; } }
    if (heap[0] != &objs[4]) { std::printf("00932370_TEST FAIL\n"); return 1; }
    // hole == top: the value is stored without a comparison
    CThing* keep = heap[0];
    _Push_heap(heap, 0, 0, heap[6], c);
    if (heap[0] != heap[6]) { std::printf("00932370_TEST FAIL\n"); return 1; }
    heap[0] = keep;
    std::printf("00932370_TEST PASS\n");
    return 0;
}