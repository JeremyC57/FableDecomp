// C3DMeshFileVerticesChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- the vertex count, then position, normal and UV of every vertex as eight floats; no base call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABBDF0, length 151 bytes.
// Leaf: WriteToFile
// Callees: 0x00993EA0 CDataOutputStream::WriteULONG, 0x00993EE0 CDataOutputStream::WriteFloat.
// Shape: a plain CChunkedFileChunk (0x10 base) with the retail three-pointer CArray<C3DVertex2> at +0x10
// (PDB header: same offset, 0x10-wide CArray). C3DVertex2 is 32 bytes (PDB: Position, Normal, U, V) so the
// count is `sar esi,5`; the indexed loop is strength-reduced to a byte offset in ebx and a down-counter in
// the dead `out` slot. The floats are forwarded by value with integer moves.
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
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteBool(bool value);                         // 0x00993E30
    void WriteMatrix3x4(const CMatrix3x4& m);           // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
public:
    void WriteULONG(unsigned long);   // 0x00993EA0
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

class C3DVertex2 { public:                      // 32 bytes (PDB)
    C3DVector Position;                  // +0x00
    C3DVector Normal;                    // +0x0c
    float     U;                         // +0x18
    float     V;                         // +0x1c
};
class C3DMeshFileVerticesChunk : public CChunkedFileChunk { public:
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    CArray<C3DVertex2> VertexList;       // +0x10 (PDB header agrees on the offset; its CArray is 0x10 wide, retail's is 0x0c)
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // slot 8
};

void C3DMeshFileVerticesChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    unsigned long count = VertexList.Size();
    out.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i) {
        const C3DVertex2& vertex = VertexList[i];
        out.WriteFloat(vertex.Position.X);
        out.WriteFloat(vertex.Position.Y);
        out.WriteFloat(vertex.Position.Z);
        out.WriteFloat(vertex.Normal.X);
        out.WriteFloat(vertex.Normal.Y);
        out.WriteFloat(vertex.Normal.Z);
        out.WriteFloat(vertex.U);
        out.WriteFloat(vertex.V);
    }
}