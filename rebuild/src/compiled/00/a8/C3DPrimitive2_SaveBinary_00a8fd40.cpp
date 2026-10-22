// C3DPrimitive2::SaveBinary(CDataOutputStream&) const -- write the primitive header, static/animated blocks, the vertex buffer (format block + raw data), the index buffer and every cloth primitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8FD40, 528 bytes (ret 4).
// Callees: CDataOutputStream::WriteLong 0x00993EB0, Write3DVector 0x00993FD0, WriteFloat 0x00993EE0, WriteULong 0x00993EA0,
//   WriteBool 0x00993E30, WriteUChar 0x00993E50, WriteData (vtable slot 4), WriteBuffer 0x00996610 (compressed block),
//   CAnimatedBlock::SaveBinary 0x00A8E770, CVertexBufferWin32::Lock 0x00AC20A0 / Unlock 0x00AC20B0,
//   C3DPrimitive2::UploadToIndexBuffer 0x00A8F9E0, CIndexBuffer::Lock 0x00AC1FB0 / Unlock 0x00AC1F00,
//   C3DClothPrimitive::SaveBinary 0x00AC3B00.
// Layout: retail C3DPrimitive2 is 0xA0 bytes (PDB 0xAC: each CArray is 12 bytes, not 16), so everything from
// Data on sits 4 below its Ego_r.pdb offset: ClothPrimitives +0x20, VertexBuffer +0x30, IndexBuffer +0x34,
// StaticBlockCount +0x40, AnimatedBlockCount +0x48, VertexCount +0x58, IndexCount +0x60, InitFlags +0x68.
// Shape: the header fields go out in declaration order, the two block counts are written twice (header, then
// in front of their loops); the static-block loop steps a byte offset kept in the dead stream-argument slot;
// the repeat count is max(RepeatingMeshReps, 1); the vertex buffer's 32-byte compression params are block-copied
// to a local before the virtual WriteData.
struct C3DVector { float X, Y, Z; };
struct CVertexCompressionParams { float PosScale[4]; float PosBias[4]; };

struct CDataOutputStream {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteBool(bool value);                         // 0x00993E30
    void WriteUChar(unsigned char value);               // 0x00993E50
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void Write3DVector(const C3DVector& value);         // 0x00993FD0
    void WriteBuffer(const void* data, long size);      // 0x00996610 (compressed block)
};

struct CVertexBufferWin32 {
    virtual ~CVertexBufferWin32();
    unsigned long m_handle;                             // +0x04
    unsigned long ByteLength;                           // +0x08
    unsigned long VertexSize;                           // +0x0C
    unsigned char VertexType;                           // +0x10
    CVertexCompressionParams CompressionParams;         // +0x14
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                      // 0x00AC20B0
};
struct CIndexBuffer {
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                      // 0x00AC1F00
};
struct C3DClothPrimitive {
    void SaveBinary(CDataOutputStream& out) const;      // 0x00AC3B00
};
template<class T> struct CCountedPointer { T* Data; void* Info; };
template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    T& operator[](unsigned long i) const { return m_begin[i]; }
};

struct CStaticBlock {                                   // 0x20 bytes
    unsigned long StartIndex;                           // +0x00
    unsigned long PrimitiveCount;                       // +0x04
    bool DiffuseFlag;                                   // +0x08
    bool BumpFlag;                                      // +0x09
    unsigned char TextureStage;                         // +0x0A
    unsigned char _0b[0x11];
    long MaterialIndex;                                 // +0x1C
};
struct CAnimatedBlock {                                 // 0x30 bytes
    unsigned char _body[0x30];
    void SaveBinary(CDataOutputStream& out) const;      // 0x00A8E770
};

struct C3DPrimitive2 {
    virtual void v00();
    long MaterialIndex;                                 // +0x04
    long RepeatingMeshReps;                             // +0x08
    C3DVector BoundingCenter;                           // +0x0C
    float BoundingRadius;                               // +0x18
    float AverageTextureStretch;                        // +0x1C
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;   // +0x20
    void* Data;                                         // +0x2C
    CVertexBufferWin32* VertexBuffer;                   // +0x30
    CIndexBuffer* IndexBuffer;                          // +0x34
    unsigned long VertexBufferStatus;                   // +0x38
    unsigned long IndexBufferStatus;                    // +0x3C
    unsigned long StaticBlockCount;                     // +0x40
    CStaticBlock* StaticBlocks;                         // +0x44
    unsigned long AnimatedBlockCount;                   // +0x48
    CAnimatedBlock* AnimatedBlocks;                     // +0x4C
    unsigned long AnimatedBlocksStatus;                 // +0x50
    unsigned long RepeatedBlocksStatus;                 // +0x54
    unsigned long VertexCount;                          // +0x58
    unsigned long TriangleCount;                        // +0x5C
    unsigned long IndexCount;                           // +0x60
    bool UploadedFlag;                                  // +0x64
    unsigned char VertexStreamFlags;                    // +0x65
    unsigned long InitFlags;                            // +0x68
    bool UploadToIndexBuffer(unsigned long flags);      // 0x00A8F9E0
    void SaveBinary(CDataOutputStream& out);
};

void C3DPrimitive2::SaveBinary(CDataOutputStream& out)
{
    out.WriteLong(MaterialIndex);
    out.WriteLong(RepeatingMeshReps);
    out.Write3DVector(BoundingCenter);
    out.WriteFloat(BoundingRadius);
    out.WriteFloat(AverageTextureStretch);
    out.WriteULong(StaticBlockCount);
    out.WriteULong(AnimatedBlockCount);
    out.WriteULong(VertexCount);
    out.WriteULong(TriangleCount);
    out.WriteULong(IndexCount);
    out.WriteULong(InitFlags);
    out.WriteULong(StaticBlockCount);
    out.WriteULong(AnimatedBlockCount);
    for (unsigned long i = 0; i < StaticBlockCount; ++i) {
        const CStaticBlock& block = StaticBlocks[i];
        out.WriteULong(block.StartIndex);
        out.WriteULong(block.PrimitiveCount);
        out.WriteBool(block.DiffuseFlag);
        out.WriteUChar(block.TextureStage);
        out.WriteBool(block.BumpFlag);
        out.WriteLong(block.MaterialIndex);
    }
    for (unsigned long j = 0; j < AnimatedBlockCount; ++j)
        AnimatedBlocks[j].SaveBinary(out);

    long reps = RepeatingMeshReps;
    if (reps <= 1)
        reps = 1;

    CVertexBufferWin32* vb = VertexBuffer;
    CVertexCompressionParams params = vb->CompressionParams;
    out.WriteData(&params, sizeof(params));
    out.WriteULong(vb->VertexType);
    out.WriteULong(vb->VertexSize);
    void* vertices = vb->Lock(0, 0, 0);
    out.WriteBuffer(vertices, vb->ByteLength);
    vb->Unlock();

    UploadToIndexBuffer(0);
    CIndexBuffer* ib = IndexBuffer;
    unsigned char* indices = ib->Lock(0, 0, 0);
    out.WriteBuffer(indices, IndexCount * reps * sizeof(unsigned short));
    ib->Unlock();

    unsigned long clothCount = ClothPrimitives.size();
    out.WriteULong(clothCount);
    for (unsigned long c = 0; c < clothCount; ++c)
        ClothPrimitives[c].Data->SaveBinary(out);
}