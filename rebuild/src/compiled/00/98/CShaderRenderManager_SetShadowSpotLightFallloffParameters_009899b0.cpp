// CShaderRenderManager::SetShadowSpotLightFallloffParameters(matrix,float,float,colour) -- upload spotlight transform, attenuation and colour
// Leaf: SetShadowSpotLightFallloffParameters
// Retail009899B0,164 bytes ending ret16 at00989A51,12 trailing int3 bytes excluded.
// Reuses canonical neighbouring upload helper00989960; shared layout offsets104/110/11c
// plus range Offset4 supply registers108/114/120. D3DDevice+1d4 COM slot94 stdcall.
// SetVSMatrixConstantTransposed009887B0 uploads3 vectors before the scalar uploads.
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

class C4DVector { public: float x, y, z, w; };
class CRGBFloatColour { public:float R,G,B,A;};
class CMatrix3x4 { public:float Values[12];};

class CShaderRenderManager { public:
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1cc];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4

    void SetShadowSpotLightFallloffParameters(const CMatrix3x4& transform,float a,float b,const CRGBFloatColour& colour);
    void SetVSMatrixConstantTransposed(long,const CMatrix3x4&,unsigned long); //009887B0

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

void CShaderRenderManager::SetShadowSpotLightFallloffParameters(const CMatrix3x4& transform,float a,float b,const CRGBFloatColour& colour) {
    SetVSMatrixConstantTransposed(ConstantRange(CurrentVSConstantLayout->ShadowedSpotlightFalloffTransform).Offset,transform,3);
    SetVertexShaderConstant(ConstantRange(CurrentVSConstantLayout->ShadowedSpotlightAttenuation).Offset,a,b,0.0f,0.0f);
    SetVertexShaderConstant(ConstantRange(CurrentVSConstantLayout->ShadowedSpotlightColour).Offset,colour.R,colour.G,colour.B,colour.A);
}