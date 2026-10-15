// C3DMeshFileClothChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const -- version 2, name, material index, particle code, then the compiled code bytes when present, then the sub-chunks
// Grade: DIFFER(146v146) -- register swap only: retail keeps out in edi and &CompiledCode in esi, the lane the reverse; tried reference local, inline helper, duplicated condition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC6600, 160 bytes. Callees: CDataOutputStream::WriteULONG 0x00993EA0,
// WriteNullTerminatedString 0x00995AE0, WriteBOOL 0x00993E30, the stream's virtual WriteBytes (slot 4, +0x10),
// CChunkedFileSuperChunk::WriteToFile 0x00AA78C0.
// The PDB header (engine/C3DMeshFileClothChunk.h) sits 8 bytes high (its super-chunk base is 0x34; retail TLC's
// is 0x2C, as in the landed C3DMeshFile*Chunk family), so the retail layout is modelled with the PDB names:
// Name +0x2C, MaterialIndex +0x30, ParticleCode +0x34, CompiledCode CArray<char> +0x38 (sizeof 0x44, the size
// CreateNewInstance allocates). The has-code flag is a bool local (homed in the dead `out` slot and pushed as a
// dword); the byte data pointer is the landed accessor that hands back the array's own address when empty.
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

struct C3DMeshFileClothChunk : CChunkedFileSuperChunk {
    CCharString Name;                       // +0x2C
    long MaterialIndex;                     // +0x30
    CCharString ParticleCode;               // +0x34
    CArray<char> CompiledCode;              // +0x38
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

inline const char* GetData(const CArray<char>& a)
{
    if (a.m_pEnd - a.m_pBegin == 0)
        return reinterpret_cast<const char*>(&a);
    return a.m_pBegin;
}

void C3DMeshFileClothChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteULONG(2);
    out.WriteNullTerminatedString(Name);
    out.WriteULONG(MaterialIndex);
    out.WriteNullTerminatedString(ParticleCode);
    bool hasCode = CompiledCode.m_pBegin != CompiledCode.m_pEnd;
    out.WriteBOOL(hasCode);
    if (hasCode) {
        out.WriteULONG(CompiledCode.Size());
        out.WriteBytes(GetData(CompiledCode), CompiledCode.Size());
    }
    CChunkedFileSuperChunk::WriteToFile(out, saver);
}
