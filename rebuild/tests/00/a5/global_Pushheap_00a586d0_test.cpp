// behaviour test for 0x00a586d0 _Push_heap<float, less> (max-heap)
#include <cstdio>
struct SFloatLess { char m_tag; bool operator()(float a, float b) const { return a < b; } };
typedef float E;
typedef SFloatLess C;
static void _Push_heap(E* first, long hole, long top, E value, C comp)
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
    static const int keys[7] = { 4, -3, 11, 0, 7, -8, 5 };
    E heap[7];
    C c;
    for (int i = 0; i < 7; ++i) {
        E v;
        v = keys[i] * 0.25f;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("00a586d0_TEST FAIL\n"); return 1; }
    }
    if (!(heap[0] == 2.75f)) { std::printf("00a586d0_TEST FAIL\n"); return 1; }
    std::printf("00a586d0_TEST PASS\n");
    return 0;
}