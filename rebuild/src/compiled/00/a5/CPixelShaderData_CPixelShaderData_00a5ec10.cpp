// CPixelShaderData::~CPixelShaderData() -- shared pixel-shader data dtor: release the Direct3D pixel shader, delete[] the constant index array, then ~CBaseClassNonCopyable
// Leaf: ??1CPixelShaderData@@EAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5EC10.
// Identity: Ego_r.pdb ??1CPixelShaderData@@EAE@XZ; overlay FABLE_RETAIL_CPIXELSHADERDATA adds the CBaseClassNonCopyable base.
// Callees: IDirect3DPixelShader9::Release (slot 2), operator delete[], ~CBaseClassNonCopyable.
// Shape: `if (PixelShader) PixelShader->Release(); delete[] ConstIndices;`.
void __cdecl operator delete[](void* block);                 // 0x00BFEB1C (MSVCR71 ??_V thunk)
struct IUnknown {                                            // <unknwn.h>: the three IUnknown slots
    virtual long __stdcall QueryInterface(const struct _GUID& iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;           // slot 2 (+0x08)
};
struct IDirect3DPixelShader9 : public IUnknown { };          // <d3d9.h> (IDirect3DPixelShader9 : IUnknown)
#define FABLE_RETAIL_CPIXELSHADERDATA
#include "engine/CPixelShader.h"

CPixelShaderData::~CPixelShaderData()
{
    if (PixelShader)
        PixelShader->Release();
    delete[] ConstIndices;
}