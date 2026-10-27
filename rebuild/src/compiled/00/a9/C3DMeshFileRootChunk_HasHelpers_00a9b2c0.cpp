// C3DMeshFileRootChunk::HasHelpers() const -- whether the chunk has a "HLPR" sub chunk
// Leaf: ?HasHelpers@C3DMeshFileRootChunk@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a9b2c0, 118 bytes, thiscall, plain `ret`.
// Identity (proven): unbound (the manifest propagates C3DAnimFileRootChunk::HasHelpers across the four bodies); Ego_r.pdb has ?HasHelpers@C3DMeshFileRootChunk@@QBE_NXZ, the next HasHelpers after the animation root's in the same address order (Ego_r 0x00B983E0 / 0x00C0C3C0 vs retail 0x00A99280 / 0x00A9B2C0); callers CPhysicsMesh::GetHelperDataFromFileData 0x00A6D0E0, CPhysicsMesh::GetContainmentDataFromFileData 0x00A6D670 and 0x00A2F740 (mesh-file readers).
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
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { public:
    bool HasHelpers() const;
};

bool C3DMeshFileRootChunk::HasHelpers() const
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk = FindFirstSubChunkOfType("HLPR");
    return chunk.m_pObject != 0;
}