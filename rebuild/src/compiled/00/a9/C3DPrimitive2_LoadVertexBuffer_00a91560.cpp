// C3DPrimitive2::LoadVertexBuffer(CMemoryDataInputStream&, unsigned long, unsigned long, unsigned long, CVertexCompressionParams&) -- read the vertex data straight into the locked vertex buffer, or through a temporary and a format conversion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A91560, length 224 bytes (`ret 14h`).
// GRAPHICS LAYER (replacement work later): Ego_r.pdb ?LoadVertexBuffer@C3DPrimitive2@@AAEXAAVCMemoryDataInputStream@@KKKAAVCVertexCompressionParams@@@Z.
// Callees: CVertexBufferWin32::Lock(0,0,0) @ 0x00AC20A0 / Unlock @ 0x00AC20B0, the stream's virtual Read (slot 4, +0x10),
// vector<unsigned char>(n) @ 0x00514E20 (catalog "std::vector::_Init"), free @ 0x00BFEA14, the __fastcall stream converters
// @ 0x00A90FC0 (animated; catalog "ConvertVertexStream_AnimatedToFormat") and @ 0x00A91390 (static).
// Retail offsets (PDB - 4): RepeatingMeshReps +8, VertexBuffer +0x30 (CVertexBufferWin32 compression params +0x14, 32 bytes),
// AnimatedBlockCount +0x48, VertexCount +0x58, InitFlags +0x68 (the format the buffer holds).
// Shape: byte count = VertexCount * max(reps, 1) * vertex size; the direct read when the file format matches InitFlags or
// the mesh repeats, else a byte vector (GetData idiom: its own address when empty) converted into the buffer; finally
// the compression params block-copied into the buffer (rep movsd).
// Leaf: LoadVertexBuffer@C3DPrimitive2
extern "C" void __cdecl free(void* p);              // 0x00BFEA14
class CVertexCompressionParams { public: float PosScale[4]; float PosBias[4]; };
class CMemoryDataInputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Read(void* buffer, long size);     // slot 4 (+0x10)
};
struct CByteArray {                                 // retail CArray<unsigned char>: begin/end/capacity
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                    // 0x00514E20
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
};
void __fastcall ConvertAnimatedVertices(const void* source, unsigned long sourceFormat, void* dest, unsigned long destFormat,
                                        unsigned long vertexCount, CVertexCompressionParams& params);   // 0x00A90FC0
void __fastcall ConvertStaticVertices(const void* source, unsigned long sourceFormat, void* dest, unsigned long destFormat,
                                      unsigned long vertexCount, CVertexCompressionParams& params);     // 0x00A91390
class CVertexBufferWin32 { public:
    virtual ~CVertexBufferWin32();
    unsigned char _04[0x10];
    CVertexCompressionParams CompressionParams;     // +0x14
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                  // 0x00AC20B0
};
class C3DPrimitive2 { public:
    unsigned char _pad[8];
    long RepeatingMeshReps;                         // +0x08
    unsigned char _0c[0x24];
    CVertexBufferWin32* VertexBuffer;               // +0x30 scoped_ptr
    unsigned char _34[0x14];
    unsigned long AnimatedBlockCount;               // +0x48
    unsigned char _4c[0x0c];
    unsigned long VertexCount;                      // +0x58
    unsigned char _5c[0x0c];
    unsigned long InitFlags;                        // +0x68
    void LoadVertexBuffer(CMemoryDataInputStream& in, unsigned long vertexType, unsigned long vertexSize, unsigned long fileFormat, CVertexCompressionParams& params);
};

void C3DPrimitive2::LoadVertexBuffer(CMemoryDataInputStream& in, unsigned long vertexType, unsigned long vertexSize, unsigned long fileFormat, CVertexCompressionParams& params)
{
    long reps = RepeatingMeshReps;
    if (reps <= 1)
        reps = 1;
    unsigned long vertices = VertexCount * reps;
    unsigned long bytes = vertices * vertexSize;
    void* dest = VertexBuffer->Lock(0, 0, 0);
    if (fileFormat != InitFlags && RepeatingMeshReps <= 0) {
        CByteArray buffer(bytes);
        unsigned char* source = buffer.GetData();
        in.Read(source, bytes);
        if (AnimatedBlockCount > 0)
            ConvertAnimatedVertices(source, fileFormat, dest, InitFlags, vertices, params);
        else
            ConvertStaticVertices(source, fileFormat, dest, InitFlags, vertices, params);
    }
    else
        in.Read(dest, bytes);
    VertexBuffer->Unlock();
    VertexBuffer->CompressionParams = params;
}