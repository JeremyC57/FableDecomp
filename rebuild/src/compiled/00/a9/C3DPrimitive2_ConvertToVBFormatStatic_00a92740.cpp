// C3DPrimitive2::ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*)
// Flags: /d2inlS100 /d2inlT200
// Retail 0x00A92740, 295 bytes (ret 0x10). Field-free C3DPrimitive2 adapter: RepeatingMeshReps +8,
// Data +0x2c, VertexCount +0x58. CVertexShaderInput is reconstructed from the landed adjacent
// destructor: CBaseClass vptr then DataStreams CArray +4 and D3DDeclaration CArray +0x10; true
// virtual destructor chain and static vtable 0x0129DFB8. Native getter @0x00AB37B0 receives
// DataStreams.first +4 (the stream's Format subobject), as shown by A927A7's mov [local+4]; add 4.
// Empty staging uses the vector object as its native sentinel.
// Invalid wrapped 32-bit byte count skips both transform paths and conversion while preserving cleanup.
#include "fable_compiler.h"
#include <string.h>

extern "C" void __cdecl free(void*); // 0x00BFEA14
class CVertexCompressionParams;
extern "C" void __fastcall ConvertStaticVertices(const void*, unsigned long, void*, unsigned long, unsigned long, CVertexCompressionParams&); // 0x00A91390

class CVertexCompressionParams { public: float PosScale[4]; float PosBias[4]; };
struct CVertexFormat { unsigned long GetVertexSize() const; }; // 0x00AB37B0
struct CStreamInfo { long Unnamed00; CVertexFormat Format; };
struct D3DVERTEXELEMENT9;
template<class T> class CArray { public:
    T* first;
    T* last;
    T* end;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CVertexShaderInput : public CBaseClass { public: // vtable 0x0129EA08, size 0x1c
    CArray<CStreamInfo> DataStreams;               // +4
    CArray<D3DVERTEXELEMENT9> D3DDeclaration;      // +0x10
    virtual ~CVertexShaderInput();                 // 0x00A5EB70 (AB4690 is deleting wrapper)
    CVertexShaderInput();                          // 0x00AB3B10
};
namespace N3DPrimitive {
struct CVertexShaderInputStatic : CVertexShaderInput { // vtable 0x0129DFB8
    CVertexShaderInputStatic() { }
    virtual ~CVertexShaderInputStatic();          // 0x00A98640
    void Initialise(unsigned long, bool);          // 0x00A90560
};
}

// NF108 (2026-11-19, q4453): the local CByteVector stand-in replaced by the shared STLport std::vector<unsigned char>
//   (stlport/fable_containers.h): its resize calls ?_M_fill_insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z 0x00485F3A.
#include "stlport/fable_containers.h"

struct CStaticVertex { unsigned char bytes[32]; };
struct CArrayStaticVertex { CStaticVertex* first; CStaticVertex* last; CStaticVertex* end; };
struct CDeviceIndependent { long pad; CArrayStaticVertex Vertices; };
class CTVertexNormTex1;
class CTVertexNormTex1Bump; class CTVertexMatrixIndexedNormTex1;
class C3DPrimitive2 { public:
    unsigned char pad0[8];
    long RepeatingMeshReps;
    unsigned char pad0c[0x20];
    CDeviceIndependent* Data;
    unsigned char pad30[0x28];
    unsigned long VertexCount;
    private: void ConvertToVBFormatRepeatingMesh(CTVertexMatrixIndexedNormTex1* dest); public: // 0x00A8F770
private:
    // NF93 (q11422 / q11423): Ego_r.pdb / FableWin.pdb private ?ConvertToVBFormatStaticBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1Bump@@@Z
    //   (0x00A8F1B0) and ?ConvertToVBFormatStaticNonBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1@@@Z (0x00A8EFA0); were the made-up
    //   TransformVertexBufferWithNormals / TransformVertexBuffer(void*).
    void ConvertToVBFormatStaticBump(CTVertexNormTex1Bump* dest);   // 0x00A8F1B0
    void ConvertToVBFormatStaticNonBump(CTVertexNormTex1* dest);    // 0x00A8EFA0
public:
    void ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00AB37B0 ?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ (was ?GetVertexSize@CVertexFormat@@QBEKXZ)
class CVertexStreamFormat { public: unsigned long CalculateVertexSize() const; };
void C3DPrimitive2::ConvertToVBFormatStatic(void* destination, unsigned long sourceFormat,
                                            unsigned long flags, CVertexCompressionParams* params)
{
    if (RepeatingMeshReps > 0) {
        ConvertToVBFormatRepeatingMesh(static_cast<CTVertexMatrixIndexedNormTex1*>(destination));
        return;
    }
    unsigned long convertedFormat = flags & ~4;
    std::vector<unsigned char> bytes;
    void* transformed = destination;
    if (flags != convertedFormat) {
        unsigned long stride;
        {
            N3DPrimitive::CVertexShaderInputStatic input;
            input.Initialise(convertedFormat, true);
            stride = reinterpret_cast<const CVertexStreamFormat*>(&(input.DataStreams.first->Format))->CalculateVertexSize();
        }
        unsigned long count = VertexCount * stride;
        __int64 checkedCount = (__int64)count;
        if (!(checkedCount >= 0 && checkedCount <= 0x7fffffffLL))
            return;
        unsigned char zero = 0;
        bytes.resize(count, zero);
        transformed = bytes.size() != 0 ? (void*)&*bytes.begin() : (void*)&bytes;
    }
    {
        if (flags & 2)
            ConvertToVBFormatStaticBump(static_cast<CTVertexNormTex1Bump*>(transformed));
        else
            ConvertToVBFormatStaticNonBump(static_cast<CTVertexNormTex1*>(transformed));
        if (flags != convertedFormat)
            ConvertStaticVertices(transformed, convertedFormat, destination, flags, VertexCount, *params);
    }
}