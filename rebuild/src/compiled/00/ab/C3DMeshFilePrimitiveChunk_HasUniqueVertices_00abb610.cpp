// C3DMeshFilePrimitiveChunk::HasUniqueVertices() const -- whether the chunk has a "UNIV" sub chunk
// Leaf: ?HasUniqueVertices@C3DMeshFilePrimitiveChunk@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00abb610, 118 bytes, thiscall, plain `ret`.
// Identity (proven): unbound (manifest label propagated); it tests for the "UNIV" sub chunk; Ego_r.pdb has ?HasUniqueVertices@C3DMeshFilePrimitiveChunk@@QBE_NXZ (after HasSmoothingGroups, as here); caller COptimisedPrimitive::COptimisedPrimitive 0x00ACDB80 (built from a mesh-file primitive chunk).
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) const 0x00AA7F40 (returns a const CCountedPointer by
//   value; the tag string at 0x0129EB0C), operator delete 0x00BFE9BC (the temporary's and the local's releases).
// Shape (family L113): a default-constructed local assigned from the returned temporary (inline operator= over a known-null
//   block: the pointer is taken only when the block is non-null), the temporary released (block re-read after the
//   decrement), the test, then the local released from registers.
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};
template <class T>
class CCountedPointer { public:
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
    ~CCountedPointer()
    {
        Release();
        m_pObject = 0;
        m_pCount = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        T* object = other.m_pObject;
        if (m_pCount != count) {
            Release();
            m_pObject = object;
            m_pCount = count;
            if (m_pCount)
                ++m_pCount->m_references;
        }
        return *this;
    }
};

class CChunkedFileChunk { public:
    virtual ~CChunkedFileChunk();
    char ChunkType[5];                      // +0x04
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* chunkType) const;   // 0x00AA7F40
};
class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
    bool HasUniqueVertices() const;
};

bool C3DMeshFilePrimitiveChunk::HasUniqueVertices() const
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk = FindFirstSubChunkOfType("UNIV");
    return chunk.m_pObject != 0;
}