// CShaderRenderManager::UpdateWorldTransform() -- uploads the world matrix (+0x1f0) to the current vertex-shader constant layout's world registers (at most 4 vectors) and clears dirty bit 0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00988E30, length 71 bytes.
// Layout: engine/CVertexShaderConstantLayout.h (its CShaderConstantRange members viewed as {vptr, Offset, Count}).
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, vtable +0x178, `__stdcall`, `this` pushed).
// Shape: the layout (+4) read once; its world vector count (+0x94) clamped to 4 with `cmp ; jae` and the
// clamp arm placed after the epilogue (`mov edx,4 ; jmp`), the upload skipped for a zero count, then
// `and [esi+3D8h],~10h`. The retail object is laid out 0x24 bytes past the PDB's (device at +0x1d4, update
// flags at +0x3d8); members are modelled at the retail offsets.
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[94];                                                       // IUnknown + device slots 0..93
    long (__stdcall *SetVertexShaderConstantF)(IDirect3DDevice9* self, unsigned long startRegister,
                                               const float* data, unsigned long vector4fCount);   // slot 94 (+0x178)
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };

#include "engine/CVertexShaderConstantLayout.h"

struct CShaderConstantRange {                   // the 12-byte ranges of engine/CVertexShaderConstantLayout.h (PDB layout)
    void*         __vftable;                    // +0
    long          Offset;                       // +4: first constant register
    unsigned long Count;                        // +8: vector count
};

inline const CShaderConstantRange& ConstantRange(const unsigned char* range)
{
    return *reinterpret_cast<const CShaderConstantRange*>(range);
}

struct CMatrix4x4 { float m[16]; };

struct CShaderRenderManager {
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1cc];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4
    unsigned char                _pad1d8[0x18];
    CMatrix4x4                   WorldMatrix;               // +0x1f0
    unsigned char                _pad230[0x1a8];
    unsigned long                UpdateFlags;               // +0x3d8

    void UpdateWorldTransform();
};

void CShaderRenderManager::UpdateWorldTransform()
{
    CVertexShaderConstantLayout* layout = CurrentVSConstantLayout;
    unsigned long count = ConstantRange(layout->WorldMatrix).Count < 4 ? ConstantRange(layout->WorldMatrix).Count : 4;
    if (count != 0)
        D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, ConstantRange(layout->WorldMatrix).Offset, WorldMatrix.m, count);
    UpdateFlags &= ~0x10u;
}