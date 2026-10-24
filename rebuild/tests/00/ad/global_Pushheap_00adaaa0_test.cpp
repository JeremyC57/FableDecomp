// behaviour test for 0x00ADAAA0 _Push_heap<CBlockInfo, less<CBlockInfo>> (max-heap on Cost)
#include <cstdio>
struct CBlockInfo {
    unsigned int BlockIndex;
    int Cost;
    int Benefit;
    bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
};
struct SBlockInfoLess {
    char m_tag;
    bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; }
};
static void _Push_heap(CBlockInfo* first, long hole, long top, CBlockInfo value, SBlockInfoLess comp)
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
    static const int costs[7] = { 4, -2, 9, 1, 7, 3, 12 };
    CBlockInfo heap[7];
    SBlockInfoLess c; c.m_tag = 0;
    for (int i = 0; i < 7; ++i) {
        CBlockInfo v; v.BlockIndex = 100 + i; v.Cost = costs[i]; v.Benefit = -i;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (heap[(k - 1) / 2].Cost < heap[k].Cost) { std::printf("00adaaa0_TEST FAIL\n"); return 1; }
    }
    if (heap[0].Cost != 12 || heap[0].BlockIndex != 106 || heap[0].Benefit != -6) { std::printf("00adaaa0_TEST FAIL\n"); return 1; }
    std::printf("00adaaa0_TEST PASS\n");
    return 0;
}