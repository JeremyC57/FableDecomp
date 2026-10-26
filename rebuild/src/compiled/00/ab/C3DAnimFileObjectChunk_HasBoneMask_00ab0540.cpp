// C3DAnimFileObjectChunk::HasBoneMask() const -- is there an "AMSK" sub-chunk?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB0540, length 80 bytes.
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType @ 0x00AA7F40, operator delete @ 0x00BFE9BC.
// Literal @ 0x0129E8F8 is the bone-mask chunk's type string. Unlike the two root-chunk twins, the object pointer is
// copied into a named local (esi) before the temporary is released and compared with 0 afterwards (`setne al`).
// the generated engine headers for these chunk classes cannot model the bodies -- a local model is used instead.
class CChunkedFileSaver;

class CCharString { public:
    void* m_pData;                          // CBasicString<char> data block
    CCharString();                          // 0x0099E4B0
};

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
public:
    CCountedPointer<CChunkedFileChunk> const FindFirstSubChunkOfType(const char*) const;   // 0x00AA7F40
};

class C3DAnimFileObjectChunk : public CChunkedFileSuperChunk { public:   // retail sizeof 0x40 (PDB 0x48: its CArray is 4 bytes wider)
    CCharString Name;                       // +0x2c
    long SubMeshIndex;                      // +0x30
    long ParentIndex;                       // +0x34
    long FirstChildIndex;                   // +0x38
    long NextSiblingIndex;                  // +0x3c
    C3DAnimFileObjectChunk() : CChunkedFileSuperChunk("AOBJ") {}   // literal @ 0x0129DFC4, vtable 0x0129E904
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
    bool HasBoneMask() const;
};

bool C3DAnimFileObjectChunk::HasBoneMask() const
{
    CChunkedFileChunk* chunk = FindFirstSubChunkOfType("AMSK").m_pObject;
    return chunk != 0 ? true : false;   // the ternary form keeps VC from zeroing eax first
}