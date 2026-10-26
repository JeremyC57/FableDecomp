// C3DMeshFileGroupChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const -- bone index, vertex blend count, each blend's vertex index (16-bit) and weight, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABAEE0, 111 bytes. Callees: CDataOutputStream::WriteLONG 0x00993EB0,
// WriteULONG 0x00993EA0, WriteUSHORT 0x00993E80, WriteFLOAT 0x00993EE0, CChunkedFileSuperChunk::WriteToFile 0x00AA78C0.
// The PDB header (engine/C3DMeshFileGroupChunk.h) sits 8 bytes high; retail layout with its names (as the parked
// GetChunkInfo 0x00ABAE50): BoneIndex +0x2C, VertexList CArray<C3DVertexBlend2> +0x30 (8-byte elements). The
// count is an unsigned local; each element is a reference to the re-indexed slot (begin re-read per pass) that
// VC spills into the dead `out` argument slot across the first call; the vertex index is written as 16 bits.
class CChunkedFileSaver;

class CCharString { public:
    void* m_pData;                          // CBasicString<char> data block
    CCharString();                          // 0x0099E4B0
};

class CMatrix3x4 { public: float m_row[3][4]; };                  // 48 bytes

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void WriteBytes(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteLONG(long value);                                    // 0x00993EB0
    void WriteULONG(unsigned long value);                          // 0x00993EA0
    void WriteFLOAT(float value);                                  // 0x00993EE0
    void WriteBOOL(bool value);                                    // 0x00993E30
    void WriteUBYTE(unsigned char value);                          // 0x00993E50
    void WriteUSHORT(unsigned short value);                        // 0x00993E80
    void WriteMatrix3x4(const CMatrix3x4& m);                      // 0x00994070
    void WriteNullTerminatedString(const CCharString& text);       // 0x00995AE0
};

template <class T>
class CArray { public:                             // retail CArray is 12 bytes (the PDB's is 16)
    T* m_pBegin;                            // +0
    T* m_pEnd;                              // +4
    T* m_pEndOfStorage;                     // +8
    CArray() : m_pBegin(0), m_pEnd(0), m_pEndOfStorage(0) {}
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4: per-type deleter, object in ecx
    void* m_pObject;                        // +8
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
};

class CChunkedFileSuperChunk;

class CChunkedFileChunk { public:                  // PDB sizeof 0x10
    virtual void v00();                     // slot 0: deleting destructor
    virtual void v01();                     // slot 1: GetChunkDescription
    virtual void v02();                     // slot 2: GetChunkInfo
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();                     // slot 7: ReadFromFile
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // slot 8
    virtual CChunkedFileChunk* CreateNewInstance() const;                                // slot 9
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;          // slot 10 (0x00AA8460 in the super chunk)
    char ChunkType[5];                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk* Parent;         // +0x0c
    CChunkedFileChunk(const char* type);    // 0x00AA7590
};

class CChunkedFileSuperChunk : public CChunkedFileChunk { public:   // retail sizeof 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c
    unsigned long CurrentChunkIndex;                                       // +0x28
    CChunkedFileSuperChunk(const char* type);                              // 0x00AA8360
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // 0x00AA78C0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const; // 0x00AA7F40
};

class C3DVertexBlend2 { public:
    unsigned long VertexIndex;              // +0
    float BlendValue;                       // +4
};

class C3DMeshFileGroupChunk : public CChunkedFileSuperChunk { public:
    long BoneIndex;                         // +0x2C
    CArray<C3DVertexBlend2> VertexList;     // +0x30
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DMeshFileGroupChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteLONG(BoneIndex);
    unsigned long count = VertexList.Size();
    out.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i) {
        const C3DVertexBlend2& blend = VertexList[i];
        out.WriteUSHORT((unsigned short)blend.VertexIndex);
        out.WriteFLOAT(blend.BlendValue);
    }
    CChunkedFileSuperChunk::WriteToFile(out, saver);
}