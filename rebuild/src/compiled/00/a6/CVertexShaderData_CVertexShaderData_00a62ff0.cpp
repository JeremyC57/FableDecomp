// CVertexShaderData::~CVertexShaderData() -- vertex shader data destructor: release the D3D vertex shader, then the vertex declaration (COM Release), then ~CBaseClassNonCopyable
// Leaf: ??1CVertexShaderData@@EAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A62FF0, 43 bytes (int3 padding follows), thiscall, tail `jmp ~CBaseClassNonCopyable` 0x0099A300.
// Identity (proven, two sources): (1) it stores vtable 0x0129DA50, RTTI .?AVCVertexShaderData@@ : CBaseClassNonCopyable : CBaseClass
//   (one slot: the deleting dtor 0x00A630D0, which inlines this same body -- this TU's ??_G grades RELOCATION_MATCH against it; no
//   caller of the out-of-line copy, the usual 0-caller ??1 of an out-of-class dtor); (2) Ego_r.pdb 0x00BCEE00 / FableWin.pdb 0x032F39A0
//   ??1CVertexShaderData@@EAE@XZ (private virtual), with Ego_r's members VertexDeclaration +0x04, VertexShader +0x08, FormatDecl +0x0C,
//   ReferenceCount +0x10 (20 bytes). The catalog label ~CVertexShaderData is right (its manifest twin label on 0x0073AEA0 is not).
// Callees: IUnknown::Release (vtable slot 2, __stdcall) of each D3D interface; ~CBaseClassNonCopyable 0x0099A300 (tail).
// Shape: `if (p) p->Release();` per interface, shader first; no null store (a SAFE_RELEASE macro would add one). FormatDecl is not
//   freed here. The D3D interfaces are d3d9.h's DECLARE_INTERFACE_(..., IUnknown) structs; only IUnknown's three slots are modelled.
// Layout: engine/CBaseClassNonCopyable.h; no engine header for CVertexShaderData (the landed 0x00B8C7C0 restates it without its base).
#include "engine/CBaseClassNonCopyable.h"
typedef long HRESULT;
typedef unsigned long ULONG;
struct _GUID;
struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(const _GUID& riid, void** ppvObj) = 0;
    virtual ULONG __stdcall AddRef() = 0;
    virtual ULONG __stdcall Release() = 0;
};
struct IDirect3DVertexDeclaration9 : public IUnknown { };
struct IDirect3DVertexShader9 : public IUnknown { };
struct _D3DVERTEXELEMENT9;
class CCharString;

class CVertexShaderData : public CBaseClassNonCopyable {     // vtable 0x0129DA50
    friend class CVertexShader;
    CVertexShaderData(IDirect3DVertexDeclaration9* declaration, IDirect3DVertexShader9* shader, const _D3DVERTEXELEMENT9* formatDecl,
                      const CCharString& name, const CCharString& defines);   // ??0CVertexShaderData@@AAE@...@Z (private)
    virtual ~CVertexShaderData();                            // ??1CVertexShaderData@@EAE@XZ (private virtual)
    void Release() { if (--ReferenceCount == 0) delete this; }   // ?Release@CVertexShaderData@@AAEXXZ
    IDirect3DVertexDeclaration9* VertexDeclaration;          // +0x04
    IDirect3DVertexShader9*      VertexShader;               // +0x08
    _D3DVERTEXELEMENT9*          FormatDecl;                 // +0x0c
    unsigned long                ReferenceCount;             // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderData) == 0x14);

CVertexShaderData::~CVertexShaderData()
{
    if (VertexShader)
        VertexShader->Release();
    if (VertexDeclaration)
        VertexDeclaration->Release();
}