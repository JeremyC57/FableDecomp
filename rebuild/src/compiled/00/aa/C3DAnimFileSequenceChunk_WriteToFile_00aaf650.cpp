// C3DAnimFileSequenceChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const -- bone indices, name, frame rate, the matrices, the bone type, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAF650, 148 bytes. Callees: CDataOutputStream::WriteLONG 0x00993EB0,
// WriteNullTerminatedString 0x00995AE0, WriteFLOAT 0x00993EE0, WriteMatrix3x4 0x00994070, WriteUBYTE 0x00993E50,
// CChunkedFileSuperChunk::WriteToFile 0x00AA78C0.
// Chunk model reused from the landed C3DAnimFile*/C3DMeshFile* WriteToFile family (retail super-chunk base
// 0x2C; the PDB's members sit 8 bytes higher). The matrix count is a signed 48-byte division held in an
// unsigned local (the loop entry is `test ; jbe`), written with WriteLONG; VC turns the indexed loop into
// a count-down with a running byte offset (the array begin is re-read each pass). The bone type is a byte.
struct CChunkedFileSaver;

struct CCharString {
    void* m_pData;                          // CBasicString<char> data block
    CCharString();                          // 0x0099E4B0
};

struct CMatrix3x4 { float m_row[3][4]; };                  // 48 bytes

struct CDataOutputStream {
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
    void WriteMatrix3x4(const CMatrix3x4& m);                      // 0x00994070
    void WriteNullTerminatedString(const CCharString& text);       // 0x00995AE0
};

template <class T>
struct CArray {                             // retail CArray is 12 bytes (the PDB's is 16)
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
struct CCountedPointer {
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

struct CChunkedFileSuperChunk;

struct CChunkedFileChunk {                  // PDB sizeof 0x10
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

struct CChunkedFileSuperChunk : CChunkedFileChunk {   // retail sizeof 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c
    unsigned long CurrentChunkIndex;                                       // +0x28
    CChunkedFileSuperChunk(const char* type);                              // 0x00AA8360
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // 0x00AA78C0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const; // 0x00AA7F40
};

struct C3DAnimFileSequenceChunk : CChunkedFileSuperChunk {
    long BoneIndex;                         // +0x2C
    long ParentIndex;                       // +0x30
    CCharString BoneName;                   // +0x34
    float SamplesPerSecond;                 // +0x38
    unsigned char BoneType;                 // +0x3C EBoneType
    CArray<CMatrix3x4> Matrices;            // +0x40
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DAnimFileSequenceChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteLONG(BoneIndex);
    out.WriteLONG(ParentIndex);
    out.WriteNullTerminatedString(BoneName);
    out.WriteFLOAT(SamplesPerSecond);
    unsigned long count = Matrices.Size();
    out.WriteLONG(count);
    for (unsigned long i = 0; i < count; ++i)
        out.WriteMatrix3x4(Matrices[i]);
    out.WriteUBYTE(BoneType);
    CChunkedFileSuperChunk::WriteToFile(out, saver);
}