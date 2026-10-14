// C3DAnimFileTimingEventChunk::CreateNewInstance() const -- allocate and register a fresh "TMEV" chunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAE250, length 80 bytes.
// Callees: operator new @ 0x00BFEA1A, CChunkedFileChunk::CChunkedFileChunk(const char*) @ 0x00AA7590, CCharString::CCharString() @ 0x0099E4B0, virtual InitNewInstance (slot 10).
// `new` with the constructor inlined (null-checked: on allocation failure the virtual gets a null), then the
// protected virtual InitNewInstance (slot 10, 0x00AA8460 in the super chunk) registers the chunk and its result
// is returned. VC duplicates the virtual-call tail on both sides of the null test.
// The inlined constructor is the one @ 0x00AAE170 (0x18-byte leaf chunk).
struct CChunkedFileSaver;

struct CCharString {
    void* m_pData;                          // CBasicString<char> data block
    CCharString();                          // 0x0099E4B0
};

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

struct C3DAnimFileTimingEventChunk : CChunkedFileChunk {   // sizeof 0x18
    CCharString Name;                       // +0x10
    float Time;                             // +0x14
    C3DAnimFileTimingEventChunk() : CChunkedFileChunk("TMEV") {}   // literal @ 0x0129E7AC, vtable 0x0129E7F0
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
};

CChunkedFileChunk* C3DAnimFileTimingEventChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileTimingEventChunk);
}
