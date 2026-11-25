// C3DPrimitive2::UploadToVertexBufferAnimated(unsigned long) -- size the animated vertex format, (re)create the vertex buffer for every animated block's vertices and fill it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A914B0, length 176 bytes (`ret 4`).
// GRAPHICS LAYER (replacement work later): Ego_r.pdb ?UploadToVertexBufferAnimated@C3DPrimitive2@@AAE_NK@Z.
// Callees: N3DPrimitive::CVertexShaderInput base ctor @ 0x00AB3B10 (catalog "CAIStateGroup_HoverHornetQueen" is propagated),
// CVertexShaderInputAnimated::Initialise(unsigned long, bool) @ 0x00A90630, the input declaration's vertex-size getter
// @ 0x00AB37B0 (catalog "CPlayerManager::PrepareForLoad", propagated), ~CVertexShaderInputAnimated @ 0x00A986D0 (catalog
// "~CPatchTesselationEdgeStrip", propagated), C3DPrimitive2::SetupVertexBuffer @ 0x00A8EDA0, CVertexBufferWin32::Lock(0,0,0)
// @ 0x00AC20A0 / Unlock @ 0x00AC20B0 (catalog "CThingFilter_IsValid::operator()", propagated),
// C3DPrimitive2::ConvertToVBFormatAnimated(void*, unsigned long, unsigned long, CVertexCompressionParams*) @ 0x00A91180.
// Vtable: N3DPrimitive::CVertexShaderInputAnimated 0x0129DFB0 (stored by the inline derived ctor).
// Retail offsets (PDB - 4): VertexBuffer +0x30 (CVertexBufferWin32: compression params +0x14), VertexBufferStatus +0x38,
// AnimatedBlockCount +0x48, AnimatedBlocks +0x4c (0x30-byte blocks, VertexCount +0x10).
// Leaf: ?UploadToVertexBufferAnimated@C3DPrimitive2@@AAE_NK@Z
// NF149 (q4552 / q4559 / q4560): the vertex buffer is the shared retail CVertexBufferWin32 (engine/CVertexBufferWin32.h, which brings
// engine/CVertexCompressionParams.h; Ego_r.pdb + FableWin.pdb CVertexBufferBase::Lock / Unlock 0x00AC20A0 / 0x00AC20B0); was a local
// CVertexBufferWin32 model (binding ?Lock@CVertexBufferWin32@@QAEPAXKKK@Z / ?Unlock@CVertexBufferWin32@@QAEXXZ) and a local
// CVertexCompressionParams model.
// NF192 (q6242): SetupVertexBuffer is private with an EVertexType first parameter (Ego_r.pdb 0x00BD15B0 / FableWin.pdb 0x032F8CD0
//   ?SetupVertexBuffer@C3DPrimitive2@@AAE_NW4EVertexType@@KKK@Z; engine/C3DPrimitive2.h _LOADER declares it so): respelled in place in the
//   row-local C3DPrimitive2 model (was the public `long` form ?SetupVertexBuffer@C3DPrimitive2@@QAE_NJKKK@Z).
#include "engine/EVertexType.h"
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/CVertexBufferWin32.h"
struct CVertexFormat {
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
struct CVertexShaderInputAnimated : CVertexShaderInput {   // vtable 0x0129DFB0
    CVertexShaderInputAnimated() {}
    virtual ~CVertexShaderInputAnimated();          // 0x00A986D0
    void Initialise(unsigned long flags, bool animated);   // 0x00A90630
};
}
struct CAnimatedBlock {                             // 0x30
    unsigned char _00[0x10];
    unsigned long VertexCount;                      // +0x10
    unsigned char _14[0x1c];
};
class C3DPrimitive2 { public:
    unsigned char _pad[0x30];
    CVertexBufferWin32* VertexBuffer;               // +0x30 scoped_ptr
    unsigned char _34[4];
    unsigned long VertexBufferStatus;                        // +0x38
    unsigned char _3c[0x0c];
    unsigned long AnimatedBlockCount;               // +0x48
    CAnimatedBlock* AnimatedBlocks;                 // +0x4c
private:
    bool SetupVertexBuffer(EVertexType vertexType, unsigned long vertexSize, unsigned long vertexCount, unsigned long flags);   // 0x00A8EDA0
public:
    void ConvertToVBFormatAnimated(void* data, unsigned long vertexSize, unsigned long flags, CVertexCompressionParams* params);   // 0x00A91180
    private:
    bool UploadToVertexBufferAnimated(unsigned long flags);
    public:
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00AB37B0 ?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ (was ?GetVertexSize@CVertexFormat@@QBEKXZ)
class CVertexStreamFormat { public: unsigned long CalculateVertexSize() const; };
bool C3DPrimitive2::UploadToVertexBufferAnimated(unsigned long flags)
{
    unsigned long vertexSize;
    {
        N3DPrimitive::CVertexShaderInputAnimated input;
        input.Initialise(flags, true);
        vertexSize = reinterpret_cast<const CVertexStreamFormat*>(&(input.Declaration->Format))->CalculateVertexSize();
    }
    unsigned long vertexCount = 0;
    for (unsigned long i = 0; i < AnimatedBlockCount; ++i)
        vertexCount += AnimatedBlocks[i].VertexCount;
    if (!SetupVertexBuffer((EVertexType)0, vertexSize, vertexCount, flags))
        return false;
    void* data = VertexBuffer->Lock(0, 0, 0);
    ConvertToVBFormatAnimated(data, vertexSize, flags, &VertexBuffer->CompressionParams);
    VertexBuffer->Unlock();
    VertexBufferStatus |= 1;
    return true;
}