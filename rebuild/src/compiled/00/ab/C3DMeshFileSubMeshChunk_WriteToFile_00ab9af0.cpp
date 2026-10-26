// C3DMeshFileSubMeshChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- the name, the four hierarchy indices, then the super-chunk base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB9AF0, length 96 bytes.
// Callees: 0x00995AE0 WriteNullTerminatedString, 0x00993EB0 WriteLong x4, 0x00AA78C0 CChunkedFileSuperChunk::WriteToFile
//
// ---- shared retail model for the C3DMeshFile*Chunk family (TLC Fable.exe) -----------------------
// The PDB header rebuild/include/engine/<Class>.h comes from Ego_r.pdb, a later build in which the
// C3DMeshFileChunk base is 0x34 bytes and CArray<T> is 0x10 bytes; in retail TLC the base is 0x2c
// (every derived member sits 8 bytes lower, the vtable slots are the same) and CArray<T> is three
// pointers (0x0c: the XUVTangent factory allocates 0x28 bytes for two CArrays behind a 0x10 base).
// The bytes decide, so this file models the retail layout with the PDB member names and says so here
// instead of including the disagreeing header.
class CCharString { public:
    CCharString();                                      // 0x0099E4B0 default
    CCharString(const char* text, long length);         // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);              // 0x0099EC30 copy
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

class C3DVector  { public: float X, Y, Z; };                   // trivial default constructor (retail leaves it uninitialised)
class CMatrix3x4 { public:                                     // 48 bytes: three axis vectors and the position, copied with rep movsd
    C3DVector XAxis, YAxis, ZAxis, Position;
};

template<class T> class CArray { public:                       // retail CArray<T>: begin/end/capacity, no allocator state
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) {}
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long index) const { return m_begin[index]; }
};

class CChunkedFileSaver;
class CChunkedFileLoader;
class CDataOutputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteBool(bool value);                         // 0x00993E30
    void WriteMatrix3x4(const CMatrix3x4& m);           // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                // slot 2 (+0x08)
    CMatrix3x4 ReadMatrix3x4();                         // 0x00995340 (by value through the hidden pointer)
};

// CChunkedFileChunk (0x10: vptr, ChunkType[5], Parent) is the base of the X* chunks.
class CChunkedFileChunk { public:
    virtual void v0();
    CChunkedFileChunk(const char* chunkType);           // 0x00AA7590
    unsigned char _base_body[0x0c];                     // ChunkType[5] + pad + Parent  (sizeof == 0x10)
};

// CChunkedFileChunk (0x10) -> CChunkedFileSuperChunk -> C3DMeshFileChunk (0x2c in retail).
struct C3DMeshFileChunk {
    virtual ~C3DMeshFileChunk();                                 // slot 0 (scalar deleting dtor)
    virtual void v1();
    virtual CCharString GetChunkInfo() const;                    // slot 2
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // slot 7, 0x00AA8830 (CChunkedFileSuperChunk)
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0 (CChunkedFileSuperChunk)
    C3DMeshFileChunk(const char* chunkType);                     // 0x00AA8360
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// The chunk factory that owns CreateNewInstance(): a `new T` registered through virtual slot 10.
// (rtti-vtables.tsv lists CreateNewInstance at slot 9 of the chunk's own vtable: the prototype chunk
// creates its sibling and registers it through slot 10; both models give the same bytes.)
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual CChunkedFileChunk* RegisterNewChunk(CChunkedFileChunk* chunk);   // slot 10 (+0x28)
};
// ------------------------------------------------------------------------------------------------

class C3DMeshFileSubMeshChunk : public C3DMeshFileChunk { public:
    CCharString Name;              // +0x2c (PDB +0x34)
    long        SubMeshIndex;      // +0x30 (PDB +0x38)
    long        ParentIndex;       // +0x34
    long        FirstChildIndex;   // +0x38
    long        NextSiblingIndex;  // +0x3c   sizeof == 0x40 (the factory's operator new size)
    C3DMeshFileSubMeshChunk();
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DMeshFileSubMeshChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteNullTerminatedString(Name);
    out.WriteLong(SubMeshIndex);
    out.WriteLong(ParentIndex);
    out.WriteLong(FirstChildIndex);
    out.WriteLong(NextSiblingIndex);
    C3DMeshFileChunk::WriteToFile(out, saver);
}