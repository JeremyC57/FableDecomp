#include <cstdio>
#include <vector>
#include <list>
struct CVertexGroupBlock {
    std::vector<unsigned long> Groups;
    CVertexGroupBlock() { }
    explicit CVertexGroupBlock(unsigned long n) { for (unsigned long i = 0; i < n; ++i) Groups.push_back(i); }
};
// behaviour test: CVertexGroupBlockOptimiser::CalcCurrentBlockSegments resizes the segment indices to the input count
// and resets them to -1, empties the segment groups, then starts one new (empty) segment group at every remaining block
// not yet painted, painting from that block and counting the segments
class CVertexGroupBlockOptimiser {
public:
    std::vector<CVertexGroupBlock> InputList;
    std::vector<bool> RemainingBlocks;
    std::vector<CVertexGroupBlock> CurrentBlockSegmentGroups;
    std::vector<long> CurrentBlockSegmentIndices;
    unsigned long CurrentBlockSegmentCount;
    std::vector<unsigned long> Painted;
    void PaintCurrentBlockSegment(unsigned long block)   // stand-in: a segment covers the block and the next one
    {
        Painted.push_back(block);
        for (unsigned long i = block; i < block + 2 && i < CurrentBlockSegmentIndices.size(); ++i)
            CurrentBlockSegmentIndices[i] = (long)CurrentBlockSegmentCount;
    }
    void CalcCurrentBlockSegments();
};
void CVertexGroupBlockOptimiser::CalcCurrentBlockSegments()
{
    CurrentBlockSegmentIndices.resize(InputList.size());
    CurrentBlockSegmentCount = 0;
    CurrentBlockSegmentGroups.clear();
    CurrentBlockSegmentGroups.reserve(InputList.size());
    for (unsigned long i = 0; i < CurrentBlockSegmentIndices.size(); ++i)
        CurrentBlockSegmentIndices[i] = -1;
    for (unsigned long i = 0; i < CurrentBlockSegmentIndices.size(); ++i) {
        if (RemainingBlocks[i] && CurrentBlockSegmentIndices[i] == -1) {
            CurrentBlockSegmentGroups.push_back(CVertexGroupBlock());
            PaintCurrentBlockSegment(i);
            ++CurrentBlockSegmentCount;
        }
    }
}
int main()
{
    CVertexGroupBlockOptimiser o;
    for (int i = 0; i < 6; ++i) o.InputList.push_back(CVertexGroupBlock(i));
    bool remaining[6] = { true, true, false, true, true, true };
    for (int i = 0; i < 6; ++i) o.RemainingBlocks.push_back(remaining[i]);
    o.CurrentBlockSegmentGroups.push_back(CVertexGroupBlock(4));
    o.CurrentBlockSegmentIndices.push_back(7);
    o.CurrentBlockSegmentCount = 9;
    o.CalcCurrentBlockSegments();
    long expect[6] = { 0, 0, -1, 1, 1, 2 };
    bool ok = o.CurrentBlockSegmentCount == 3 && o.CurrentBlockSegmentGroups.size() == 3 && o.CurrentBlockSegmentIndices.size() == 6
        && o.Painted.size() == 3 && o.Painted[0] == 0 && o.Painted[1] == 3 && o.Painted[2] == 5
        && o.CurrentBlockSegmentGroups[0].Groups.empty();
    for (int i = 0; ok && i < 6; ++i) ok = o.CurrentBlockSegmentIndices[i] == expect[i];
    if (!ok) { std::printf("00ad9b20_TEST FAIL\n"); return 1; }
    std::printf("00ad9b20_TEST PASS\n");
    return 0;
}
