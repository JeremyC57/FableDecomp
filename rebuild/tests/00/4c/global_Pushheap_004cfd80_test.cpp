// behaviour test for 0x004cfd80 _Push_heap<pair<float,EConversationAttitude>, less on the float key>
#include <cstdio>
struct SPair { float m_first; int m_second; };
struct SLess { char m_tag; bool operator()(const SPair& a, const SPair& b) const { return a.m_first < b.m_first; } };
typedef SPair E;
typedef SLess C;
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
        v.m_first = keys[i] * 0.5f; v.m_second = 100 + i;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("004cfd80_TEST FAIL\n"); return 1; }
    }
    if (!(heap[0].m_first == 5.5f && heap[0].m_second == 102)) { std::printf("004cfd80_TEST FAIL\n"); return 1; }
    std::printf("004cfd80_TEST PASS\n");
    return 0;
}