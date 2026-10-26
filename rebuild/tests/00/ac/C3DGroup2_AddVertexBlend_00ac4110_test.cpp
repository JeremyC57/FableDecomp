// Behaviour test for C3DGroup2::AddVertexBlend(const C3DVertexBlend2&) @ 0x00AC4110
// Appends one {vertex index, blend} pair to VertexBlends, growing storage when full.
#include <cstdio>
#include <cstdlib>
#include <cstring>

class C3DVertexBlend2 { public: unsigned int VertexIndex; float Blend; };

struct CBlendList {
    C3DVertexBlend2* start;
    C3DVertexBlend2* finish;
    C3DVertexBlend2* end;
    CBlendList() : start(0), finish(0), end(0) { }
    void insert_overflow(const C3DVertexBlend2& x)
    {
        unsigned int size = (unsigned int)(finish - start);
        unsigned int len = size + (size > 1 ? size : 1);
        C3DVertexBlend2* fresh = (C3DVertexBlend2*)std::malloc(len * sizeof(C3DVertexBlend2));
        if (size) std::memcpy(fresh, start, size * sizeof(C3DVertexBlend2));
        fresh[size] = x;
        std::free(start);
        start = fresh; finish = fresh + size + 1; end = fresh + len;
    }
    void push_back(const C3DVertexBlend2& x)
    {
        if (finish != end) { *finish = x; ++finish; }
        else insert_overflow(x);
    }
};

struct C3DGroup2Model {
    int BoneIndex;
    CBlendList VertexBlends;
    void AddVertexBlend(const C3DVertexBlend2& blend) { VertexBlends.push_back(blend); }
};

int main()
{
    C3DGroup2Model group;
    group.BoneIndex = 2;
    for (unsigned int i = 0; i < 6; ++i) {
        C3DVertexBlend2 b;
        b.VertexIndex = i * 10;
        b.Blend = 0.25f * (float)i;
        group.AddVertexBlend(b);
    }
    int ok = group.VertexBlends.finish - group.VertexBlends.start == 6 && group.BoneIndex == 2;
    for (unsigned int i = 0; i < 6 && ok; ++i) {
        float d = group.VertexBlends.start[i].Blend - 0.25f * (float)i;
        if (group.VertexBlends.start[i].VertexIndex != i * 10 || d > 0.0001f || d < -0.0001f) ok = 0;
    }
    std::free(group.VertexBlends.start);
    if (!ok) { std::printf("00ac4110_TEST FAIL\n"); return 1; }
    std::printf("00ac4110_TEST PASS\n");
    return 0;
}