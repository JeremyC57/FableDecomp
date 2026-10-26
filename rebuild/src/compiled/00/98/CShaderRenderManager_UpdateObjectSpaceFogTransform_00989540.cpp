// CShaderRenderManager::UpdateObjectSpaceFogTransform() -- transform the world fog plane (+0x370) by the world matrix (+0x1f0), upload it to the layout's fog register and flip the update flags (clear 0x4000, set 0x2000)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00989540, length 247 bytes.
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, vtable +0x178, `__stdcall`, `this` pushed).
// Layout: as the landed UpdateWorldTransform 0x00988E30 (retail = PDB - 0x10 here: D3DDevice +0x1d4, WorldMatrix +0x1f0,
// FogTransform C4DVector +0x370, UpdateFlags +0x3d8); the constant layout's FogTransform range sits at +0x80 (register +0x84).
// Shape: each component is a three-term dot of the plane's x,y,z with one matrix column; VC evaluates the right operand of
// the inner `+` first, so `x*m0 + y*m4 + z*m8` loads the y term, then x, then z; the w component adds the plane's w last.
// The 16-byte result is a frame local passed by address with a count of 1.
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

class CMatrix4x4 { public: float m[16]; };
class C4DVector { public: float X, Y, Z, W; };

class CShaderRenderManager { public:
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1cc];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4
    unsigned char                _pad1d8[0x18];
    CMatrix4x4                   WorldMatrix;               // +0x1f0
    unsigned char                _pad230[0x140];
    C4DVector                    FogTransform;              // +0x370
    unsigned char                _pad380[0x58];
    unsigned long                UpdateFlags;               // +0x3d8

    void UpdateObjectSpaceFogTransform();
};

void CShaderRenderManager::UpdateObjectSpaceFogTransform()
{
    C4DVector fog;
    fog.X = FogTransform.X * WorldMatrix.m[0] + FogTransform.Y * WorldMatrix.m[4] + FogTransform.Z * WorldMatrix.m[8];
    fog.Y = FogTransform.X * WorldMatrix.m[1] + FogTransform.Y * WorldMatrix.m[5] + FogTransform.Z * WorldMatrix.m[9];
    fog.Z = FogTransform.X * WorldMatrix.m[2] + FogTransform.Y * WorldMatrix.m[6] + FogTransform.Z * WorldMatrix.m[10];
    fog.W = FogTransform.X * WorldMatrix.m[3] + FogTransform.Y * WorldMatrix.m[7] + FogTransform.Z * WorldMatrix.m[11] + FogTransform.W;
    D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, ConstantRange(CurrentVSConstantLayout->FogTransform).Offset, &fog.X, 1);
    UpdateFlags = (UpdateFlags & ~0x4000u) | 0x2000u;
}