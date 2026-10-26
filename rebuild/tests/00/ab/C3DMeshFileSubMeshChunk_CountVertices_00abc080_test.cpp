// Behaviour test for 0x00ABC080 C3DMeshFileSubMeshChunk::CountVertices (model copy over stub chunk finders).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> class CCountedPointer { public:
    T* Object; SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->references; }
    ~CCountedPointer() { if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } } Object = 0; Count = 0; }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        SCountBlock* c = o.Count; T* p = o.Object;
        if (Count != c) {
            if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
            Object = p; Count = c; if (c) ++c->references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
template <class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
class C3DVertex2 { public: unsigned char _body[0x20]; };
class C3DMeshFileVerticesChunk { public: CArray<C3DVertex2> VertexList; };

static C3DMeshFileVerticesChunk g_vertices[2]; static SCountBlock g_vertexBlocks[2];
class C3DMeshFilePrimitiveChunk { public:
    int Index;
    CCountedPointer<const C3DMeshFileVerticesChunk> PeekVertices() const
    {
        CCountedPointer<const C3DMeshFileVerticesChunk> p; p.Object = &g_vertices[Index]; p.Count = &g_vertexBlocks[Index]; ++p.Count->references;
        return p;
    }
};
static C3DMeshFilePrimitiveChunk g_prims[2]; static SCountBlock g_primBlocks[2];

class C3DMeshFileSubMeshChunk { public:
    CCountedPointer<const C3DMeshFilePrimitiveChunk> Make(int i) const
    {
        CCountedPointer<const C3DMeshFilePrimitiveChunk> p;
        if (i < 2) { p.Object = &g_prims[i]; p.Count = &g_primBlocks[i]; ++p.Count->references; }
        return p;
    }
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindFirstPrimitive() const { return Make(0); }
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& current) const { return Make(current->Index + 1); }
    unsigned long CountVertices() const;
};

unsigned long C3DMeshFileSubMeshChunk::CountVertices() const
{
    unsigned long count = 0;
    for (CCountedPointer<const C3DMeshFilePrimitiveChunk> primitive = FindFirstPrimitive(); primitive.Object; primitive = FindNextPrimitive(primitive))
        count += primitive->PeekVertices()->VertexList.size();
    return count;
}

static void __fastcall Destroy(void*) {}

int main()
{
    static C3DVertex2 verts[8];
    g_vertices[0].VertexList._M_start = verts; g_vertices[0].VertexList._M_finish = verts + 3;
    g_vertices[1].VertexList._M_start = verts + 3; g_vertices[1].VertexList._M_finish = verts + 8;
    for (int i = 0; i < 2; ++i) {
        g_prims[i].Index = i;
        g_primBlocks[i].references = 1; g_primBlocks[i].destroy = Destroy; g_primBlocks[i].object = &g_prims[i];
        g_vertexBlocks[i].references = 1; g_vertexBlocks[i].destroy = Destroy; g_vertexBlocks[i].object = &g_vertices[i];
    }
    C3DMeshFileSubMeshChunk sub;
    if (sub.CountVertices() != 8) { std::printf("00abc080_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 2; ++i)
        if (g_primBlocks[i].references != 1 || g_vertexBlocks[i].references != 1) { std::printf("00abc080_TEST FAIL\n"); return 1; }
    std::printf("00abc080_TEST PASS\n");
    return 0;
}