// behaviour test for 0x0088c7c0 _Push_heap<SKeyedPair, greater on the key (min-heap)>
#include <cstdio>
struct SKeyedPair { int m_key; unsigned int m_value; };
struct SKeyGreater { char m_tag; bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key > b.m_key; } };
typedef SKeyedPair E;
typedef SKeyGreater C;
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
        v.m_key = keys[i]; v.m_value = 100u + i;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("0088c7c0_TEST FAIL\n"); return 1; }
    }
    if (!(heap[0].m_key == -8 && heap[0].m_value == 105u)) { std::printf("0088c7c0_TEST FAIL\n"); return 1; }
    std::printf("0088c7c0_TEST PASS\n");
    return 0;
}