// C3DMeshFilePrimitiveChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const -- one material index as a plain long, otherwise the count tagged with the top bit followed by every index; then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC390, 144 bytes. Callees: CDataOutputStream::WriteSLONG 0x00993EB0,
// CChunkedFileSuperChunk::WriteToFile 0x00AA78C0.
// The PDB header (engine/C3DMeshFilePrimitiveChunk.h) sits 8 bytes high; retail MaterialIndices CArray<long> +0x2C
// (as the CreateNewInstance 0x00ABD0D0 of this chunk). `size() == 1` folds to `((end - begin) & ~3) == 4`; the
// element reads go through the STLport iterator form `*(begin() + n)`; the multi-index count is a signed long
// (the loop test is `jl`) recomputed from the members, written with 0x80000000 or-ed in. One shared super-chunk
// call after an if/else (VC tail-duplicates it); an early `return` in the single-index arm allocates differently.
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
    void WriteULONG(unsigned long value);                          // 0x00993EA0
    void WriteFLOAT(float value);                                  // 0x00993EE0
    void WriteBOOL(bool value);                                    // 0x00993E30
    void WriteUBYTE(unsigned char value);                          // 0x00993E50
    void WriteMatrix3x4(const CMatrix3x4& m);                      // 0x00994070
    void WriteNullTerminatedString(const CCharString& text);       // 0x00995AE0
public:
    void WriteSLONG(long);   // 0x00993EB0
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

template <class T>
struct CVectorArray {                       // retail CArray = STLport vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned long size() const { return (unsigned long)(end() - begin()); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};

class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
    CVectorArray<long> MaterialIndices;     // +0x2C
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DMeshFilePrimitiveChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    if (MaterialIndices.size() == 1)
        out.WriteSLONG(MaterialIndices[0]);
    else {
        long count = (long)MaterialIndices.size();
        out.WriteSLONG(count | 0x80000000);
        for (long i = 0; i < count; ++i)
            out.WriteSLONG(MaterialIndices[i]);
    }
    CChunkedFileSuperChunk::WriteToFile(out, saver);
}