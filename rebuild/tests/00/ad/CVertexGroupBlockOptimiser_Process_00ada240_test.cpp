#include <cstdio>
#include <vector>
#include <list>
struct CVertexGroupBlock {
    std::vector<unsigned long> Groups;
    CVertexGroupBlock() { }
    explicit CVertexGroupBlock(unsigned long n) { for (unsigned long i = 0; i < n; ++i) Groups.push_back(i); }
};
// behaviour test: CVertexGroupBlockOptimiser::Process (non-empty input) clears the optimal list, marks every input block
// remaining, recomputes the segments, restarts the working list with one empty block as the current block, sets the
// optimal-size bound (remaining groups + MaxN - 3) / (MaxN - 1), greedy = !exhaustive, zero iterations, runs AddBlocks(0)
// and appends the optimal blocks to the output; an empty input does nothing
struct CVertexGroupBlockList {
    std::vector<CVertexGroupBlock> Blocks;
    void AddBlock(const CVertexGroupBlock& block) { Blocks.push_back(block); }
};
class CVertexGroupBlockOptimiser {
public:
    std::vector<CVertexGroupBlock> InputList;
    std::list<CVertexGroupBlock> OptimalBlockList;
    std::list<CVertexGroupBlock> Blocks;
    CVertexGroupBlock* CurrentBlock;
    std::vector<bool> RemainingBlocks;
    unsigned long MaxN;
    unsigned long OptimalSize;
    bool Greedy;
    unsigned long Iterations;
    int SegmentCalls;
    unsigned long AddBlocksFrom;
    void CalcCurrentBlockSegments() { ++SegmentCalls; }
    unsigned long CountRemainingGroups() const { return 11; }
    void AddBlocks(unsigned long block)                  // stand-in: the optimum found is two blocks
    {
        AddBlocksFrom = block;
        OptimalBlockList.push_back(CVertexGroupBlock(2));
        OptimalBlockList.push_back(CVertexGroupBlock(3));
    }
    void Process(CVertexGroupBlockList& output, bool exhaustive);
};
void CVertexGroupBlockOptimiser::Process(CVertexGroupBlockList& output, bool exhaustive)
{
    if (InputList.size() == 0)
        return;
    OptimalBlockList.clear();
    RemainingBlocks.clear();
    RemainingBlocks.insert(RemainingBlocks.end(), InputList.size(), true);
    CalcCurrentBlockSegments();
    Blocks.clear();
    {
        CVertexGroupBlock block;
        Blocks.push_back(block);
    }
    CurrentBlock = &Blocks.front();
    unsigned long maxN = MaxN;
    OptimalSize = (CountRemainingGroups() + maxN - 3) / (maxN - 1);
    Greedy = !exhaustive;
    Iterations = 0;
    AddBlocks(0);
    for (std::list<CVertexGroupBlock>::iterator it = OptimalBlockList.begin(); it != OptimalBlockList.end(); ++it)
        output.AddBlock(*it);
}
int main()
{
    CVertexGroupBlockOptimiser o;
    o.SegmentCalls = 0; o.Iterations = 9; o.Greedy = false; o.OptimalSize = 0; o.MaxN = 4; o.AddBlocksFrom = 99;
    CVertexGroupBlockList output;
    o.Process(output, false);
    bool ok = o.SegmentCalls == 0 && output.Blocks.empty() && o.Iterations == 9;
    for (int i = 0; i < 3; ++i) o.InputList.push_back(CVertexGroupBlock(i + 1));
    o.OptimalBlockList.push_back(CVertexGroupBlock(9));
    o.Blocks.push_back(CVertexGroupBlock(8));
    o.RemainingBlocks.push_back(false);
    output.Blocks.push_back(CVertexGroupBlock(1));
    o.Process(output, false);
    ok = ok && o.SegmentCalls == 1 && o.RemainingBlocks.size() == 3 && o.RemainingBlocks[0] && o.RemainingBlocks[2]
        && o.Blocks.size() == 1 && o.CurrentBlock == &o.Blocks.front() && o.CurrentBlock->Groups.empty()
        && o.OptimalSize == (11 + 4 - 3) / 3 && o.Greedy && o.Iterations == 0 && o.AddBlocksFrom == 0
        && output.Blocks.size() == 3 && output.Blocks[1].Groups.size() == 2 && output.Blocks[2].Groups.size() == 3;
    o.Process(output, true);
    ok = ok && !o.Greedy && output.Blocks.size() == 5;
    if (!ok) { std::printf("00ada240_TEST FAIL\n"); return 1; }
    std::printf("00ada240_TEST PASS\n");
    return 0;
}