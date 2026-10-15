// C3DMeshFileXVertexDataChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- version 2, the four format words, the vertex-compression parameters, then the raw vertex bytes in one WriteData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA58C0, length 109 bytes.
// Callees: 0x00993EA0 CDataOutputStream::WriteULong, 0x00AA4D20 __fastcall WriteToFile(const CVertexCompressionParams&,
// CDataOutputStream&) (eight WriteFloat), CDataOutputStream slot 4 (+0x10) WriteData(data, size).
// Layout: the PDB header (engine/C3DMeshFileXVertexDataChunk.h) agrees on every offset up to VertexData (+0x40)
// but its CArray<char> is 0x10 wide; retail's is three pointers (the factory 0x00AA59C0 allocates 0x4c), so
// this file models the retail layout with the PDB names. The raw bytes go out through the CArray GetData()
// idiom (`count ? first : this`).
// Leaf: ?WriteToFile@C3DMeshFileXVertexDataChunk
//
struct CCharString {
    CCharString();                                      // 0x0099E4B0 default
    CCharString(const char* text, long length);         // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);              // 0x0099EC30 copy
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

struct C3DVector  { float X, Y, Z; };                   // trivial default constructor (retail leaves it uninitialised)
struct CMatrix3x4 {                                     // 48 bytes: three axis vectors and the position, copied with rep movsd
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                  // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity, no allocator state
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) {}
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long index) const { return m_begin[index]; }
};

struct CChunkedFileSaver;
struct CChunkedFileLoader;
struct CDataOutputStream {
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
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                // slot 2 (+0x08)
    CMatrix3x4 ReadMatrix3x4();                         // 0x00995340 (by value through the hidden pointer)
};

// CChunkedFileChunk (0x10: vptr, ChunkType[5], Parent) is the base of the X* chunks.
struct CChunkedFileChunk {
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

struct CVertexCompressionParams {        // 32 bytes (PDB)
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
void __fastcall WriteToFile(const CVertexCompressionParams& params, CDataOutputStream& out);   // 0x00AA4D20

template<class T> struct CRawArray : CArray<T> {
    const T* GetData() const { return this->Size() != 0 ? this->m_begin : (const T*)this; }
};

struct C3DMeshFileXVertexDataChunk : CChunkedFileChunk {
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    unsigned long            FormatID;       // +0x10
    unsigned long            VertexCount;    // +0x14
    unsigned long            VertexSize;     // +0x18
    unsigned long            InitFlags;      // +0x1c
    CVertexCompressionParams VBCParams;      // +0x20
    CRawArray<char>          VertexData;     // +0x40   sizeof == 0x4c (PDB 0x50)
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // slot 8
};

void C3DMeshFileXVertexDataChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteULong(2);
    out.WriteULong(FormatID);
    out.WriteULong(VertexCount);
    out.WriteULong(VertexSize);
    out.WriteULong(InitFlags);
    ::WriteToFile(VBCParams, out);
    out.WriteData(VertexData.GetData(), VertexData.Size());
}
