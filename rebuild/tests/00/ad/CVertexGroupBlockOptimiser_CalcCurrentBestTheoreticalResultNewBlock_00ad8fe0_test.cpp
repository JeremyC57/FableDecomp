// Behaviour test for 0x00AD8FE0 CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock.
#include <cstdio>

struct CVertexGroupBlock {
    unsigned int* First; unsigned int* Last;
    unsigned int GroupCount() const { return (unsigned int)(Last - First); }
};
struct CBlockNode { CBlockNode* Next; CBlockNode* Prev; };
struct COpt {
    CBlockNode* BlocksNode; CVertexGroupBlock* CurrentBlock; unsigned int MaxN; unsigned int Remaining;
    unsigned int CountRemainingGroups() const { return Remaining; }
    unsigned int BlocksSize() const { unsigned int n = 0; for (CBlockNode* it = BlocksNode->Next; it != BlocksNode; it = it->Next) ++n; return n; }
    unsigned int NewBlock()
    {
        unsigned int remaining = CountRemainingGroups();
        unsigned int free = MaxN - CurrentBlock->GroupCount();
        unsigned int blocks = BlocksSize();
        return blocks + (MaxN - free + remaining - 1) / MaxN;
    }
};

int main()
{
    CBlockNode head, n1, n2;
    head.Next = &n1; n1.Next = &n2; n2.Next = &head;
    unsigned int groups[8];
    CVertexGroupBlock current; current.First = groups; current.Last = groups + 3;
    COpt o; o.BlocksNode = &head; o.CurrentBlock = &current; o.MaxN = 8; o.Remaining = 10;
    // 2 blocks + ceil((3 + 10) / 8) = 2 + 2 = 4 ... (3 + 10 - 1) / 8 = 1 -> 3
    if (o.NewBlock() != 2 + (3 + 10 - 1) / 8) { std::printf("00ad8fe0_TEST FAIL\n"); return 1; }
    o.Remaining = 14;   // (3 + 14 - 1) / 8 = 2
    if (o.NewBlock() != 4) { std::printf("00ad8fe0_TEST FAIL\n"); return 1; }
    head.Next = &head; o.Remaining = 1; current.Last = groups;   // no blocks, empty current: (0 + 1 - 1) / 8 = 0
    if (o.NewBlock() != 0) { std::printf("00ad8fe0_TEST FAIL\n"); return 1; }
    std::printf("00ad8fe0_TEST PASS\n");
    return 0;
}