// C3DMeshFileRootChunk::CountVertices() const -- sum CountVertices() over every SUBM sub-mesh chunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9B460, length 240 bytes.
// Callees: C3DMeshFileRootChunk::GetFirstSubMesh @ 0x00A9B030 (FindFirstSubChunkOfType("SUBM") cast, by value;
// catalog "C3DMeshFileSubMeshChunk::FindFirstBone"), GetNextSubMesh(const CCountedPointer&) @ 0x00A9B0C0 (by value;
// catalog "FindNextBone"; both modelled with bodies above the caller), C3DMeshFileSubMeshChunk::CountVertices @ 0x00ABC080, operator delete @ 0x00BFE9BC.
// Shape: the sub-mesh CCountedPointer local is reassigned from the by-value GetNext temporary each pass
// (inline operator= reads the source's count and object into locals first, then skips when the count blocks
// match, else releases, copies and add-refs; then the temporary's
// destructor re-reads its escaped count word); the frame is 8-byte aligned (`and esp,-8`).
// Leaf: CountVertices@C3DMeshFileRootChunk
// CCountedPointer<T>: object pointer + shared count block {references, destroy hook, hooked object}
// (as in the landed CChunkedFileSuperChunk::CountSubChunksOfType 0x00AA7EA0).
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};
template<class T> struct CCountedPointer {
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    CCountedPointer(const CCountedPointer& other) : Object((T*)other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer() { Release(); Object = 0; Count = 0; }
    void Release()
    {
        if (Count) {
            --Count->m_references;
            if (Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.Count;
        T* object = other.Object;
        if (Count != count) {
            Release();
            Object = object;
            Count = count;
            if (count) ++count->m_references;
        }
        return *this;
    }
    // upcast (bone/sub-mesh -> chunk): a plain counted copy
    template<class U> CCountedPointer(const CCountedPointer<U>& other) : Object((T*)other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    // downcast (chunk -> bone/sub-mesh): only a live block is adopted
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& other, CastTag) : Object(0), Count(0)
    {
        if (other.Count) { Object = (T*)other.Object; Count = other.Count; ++Count->m_references; }
    }
    T* operator->() const { return Object; }
};

struct C3DMeshFileSubMeshChunk {
    unsigned long CountVertices() const;               // 0x00ABC080
};

struct CChunkedFileChunk;
struct CChunkedFileSuperChunk {
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;                       // 0x00AA7F40
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& prev) const; // 0x00AA7FF0
};

struct C3DMeshFileRootChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DMeshFileSubMeshChunk> GetFirstSubMesh() const;                                                  // 0x00A9B030
    CCountedPointer<const C3DMeshFileSubMeshChunk> GetNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& current) const;   // 0x00A9B0C0
    unsigned long CountVertices() const;
};

// The two finders are defined above the caller (they are in the same TU in retail): VC7.1 then knows they
// leave the caller's counted pointer alone and keeps its words in ebx/edi across the GetNextSubMesh call.
#pragma auto_inline(off)
CCountedPointer<const C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::GetFirstSubMesh() const
{
    CCountedPointer<const C3DMeshFileSubMeshChunk> subMesh(FindFirstSubChunkOfType("SUBM"), CCountedPointer<const C3DMeshFileSubMeshChunk>::CastTag());
    return subMesh;
}

CCountedPointer<const C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::GetNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& current) const
{
    CCountedPointer<CChunkedFileChunk> prev(current);
    CCountedPointer<const C3DMeshFileSubMeshChunk> subMesh(FindNextSubChunk(prev), CCountedPointer<const C3DMeshFileSubMeshChunk>::CastTag());
    return subMesh;
}
#pragma auto_inline(on)

unsigned long C3DMeshFileRootChunk::CountVertices() const
{
    unsigned long total = 0;
    CCountedPointer<const C3DMeshFileSubMeshChunk> subMesh = GetFirstSubMesh();
    while (subMesh.Object) {
        total += subMesh->CountVertices();
        subMesh = GetNextSubMesh(subMesh);
    }
    return total;
}
