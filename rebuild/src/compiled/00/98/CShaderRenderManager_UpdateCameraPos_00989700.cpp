// CShaderRenderManager::UpdateCameraPos() -- uploads the camera position (+0x380, w = 0) as one vector to the current layout's camera register (+0x44) and clears dirty bit 8
// Leaf: ?UpdateCameraPos@CShaderRenderManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00989700, length 85 bytes.
// Name fix (NF34, 2026-11-16): Ego_r.pdb 0x00AE5A00 + FableWin.pdb ?UpdateCameraPos@CShaderRenderManager@@AAEXXZ (private); the
// body uploads CameraPos (w = 0); UpdateAmbient is the twin 0x00989760. PrepareForRender 0x00B54100 already binds this spelling.
// Layout: engine/CVertexShaderConstantLayout.h (its CShaderConstantRange members viewed as {vptr, Offset, Count}).
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, vtable +0x178, `__stdcall`, `this` pushed).
// The twin of the landed UpdateAmbient 0x00989760 (ambient colour, w = 1.0f), whose label this row used to carry:
// the data is the PDB's CameraPos (retail +0x380, 0x10 below the PDB offset) with w = 0.
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

class CShaderConstantRange { public:                   // the 12-byte ranges of engine/CVertexShaderConstantLayout.h (PDB layout)
    void*         __vftable;                    // +0
    long          Offset;                       // +4: first constant register
    unsigned long Count;                        // +8: vector count
};

inline const CShaderConstantRange& ConstantRange(const unsigned char* range)
{
    return *reinterpret_cast<const CShaderConstantRange*>(range);
}

class C3DVector { public: float x, y, z; };
class C4DVector { public: float x, y, z, w; };

class CShaderRenderManager { public:
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1cc];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4
    unsigned char                _pad1d8[0x1a8];
    C3DVector                    CameraPos;                 // +0x380
    unsigned char                _pad38c[0x4c];
    unsigned long                UpdateFlags;               // +0x3d8

private:
    void UpdateCameraPos();
public:

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

void CShaderRenderManager::UpdateCameraPos()
{
    SetVertexShaderConstant(ConstantRange(CurrentVSConstantLayout->CameraPos).Offset, CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
    UpdateFlags &= ~8u;
}