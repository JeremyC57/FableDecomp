// C3DMeshFileSubMeshChunk::CountTriangles() const -- total triangle count over every TRIS chunk of every primitive of the sub mesh
// Leaf: CountTriangles@C3DMeshFileSubMeshChunk
// Retail @ 0x00ABC1B0, 438 bytes (`ret`, unsigned long in eax; 8-byte aligned frame).
// Callees: C3DMeshFileSubMeshChunk::FindFirstPrimitive() const 0x00ABA980 / FindNextPrimitive(const CCountedPointer&)
//   const 0x00ABAA10, C3DMeshFilePrimitiveChunk::FindFirstTriangles() const 0x00ABB300 ("TRIS") / FindNextTriangles(const
//   CCountedPointer&) const 0x00ABB390 (all four labelled FindFirstBone/FindNextBone in the manifest: propagated),
//   CChunkedFileSuperChunk::FindFirstSubChunkOfType 0x00AA7F40 / FindNextSubChunk 0x00AA7FF0 (called by the stand-ins),
//   operator delete 0x00BFE9BC.
// Shape: two nested counted-pointer for-loops; each pass adds TriangleList.size() (6-byte C3DTriangle2: the
// `imul 2AAAAAABh` divide by 6). The four finders are same-TU callees: stand-in bodies sit above under
// auto_inline(off) as in the landed sibling CountVertices 0x00ABC080, so the inner pointer stays in edi/ebx.
// Layout: Ego_r.pdb C3DMeshFileTrianglesChunk TriangleList +0x34 -> retail +0x2c (C3DMeshFileChunk base 0x2c, not 0x34).
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
struct C3DTriangle2 { unsigned short Index[3]; };

struct CChunkedFileChunk;
struct CChunkedFileSuperChunk {
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;   // 0x00AA7F40
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& current) const;   // 0x00AA7FF0
};

struct C3DMeshFileTrianglesChunk {
    unsigned char _base_body[0x2c];        // C3DMeshFileChunk (retail 0x2c)
    CArray<C3DTriangle2> TriangleList;     // +0x2c
};

struct C3DMeshFilePrimitiveChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DMeshFileTrianglesChunk> FindFirstTriangles() const;   // 0x00ABB300
    CCountedPointer<const C3DMeshFileTrianglesChunk> FindNextTriangles(const CCountedPointer<const C3DMeshFileTrianglesChunk>& current) const;   // 0x00ABB390
};

struct C3DMeshFileSubMeshChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindFirstPrimitive() const;   // 0x00ABA980
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& current) const;   // 0x00ABAA10
    unsigned long CountTriangles() const;
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
CCountedPointer<const C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::FindFirstTriangles() const
{
    return CCountedPointer<const C3DMeshFileTrianglesChunk>(FindFirstSubChunkOfType("TRIS"), CCountedPointer<const C3DMeshFileTrianglesChunk>::CastTag());
}
CCountedPointer<const C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::FindNextTriangles(const CCountedPointer<const C3DMeshFileTrianglesChunk>& current) const
{
    CCountedPointer<CChunkedFileChunk> prev(current);
    return CCountedPointer<const C3DMeshFileTrianglesChunk>(FindNextSubChunk(prev), CCountedPointer<const C3DMeshFileTrianglesChunk>::CastTag());
}
#pragma auto_inline(on)

unsigned long C3DMeshFileSubMeshChunk::CountTriangles() const
{
    unsigned long count = 0;
    CCountedPointer<const C3DMeshFilePrimitiveChunk> primitive = FindFirstPrimitive();
    while (primitive.Object) {
        CCountedPointer<const C3DMeshFileTrianglesChunk> triangles = primitive->FindFirstTriangles();
        while (triangles.Object) {
            count += triangles->TriangleList.size();
            triangles = primitive->FindNextTriangles(triangles);
        }
        primitive = FindNextPrimitive(primitive);
    }
    return count;
}
