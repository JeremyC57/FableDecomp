// CEngineParticleSimulationRenderData::CEngineParticleSimulationRenderData() -- the CBase shell, two default (empty) Win32 vertex buffers and CurrentVB = 0
// Leaf: ??0CEngineParticleSimulationRenderData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB63A0, length 128 bytes.
// Callees: CBase::CBase @ 0x0099A2F0 (for the object and for each buffer). Vtables (rtti-vtables.tsv): 0x012A56BC =
// CEngineParticleSimulationRenderData, 0x0129BA20 = CVertexBufferWin32.
// Shape: the PDB layout (0x70 bytes): vptr, CVertexBufferWin32 VertexBuffers[2] at +4 (0x34 each), long CurrentVB at +0x6C.
// The array is built by an inlined counted loop (`dec eax` on a frame counter, two element pointers stepping 0x34) because
// the buffer has an inline default constructor and no destructor in this TU; each element is the landed
// CVertexBufferWin32() inlined: CBase call, the base init-list zeros and the 1/0 compression params (ebp = 1.0f,
// ebx = 0), the CVertexBufferWin32 vtable store, then PVertexBuffer/Stride cleared in the DERIVED constructor's body (in
// the base body they are scheduled before the vtable store, as in the standalone ctor @ 0x00A8E160). `this` is spilled to the frame (all registers
// busy) and CurrentVB is cleared last from the zero register. The element models are the landed ones (CBase,
// CVertexBufferBase, CVertexCompressionParams); the generated CVertexBufferWin32.h is an opaque 0x30-byte flattening.
struct CBase {
    virtual void VSlot0();
    CBase();                                    // 0x0099A2F0
};

struct IDirect3DVertexBuffer9;

struct CVertexCompressionParams {               // PDB: PosScale XYZW then PosBias XYZW
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};

struct CVertexBufferBase : CBase {              // 0x34 bytes, PDB member names
    IDirect3DVertexBuffer9*  PVertexBuffer;     // +0x04
    unsigned long            Length;            // +0x08
    long                     Type;              // +0x0c (EVertexType)
    unsigned char            Stride;            // +0x10
    CVertexCompressionParams CompressionParams; // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};

struct CVertexBufferWin32 : CVertexBufferBase { // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32() : CVertexBufferBase()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};

struct CEngineParticleSimulationRenderData : CBase {   // vtable 0x012A56BC, 0x70 bytes
    CVertexBufferWin32 VertexBuffers[2];        // +0x04
    long               CurrentVB;               // +0x6C
    virtual void VSlot0();
    CEngineParticleSimulationRenderData();
};

CEngineParticleSimulationRenderData::CEngineParticleSimulationRenderData()
    : CurrentVB(0)
{
}
