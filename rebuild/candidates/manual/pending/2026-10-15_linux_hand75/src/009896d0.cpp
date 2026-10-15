// CShaderRenderManager::SetUserVertexShaderConstants(unsigned long index, unsigned long count, const float* data) -- uploads count user vectors at the current layout's user-constant base (+0x14) plus index; nothing for a zero count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009896D0, length 45 bytes.
// Layout: engine/CVertexShaderConstantLayout.h (its CShaderConstantRange members viewed as {vptr, Offset, Count}).
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, vtable +0x178, `__stdcall`, `this` pushed).
// Shape: the count tested first (`test edx,edx ; je` straight to `ret 0Ch`), then the device and the layout
// loaded, the arguments pushed right to left with the register computed as `base + index` from the stack slot.
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

struct CShaderRenderManager {
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1cc];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4

    void SetUserVertexShaderConstants(unsigned long index, unsigned long count, const float* data);
};

void CShaderRenderManager::SetUserVertexShaderConstants(unsigned long index, unsigned long count, const float* data)
{
    if (count != 0)
        D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, ConstantRange(CurrentVSConstantLayout->User).Offset + index, data, count);
}
