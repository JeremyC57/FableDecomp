// 00AD9D70 CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult
// Leaf: ?CalcCurrentBestTheoreticalResult@CVertexGroupBlockOptimiser@@AAEKXZ
#define FABLE_RETAIL_CVERTEXGROUPBLOCKOPTIMISER_PASS30
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CTHREADEDFILE_ULONG_OVERFLOW_PASS30
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#include "fable_compiler.h"
#include "engine/CVertexGroupBlockOptimiser.h"

#include <stdio.h>
#include <string.h>

#ifndef FABLE_TEST_LINK_PRODUCTION
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
#endif

namespace {
    struct Method {
        typedef unsigned long (CVertexGroupBlockOptimiser::*Type)();
        friend Type Select(Method);
    };
    template <class Key, typename Key::Type Value> struct Access {
        friend typename Key::Type Select(Key) { return Value; }
    };
    template struct Access<Method, &CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult>;

    union { double align; unsigned char bytes[sizeof(CVertexGroupBlockOptimiser)]; } optimizer_storage;
    union { double align; unsigned char bytes[4 * sizeof(CVertexGroupBlock)]; } block_storage;
    std::_List_node_base sentinel, nodes[3];
    unsigned long values[12];
    unsigned long sizes[] = {7,3,4};
    unsigned long expected_minimum, block_count, segment_count, max_groups, segment_calls;
    bool valid;
}

void CVertexGroupBlockOptimiser::CalcCurrentBlockSegments()
{
    ++segment_calls;
    sentinel._M_next = block_count ? nodes : &sentinel;
    sentinel._M_prev = block_count ? nodes + block_count - 1 : &sentinel;
    for (unsigned long i = 0; i < block_count; ++i) {
        nodes[i]._M_next = i + 1 < block_count ? nodes + i + 1 : &sentinel;
        nodes[i]._M_prev = i ? nodes + i - 1 : &sentinel;
    }
    std::_List_node_base* node = &sentinel;
    memcpy(&Blocks, &node, sizeof node);
    CVertexGroupBlock* blocks = reinterpret_cast<CVertexGroupBlock*>(block_storage.bytes);
    for (unsigned long i = 0; i < 4; ++i) {
        unsigned long n = i < 3 ? sizes[i] : 6;
        unsigned long* pointers[] = {values, values+n, values+n};
        memcpy(blocks+i, pointers, sizeof pointers);
    }
    CVertexGroupBlock* pointers[] = {blocks, blocks+segment_count, blocks+segment_count};
    memcpy(&CurrentBlockSegmentGroups, pointers, sizeof pointers);
    CurrentBlock = blocks+3;
    MaxN = max_groups;
    CurrentBlockSegmentCount = segment_count;
}

unsigned long CVertexGroupBlockOptimiser::CalcMinimumBlocks(const CArray<unsigned long>& remainders,
    unsigned long& groups,CArray<bool>& available,unsigned long count,unsigned long start)
{
    valid = valid && segment_calls == 1 && groups == 6 && count == segment_count && start == 0;
    valid = valid && remainders.size() == segment_count && available.size() == segment_count;
    for (unsigned long i = 0; i < count && i < remainders.size() && i < available.size(); ++i)
        valid = valid && remainders[i] == 1+(sizes[i]-1)%(max_groups-1) && available[i];
    return expected_minimum;
}

int main()
{
    CVertexGroupBlockOptimiser* opt = reinterpret_cast<CVertexGroupBlockOptimiser*>(optimizer_storage.bytes);
    for (max_groups = 2; max_groups != 12; ++max_groups) {
        for (block_count = 0; block_count != 4; ++block_count) {
            for (segment_count = 0; segment_count != 4; ++segment_count) {
                segment_calls = 0;
                expected_minimum = (max_groups+block_count)%4;
                valid = true;
                unsigned long result = expected_minimum+block_count;
                for (unsigned long i = 0; i < segment_count; ++i)
                    result += (sizes[i]-1)/(max_groups-1);
                unsigned long observed = (opt->*Select(Method()))();
                if (!valid || observed != result)
                    return 1;
            }
        }
    }
    puts("00ad9d70_TEST PASS");
    return 0;
}
