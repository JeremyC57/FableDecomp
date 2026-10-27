// Behaviour test for 0x00AD83D0 std::vector<COptimisedSubMesh>::_M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0, g_dtors = 0;
struct SubMesh {                     // stands in for { vector<CCountedPointer<COptimisedPrimitive> >; vector<COptimisedBone>; }
    int primitives, bones;
    SubMesh(int p, int b) : primitives(p), bones(b) { }
    SubMesh(const SubMesh& o) : primitives(o.primitives), bones(o.bones) { ++g_copies; }
    ~SubMesh() { ++g_dtors; }
};

struct Vec {
    SubMesh* start; SubMesh* finish; SubMesh* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(SubMesh* pos, const SubMesh& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        SubMesh* ns = len ? (SubMesh*)std::malloc(len * sizeof(SubMesh)) : 0;
        SubMesh* nf = ns;
        for (SubMesh* p = start; p != pos; ++p, ++nf) new (nf) SubMesh(*p);
        if (fill_len == 1) { new (nf) SubMesh(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) SubMesh(x);
        if (!atend) for (SubMesh* p = pos; p != finish; ++p, ++nf) new (nf) SubMesh(*p);
        for (SubMesh* p = start; p != finish; ++p) p->~SubMesh();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    SubMesh a(1, 2), b(3, 4);
    v.insert_overflow(v.finish, a, 1, true);
    v.insert_overflow(v.finish, b, 1, true);
    if (v.size() != 2 || v.eos - v.start != 2 || v.start[1].bones != 4 || g_dtors != 1) { std::printf("00ad83d0_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, a, 3, false);      // len = 2 + max(2, 3) = 5
    if (v.size() != 5 || v.eos - v.start != 5 || v.start[0].primitives != 1 || v.start[3].primitives != 1 || v.start[4].bones != 4) { std::printf("00ad83d0_TEST FAIL\n"); return 1; }
    if (g_copies != 1 + 2 + 5) { std::printf("00ad83d0_TEST FAIL\n"); return 1; }
    for (SubMesh* p = v.start; p != v.finish; ++p) p->~SubMesh();
    std::free(v.start);
    std::printf("00ad83d0_TEST PASS\n");
    return 0;
}