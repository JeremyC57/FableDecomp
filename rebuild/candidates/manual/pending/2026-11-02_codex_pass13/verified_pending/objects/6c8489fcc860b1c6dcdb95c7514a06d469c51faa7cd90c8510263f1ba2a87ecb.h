#ifndef FABLE_ENGINE_CSHADERRENDERMANAGER_H
#define FABLE_ENGINE_CSHADERRENDERMANAGER_H
// Original Ego_r TPI0x40d2f, RTTI native vtable0129a080 and ctor0098ab00.
// Layout: all original members at native offsets, confirmed by Initialise0098acf0.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShaderConstantLayout.h"
#include "engine/CMatrix4x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CIndexedArray.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_SHADER_LIGHTING
#include "engine/CShaderGenericLight.h"
#include "engine/Direct3D9Device.h"
#endif
class CRenderManager;class CShaderResource;class CTexture;class CPreTransposedBoneMatrix;
struct IDirect3DDevice9;struct IDirect3DVertexShader9;struct IDirect3DVertexDeclaration9;
class CShaderRenderManager : public CBaseClassNonCopyable {
public:
    virtual ~CShaderRenderManager();
    void Initialise(CRenderManager*,IDirect3DDevice9*);
private:
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    class CLightInfo { public: CShaderGenericLight Light;bool IsEnabled;bool UpdateFlag; };
    void UpdateObjectSpaceLighting(unsigned long);
    void SetVSConstants(long start,const void* data,unsigned long count) {
        if(count!=0) D3DDevice->SetVertexShaderConstantF(start,(const float*)data,count);
    }
#else
    class CLightInfo;
#endif
    const CVertexShaderConstantLayout* CurrentVSConstantLayout; //04
    long CurrentVSConstantLayoutIndex; //08
    CArray<const CVertexShaderConstantLayout*> VertexShaderConstantLayouts; //0c
    CVertexShaderConstantLayout NullConstantLayout; //18
    CLightInfo* Lights; //164
    C4DVector* LightBuffer; //168
    unsigned long LightCount,AllocatedLights,AllocatedLightSize; //16c/170/174
    unsigned char LightPool[0x1c]; //178 CSimpleMemoryPool, complete original object.
    CArray<C4DVector> LightGlobals; //194
    CArray<C4DVector> LightAttenuations; //1a0
    CRGBFloatColour AmbientColour; //1ac
    CRGBFloatColour FogColour; //1bc
    CRGBColour TextureFactor; //1cc
    CRenderManager* RenderManager; //1d0
    IDirect3DDevice9* D3DDevice; //1d4
    IDirect3DVertexShader9* CurrentVertexShader; //1d8
    IDirect3DVertexDeclaration9* CurrentVertexShaderVertexDeclairation; //1dc original spelling.
    unsigned char CurrentPixelShader[8]; //1e0 CPixelShader, original vptr and Data pointer.
    bool WorldMatrixIsIdentity; //1e8
    unsigned char _pad_1e9[7]; // Native first matrix begins1f0; donor also has7 alignment bytes after flag.
    CMatrix4x4 WorldMatrix; //1f0
    CMatrix4x4 ViewMatrix; //230
    CMatrix4x4 ProjectionMatrix; //270
    CMatrix4x4 ViewProjectionMatrix; //2b0
    CMatrix4x4 CombinedProjectionMatrix; //2f0
    CMatrix4x4 TextureTransforms[1]; //330
    C4DVector FogTransform; //370
    C3DVector CameraPos; //380
    long TextureTransformCount; //38c
    bool VertexShadersEnabled,PixelShadersEnabled,SWLightingEnable,SoftwareVertexShaderEnable,SoftwareFixedFunctionEnable,FixedFunctionVertexMode; //390..395
    float MultisampleViewPortScale; //398
    CArray<CTexture>* CycleTextures; //39c
    unsigned long NextCycleTexture,CycleTextureStage; //3a0/3a4
    CArray<CCountedPointer<CShaderResource> > ResourceManagers; //3a8
    CIndexedArray<CCharString> PSConstNames; //3b4
    CArray<C4DVector> PSConstTable; //3cc
    unsigned long UpdateFlags; //3d8
    unsigned char BoneMatrices[0xc]; //3dc CArray<CPreTransposedBoneMatrix>, complete container.
};
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager)==0x3e8);
#endif
