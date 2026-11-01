// Test for CVertexGroupBlockOptimiser::CountRemainingGroups (0x00AD8ED0): mark every group used by any input block in a
// bool vector of MaxGroupIndex+1, then count the marked groups the current block does not already hold.
#include <cstdio>
struct Block { unsigned long Groups[4]; unsigned long Count; bool HasGroup(unsigned long g) const { for (unsigned long i = 0; i < Count; ++i) if (Groups[i] == g) return true; return false; } };
struct Optimiser {
    Block Input[3]; unsigned long InputCount; const Block* CurrentBlock; unsigned long MaxGroupIndex;
    unsigned long CountRemainingGroups() const
    {
        bool used[16];
        for (unsigned long i = 0; i <= MaxGroupIndex; ++i) used[i] = false;
        for (unsigned long b = 0; b < InputCount; ++b) {
            const Block& block = Input[b];
            for (unsigned long n = 0; n < block.Count; ++n) used[block.Groups[n]] = true;
        }
        unsigned long count = 0;
        for (unsigned long i = 0; i <= MaxGroupIndex; ++i) if (used[i] && !CurrentBlock->HasGroup(i)) ++count;
        return count;
    }
};
int main()
{
    Optimiser o; o.InputCount = 2; o.MaxGroupIndex = 9;
    o.Input[0].Count = 3; o.Input[0].Groups[0] = 1; o.Input[0].Groups[1] = 4; o.Input[0].Groups[2] = 9;
    o.Input[1].Count = 2; o.Input[1].Groups[0] = 4; o.Input[1].Groups[1] = 6;
    Block current; current.Count = 2; current.Groups[0] = 4; current.Groups[1] = 7;
    o.CurrentBlock = &current;
    bool ok = o.CountRemainingGroups() == 3;          // used {1,4,6,9}; current holds 4 -> 1, 6, 9 remain
    current.Count = 0;
    ok = ok && o.CountRemainingGroups() == 4;
    std::printf(ok ? "00ad8ed0_TEST PASS\n" : "00ad8ed0_TEST FAIL\n");
    return ok ? 0 : 1;
}