// C3DAnimFileRootChunk::HasHelpers() const -- whether the chunk has a "HLPR" sub chunk
// Leaf: ?HasHelpers@C3DAnimFileRootChunk@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a99280, 118 bytes, thiscall, plain `ret`.
// Identity (proven): bound ?HasHelpers@C3DAnimFileRootChunk@@QBE_NXZ; callers C3DAnimationStats::InitialiseStats 0x00A4EBB0 (landed) and 0x00A4FE60; the tag "HLPR" is the helpers chunk the landed C3DAnimFileLoader registers under the animation root.
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) const 0x00AA7F40 (returns a const CCountedPointer by
//   value; the tag string at 0x0129DFCC), operator delete 0x00BFE9BC (the temporary's and the local's releases).
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
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { public:
    bool HasHelpers() const;
};

bool C3DAnimFileRootChunk::HasHelpers() const
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk = FindFirstSubChunkOfType("HLPR");
    return chunk.m_pObject != 0;
}