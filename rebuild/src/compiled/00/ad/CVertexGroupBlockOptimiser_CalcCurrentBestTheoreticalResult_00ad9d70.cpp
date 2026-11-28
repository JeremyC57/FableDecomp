// 00AD9D70 CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult
// Leaf: ?CalcCurrentBestTheoreticalResult@CVertexGroupBlockOptimiser@@AAEKXZ
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26
#define FABLE_RETAIL_VERTEX_BLOCK_BOOL_FILL_PASS30
#define FABLE_RETAIL_CVERTEXGROUPBLOCKOPTIMISER_PASS30
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CTHREADEDFILE_ULONG_OVERFLOW_PASS30
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27
#include "fable_compiler.h"
#include "engine/CVertexGroupBlockOptimiser.h"

unsigned long CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult()
{
    CalcCurrentBlockSegments();
    unsigned long result = Blocks.size();
    CArray<unsigned long> remainders;
    remainders.reserve(CurrentBlockSegmentCount);
    for (unsigned long i = 0; i < CurrentBlockSegmentCount; ++i) {
        unsigned long groupCount = CurrentBlockSegmentGroups[i].GetGroupCount();
        unsigned long fullBlocks = (groupCount - 1) / (MaxN - 1);
        result += fullBlocks;
        unsigned long remainder = groupCount - fullBlocks * (MaxN - 1);
        if (remainder)
            remainders.push_back(remainder);
    }
    unsigned long groupCount = CurrentBlock->GetGroupCount();
    CArray<bool> available(remainders.size(), true);
    return CalcMinimumBlocks(remainders, groupCount, available, remainders.size(), 0) + result;
}
