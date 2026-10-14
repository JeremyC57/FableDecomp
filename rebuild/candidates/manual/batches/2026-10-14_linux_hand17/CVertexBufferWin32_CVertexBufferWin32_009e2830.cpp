// CVertexBufferWin32::CVertexBufferWin32(EVertexType, long, long, ulong, EVertexBufferPool) -- the
// CVertexBufferBase shell (no D3D buffer, zero length/type/stride, identity compression params) then
// Initialise(...) with the five arguments. Vtable 0x0129BA20 (RTTI: CVertexBufferWin32).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2830, length 95 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CVertexBufferWin32::Initialise @ 0x00A63150.
// Manifest label (CChangingStateComponent::Initialise) corrected from the stored vtable.
// CBase: the engine's polymorphic root (vtable @ 0x0129A7C4 in the ctor, 0x01231710 in the dtor);
// both members are out of line in retail.
struct CBase {
    virtual void VSlot0();
    CBase();                                    // 0x0099A2F0
    ~CBase();                                   // 0x0099A300
};
#include "engine/CVertexBufferWin32.h"   // PDB header: an opaque 0x30-byte flattened base, so the retail
                                            // members (CVertexBufferBase in the PDB) are modelled locally
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
    CVertexBufferBase() : Length(0), Type(0), CompressionParams()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};
struct CVertexBufferWin32_Retail : CVertexBufferBase {
    virtual void VSlot1();
    CVertexBufferWin32_Retail();
    CVertexBufferWin32_Retail(long type, long a, long b, unsigned long c, long pool);
    bool Initialise(long type, long a, long b, unsigned long c, long pool);   // 0x00A63150
};
FABLE_STATIC_ASSERT(sizeof(CVertexBufferWin32_Retail) == sizeof(CVertexBufferWin32));

CVertexBufferWin32_Retail::CVertexBufferWin32_Retail(long type, long a, long b, unsigned long c, long pool)
    : CVertexBufferBase()
{
    Initialise(type, a, b, c, pool);
}
