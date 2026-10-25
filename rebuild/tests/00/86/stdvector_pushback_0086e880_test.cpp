// Behaviour test for 0x0086E880 vector<CBodyPartMesh>::push_back: the mesh index, priority, texture-morph map and Dirty flag are copied.
#include <cstdio>
#include <new>
struct TexMorph { unsigned int key, NewTexture, GroupID; };
struct MorphMap {
    TexMorph* start; TexMorph* finish; TexMorph* eos; unsigned char Compare; bool Dirty;
    MorphMap() : start(0), finish(0), eos(0), Dirty(false) {}
    MorphMap(const MorphMap& o) : Dirty(o.Dirty)
    {
        unsigned int n = o.finish - o.start;
        start = n ? (TexMorph*)::operator new(n * sizeof(TexMorph)) : 0;
        for (unsigned int i = 0; i < n; ++i) start[i] = o.start[i];
        finish = start + n; eos = finish;
    }
    ~MorphMap() { ::operator delete(start); }
};
struct CBodyPartMesh { long MeshIndex; long Priority; MorphMap TextureMorphs; };
struct Vec {
    CBodyPartMesh* start; CBodyPartMesh* finish; CBodyPartMesh* eos;
    void push_back(const CBodyPartMesh& x) { if (finish != eos) { new (finish) CBodyPartMesh(x); ++finish; } }
};
int main()
{
    CBodyPartMesh* storage = (CBodyPartMesh*)::operator new(2 * sizeof(CBodyPartMesh));
    Vec v; v.start = v.finish = storage; v.eos = storage + 2;
    CBodyPartMesh m; m.MeshIndex = 4; m.Priority = 2; m.TextureMorphs.Dirty = true;
    TexMorph t = { 1, 2, 3 };
    m.TextureMorphs.start = (TexMorph*)::operator new(sizeof(TexMorph)); m.TextureMorphs.start[0] = t; m.TextureMorphs.finish = m.TextureMorphs.eos = m.TextureMorphs.start + 1;
    v.push_back(m);
    CBodyPartMesh& c = storage[0];
    if (v.finish != storage + 1 || c.MeshIndex != 4 || c.Priority != 2 || !c.TextureMorphs.Dirty || c.TextureMorphs.finish - c.TextureMorphs.start != 1 ||
        c.TextureMorphs.start == m.TextureMorphs.start || c.TextureMorphs.start[0].GroupID != 3) {
        std::printf("0086e880_TEST FAIL\n"); return 1;
    }
    c.~CBodyPartMesh(); ::operator delete(storage);
    std::printf("0086e880_TEST PASS\n");
    return 0;
}