// Grade: DIFFER(207v209) -- retail loads the found object (mov eax,[eax]) before the null-count je; ours sinks the load into the taken branch (mov ebx,[eax]); everything else matches
// C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>&) const -- the bone chunk after the given one, by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC09A0, length 209 bytes (`ret 8`: hidden result + current).
// Callees: CChunkedFileSuperChunk::FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>&) @ 0x00AA7FF0 (by value),
// operator delete @ 0x00BFE9BC.
// Shape: the current bone pointer is converted into a CCountedPointer<CChunkedFileChunk> local (add-ref), the
// by-value find result converts into the bone-typed local (add-ref when non-null), the temporaries' destructors
// re-read their escaped count words and zero them, and the local is copied out (no NRVO: merged inc/dec).
// The same body as C3DMeshFileRootChunk::GetNextSubMesh @ 0x00A9B0C0 (used as the callee model in 0x00A9B460).
// Leaf: FindNextBone@C3DMeshFileSubMeshChunk
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
    CCountedPointer() : Object(0), Count(0) {}
    template<class U> void AdoptCast(const CCountedPointer<U>& other)
    {
        SCountBlock* count = other.Count;
        T* object = (T*)other.Object;
        if (count) { Object = object; Count = count; ++count->m_references; }
    }
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& other, CastTag) : Object(0), Count(0)
    {
        if (other.Count) { Object = (T*)other.Object; Count = other.Count; ++Count->m_references; }
    }
    T* operator->() const { return Object; }
};

struct C3DMeshFileBoneChunk;
struct CChunkedFileChunk;
struct CChunkedFileSuperChunk {
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& prev) const; // 0x00AA7FF0
};

struct C3DMeshFileSubMeshChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DMeshFileBoneChunk> FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& current) const;
};

CCountedPointer<const C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& current) const
{
    CCountedPointer<const C3DMeshFileBoneChunk> bone;
    bone.AdoptCast(FindNextSubChunk(current));
    return bone;
}
