// Behaviour test for CVertexGroupBlockOptimiser::CanInsertMoreBlocks (0x00AD8DD0), g++ model of the retail logic.
#include <cstdio>
#include <set>
#include <vector>
struct Block { std::vector<unsigned long> groups; };
static bool CanInsertMoreBlocks(const std::vector<Block>& input, const std::vector<bool>& remaining, const std::set<unsigned long>& current,
                                unsigned long currentCount, unsigned long maxN)
{
    unsigned long space = maxN - currentCount;
    for (unsigned long i = 0; i < input.size(); ++i) {
        if (remaining[i]) {
            unsigned long fresh = 0;
            for (unsigned long j = 0; j < input[i].groups.size(); ++j)
                if (!current.count(input[i].groups[j])) ++fresh;
            if (fresh <= space) return true;
        }
    }
    return false;
}
int main()
{
    std::set<unsigned long> cur; cur.insert(1); cur.insert(2);
    std::vector<Block> in(2);
    in[0].groups.push_back(3); in[0].groups.push_back(4); in[0].groups.push_back(5);
    in[1].groups.push_back(1); in[1].groups.push_back(6);
    std::vector<bool> rem(2, true);
    int ok = CanInsertMoreBlocks(in, rem, cur, 2, 3);            // block 1 adds one group, room 1
    rem[1] = false;
    ok &= !CanInsertMoreBlocks(in, rem, cur, 2, 3);              // block 0 adds three, room 1
    ok &= CanInsertMoreBlocks(in, rem, cur, 2, 5);               // room 3
    rem[0] = false;
    ok &= !CanInsertMoreBlocks(in, rem, cur, 2, 8);
    std::printf(ok ? "00ad8dd0_TEST PASS\n" : "00ad8dd0_TEST FAIL\n");
    return ok ? 0 : 1;
}