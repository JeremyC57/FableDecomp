#include <cstdio>
#include <vector>
#include <list>
struct CVertexGroupBlock {
    std::vector<unsigned long> Groups;
    CVertexGroupBlock() { }
    explicit CVertexGroupBlock(unsigned long n) { for (unsigned long i = 0; i < n; ++i) Groups.push_back(i); }
};
// behaviour test: CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult recomputes the segments, then counts the
// existing blocks, adds (n - 1) / (MaxN - 1) full blocks per segment of n groups, collects the non-zero remainders and
// adds the minimum packing CalcMinimumBlocks finds for them (all available, starting at 0)
class CVertexGroupBlockOptimiser {
public:
    std::list<CVertexGroupBlock> Blocks;
    CVertexGroupBlock* CurrentBlock;
    std::vector<CVertexGroupBlock> CurrentBlockSegmentGroups;
    unsigned long CurrentBlockSegmentCount;
    unsigned long MaxN;
    std::vector<unsigned long> SeenSizes;
    unsigned long SeenGroupCount;
    int SegmentCalls;
    void CalcCurrentBlockSegments()                      // stand-in: segments of 7, 3 and 4 groups
    {
        ++SegmentCalls;
        CurrentBlockSegmentGroups.clear();
        CurrentBlockSegmentGroups.push_back(CVertexGroupBlock(7));
        CurrentBlockSegmentGroups.push_back(CVertexGroupBlock(3));
        CurrentBlockSegmentGroups.push_back(CVertexGroupBlock(4));
        CurrentBlockSegmentCount = 3;
    }
    unsigned long CalcMinimumBlocks(const std::vector<unsigned long>& sizes, unsigned long& groupCount,
                                    std::vector<bool>& available, unsigned long count, unsigned long start)
    {
        SeenSizes = sizes;
        SeenGroupCount = groupCount;
        bool allAvailable = available.size() == sizes.size();
        for (unsigned long i = 0; i < available.size(); ++i) allAvailable = allAvailable && available[i];
        return allAvailable && count == sizes.size() && start == 0 ? 100 : 1000;
    }
    unsigned long CalcCurrentBestTheoreticalResult();
};
unsigned long CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult()
{
    CalcCurrentBlockSegments();
    unsigned long result = (unsigned long)Blocks.size();
    std::vector<unsigned long> remainders;
    remainders.reserve(CurrentBlockSegmentCount);
    for (unsigned long i = 0; i < CurrentBlockSegmentCount; ++i) {
        unsigned long groupCount = (unsigned long)CurrentBlockSegmentGroups[i].Groups.size();
        unsigned long fullBlocks = (groupCount - 1) / (MaxN - 1);
        result += fullBlocks;
        unsigned long remainder = groupCount - fullBlocks * (MaxN - 1);
        if (remainder)
            remainders.push_back(remainder);
    }
    unsigned long groupCount = (unsigned long)CurrentBlock->Groups.size();
    std::vector<bool> available(remainders.size(), true);
    return CalcMinimumBlocks(remainders, groupCount, available, (unsigned long)remainders.size(), 0) + result;
}
int main()
{
    CVertexGroupBlockOptimiser o;
    o.SegmentCalls = 0;
    o.Blocks.push_back(CVertexGroupBlock(5));
    o.Blocks.push_back(CVertexGroupBlock(2));
    o.CurrentBlock = &o.Blocks.front();
    o.MaxN = 4;
    unsigned long r = o.CalcCurrentBestTheoreticalResult();
    // 7 groups: (6 / 3) = 2 full, remainder 1; 3 groups: 0 full, remainder 3; 4 groups: 1 full, remainder 1
    bool ok = r == 100 + 2 + 3 && o.SegmentCalls == 1 && o.SeenGroupCount == 5 && o.SeenSizes.size() == 3
        && o.SeenSizes[0] == 1 && o.SeenSizes[1] == 3 && o.SeenSizes[2] == 1;
    if (!ok) { std::printf("00ad9d70_TEST FAIL\n"); return 1; }
    std::printf("00ad9d70_TEST PASS\n");
    return 0;
}
