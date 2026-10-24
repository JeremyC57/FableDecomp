// behaviour test for 0x00776750 _Push_heap (long elements)
#include <cstdio>

struct SLongLess { char m_tag; bool operator()(const long& a, const long& b) const { return a < b; } };
static void _Push_heap(long* first, long hole, long top, long value, SLongLess comp)
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
    SLongLess c; c.m_tag = 0;
    static const long vals[7] = { 5, -1, 8, 3, -9, 12, 4 };
    long heap[7];
    for (int i = 0; i < 7; ++i) { _Push_heap(heap, i, 0, vals[i], c);
        for (int k = 1; k <= i; ++k) if (c(heap[(k - 1) / 2], heap[k])) { std::printf("00776750_TEST FAIL\n"); return 1; } }
    if (heap[0] != 12) { std::printf("00776750_TEST FAIL\n"); return 1; }
    // hole == top: the value is stored without a comparison
    long keep = heap[0];
    _Push_heap(heap, 0, 0, heap[6], c);
    if (heap[0] != heap[6]) { std::printf("00776750_TEST FAIL\n"); return 1; }
    heap[0] = keep;
    std::printf("00776750_TEST PASS\n");
    return 0;
}