// Behaviour test for 0x00AD9D10 CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock2.
#include <cstdio>

struct CVertexGroupBlock {
    unsigned int* First; unsigned int* Last;
    unsigned int GroupCount() const { return (unsigned int)(Last - First); }
};
struct CBlockNode { CBlockNode* Next; CBlockNode* Prev; };
static int g_segmentCalcs = 0;
struct COpt {
    CBlockNode* BlocksNode; CVertexGroupBlock* Segments; unsigned int SegmentCount; unsigned int MaxN;
    void CalcCurrentBlockSegments() { ++g_segmentCalcs; }
    unsigned int BlocksSize() const { unsigned int n = 0; for (CBlockNode* it = BlocksNode->Next; it != BlocksNode; it = it->Next) ++n; return n; }
    unsigned int NewBlock2()
    {
        CalcCurrentBlockSegments();
        unsigned int result = BlocksSize();
        for (unsigned int i = 0; i < SegmentCount; ++i)
            result += (Segments[i].GroupCount() + MaxN - 3) / (MaxN - 1);
        return result;
    }
};

int main()
{
    CBlockNode head, n1;
    head.Next = &n1; n1.Next = &head;
    unsigned int g[32];
    CVertexGroupBlock seg[3];
    seg[0].First = g; seg[0].Last = g + 5;    // (5 + 6 - 3) / 5 = 1
    seg[1].First = g; seg[1].Last = g + 12;   // (12 + 3) / 5 = 3
    seg[2].First = g; seg[2].Last = g + 2;    // (2 + 3) / 5 = 1
    COpt o; o.BlocksNode = &head; o.Segments = seg; o.SegmentCount = 3; o.MaxN = 6;
    if (o.NewBlock2() != 1 + 1 + 3 + 1 || g_segmentCalcs != 1) { std::printf("00ad9d10_TEST FAIL\n"); return 1; }
    o.SegmentCount = 0;
    if (o.NewBlock2() != 1 || g_segmentCalcs != 2) { std::printf("00ad9d10_TEST FAIL\n"); return 1; }
    std::printf("00ad9d10_TEST PASS\n");
    return 0;
}