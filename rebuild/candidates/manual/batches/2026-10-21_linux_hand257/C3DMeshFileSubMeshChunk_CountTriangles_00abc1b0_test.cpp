// Behaviour test for C3DMeshFileSubMeshChunk::CountTriangles (0x00ABC1B0): nested counted-pointer walks over
// primitives and their TRIS chunks, with the reference counts back to their start values afterwards.
#include <cstdio>
struct SCountBlock { long references; };
template<class T> struct CCountedPointer {
    T* Object; SCountBlock* Count;
    CCountedPointer(T* o, SCountBlock* c) : Object(o), Count(c) { if (Count) ++Count->references; }
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->references; }
    ~CCountedPointer() { if (Count) --Count->references; Object = 0; Count = 0; }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        SCountBlock* c = o.Count; T* p = o.Object;
        if (Count != c) { if (Count) --Count->references; Object = p; Count = c; if (c) ++c->references; }
        return *this;
    }
    T* operator->() const { return Object; }
};
struct CTris { unsigned long n; SCountBlock block; unsigned long size() const { return n; } };
struct CPrim {
    CTris tris[3]; int ntris; SCountBlock block;
    CCountedPointer<CTris> Find(int i) const { return i < ntris ? CCountedPointer<CTris>((CTris*)&tris[i], (SCountBlock*)&tris[i].block) : CCountedPointer<CTris>(0, 0); }
    CCountedPointer<CTris> FindFirstTriangles() const { return Find(0); }
    CCountedPointer<CTris> FindNextTriangles(const CCountedPointer<CTris>& cur) const { return Find((int)(cur.Object - tris) + 1); }
};
struct CSubMesh {
    CPrim prims[2]; int nprims;
    CCountedPointer<CPrim> Find(int i) const { return i < nprims ? CCountedPointer<CPrim>((CPrim*)&prims[i], (SCountBlock*)&prims[i].block) : CCountedPointer<CPrim>(0, 0); }
    CCountedPointer<CPrim> FindFirstPrimitive() const { return Find(0); }
    CCountedPointer<CPrim> FindNextPrimitive(const CCountedPointer<CPrim>& cur) const { return Find((int)(cur.Object - prims) + 1); }
    unsigned long CountTriangles() const
    {
        unsigned long count = 0;
        CCountedPointer<CPrim> primitive = FindFirstPrimitive();
        while (primitive.Object) {
            CCountedPointer<CTris> triangles = primitive->FindFirstTriangles();
            while (triangles.Object) {
                count += triangles->size();
                triangles = primitive->FindNextTriangles(triangles);
            }
            primitive = FindNextPrimitive(primitive);
        }
        return count;
    }
};
int main()
{
    CSubMesh m;
    m.nprims = 2;
    m.prims[0].ntris = 2; m.prims[0].tris[0].n = 10; m.prims[0].tris[1].n = 5;
    m.prims[1].ntris = 1; m.prims[1].tris[0].n = 7;
    for (int p = 0; p < 2; ++p) { m.prims[p].block.references = 1; for (int t = 0; t < 3; ++t) m.prims[p].tris[t].block.references = 1; }
    unsigned long n = m.CountTriangles();
    bool balanced = m.prims[0].block.references == 1 && m.prims[1].block.references == 1 &&
        m.prims[0].tris[1].block.references == 1 && m.prims[1].tris[0].block.references == 1;
    CSubMesh empty; empty.nprims = 0;
    if (n != 22 || !balanced || empty.CountTriangles() != 0) { std::printf("00abc1b0_TEST FAIL\n"); return 1; }
    std::printf("00abc1b0_TEST PASS\n");
    return 0;
}
