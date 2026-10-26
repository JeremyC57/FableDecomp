// C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk() -- "ASEQ" super chunk: bone index -1, empty name, type 0, no matrices
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAF730, length 52 bytes.
// Callees: CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) @ 0x00AA8360, CCharString::CCharString() @ 0x0099E4B0.
// Base constructor with the literal @ 0x0129E7B4, vtable 0x0129E8B8, BoneIndex (+0x2c) = -1 stored before the
// BoneName (+0x34) constructor call, then BoneType (+0x3c) and the three Matrices pointers (+0x40..+0x48) zeroed
// through one register. ParentIndex and SamplesPerSecond are left uninitialised.
// Layout: the PDB (Ego_r) CChunkedFileSuperChunk is 0x34 bytes with 16-byte CArrays; retail's is 0x2c with 12-byte
// CArrays (begin/end/end-of-storage), which is why every derived member sits 8 bytes below the PDB offset and why
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
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const; // 0x00AA7F40
};

class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk { public:   // retail sizeof 0x4c (PDB 0x58: two CArrays 4 bytes wider each)
    long BoneIndex;                         // +0x2c
    long ParentIndex;                       // +0x30
    CCharString BoneName;                   // +0x34
    float SamplesPerSecond;                 // +0x38
    long BoneType;                          // +0x3c EBoneType
    CArray<unsigned char> Matrices;         // +0x40 CArray<CMatrix3x4>
    C3DAnimFileSequenceChunk();
    virtual CChunkedFileChunk* CreateNewInstance() const;
};

C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk()
    : CChunkedFileSuperChunk("ASEQ")        // literal @ 0x0129E7B4
    , BoneIndex(-1)
    , BoneName()
    , BoneType(0)
    , Matrices()
{
}