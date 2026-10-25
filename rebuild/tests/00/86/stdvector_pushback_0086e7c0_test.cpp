// Behaviour test for 0x0086E7C0 vector<CMeshTextureSet>::push_back(x): with room x is copy-constructed at finish; when full the block grows to old_size + max(old_size, 1), the old sets are copied over, x appended, the old sets cleared and the old block freed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct Morph { long OriginalTexture, NewTexture; };
struct MeshSet {
    long MeshIndex; Morph* start; Morph* finish; Morph* eos;
    MeshSet(long m) : MeshIndex(m), start(0), finish(0), eos(0) { }
    MeshSet(const MeshSet& o) : MeshIndex(o.MeshIndex)
    {
        unsigned int n = (unsigned int)(o.finish - o.start);
        start = n ? (Morph*)std::malloc(n * sizeof(Morph)) : 0;
        if (n) std::memcpy(start, o.start, n * sizeof(Morph));
        finish = start + n; eos = finish;
    }
};
struct Vec {
    MeshSet* start; MeshSet* finish; MeshSet* eos;
    void clear() { for (MeshSet* p = start; p != finish; ++p) if (p->start) std::free(p->start); if (start) std::free(start); }
    void push_back(const MeshSet& x)
    {
        if (finish != eos) { new (finish) MeshSet(x); ++finish; return; }
        unsigned int old_size = (unsigned int)(finish - start);
        unsigned int len = old_size + (old_size < 1 ? 1 : old_size);
        MeshSet* ns = len ? (MeshSet*)std::malloc(len * sizeof(MeshSet)) : 0;
        MeshSet* nf = ns;
        for (MeshSet* p = start; p != finish; ++p, ++nf) new (nf) MeshSet(*p);
        new (nf) MeshSet(x); ++nf;
        clear();
        start = ns; finish = nf; eos = ns + len;
    }
};
static int fail() { std::printf("0086e7c0_TEST FAIL\n"); return 1; }
int main()
{
    Vec v; v.start = 0; v.finish = 0; v.eos = 0;
    MeshSet a(1), b(2), c(3);
    a.start = (Morph*)std::malloc(sizeof(Morph)); a.finish = a.start + 1; a.eos = a.finish; a.start[0].NewTexture = 42;
    v.push_back(a);                                    // 0 -> 1
    v.push_back(b);                                    // 1 -> 2
    v.push_back(c);                                    // 2 -> 4
    if (v.finish - v.start != 3 || v.eos - v.start != 4 || v.start[0].MeshIndex != 1 || v.start[2].MeshIndex != 3 ||
        v.start[0].start == a.start || v.start[0].start[0].NewTexture != 42) return fail();
    v.push_back(a);                                    // room: no regrowth
    if (v.finish - v.start != 4 || v.eos - v.start != 4 || v.start[3].MeshIndex != 1) return fail();
    v.clear(); std::free(a.start);
    std::printf("0086e7c0_TEST PASS\n");
    return 0;
}