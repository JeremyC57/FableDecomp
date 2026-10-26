#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
// Host adapter for pointer/long widths; VC always includes the actual shared layout.
struct HostConstantRange { void* VTable;long Offset;unsigned long Count; };
class CVertexShaderConstantLayout { public:
    void* Alignment;
    unsigned char ShadowedSpotlightFalloffTransform[sizeof(HostConstantRange)];
    unsigned char ShadowedSpotlightAttenuation[sizeof(HostConstantRange)];
    unsigned char ShadowedSpotlightColour[sizeof(HostConstantRange)];
};
#endif
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

static CShaderRenderManager* active;
static CVertexShaderConstantLayout* layouts[3];
static IDirect3DDevice9* devices[3];
static const CMatrix3x4* matrix;
static CRGBFloatColour* inputColour;
static int bad,matrixCalls,uploads,mutate;
static float attenuationA,attenuationB;
static void setRange(unsigned char* range,long offset) {
    CShaderConstantRange& r=*reinterpret_cast<CShaderConstantRange*>(range);
    r.__vftable=0;r.Offset=offset;r.Count=9;
}
void CShaderRenderManager::SetVSMatrixConstantTransposed(long start,const CMatrix3x4& value,unsigned long count) {
    ++matrixCalls;
    if(this!=active || start!=11 || &value!=matrix || count!=3 || uploads) bad=1;
    if(mutate) {CurrentVSConstantLayout=layouts[1];D3DDevice=devices[1];}
}
static long __stdcall upload(IDirect3DDevice9* self,unsigned long start,const float* data,unsigned long count) {
    if(matrixCalls!=1 || count!=1 || uploads>=2) {bad=1;return -1;}
    if(uploads==0) {
        if(self!=devices[mutate?1:0] || start!=(unsigned long)(mutate?22:12) ||
           data[0]!=attenuationA || data[1]!=attenuationB || data[2]!=0 || data[3]!=0) bad=1;
        if(mutate) {
            active->CurrentVSConstantLayout=layouts[2];active->D3DDevice=devices[2];
            inputColour->R=7.5f;inputColour->G=-8.5f;inputColour->B=9.25f;inputColour->A=0.75f;
        }
    } else if(self!=devices[mutate?2:0] || start!=(unsigned long)(mutate?33:13) ||
       data[0]!=inputColour->R || data[1]!=inputColour->G || data[2]!=inputColour->B || data[3]!=inputColour->A) bad=1;
    ++uploads;return -1;
}
static int run(int change,float a,float b) {
    CShaderRenderManager manager={};active=&manager;
    CVertexShaderConstantLayout x={},y={},z={};layouts[0]=&x;layouts[1]=&y;layouts[2]=&z;
    for(int i=0;i<3;++i) {
        setRange(layouts[i]->ShadowedSpotlightFalloffTransform,11+i*10);
        setRange(layouts[i]->ShadowedSpotlightAttenuation,12+i*10);
        setRange(layouts[i]->ShadowedSpotlightColour,13+i*10);
    }
    IDirect3DDevice9Vtbl table={};table.SetVertexShaderConstantF=upload;
    IDirect3DDevice9 d0={&table},d1={&table},d2={&table};devices[0]=&d0;devices[1]=&d1;devices[2]=&d2;
    manager.CurrentVSConstantLayout=&x;manager.D3DDevice=&d0;
    CRGBFloatColour colour={1.25f,-2.5f,3.75f,0.5f};inputColour=&colour;
    CMatrix3x4 transform;for(int j=0;j<12;++j) transform.Values[j]=(float)j+0.25f;matrix=&transform;
    bad=matrixCalls=uploads=0;mutate=change;attenuationA=a;attenuationB=b;
    manager.SetShadowSpotLightFallloffParameters(transform,a,b,colour);
    if(bad || matrixCalls!=1 || uploads!=2 || manager.CurrentVSConstantLayout!=layouts[change?2:0] || manager.D3DDevice!=devices[change?2:0]) return 0;
    for(int k=0;k<12;++k) if(transform.Values[k]!=(float)k+0.25f) return 0;
    return true;
}
int main() {
    if(!run(0,0.0f,1.5f) || !run(1,-2.25f,3.125f)) {printf("009899b0_TEST FAIL\n");return 1;}
    printf("009899b0_TEST PASS\n");return 0;
}