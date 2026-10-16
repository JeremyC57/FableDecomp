#include <cstdio>
// Behaviour model of CVertexGroupBlockOptimiser::InsertBlock: the block leaves the remaining set and its new groups are
// merged into the current block and counted.
struct Block {
    unsigned int groups[16]; unsigned int count;
    bool Has(unsigned int g) const { for (unsigned int i = 0; i < count; ++i) if (groups[i] == g) return true; return false; }
    void AddGroup(unsigned int g) { groups[count++] = g; }
};
struct Opt {
    Block input[3]; bool remaining[3]; Block* current;
    void InsertBlock(unsigned int index, unsigned int* added)
    {
        remaining[index] = false;
        *added = 0;
        const Block& b = input[index];
        for (unsigned int i = 0; i < b.count; ++i) {
            unsigned int g = b.groups[i];
            if (!current->Has(g)) { current->AddGroup(g); ++*added; }
        }
    }
};
int main()
{
    Opt o; Block cur; cur.count = 0; cur.AddGroup(2);
    for (int i = 0; i < 3; ++i) { o.input[i].count = 0; o.remaining[i] = true; }
    o.input[1].AddGroup(2); o.input[1].AddGroup(3); o.input[1].AddGroup(4);
    o.current = &cur;
    unsigned int added = 99;
    o.InsertBlock(1, &added);
    if (added != 2 || cur.count != 3 || o.remaining[1] || !o.remaining[0]) { std::printf("00ad92e0_TEST FAIL\n"); return 1; }
    std::printf("00ad92e0_TEST PASS\n");
    return 0;
}
