// C3DMeshFileSubMeshChunk::CountVertices() const -- total vertex count over the sub mesh's primitives (each primitive's VERT chunk list size)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC080, 294 bytes (`ret`, unsigned long in eax).
// Ego_r.pdb: ?CountVertices@C3DMeshFileSubMeshChunk@@QBEKXZ.
// Callees: C3DMeshFileSubMeshChunk::FindFirstPrimitive() const 0x00ABA980 and FindNextPrimitive(const
// CCountedPointer<const C3DMeshFilePrimitiveChunk>&) const 0x00ABAA10, C3DMeshFilePrimitiveChunk::PeekVertices() const
// 0x00ABAFE0 (all three labelled "C3DMeshFileSubMeshChunk::FindFirstBone/FindNextBone" in the manifest: propagated),
// operator delete 0x00BFE9BC. Same TU: stand-in bodies in the shape of their retail code (FindFirstSubChunkOfType
// 0x00AA7F40 with "PRIM" / "VERT", FindNextSubChunk 0x00AA7FF0) sit above under auto_inline(off), as in the landed
// GetMap 0x00AC1160.
// Shape: `for (p = FindFirstPrimitive(); p; p = FindNextPrimitive(p)) count += p->PeekVertices()->VertexList.size();`
// with the zeroing counted-pointer dtor (its zero stores scheduled into the next call's argument setup).
// Layout: Ego_r.pdb C3DMeshFileVerticesChunk VertexList +0x10 (CArray<C3DVertex2>, 32-byte elements, retail 12-byte vector).
// Leaf: CountVertices@C3DMeshFileSubMeshChunk
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> struct CCountedPointer {
    T* Object; SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->references; }
    template<class U> CCountedPointer(const CCountedPointer<U>& o) : Object((T*)o.Object), Count(o.Count) { if (Count) ++Count->references; }
    ~CCountedPointer() { if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } } Object = 0; Count = 0; }
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& o, CastTag) : Object(0), Count(0) { if (o.Count) { Object = (T*)o.Object; Count = o.Count; ++Count->references; } }
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

template <class T> struct CArray {                 // retail STLport vector layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct C3DVertex2 { unsigned char _body[0x20]; };

struct CChunkedFileChunk;
struct CChunkedFileSuperChunk {
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;   // 0x00AA7F40
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& current) const;   // 0x00AA7FF0
};

struct C3DMeshFileVerticesChunk {
    unsigned char _base_body[0x10];        // CChunkedFileChunk
    CArray<C3DVertex2> VertexList;         // +0x10
};

struct C3DMeshFilePrimitiveChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DMeshFileVerticesChunk> PeekVertices() const;   // 0x00ABAFE0
};

struct C3DMeshFileSubMeshChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindFirstPrimitive() const;   // 0x00ABA980
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& current) const;   // 0x00ABAA10
    unsigned long CountVertices() const;
};

#pragma auto_inline(off)
CCountedPointer<const C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindFirstPrimitive() const
{
    return CCountedPointer<const C3DMeshFilePrimitiveChunk>(FindFirstSubChunkOfType("PRIM"), CCountedPointer<const C3DMeshFilePrimitiveChunk>::CastTag());
}
CCountedPointer<const C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& current) const
{
    CCountedPointer<CChunkedFileChunk> prev(current);
    return CCountedPointer<const C3DMeshFilePrimitiveChunk>(FindNextSubChunk(prev), CCountedPointer<const C3DMeshFilePrimitiveChunk>::CastTag());
}
CCountedPointer<const C3DMeshFileVerticesChunk> C3DMeshFilePrimitiveChunk::PeekVertices() const
{
    return CCountedPointer<const C3DMeshFileVerticesChunk>(FindFirstSubChunkOfType("VERT"), CCountedPointer<const C3DMeshFileVerticesChunk>::CastTag());
}
#pragma auto_inline(on)

unsigned long C3DMeshFileSubMeshChunk::CountVertices() const
{
    unsigned long count = 0;
    for (CCountedPointer<const C3DMeshFilePrimitiveChunk> primitive = FindFirstPrimitive(); primitive.Object; primitive = FindNextPrimitive(primitive))
        count += primitive->PeekVertices()->VertexList.size();
    return count;
}
