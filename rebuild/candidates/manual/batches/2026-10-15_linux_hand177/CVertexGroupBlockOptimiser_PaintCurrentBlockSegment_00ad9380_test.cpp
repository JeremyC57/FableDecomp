#include <cstdio>
// Behaviour model of CVertexGroupBlockOptimiser::PaintCurrentBlockSegment: a flood fill over intersecting input blocks.
struct Block {
    unsigned int groups[16]; unsigned int count;
    bool Has(unsigned int g) const { for (unsigned int i = 0; i < count; ++i) if (groups[i] == g) return true; return false; }
    void AddGroup(unsigned int g) { groups[count++] = g; }
    void AddGroupsFrom(const Block& o) { for (unsigned int i = 0; i < o.count; ++i) if (!Has(o.groups[i])) AddGroup(o.groups[i]); }
    bool HasIntersection(const Block& o) const { for (unsigned int i = 0; i < o.count; ++i) if (Has(o.groups[i])) return true; return false; }
};
struct Opt {
    Block input[4]; bool remaining[4]; long indices[4]; unsigned int n; Block segment; long segCount;
    void Paint(unsigned int index)
    {
        indices[index] = segCount;
        segment.AddGroupsFrom(input[index]);
        for (unsigned int i = 0; i < n; ++i)
            if (remaining[i] && indices[i] == -1 && segment.HasIntersection(input[i]))
                Paint(i);
    }
};
int main()
{
    Opt o; o.n = 4; o.segCount = 7; o.segment.count = 0;
    for (int i = 0; i < 4; ++i) { o.input[i].count = 0; o.remaining[i] = true; o.indices[i] = -1; }
    o.input[0].AddGroup(1); o.input[0].AddGroup(2);
    o.input[1].AddGroup(5);
    o.input[2].AddGroup(2); o.input[2].AddGroup(3);
    o.input[3].AddGroup(3); o.input[3].AddGroup(4);
    o.Paint(0);
    if (o.indices[0] != 7 || o.indices[2] != 7 || o.indices[3] != 7 || o.indices[1] != -1 || o.segment.count != 4) {
        std::printf("00ad9380_TEST FAIL\n"); return 1;
    }
    std::printf("00ad9380_TEST PASS\n");
    return 0;
}
