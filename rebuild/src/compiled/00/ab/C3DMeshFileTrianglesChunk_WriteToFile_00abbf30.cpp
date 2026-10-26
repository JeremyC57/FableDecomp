// C3DMeshFileTrianglesChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- the triangle count, the three 16-bit indices of every triangle, then the super-chunk's children
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABBF30, length 131 bytes.
// Leaf: ?WriteToFile@C3DMeshFileTrianglesChunk
// Callees: 0x00993EA0 CDataOutputStream::WriteULong, 0x00993E80 CDataOutputStream::WriteUShort (writes 2 bytes
// through slot 4), 0x00AA78C0 CChunkedFileSuperChunk::WriteToFile.
// Shape: Triangles is a retail CArray (three pointers) of 6-byte index triples at +0x2c, so the count is the
// byte span divided by 6 (`imul 0x2AAAAAAB`, no shift). VC strength-reduces the indexed loop into a byte
// offset in ebp and a down-counter parked in the dead `out` argument slot (`out` lives in edi).
//
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
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                  // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
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
    void WriteUShort(unsigned short value);             // 0x00993E80
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

struct C3DMeshFileTriangle { unsigned short Index[3]; };          // 6 bytes
class C3DMeshFileTrianglesChunk : public C3DMeshFileChunk { public:
    CArray<C3DMeshFileTriangle> Triangles;   // +0x2c (PDB +0x34)   sizeof == 0x38
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DMeshFileTrianglesChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    unsigned long count = Triangles.Size();
    out.WriteULong(count);
    for (unsigned long i = 0; i < count; ++i) {
        const C3DMeshFileTriangle& triangle = Triangles[i];
        out.WriteUShort(triangle.Index[0]);
        out.WriteUShort(triangle.Index[1]);
        out.WriteUShort(triangle.Index[2]);
    }
    C3DMeshFileChunk::WriteToFile(out, saver);
}