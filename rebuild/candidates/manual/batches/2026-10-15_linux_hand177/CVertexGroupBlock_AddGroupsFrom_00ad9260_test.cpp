#include <cstdio>
// Behaviour model of CVertexGroupBlock::AddGroupsFrom: groups of `other` missing from this block's group->index map are
// added (in order, once each). The map is modelled as a small array searched linearly.
struct Block {
    unsigned int groups[16]; unsigned int count;
    bool Has(unsigned int g) const { for (unsigned int i = 0; i < count; ++i) if (groups[i] == g) return true; return false; }
    void AddGroup(unsigned int g) { groups[count++] = g; }
    void AddGroupsFrom(const Block& other)
    {
        for (unsigned int i = 0; i < other.count; ++i) {
            unsigned int g = other.groups[i];
            if (!Has(g)) AddGroup(g);
        }
    }
};
int main()
{
    Block a; a.count = 0; a.AddGroup(1); a.AddGroup(5);
    Block b; b.count = 0; b.AddGroup(5); b.AddGroup(7); b.AddGroup(1); b.AddGroup(9);
    a.AddGroupsFrom(b);
    if (a.count != 4 || a.groups[2] != 7 || a.groups[3] != 9) { std::printf("00ad9260_TEST FAIL\n"); return 1; }
    a.AddGroupsFrom(a);
    if (a.count != 4) { std::printf("00ad9260_TEST FAIL\n"); return 1; }
    std::printf("00ad9260_TEST PASS\n");
    return 0;
}
