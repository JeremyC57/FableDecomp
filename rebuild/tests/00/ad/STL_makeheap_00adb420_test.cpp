#include <cstdio>

// Host model of _STL::__make_heap over CVertexGroupBlockOptimiser::CBlockInfo (12 bytes) under less<CBlockInfo>
// (CBlockInfo::operator< on Cost): every parent from (len - 2) / 2 down to the root is sifted down (__adjust_heap +
// __push_heap), leaving the highest cost at the top.
struct CBlockInfo {
    unsigned int BlockIndex;
    long         Cost;
    long         Benefit;
    bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
};
struct CBlockLess { bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; } };

static void push_heap_(CBlockInfo* first, int hole, int top, CBlockInfo value, CBlockLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void adjust_heap(CBlockInfo* first, int hole, int len, CBlockInfo value, CBlockLess comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1]))
            second--;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        first[hole] = first[second - 1];
        hole = second - 1;
    }
    push_heap_(first, hole, top, value, comp);
}
static void make_heap_row(CBlockInfo* first, CBlockInfo* last, CBlockLess comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, CBlockInfo(first[parent]), comp);
        if (parent == 0) return;
        parent--;
    }
}

int main()
{
    const long cost[11] = { 40, 15, 90, 5, 60, 75, 20, 85, 10, 55, 30 };
    CBlockInfo b[11];
    for (int i = 0; i < 11; ++i) { b[i].BlockIndex = i; b[i].Cost = cost[i]; b[i].Benefit = 100 - cost[i]; }
    make_heap_row(b, b + 11, CBlockLess());
    for (int i = 1; i < 11; ++i)
        if (b[(i - 1) / 2] < b[i]) { std::printf("00adb420_TEST FAIL\n"); return 1; }
    if (b[0].BlockIndex != 2 || b[0].Cost != 90) { std::printf("00adb420_TEST FAIL\n"); return 1; }
    unsigned int idx = 0;
    for (int i = 0; i < 11; ++i) {
        idx += b[i].BlockIndex;
        if (b[i].Benefit != 100 - b[i].Cost) { std::printf("00adb420_TEST FAIL\n"); return 1; }   // records moved whole
    }
    if (idx != 55) { std::printf("00adb420_TEST FAIL\n"); return 1; }
    std::printf("00adb420_TEST PASS\n");
    return 0;
}