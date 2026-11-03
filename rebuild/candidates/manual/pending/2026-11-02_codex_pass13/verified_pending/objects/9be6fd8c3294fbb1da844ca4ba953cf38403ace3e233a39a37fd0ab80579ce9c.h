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
#ifdef FABLE_RETAIL_RENDER2D_TYPES
#include "engine/CSimpleMemoryPool.h"
#include "engine/CPixelShader.h"
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
class CShaderGenericLight;class CShaderLight;class CVertexBufferWin32;class CIndexBuffer;class CMatrix3x4;struct _D3DMATRIX;
#endif
#ifdef FABLE_RETAIL_SHADER_LIGHTING
#include "engine/CShaderGenericLight.h"
#include "engine/Direct3D9Device.h"
#endif
class CVertexShader;class CRenderManager;class CShaderResource;class CTexture;class CPreTransposedBoneMatrix;
struct IDirect3DDevice9;struct IDirect3DVertexShader9;struct IDirect3DVertexDeclaration9;
class CShaderRenderManager : public CBaseClassNonCopyable {
public:
    virtual ~CShaderRenderManager();
    void Initialise(CRenderManager*,IDirect3DDevice9*);

#ifdef FABLE_RETAIL_RENDER2D_TYPES
 static CShaderRenderManager& __fastcall Get() {return SingletonInstance;}
 void EnableVertexShaders();void DisableVertexShaders();void EnablePixelShaders();void DisablePixelShaders();
 void SetVertexShadersEnable(bool enable,bool* old) {if(old)*old=VertexShadersEnabled;if(enable!=VertexShadersEnabled) {if(enable)EnableVertexShaders();else DisableVertexShaders();}}
 void SetPixelShadersEnable(bool enable,bool* old) {if(old)*old=PixelShadersEnabled;if(enable!=PixelShadersEnabled) {if(enable)EnablePixelShaders();else DisablePixelShaders();}}
 void RegisterVertexShaderConstantLayout(long,const CVertexShaderConstantLayout*);
 long PeekVertexShaderConstantLayout() {return CurrentVSConstantLayoutIndex;}
 void SetVertexShaderConstantLayout(long);
 void ApplyVertexShader(const CVertexShader&,bool);
 void ResetWorldTransform();
 void PrepareForRender(unsigned long flags) {
  if(UpdateFlags&4)SetVSConstants(CurrentVSConstantLayout->Reserved.Offset,CurrentVSConstantLayout->ReservedPresets.GetAsCArray(),CurrentVSConstantLayout->ReservedPresets.size());
  if(UpdateFlags&1)UpdatePixelShader();
  if(UpdateFlags&2)UpdatePixelShaderConstants();
  if((flags&0x400)&&(UpdateFlags&0x20))UpdateBoneTransform();
  if(flags&0x40) {if(UpdateFlags&0x1000)UpdateWorldSpaceTextureTransform();}
  else if(flags&0x80) {if(UpdateFlags&0x800)UpdateObjectSpaceTextureTransform();}
  if(flags&0x200) {if(UpdateFlags&0x2000)UpdateWorldSpaceFogTransform();}
  else if(flags&0x100) {if(UpdateFlags&0x4000)UpdateObjectSpaceFogTransform();}
  if(UpdateFlags&4)UpdateAmbient();
  if(UpdateFlags&0x20000)UpdateFogColour();
  if(UpdateFlags&0x400)UpdateLightGlobals();
  if(flags&8) {if(UpdateFlags&0x100)UpdateObjectSpaceLighting(flags);}
  else if(flags&0x10) {if(UpdateFlags&0x200)UpdateWorldSpaceLighting();}
  if((flags&0x20)&&(UpdateFlags&0x8000))UpdateLocalisedSphereMapping();
  if(!(flags&1)&&!WorldMatrixIsIdentity)ResetWorldTransform();
  if(UpdateFlags&0x80)UpdateCombinedProjectionTransform();
  if((flags&4)&&(UpdateFlags&8))UpdateCameraPos();
 }
#endif

#ifdef FABLE_RETAIL_RENDER2D_TYPES
public: void Uninitialise();
public: void ResetStats();
public: void GetStatsText(CCharString &,unsigned long);
public: void ApplyPixelShader(const CPixelShader &);
public: bool GetPixelShadersEnabled() const;
public: bool GetVertexShadersEnabled() const;
public: bool IsFixedFunctionVertexMode() const;
public: void SelectVertexBuffer(CVertexBufferWin32 &);
public: void DrawBuffersIndexed(const CTexture * *,unsigned long,CVertexBufferWin32 &,unsigned long,unsigned long,CIndexBuffer &,unsigned long,unsigned long,EPrimitiveType);
public: void DrawBuffersIndexed(const CTexture * *,unsigned long,unsigned long,unsigned long,CIndexBuffer &,unsigned long,unsigned long,EPrimitiveType);
public: void DrawBuffersIndexed(CVertexBufferWin32 &,unsigned long,unsigned long,CIndexBuffer &,unsigned long,unsigned long,EPrimitiveType);
public: void DrawBuffersIndexed(unsigned long,unsigned long,CIndexBuffer &,unsigned long,unsigned long,EPrimitiveType);
public: void DrawBuffersNonIndexed(const CTexture * *,unsigned long,CVertexBufferWin32 &,unsigned long,unsigned long,EPrimitiveType);
public: void DrawBuffersNonIndexed(CVertexBufferWin32 &,unsigned long,unsigned long,EPrimitiveType);
public: void SetWorldTransform(const CMatrix3x4 &);
public: void SetBoneTransform(unsigned long,const CMatrix3x4 &);
public: void SetBoneTransformTransposed(unsigned long,const CMatrix4x4 &);
public: void SetBoneTransforms(long,long,const CPreTransposedBoneMatrix *);
public: void SetViewTransform(const CMatrix3x4 &);
public: void SetCameraPos(const C3DVector &);
public: void SetProjectionTransform(const CMatrix4x4 &);
public: void SetProjectionTransform(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);
public: void SetProjectionTransform(float,float,float,float);
public: void SetTextureTransform(unsigned long,const CMatrix3x4 &);
public: void SetTextureTransformCount(long);
public: void SetFogTransform(const C4DVector &);
public: void SetXBoxUserVertexShaderConstant(long,const CRGBFloatColour &);
public: void SetBoneMatrixCount(long);
public: void SetShadowTextureTransform(const CMatrix4x4 &);
public: void SetShadowFadeTransform(const C4DVector &);
public: void SetShadowFadeFactors(float,float,float);
public: void SetShadowSpotLightFallloffParameters(const CMatrix3x4 &,float,float,const CRGBFloatColour &);
public: void ResetLight(unsigned long);
public: void InitialiseLight(unsigned long,const CShaderGenericLight &);
public: void InitialiseLight(unsigned long,const CShaderLight &);
public: void SetLightGlobal(unsigned long,const C4DVector &);
public: void SetPixelShaderConstant(unsigned long,const C4DVector &);
public: void SetPixelShaderConstant(const CCharString &,const C4DVector &);
public: long GetPixelShaderConstantTableIndex(const CCharString &);
public: void SetAmbientColour(const CRGBFloatColour &);
public: void SetFogColour(const CRGBFloatColour &);
public: void SetUserVertexShaderConstant(unsigned long,const CRGBFloatColour &);
public: void SetUserVertexShaderConstant(unsigned long,float,float,float,float);
public: void SetUserVertexShaderConstant(unsigned long,const C4DVector &);
public: void SetUserVertexShaderConstant(unsigned long,const C3DVector &,float);
public: void SetPosCompressionVertexShaderConstant(float,float,float,float,float,float,float,float);
public: void SetPosCompressionVertexShaderConstant(float,float,float,float,float,float);
public: void SetUVCompressionVertexShaderConstant(float,float,float,float);
public: void SetUserVertexShaderConstants(unsigned long,long,float *);
public: unsigned long GetMaxUserVertexShaderConstants() const;
public: void SetVertexShaderConstantsDirect(unsigned long,const void *,unsigned long);
public: bool GetSoftwareVertexShaderEnable() const;
public: bool GetSoftwareFixedFunctionEnable() const;
public: void SetSoftwareVertexShaderEnable(bool);
public: void SetSoftwareFixedFunctionEnable(bool);
public: CCountedPointer<CShaderResource> CreateResource();
public: CVertexShader GetFixedFunctionVertexShader(EVertexType);
public: IDirect3DDevice9 * GetD3DDevice();
public: long GetMaximumLights();
public: void UnregisterVertexShaderConstantLayouts();
public: void OnPostViewportChanged();
public: void SetMultisampleViewportScale(float);
private: unsigned long CreateFixedFunctionVertexShader(const unsigned long *);
private: CPixelShader CreatePixelShader(const CArray<unsigned long> &,const CArray<CCharString> &,const CCharString &);
private: void UpdateWorldTransform();
private: void UpdateScreenSpaceTransform();
private: void SetVSVectorAsColour(const CRGBColour &,float *);
private: void SetVSColourConstant(long,const CRGBFloatColour &,float);
private: void SetVSColourConstant(long,const CRGBFloatColour &);
private: void SetVSVectorConstant(long,const C4DVector &);
private: void SetVSVectorConstant(long,const C3DVector &,float);
private: void SetVSConstant(long,float,float,float,float);
private: void SetVSMatrixConstantTransposed(long,const CMatrix4x4 &,unsigned long);
private: void SetVSMatrixConstantTransposed(long,const CMatrix3x4 &,unsigned long);
private: void SetPSConstants(unsigned long,const void *,unsigned long);
private: void SetD3DMatrix(_D3DMATRIX &,const CMatrix3x4 &);
private: void SetD3DMatrix(_D3DMATRIX &,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);
private: CVertexShaderConstant MultiplyMatrixColumn(const CVertexShaderConstant &,const CMatrix3x4 &);
private: void AllocateLights(unsigned long,unsigned long);
#endif
private:

#ifdef FABLE_RETAIL_RENDER2D_TYPES
 static CShaderRenderManager SingletonInstance;
 void SetVSConstants(long,const void*,unsigned long);
 void UpdatePixelShader();void UpdatePixelShaderConstants();void UpdateBoneTransform();
 void UpdateWorldSpaceTextureTransform();void UpdateObjectSpaceTextureTransform();
 void UpdateWorldSpaceFogTransform();void UpdateObjectSpaceFogTransform();
 void UpdateAmbient();void UpdateFogColour();void UpdateLightGlobals();
 void UpdateObjectSpaceLighting(unsigned long);void UpdateWorldSpaceLighting();void UpdateLocalisedSphereMapping();
 void UpdateCombinedProjectionTransform();void UpdateCameraPos();
#endif
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
#ifdef FABLE_RETAIL_RENDER2D_TYPES
    CSimpleMemoryPool LightPool;
#else
    unsigned char LightPool[0x1c]; //178 CSimpleMemoryPool, complete original object.
#endif
    CArray<C4DVector> LightGlobals; //194
    CArray<C4DVector> LightAttenuations; //1a0
    CRGBFloatColour AmbientColour; //1ac
    CRGBFloatColour FogColour; //1bc
    CRGBColour TextureFactor; //1cc
    CRenderManager* RenderManager; //1d0
    IDirect3DDevice9* D3DDevice; //1d4
    IDirect3DVertexShader9* CurrentVertexShader; //1d8
    IDirect3DVertexDeclaration9* CurrentVertexShaderVertexDeclairation; //1dc original spelling.
#ifdef FABLE_RETAIL_RENDER2D_TYPES
    CPixelShader CurrentPixelShader;
#else
    unsigned char CurrentPixelShader[8]; //1e0 CPixelShader, original vptr and Data pointer.
#endif
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
#ifdef FABLE_RETAIL_RENDER2D_TYPES
    CArray<CPreTransposedBoneMatrix> BoneMatrices;
#else
    unsigned char BoneMatrices[0xc]; //3dc CArray<CPreTransposedBoneMatrix>, complete container.
#endif
};
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager)==0x3e8);
#endif
