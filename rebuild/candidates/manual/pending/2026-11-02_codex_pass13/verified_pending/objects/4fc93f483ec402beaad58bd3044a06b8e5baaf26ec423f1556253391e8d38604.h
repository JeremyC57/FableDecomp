#ifdef FABLE_RETAIL_RENDER2D_TYPES
#ifndef FABLE_ENGINE_CRENDERSTATEMANAGER_RENDER2D_H
#define FABLE_ENGINE_CRENDERSTATEMANAGER_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/Direct3D9Device.h"
#include "engine/RenderStateEnums.h"
class CRGBColour;
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CRenderStateManager : public CBaseClassNonCopyable {
private:
 struct CStateInfo {
  unsigned long CurrentState,DesiredState,BookmarkMask;
  union {_D3DTEXTURESTAGESTATETYPE TextureStageStateParam;_D3DRENDERSTATETYPE RenderStateParam;_D3DSAMPLERSTATETYPE SamplerStageStateParam;};
  bool DirtyListFlag;unsigned char StateType,TextureStage;
  CStateInfo();
 };
struct CStateStackElement {
public: CRenderStateManager::CStateInfo * State;
public: unsigned long Value;
};
struct CDeviceState {
public: CRenderStateManager::CStateInfo PolyShadeMode;
public: CRenderStateManager::CStateInfo PolyFillMode;
public: CRenderStateManager::CStateInfo ColourWriteEnable;
public: CRenderStateManager::CStateInfo ZBufferWriteEnable;
public: CRenderStateManager::CStateInfo ZBufferEnable;
public: CRenderStateManager::CStateInfo ZBufferFunc;
public: CRenderStateManager::CStateInfo PolyCullMode;
public: CRenderStateManager::CStateInfo AlphaTestFunc;
public: CRenderStateManager::CStateInfo AlphaBlendEnable;
public: CRenderStateManager::CStateInfo AlphaTestEnable;
public: CRenderStateManager::CStateInfo AlphaRef;
public: CRenderStateManager::CStateInfo VertexBlendMode;
public: CRenderStateManager::CStateInfo SoftwareVertexProcessing;
public: CRenderStateManager::CStateInfo SourceAlphaBlendMode;
public: CRenderStateManager::CStateInfo DestAlphaBlendMode;
public: CRenderStateManager::CStateInfo FogEnable;
public: CRenderStateManager::CStateInfo FogColour;
public: CRenderStateManager::CStateInfo FogStart;
public: CRenderStateManager::CStateInfo FogEnd;
public: CRenderStateManager::CStateInfo FogDensity;
public: CRenderStateManager::CStateInfo FogMode;
public: CRenderStateManager::CStateInfo FogVertexMode;
public: CRenderStateManager::CStateInfo TextureFactor;
public: CRenderStateManager::CStateInfo SpecularEnable;
public: CRenderStateManager::CStateInfo AntialiasingOn;
public: CRenderStateManager::CStateInfo DitherEnable;
public: CRenderStateManager::CStateInfo ClippingEnable;
public: CRenderStateManager::CStateInfo LightingEnable;
public: CRenderStateManager::CStateInfo NormalizeNormals;
public: CRenderStateManager::CStateInfo AmbientMaterialColourSource;
public: CRenderStateManager::CStateInfo DiffuseMaterialColourSource;
public: CRenderStateManager::CStateInfo SpecularMaterialColourSource;
public: CRenderStateManager::CStateInfo PointSpriteEnable;
public: CRenderStateManager::CStateInfo PointScaleEnable;
public: CRenderStateManager::CStateInfo PointSize;
public: CRenderStateManager::CStateInfo PointSizeMin;
public: CRenderStateManager::CStateInfo PointSizeMax;
public: CRenderStateManager::CStateInfo PointScaleConstantFactor;
public: CRenderStateManager::CStateInfo PointScaleDistanceFactor;
public: CRenderStateManager::CStateInfo PointScaleDistanceSqrFactor;
public: CRenderStateManager::CStateInfo ShadowFunc;
public: CRenderStateManager::CStateInfo SolidOffsetEnable;
public: CRenderStateManager::CStateInfo DepthOffset;
public: CRenderStateManager::CStateInfo DepthOffsetSlope;
public: CRenderStateManager::CStateInfo StencilEnable;
public: CRenderStateManager::CStateInfo StencilOpFail;
public: CRenderStateManager::CStateInfo StencilOpZFail;
public: CRenderStateManager::CStateInfo StencilOpPass;
public: CRenderStateManager::CStateInfo StencilFunc;
public: CRenderStateManager::CStateInfo StencilRef;
public: CRenderStateManager::CStateInfo StencilMask;
public: CRenderStateManager::CStateInfo StencilWriteMask;
public: CRenderStateManager::CStateInfo StippleEnable;
public: CRenderStateManager::CStateInfo BlendOpMode;
public: CRenderStateManager::CStateInfo BlendColour;
public: CRenderStateManager::CStateInfo TextureCoordWrapMode[8];
public: CRenderStateManager::CStateInfo TextureMaxAnisotropy[8];
public: CRenderStateManager::CStateInfo TextureMipmapLODBias[8];
public: CRenderStateManager::CStateInfo TextureMipMapMode[8];
public: CRenderStateManager::CStateInfo TextureUVModeU[8];
public: CRenderStateManager::CStateInfo TextureUVModeV[8];
public: CRenderStateManager::CStateInfo TextureUVModeW[8];
public: CRenderStateManager::CStateInfo TextureFilterModeMin[8];
public: CRenderStateManager::CStateInfo TextureFilterModeMag[8];
public: CRenderStateManager::CStateInfo TextureOperation[8];
public: CRenderStateManager::CStateInfo TextureAlphaOperation[8];
public: CRenderStateManager::CStateInfo TextureArgument1[8];
public: CRenderStateManager::CStateInfo TextureArgument2[8];
public: CRenderStateManager::CStateInfo TextureAlphaArgument1[8];
public: CRenderStateManager::CStateInfo TextureAlphaArgument2[8];
public: CRenderStateManager::CStateInfo TextureCoordIndex[8];
public: CRenderStateManager::CStateInfo TextureTransformMode[8];
public: CRenderStateManager::CStateInfo TextureBorderColour[8];
public: CRenderStateManager::CStateInfo TextureBumpEnvMat00[8];
public: CRenderStateManager::CStateInfo TextureBumpEnvMat01[8];
public: CRenderStateManager::CStateInfo TextureBumpEnvMat10[8];
public: CRenderStateManager::CStateInfo TextureBumpEnvMat11[8];
};
private: IDirect3DDevice9 * PD3DDevice;
private: static unsigned long D3DTextureUVModeTable[4];
private: static unsigned long D3DTextureFilterModeTable[3];
private: static unsigned long D3DTextureMipMapModeTable[3];
private: static unsigned long D3DTextureOperationModeTable[24];
private: static unsigned long D3DTextureArgumentModeTable[20];
private: static unsigned long D3DStencilOperationModeTable[8];
private: static unsigned long D3DPolyShadeModeTable[2];
private: static unsigned long D3DPolyFillModeTable[3];
private: static unsigned long D3DPolyCullModeTable[3];
private: static unsigned long D3DAlphaBlendModeTable[13];
private: static unsigned long D3DAlphaFuncModeTable[8];
private: static unsigned long D3DZBufferFuncModeTable[8];
private: static unsigned long D3DStencilFuncModeTable[8];
private: static unsigned long D3DVertexBlendModeTable[4];
private: static unsigned long D3DShadowFuncModeTable[8];
private: static unsigned long D3DTextureTransformModeTable[10];
private: static unsigned long D3DMaterialColourSourceTable[3];
private: static unsigned long D3DFogTableMode[4];
private: static unsigned long D3DBlendOpModeTable[5];
private: static _D3DRENDERSTATETYPE D3DTextureCoordWrapStageTable[8];
private: CRenderStateManager::CStateStackElement StateStack[1024];
private: CRenderStateManager::CStateInfo * StateUpdateList[256];
private: CRenderStateManager::CStateInfo * AllStatesList[256];
private: long StateStackSize;
private: long StateUpdateListSize;
private: long AllStatesListSize;
private: unsigned long CurrentBookmarkMask;
private: CRenderStateManager::CDeviceState DeviceState;
private: CRenderStateManager::CStateInfo BookmarkDummyState;
private: unsigned long InitFlags;
public:
 CRenderStateManager(unsigned long);
 virtual ~CRenderStateManager();
 void RealiseRenderState();
 void RestoreCaptureBlock();
 void StartCaptureBlock() {StateStack[StateStackSize].State=&BookmarkDummyState;++StateStackSize;CurrentBookmarkMask<<=1;}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetPolyCullMode(POLY_CULL_MODE m) {SetRenderState(D3DPolyCullModeTable[m],DeviceState.PolyCullMode);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetPolyFillMode(POLY_FILL_MODE m) {SetRenderState(D3DPolyFillModeTable[m],DeviceState.PolyFillMode);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetPolyShadeMode(POLY_SHADE_MODE m) {SetRenderState(D3DPolyShadeModeTable[m],DeviceState.PolyShadeMode);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetTextureFilterModeMin(long s,TEXTURE_FILTER_MODE m) {SetRenderState(D3DTextureFilterModeTable[m],DeviceState.TextureFilterModeMin[s]);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetTextureFilterModeMag(long s,TEXTURE_FILTER_MODE m) {SetRenderState(D3DTextureFilterModeTable[m],DeviceState.TextureFilterModeMag[s]);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetTextureMipMapMode(long s,TEXTURE_MIPMAP_MODE m) {SetRenderState(D3DTextureMipMapModeTable[m],DeviceState.TextureMipMapMode[s]);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetTextureUVModeU(long s,TEXTURE_UV_MODE m) {SetRenderState(D3DTextureUVModeTable[m],DeviceState.TextureUVModeU[s]);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetTextureUVModeV(long s,TEXTURE_UV_MODE m) {SetRenderState(D3DTextureUVModeTable[m],DeviceState.TextureUVModeV[s]);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetFogEnable(bool v) {SetRenderState(v?1:0,DeviceState.FogEnable);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetZBufferEnable(bool v) {SetRenderState(v?1:0,DeviceState.ZBufferEnable);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetZBufferTestFunc(ZBUFFER_FUNC v) {SetRenderState(D3DZBufferFuncModeTable[v],DeviceState.ZBufferFunc);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetZBufferWriteEnable(bool v) {SetRenderState(v?1:0,DeviceState.ZBufferWriteEnable);}
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetClippingEnable(bool v) {SetRenderState(v?1UL:0UL,DeviceState.ClippingEnable);}
private:
#ifdef FABLE_RETAIL_RENDERSTATE_EXPAND_SET
 __forceinline
#endif
 void SetRenderState(unsigned long value,CStateInfo& info) {
  if(info.DesiredState==value)return;
  if(!(info.BookmarkMask&CurrentBookmarkMask)) {
   info.BookmarkMask|=CurrentBookmarkMask;
   StateStack[StateStackSize].State=&info;
   StateStack[StateStackSize].Value=info.DesiredState;
   ++StateStackSize;
  }
  info.DesiredState=value;AddStateToUpdateList(info);
 }
 void AddStateToUpdateList(CStateInfo& info) {
  if(!info.DirtyListFlag) {info.DirtyListFlag=true;StateUpdateList[StateUpdateListSize]=&info;++StateUpdateListSize;}
 }

// Remaining original authored interface declarations (generated specials stay implicit).
public: void Init(IDirect3DDevice9 *);
public: void SetDefaultRenderStates();
public: void ForceResendRenderStatesToHardware();
public: void SetColourWriteEnable(bool,bool,bool,bool);
public: void SetColourWriteEnable(bool);
public: void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE);
public: void SetDestAlphaBlendMode(ALPHA_BLEND_MODE);
public: void SetAlphaBlendEnable(bool);
public: void SetAlphaTestEnable(bool);
public: void SetAlphaTestFunc(ALPHA_TEST_FUNC);
public: void SetAlphaRef(unsigned long);
public: void SetVertexBlending(VERTEX_BLEND_MODE);
public: void SetDitherEnable(bool);
public: void SetFogStart(float);
public: void SetFogEnd(float);
public: void SetFogDensity(float);
public: void SetFogColour(unsigned long);
public: void SetFogMode(FOG_MODE);
public: void SetFogVertexMode(FOG_MODE);
public: void SetTextureFactor(unsigned long);
public: void SetSpecularEnable(bool);
public: void SetLightingEnable(bool);
public: void SetNormalizeNormals(bool);
public: void SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE);
public: void SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE);
public: void SetSpecularMaterialColourSource(MATERIAL_COLOUR_SOURCE);
public: void SetPointSpriteEnable(bool);
public: void SetPointScaleEnable(bool);
public: void SetPointSize(float);
public: void SetPointSizeMin(float);
public: void SetPointSizeMax(float);
public: void SetPointScaleConstantFactor(float);
public: void SetPointScaleDistanceFactor(float);
public: void SetPointScaleDistanceSqrFactor(float);
public: void SetBlendOp(BLEND_OP);
public: void SetTextureCoordWrapUVW(long,bool,bool,bool);
public: void SetTextureUVModeW(long,TEXTURE_UV_MODE);
public: void SetTextureMipmapLODBias(long,float);
public: void SetTextureOperation(long,TEXTURE_OP_MODE);
public: void SetTextureAlphaOperation(long,TEXTURE_OP_MODE);
public: void SetTextureArgument1(long,TEXTURE_ARG_MODE);
public: void SetTextureArgument2(long,TEXTURE_ARG_MODE);
public: void SetTextureAlphaArgument1(long,TEXTURE_ARG_MODE);
public: void SetTextureAlphaArgument2(long,TEXTURE_ARG_MODE);
public: void SetTextureMaxAnisotropy(long,long);
public: void SetTextureCoordIndex(long,long);
public: void SetTextureTransformMode(long,TEXTURE_TRANSFORM_MODE);
public: void SetTextureBorderColour(long,unsigned long);
public: void SetTextureBumpEnvMatrix(long,float,float,float,float);
public: void SetStencilEnable(bool);
public: void SetStencilOpFail(STENCIL_OP_MODE);
public: void SetStencilOpZFail(STENCIL_OP_MODE);
public: void SetStencilOpPass(STENCIL_OP_MODE);
public: void SetStencilFunc(STENCIL_FUNC);
public: void SetStencilRef(unsigned long);
public: void SetStencilMask(unsigned long);
public: void SetStencilWriteMask(unsigned long);
public: void SetDepthOffset(float);
public: void SetSlopeScaledDepthOffset(float);
public: void SetSoftwareVertexProcessing(bool);
public: void SetShadowFunc(SHADOW_FUNC);
public: void SetSolidOffsetEnable(bool);
public: bool IsSoftwareProcessingEnabled();
public: void SetStippleEnable(bool);
public: void SetStipplePattern(unsigned long *);
public: void SetMultiSampleMode(MULTI_SAMPLE_MODE);
public: void SetMultiSampleModeRenderTarget(MULTI_SAMPLE_MODE);
public: void SetTextureColourSign(long,TEXTURE_COLOUR_SIGN);
public: void SetBlendColour(unsigned long);
private: void SetupRenderStates();
private: void SetupRenderState(CRenderStateManager::CStateInfo &,_D3DRENDERSTATETYPE);
private: void SetupTextureStageState(CRenderStateManager::CStateInfo &,_D3DTEXTURESTAGESTATETYPE,long);
private: void SetupSamplerStageState(CRenderStateManager::CStateInfo &,_D3DSAMPLERSTATETYPE,long);
private: void CheckDeviceStateMatchesCache();
};
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager)==0x3a3c);
#endif
#else
#ifndef FABLE_ENGINE_CRENDERSTATEMANAGER_H
#define FABLE_ENGINE_CRENDERSTATEMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CRenderStateManager = 0x3a3c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CStateInfo;
struct IDirect3DDevice9;

#pragma pack(push, 1)
class CRenderStateManager {
public:
    void*             __vftable;                             // +0x00 vptr, or first dword of a flattened base subobject
    IDirect3DDevice9* PD3DDevice;                            // +0x04
    unsigned char     StateStack[0x2000];                    // +0x08 CStateStackElement[1024]
    CStateInfo*       StateUpdateList[256];                  // +0x2008
    CStateInfo*       AllStatesList[256];                    // +0x2408
    long              StateStackSize;                        // +0x2808
    long              StateUpdateListSize;                   // +0x280c
    long              AllStatesListSize;                     // +0x2810
    unsigned long     CurrentBookmarkMask;                   // +0x2814
    unsigned char     DeviceState[0x120c];                   // +0x2818 CDeviceState
    unsigned char     BookmarkDummyState[0x14];              // +0x3a24 CStateInfo
    unsigned long     InitFlags;                             // +0x3a38
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CRenderStateManager) == 0x3a3c);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, PD3DDevice) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateStack) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateUpdateList) == 0x2008);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, AllStatesList) == 0x2408);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateStackSize) == 0x2808);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateUpdateListSize) == 0x280c);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, AllStatesListSize) == 0x2810);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, CurrentBookmarkMask) == 0x2814);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, DeviceState) == 0x2818);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, BookmarkDummyState) == 0x3a24);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, InitFlags) == 0x3a38);

#endif // FABLE_ENGINE_CRENDERSTATEMANAGER_H

#endif
