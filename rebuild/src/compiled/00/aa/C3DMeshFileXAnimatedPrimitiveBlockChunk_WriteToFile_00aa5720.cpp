// C3DMeshFileXAnimatedPrimitiveBlockChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- the group list, BonesPerVertex, the vertex list and every bone weight, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA5720, length 208 bytes (virtual, slot 8).
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0, _ftol2 @ 0x00BFEA70, CChunkedFileSuperChunk::WriteToFile @ 0x00AA78C0.
// Shape: each CArray<unsigned long> goes out as its count (kept in ebx) then one WriteULong per element (begin
// re-read each pass, STLport `*(begin() + i)`); the bone weights (BonesPerVertex * vertex count records) write
// BoneIndex and then the weight converted unsigned -> float -> long (`fild ; fadd 2^32 if negative ; _ftol2`).
// Layout: retail CArray is 12 bytes (PDB 16): GroupBlock.Groups +0x2c, BonesPerVertex +0x44, VertexList +0x48,
// BoneWeights +0x54, as in the landed dtor 0x00AA7480.
// Leaf: WriteToFile@C3DMeshFileXAnimatedPrimitiveBlockChunk
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

template<class T> struct CVector : CArray<T> {
    const T* begin() const { return this->m_begin; }
    const T& at(unsigned long n) const { return *(begin() + n); }
};
struct C3DIndexedBoneWeight {
    unsigned long BoneIndex;               // +0
    unsigned long Weight;                  // +4
};
struct CVertexGroupBlock {                 // retail 0x18 bytes
    CVector<unsigned long> Groups;         // +0x00
    unsigned char GroupToIndexMap[0x0c];   // +0x0c STLport map
};

struct CChunkedFileSuperChunkBase {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // 0x00AA78C0
};

struct C3DMeshFileXAnimatedPrimitiveBlockChunk : CChunkedFileSuperChunkBase {
    CVertexGroupBlock GroupBlock;                  // +0x2c
    unsigned long BonesPerVertex;                  // +0x44
    CVector<unsigned long> VertexList;             // +0x48
    CVector<C3DIndexedBoneWeight> BoneWeights;     // +0x54
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DMeshFileXAnimatedPrimitiveBlockChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    unsigned long groups = GroupBlock.Groups.Size();
    out.WriteULong(groups);
    for (unsigned long i = 0; i < groups; ++i)
        out.WriteULong(GroupBlock.Groups.at(i));
    out.WriteULong(BonesPerVertex);
    unsigned long vertices = VertexList.Size();
    out.WriteULong(vertices);
    for (unsigned long j = 0; j < vertices; ++j)
        out.WriteULong(VertexList.at(j));
    unsigned long weights = BonesPerVertex * vertices;
    for (unsigned long k = 0; k < weights; ++k) {
        const C3DIndexedBoneWeight& w = BoneWeights.at(k);
        out.WriteULong(w.BoneIndex);
        out.WriteULong((long)(float)w.Weight);
    }
    CChunkedFileSuperChunkBase::WriteToFile(out, saver);
}