// C3DPrimitive2::UploadToVertexBufferStatic(unsigned long) -- size the static vertex format (0x24 bytes for repeating meshes), (re)create the vertex buffer for vertices x repetitions and fill it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A931F0, length 208 bytes (`ret 4`).
// GRAPHICS LAYER (replacement work later): Ego_r.pdb ?UploadToVertexBufferStatic@C3DPrimitive2@@AAE_NK@Z.
// Callees: N3DPrimitive::CVertexShaderInput base ctor @ 0x00AB3B10 (catalog "CAIStateGroup_HoverHornetQueen" is propagated),
// CVertexShaderInputStatic::Initialise(unsigned long, bool) @ 0x00A90560, the input declaration's vertex-size getter
// @ 0x00AB37B0 (catalog "CPlayerManager::PrepareForLoad", propagated), ~CVertexShaderInputStatic @ 0x00A98640 (catalog
// "~CPatchTesselationEdgeStrip", propagated), C3DPrimitive2::SetupVertexBuffer @ 0x00A8EDA0, CVertexBufferWin32::Lock(0,0,0)
// @ 0x00AC20A0 / Unlock @ 0x00AC20B0 (catalog "CThingFilter_IsValid::operator()", propagated),
// C3DPrimitive2::ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*) @ 0x00A92740.
// Vtable: N3DPrimitive::CVertexShaderInputStatic 0x0129DFB8 (stored by the inline derived ctor).
// Retail offsets (PDB - 4): VertexBuffer +0x30 (CVertexBufferWin32: compression params +0x14), VertexBufferStatus +0x38,
// RepeatingMeshReps +8, Data (scoped_ptr<N3DPrimitive::CDeviceIndependent>, 32-byte vertex vector at +4) +0x2c.
// Leaf: UploadToVertexBufferStatic@C3DPrimitive2
struct CVertexCompressionParams { float PosScale[4]; float PosBias[4]; };
struct CVertexFormat {
    unsigned long GetVertexSize() const;            // 0x00AB37B0
};
struct CVertexDeclaration {
    long m_00;
    CVertexFormat Format;                           // +4
};
namespace N3DPrimitive {
struct CVertexShaderInput {
    CVertexShaderInput();                           // 0x00AB3B10
    virtual ~CVertexShaderInput();
    CVertexDeclaration* Declaration;                // +4
    unsigned char _08[0x14];
};
struct CVertexShaderInputStatic : CVertexShaderInput {   // vtable 0x0129DFB8
    CVertexShaderInputStatic() {}
    virtual ~CVertexShaderInputStatic();            // 0x00A98640
    void Initialise(unsigned long flags, bool useDeclaration);   // 0x00A90560
};
}
struct CVertexBufferWin32 {
    virtual ~CVertexBufferWin32();
    unsigned char _04[0x10];
    CVertexCompressionParams CompressionParams;     // +0x14
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                  // 0x00AC20B0
};
struct CStaticVertex { unsigned char _body[0x20]; };
template<class T> struct CArray {                   // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
};
namespace N3DPrimitive {
struct CDeviceIndependent {
    long m_00;
    CArray<CStaticVertex> Vertices;                 // +4
};
}
struct C3DPrimitive2 {
    unsigned char _pad[8];
    long RepeatingMeshReps;                         // +0x08
    unsigned char _0c[0x20];
    N3DPrimitive::CDeviceIndependent* Data;         // +0x2c scoped_ptr
    CVertexBufferWin32* VertexBuffer;               // +0x30 scoped_ptr
    unsigned char _34[4];
    unsigned long VertexBufferStatus;                        // +0x38
    bool SetupVertexBuffer(long vertexType, unsigned long vertexSize, unsigned long vertexCount, unsigned long flags);   // 0x00A8EDA0
    void ConvertToVBFormatStatic(void* data, unsigned long vertexSize, unsigned long flags, CVertexCompressionParams* params);   // 0x00A92740
    bool UploadToVertexBufferStatic(unsigned long flags);
};

bool C3DPrimitive2::UploadToVertexBufferStatic(unsigned long flags)
{
    unsigned long vertexSize;
    if (RepeatingMeshReps > 0)
        vertexSize = 0x24;
    else {
        N3DPrimitive::CVertexShaderInputStatic input;
        input.Initialise(flags, true);
        vertexSize = input.Declaration->Format.GetVertexSize();
    }
    long reps = RepeatingMeshReps;
    if (reps < 1)
        reps = 1;
    unsigned long vertexCount = Data->Vertices.size() * reps;
    if (!SetupVertexBuffer(0, vertexSize, vertexCount, flags))
        return false;
    void* data = VertexBuffer->Lock(0, 0, 0);
    ConvertToVBFormatStatic(data, vertexSize, flags, &VertexBuffer->CompressionParams);
    VertexBuffer->Unlock();
    VertexBufferStatus |= 1;
    return true;
}