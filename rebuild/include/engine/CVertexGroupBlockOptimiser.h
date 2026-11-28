#ifndef FABLE_ENGINE_CVERTEXGROUPBLOCKOPTIMISER_H
#define FABLE_ENGINE_CVERTEXGROUPBLOCKOPTIMISER_H
#ifdef FABLE_RETAIL_CVERTEXGROUPBLOCKOPTIMISER_PASS30
#include "engine/CVertexGroupBlock.h"
#include "engine/CArray.h"

// Ego_r original member names. Retail uses 12-byte CArray/vector and 4-byte list.
class CVertexGroupBlockList {
    CArray<CVertexGroupBlock> Blocks;
};

class CVertexGroupBlockOptimiser {
    CVertexGroupBlockList InputList;
    std::list<CVertexGroupBlock> OptimalBlockList;
    std::list<CVertexGroupBlock> Blocks;
    CVertexGroupBlock* CurrentBlock;
    std::vector<bool> RemainingBlocks;
    std::vector<CVertexGroupBlock> CurrentBlockSegmentGroups;
    std::vector<long> CurrentBlockSegmentIndices;
    unsigned long CurrentBlockSegmentCount;
    unsigned long MaxGroupIndex;
    unsigned long MaxN;
    unsigned long OptimalSize;
    unsigned long MaxIterations;
    bool Greedy;
    unsigned long Iterations;
    bool Debug;

    void CalcCurrentBlockSegments();
    unsigned long CalcMinimumBlocks(const CArray<unsigned long>&, unsigned long&, CArray<bool>&,
                                    unsigned long, unsigned long);
    unsigned long CalcCurrentBestTheoreticalResult();
};
#endif // FABLE_RETAIL_CVERTEXGROUPBLOCKOPTIMISER_PASS30
#endif
