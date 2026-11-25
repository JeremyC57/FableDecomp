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
// Leaf: ?UploadToVertexBufferStatic@C3DPrimitive2@@AAE_NK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
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
struct CVertexShaderInputStatic : CVertexShaderInput {   // vtable 0x0129DFB8
    CVertexShaderInputStatic() {}
    virtual ~CVertexShaderInputStatic();            // 0x00A98640
    void Initialise(unsigned long flags, bool useDeclaration);   // 0x00A90560
};
}
struct CStaticVertex { unsigned char _body[0x20]; };
#include "engine/CArray.h"
namespace N3DPrimitive {
class CDeviceIndependent { public:
    long m_00;
    CArray<CStaticVertex> Vertices;                 // +4
};
}
class C3DPrimitive2 { public:
    unsigned char _pad[8];
    long RepeatingMeshReps;                         // +0x08
    unsigned char _0c[0x20];
    N3DPrimitive::CDeviceIndependent* Data;         // +0x2c scoped_ptr
    CVertexBufferWin32* VertexBuffer;               // +0x30 scoped_ptr
    unsigned char _34[4];
    unsigned long VertexBufferStatus;                        // +0x38
private:
    bool SetupVertexBuffer(EVertexType vertexType, unsigned long vertexSize, unsigned long vertexCount, unsigned long flags);   // 0x00A8EDA0
public:
    void ConvertToVBFormatStatic(void* data, unsigned long vertexSize, unsigned long flags, CVertexCompressionParams* params);   // 0x00A92740
    private:
    bool UploadToVertexBufferStatic(unsigned long flags);
    public:
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00AB37B0 ?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ (was ?GetVertexSize@CVertexFormat@@QBEKXZ)
class CVertexStreamFormat { public: unsigned long CalculateVertexSize() const; };
bool C3DPrimitive2::UploadToVertexBufferStatic(unsigned long flags)
{
    unsigned long vertexSize;
    if (RepeatingMeshReps > 0)
        vertexSize = 0x24;
    else {
        N3DPrimitive::CVertexShaderInputStatic input;
        input.Initialise(flags, true);
        vertexSize = reinterpret_cast<const CVertexStreamFormat*>(&(input.Declaration->Format))->CalculateVertexSize();
    }
    long reps = RepeatingMeshReps;
    if (reps < 1)
        reps = 1;
    unsigned long vertexCount = Data->Vertices.size() * reps;
    if (!SetupVertexBuffer((EVertexType)0, vertexSize, vertexCount, flags))
        return false;
    void* data = VertexBuffer->Lock(0, 0, 0);
    ConvertToVBFormatStatic(data, vertexSize, flags, &VertexBuffer->CompressionParams);
    VertexBuffer->Unlock();
    VertexBufferStatus |= 1;
    return true;
}