// CEngineBillboardGenerator::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CEngineComponent-subobject vtable entry: this -= 0x4, jump to the deleting destructor 0x00B8C7A0
// Leaf: ??_ECEngineBillboardGenerator@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8C790, 3+5 bytes: `sub ecx,0x4 ; jmp 0x00B8C7A0`. Compiler-generated adjustor thunk.
// Identity (two sources): (1) slot 0 of the CEngineComponent (+4) vtable 0x012A3BFC (RTTI complete-object locator offset 0x4, class
//   .?AVCEngineBillboardGenerator@@; the catalog's "vfunc_0"); (2) its jump target 0x00B8C7A0 is slot 0 of the primary vtable 0x012A3C04, the class's
//   ??_G. VC7.1 names the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the adjustor in the
//   thunk name (??_ECEngineBillboardGenerator@@W3AEPAXI@Z); objdump -t of this object shows the leaf.
// TU: the thunk is emitted with the vtable in any TU that defines the class's constructor or destructor (chunk 765 shape). This
//   source reuses the landed definer CEngineBillboardGenerator::~CEngineBillboardGenerator 0x00B8C7C0 (landed destructor, verbatim) as that TU;
//   only the thunk is graded here. Landed definer: rebuild/src/compiled/00/b8/CEngineBillboardGenerator_CEngineBillboardGenerator_00b8c7c0.cpp
// Callees: ??_GCEngineBillboardGenerator@@UAEPAXI@Z 0x00B8C7A0 (bound as its ??_E alias).
// ---- definer TU (verbatim from the landed row, minus its Leaf/Flags lines) ----
// CEngineBillboardGenerator::~CEngineBillboardGenerator() -- billboard generator destructor (compiler-generated): the PixelShader and VertexShader members' destructors (each drops its shader data reference, then the CShaderBase vtable), then the CEngineComponent base at +4 and the CEnginePrimitiveRenderUtil / CBaseClassNonCopyable base
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8C7C0, 102 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300), two copies.
// Identity (proven): sole caller is the slot-0 scalar deleting destructor 0x00B8C7A0 (call at 0x00B8C7A3) of vtable 0x012A3BF4
//   (RTTI .?AVCEngineBillboardGenerator@@ : CEnginePrimitiveRenderUtil : CBaseClassNonCopyable : CBaseClass, + CEngineComponent at
//   mdisp 4); Ego_r.pdb ??1CEngineBillboardGenerator@@UAE@XZ / ??_GCEngineBillboardGenerator@@UAEPAXI@Z; the body walks Ego_r's
//   CEngineBillboardGenerator members VertexShader +0x10 / PixelShader +0x18 and stores the CPixelShader (0x0129A054), CVertexShader
//   (0x0129B3D0) and CShaderBase (0x0129A030) vtables. No store of its own vtable: the destructor is compiler-generated (as is
//   CEnginePrimitiveRenderUtil's, whose vtable is never stored either). The catalog label is right.
// Callees: the shader data's slot-0 deleting destructor (push 1), ~CEngineComponent 0x00B59730 (this+4), ~CBaseClassNonCopyable 0x0099A300.
// Layout: CEngineComponent re-declared over its real vtable (engine/CEngineComponent.h is the flattened donor layout with a
//   `void* __vftable` word; same members GlobalPointerAddress +0x04, RenderEnabled +0x08, KeepAlive +0x09).
// Layout: Ego_r (engine/CEngineBillboardGenerator.h and engine/CVertexShader.h are the flattened donor layouts with `void* __vftable`
//   words and byte-array members, so the classes are re-declared with their real bases): CEngineComponent (12 bytes:
//   GlobalPointerAddress, RenderEnabled, KeepAlive) at +4; CPixelShader / CVertexShader = CShaderBase vptr + Data; the shader
//   data reference counts are Ego_r's CPixelShaderData::ReferenceCount +0x08 and CVertexShaderData::ReferenceCount +0x10.
// Shape: PDB ??1CPixelShader@@UAE@XZ / ??1CVertexShader@@UAE@XZ (user dtors: own vtable store) = `if (Data) Data->Release();` with the
//   PDB's private ?Release@CPixelShaderData@@AAEXXZ = `if (--ReferenceCount == 0) delete this;` (private virtual ??1..EAE: `push 1 ;
//   call [vtbl]`); ??1CShaderBase@@MAE@XZ protected virtual (its vtable store). VC7.1's generated destructor converts `this` to the
//   CEngineComponent base with the null-checked adjustment (`test esi,esi ; je -> xor ecx,ecx`), which duplicates the tail.
//   The implicit ??1 is emitted, as in retail, by defining the PDB constructor ??0CEngineBillboardGenerator@@QAE@XZ in the TU
//   (vtable -> ??_G -> ??1); its empty body here is not retail's (not graded).
#include "engine/CBaseClassNonCopyable.h"
struct IDirect3DPixelShader9;
struct IDirect3DVertexShader9;
struct IDirect3DVertexDeclaration9;
struct _D3DVERTEXELEMENT9;
class CShaderBase {
protected:
    CShaderBase() { }
    virtual ~CShaderBase() { }                               // ??1CShaderBase@@MAE@XZ
};
class CPixelShaderData {
    friend class CPixelShader;
    IDirect3DPixelShader9* PixelShader;                      // +0x04
    unsigned long          ReferenceCount;                   // +0x08
    unsigned long          ConstCount;                       // +0x0c
    long*                  ConstIndices;                     // +0x10
    virtual ~CPixelShaderData();                             // ??1CPixelShaderData@@EAE@XZ
    void Release() { if (--ReferenceCount == 0) delete this; }   // ?Release@CPixelShaderData@@AAEXXZ
};
class CVertexShaderData {
    friend class CVertexShader;
    IDirect3DVertexDeclaration9* VertexDeclaration;          // +0x04
    IDirect3DVertexShader9*      VertexShader;               // +0x08
    _D3DVERTEXELEMENT9*          FormatDecl;                 // +0x0c
    unsigned long                ReferenceCount;             // +0x10
    virtual ~CVertexShaderData();                            // ??1CVertexShaderData@@EAE@XZ
    void Release() { if (--ReferenceCount == 0) delete this; }   // ?Release@CVertexShaderData@@AAEXXZ
};
class CPixelShader : public CShaderBase {
public:
    CPixelShader() : Data(0) { }
    virtual ~CPixelShader() { if (Data) Data->Release(); }   // ??1CPixelShader@@UAE@XZ
    CPixelShaderData* Data;                                  // +0x04
};
class CVertexShader : public CShaderBase {
public:
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader() { if (Data) Data->Release(); }  // ??1CVertexShader@@UAE@XZ
    CVertexShaderData* Data;                                 // +0x04
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {   // RTTI; vtable never stored here (implicit dtor)
public:
    CEnginePrimitiveRenderUtil() { }
};
class CEngineComponent {
public:
    CEngineComponent();
    virtual ~CEngineComponent();                             // 0x00B59730 ??1CEngineComponent@@UAE@XZ
    void**        GlobalPointerAddress;                      // +0x04
    bool          RenderEnabled;                             // +0x08
    unsigned char KeepAlive;                                 // +0x09
};
class CEngineBillboardGenerator : public CEnginePrimitiveRenderUtil, public CEngineComponent {
public:
    CEngineBillboardGenerator();
    CVertexShader VertexShader;                              // +0x10
    CPixelShader  PixelShader;                               // +0x18
};
CEngineBillboardGenerator::CEngineBillboardGenerator()       // ??0CEngineBillboardGenerator@@QAE@XZ (emits the vtable, ??_G and the implicit ??1)
{
}