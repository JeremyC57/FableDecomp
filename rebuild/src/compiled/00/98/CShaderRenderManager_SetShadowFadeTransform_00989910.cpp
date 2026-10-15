// CShaderRenderManager::SetShadowFadeTransform(const C4DVector* transform) -- uploads one vector (copied to the frame) to the current layout's shadow-fade-transform register (+0xf0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00989910, length 73 bytes.
// Layout: engine/CVertexShaderConstantLayout.h (its CShaderConstantRange members viewed as {vptr, Offset, Count}).
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, vtable +0x178, `__stdcall`, `this` pushed).
// Shape: a 16-byte frame vector handed to the device as one constant. It reproduces only through an
// inline helper that takes the register FIRST and the four components by value: the components are copied
// into the frame around the device / layout loads and the register push lands where retail puts it (the
// same body written inline, with a device local, a register local or a float[4] all schedule the push late).
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

struct C4DVector { float x, y, z, w; };

struct CShaderRenderManager {
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1cc];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4

    void SetShadowFadeTransform(const C4DVector* transform);

    // inline upload of one vector: the four components arrive by value (right-to-left argument
    // evaluation puts the register expression last, which is what fixes retail's push schedule)
    void SetVertexShaderConstant(unsigned long startRegister, float x, float y, float z, float w)
    {
        C4DVector v;
        v.x = x;
        v.y = y;
        v.z = z;
        v.w = w;
        D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, startRegister, &v.x, 1);
    }
};

void CShaderRenderManager::SetShadowFadeTransform(const C4DVector* transform)
{
    SetVertexShaderConstant(ConstantRange(CurrentVSConstantLayout->ShadowFadeTransform).Offset, transform->x, transform->y, transform->z, transform->w);
}