// Behaviour test for 0x00AD9120 CVertexGroupBlockOptimiser::CountRemainingCurrentBlockSegments.
#include <cstdio>

// tiny ordered set of longs (stand-in for the STLport set<long>)
struct SLongSet {
    long Items[64]; unsigned int Count;
    SLongSet() : Count(0) { }
    void insert(long v) { for (unsigned int i = 0; i < Count; ++i) if (Items[i] == v) return; Items[Count++] = v; }
    unsigned int size() const { return Count; }
};

struct COpt {
    const bool* RemainingBlocks; const long* IndicesStart; const long* IndicesFinish; unsigned int CurrentBlockSegmentCount;
    unsigned int IndicesSize() const { return (unsigned int)(IndicesFinish - IndicesStart); }
    unsigned int CountRemainingCurrentBlockSegments() const
    {
        if (CurrentBlockSegmentCount == 1) return 1;
        SLongSet segments;
        for (unsigned int i = 0; i < IndicesSize(); ++i)
            if (RemainingBlocks[i]) segments.insert(IndicesStart[i]);
        return segments.size();
    }
};

int main()
{
    bool remaining[8] = { true, false, true, true, false, true, true, false };
    long indices[8] = { 0, 1, 0, 2, 3, 2, 4, 5 };
    COpt o; o.RemainingBlocks = remaining; o.IndicesStart = indices; o.IndicesFinish = indices + 8; o.CurrentBlockSegmentCount = 6;
    if (o.CountRemainingCurrentBlockSegments() != 3) { std::printf("00ad9120_TEST FAIL\n"); return 1; }   // {0, 2, 4}
    o.CurrentBlockSegmentCount = 1;
    if (o.CountRemainingCurrentBlockSegments() != 1) { std::printf("00ad9120_TEST FAIL\n"); return 1; }
    o.CurrentBlockSegmentCount = 0; o.IndicesFinish = indices;
    if (o.CountRemainingCurrentBlockSegments() != 0) { std::printf("00ad9120_TEST FAIL\n"); return 1; }
    std::printf("00ad9120_TEST PASS\n");
    return 0;
}