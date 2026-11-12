// Behaviour test for CVertexGroupBlockList::RemoveAllSubsets (0x00AD9770): every block that is a proper subset of another block
// is dropped; the others keep their order (equal blocks are not proper subsets of each other, so duplicates stay).
#include <cstdio>
#include <vector>
#include <set>
struct Block {
    std::vector<unsigned long> Groups; std::set<unsigned long> Index;
    void Add(unsigned long g) { Groups.push_back(g); Index.insert(g); }
    bool IsProperSubsetOf(const Block& other) const
    {
        unsigned long count = (unsigned long)Groups.size();
        if (count < other.Groups.size()) {
            for (unsigned long i = 0; i < count; ++i)
                if (other.Index.find(Groups[i]) == other.Index.end()) return false;
            return true;
        }
        return false;
    }
};
struct List {
    std::vector<Block> Blocks;
    unsigned long GetBlockCount() const { return (unsigned long)Blocks.size(); }
    void RemoveAllSubsets()
    {
        std::vector<Block> new_blocks;
        for (unsigned long i = 0; i < GetBlockCount(); ++i) {
            const Block& block = Blocks[i];
            bool is_subset = false;
            unsigned long count = GetBlockCount();
            for (unsigned long j = 0; j < count; ++j)
                if (block.IsProperSubsetOf(Blocks[j])) { is_subset = true; break; }
            if (!is_subset) new_blocks.push_back(block);
        }
        Blocks = new_blocks;
    }
};
static Block Make(unsigned long a, unsigned long b = 0, unsigned long c = 0)
{ Block k; k.Add(a); if (b) k.Add(b); if (c) k.Add(c); return k; }
int main()
{
    List l;
    l.Blocks.push_back(Make(1, 2));          // subset of {1,2,3}: dropped
    l.Blocks.push_back(Make(1, 2, 3));       // kept
    l.Blocks.push_back(Make(4));             // kept (nothing contains it)
    l.Blocks.push_back(Make(3));             // subset of {1,2,3}: dropped
    l.Blocks.push_back(Make(4));             // equal to an earlier block, not a proper subset: kept
    l.Blocks.push_back(Make(5, 6));          // kept
    l.RemoveAllSubsets();
    bool ok = l.Blocks.size() == 4
           && l.Blocks[0].Groups.size() == 3 && l.Blocks[0].Groups[2] == 3
           && l.Blocks[1].Groups.size() == 1 && l.Blocks[1].Groups[0] == 4
           && l.Blocks[2].Groups.size() == 1 && l.Blocks[2].Groups[0] == 4
           && l.Blocks[3].Groups.size() == 2 && l.Blocks[3].Groups[1] == 6;
    List e; e.RemoveAllSubsets(); ok = ok && e.Blocks.empty();
    if (!ok) { std::printf("00ad9770_TEST FAIL\n"); return 1; }
    std::printf("00ad9770_TEST PASS\n");
    return 0;
}