// C3DAnimFileRootChunk::GetNonLoopingDuration() const -- (frame count - 1) / samples per second of the first object's first sequence
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A99610, length 192 bytes.
// Ego_r.pdb: ?GetNonLoopingDuration@C3DAnimFileRootChunk@@QBEMXZ.
// Callees: C3DAnimFileRootChunk::GetFirstObject @ 0x00A99110 (modelled above; FindFirstSubChunkOfType("AOBJ"), by value; catalog
// "C3DMeshFileSubMeshChunk::FindFirstBone" is propagated), C3DAnimFileObjectChunk::GetFirstSequence @ 0x00AB03D0
// (FindFirstSubChunkOfType("ASEQ"), by value; same propagated label), operator delete @ 0x00BFE9BC.
// Shape: two by-value counted-pointer locals; the frame count is the unsigned CArray<CMatrix3x4> size (/48, the
// unsigned->float `fild ; fadd 2^32` fix-up), minus 1.0f, divided by SamplesPerSecond into a named float local
// that is reloaded after both releases.
// Layout: retail C3DAnimFileSequenceChunk (0x4c, PDB 0x58): SamplesPerSecond +0x38, Matrices +0x40 (12-byte CArray),
// as in the landed ctor 0x00AAF730.
// Leaf: GetNonLoopingDuration@C3DAnimFileRootChunk
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

template<class T> struct CCountedPointer {
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    ~CCountedPointer()
    {
        if (Count) {
            --Count->m_references;
            if (Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
        Object = 0;
        Count = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& other, CastTag) : Object(0), Count(0)
    {
        if (other.Count) { Object = (T*)other.Object; Count = other.Count; ++Count->m_references; }
    }
    T* operator->() const { return Object; }
};

struct CMatrix3x4 { float m[12]; };
template<class T> struct CArray {                   // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
};

struct C3DAnimFileSequenceChunk {
    unsigned char _base[0x38];                      // CChunkedFileSuperChunk base, BoneIndex, ParentIndex, BoneName
    float SamplesPerSecond;                         // +0x38
    long BoneType;                                  // +0x3c
    CArray<CMatrix3x4> Matrices;                    // +0x40
};

struct C3DAnimFileObjectChunk {
    CCountedPointer<const C3DAnimFileSequenceChunk> GetFirstSequence() const;   // 0x00AB03D0
};

struct CChunkedFileChunk;
struct CChunkedFileSuperChunk {
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;   // 0x00AA80E0
};

struct C3DAnimFileRootChunk : CChunkedFileSuperChunk {
    CCountedPointer<const C3DAnimFileObjectChunk> GetFirstObject() const;       // 0x00A99110
    float GetNonLoopingDuration() const;
};

// GetFirstObject shares the TU (0x00A99110, defined above as in retail): VC7.1 then knows the object
// pointer's address does not escape (register-form release, its zero stores dropped), while the
// sequence pointer filled by the other TU's GetFirstSequence keeps the re-read release and zero stores.
#pragma auto_inline(off)
CCountedPointer<const C3DAnimFileObjectChunk> C3DAnimFileRootChunk::GetFirstObject() const
{
    CCountedPointer<const C3DAnimFileObjectChunk> object(FindFirstSubChunkOfType("AOBJ"), CCountedPointer<const C3DAnimFileObjectChunk>::CastTag());
    return object;
}
#pragma auto_inline(on)

float C3DAnimFileRootChunk::GetNonLoopingDuration() const
{
    CCountedPointer<const C3DAnimFileObjectChunk> object = GetFirstObject();
    CCountedPointer<const C3DAnimFileSequenceChunk> sequence = object->GetFirstSequence();
    float duration = ((float)sequence->Matrices.size() - 1.0f) / sequence->SamplesPerSecond;
    return duration;
}
