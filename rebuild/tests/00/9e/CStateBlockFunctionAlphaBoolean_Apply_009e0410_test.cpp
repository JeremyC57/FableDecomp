#include <cstdio>
#include <cstdlib>
#include <cstring>
static void Check(bool ok){if(!ok){std::puts("009e0410_TEST FAIL");std::exit(1);}}

enum POLY_CULL_MODE {
POLY_CULL_CCW=0,
POLY_CULL_CW=1,
POLY_CULL_NONE=2,
POLY_CULL_MODE_COUNT=3
};
enum POLY_FILL_MODE {
POLY_FILL_SOLID=0,
POLY_FILL_WIRE=1,
POLY_FILL_POINT=2,
POLY_FILL_MODE_COUNT=3
};
enum POLY_SHADE_MODE {
POLY_SHADE_FLAT=0,
POLY_SHADE_GOURAUD=1,
POLY_SHADE_MODE_COUNT=2
};
enum TEXTURE_FILTER_MODE {
TEXTURE_FILTER_POINT=0,
TEXTURE_FILTER_LINEAR=1,
TEXTURE_FILTER_ANISOTROPIC=2,
TEXTURE_FILTER_MODE_COUNT=3
};
enum TEXTURE_MIPMAP_MODE {
TEXTURE_MIPMAP_POINT=0,
TEXTURE_MIPMAP_LINEAR=1,
TEXTURE_MIPMAP_DISABLE=2,
TEXTURE_MIPMAP_MODE_COUNT=3
};
enum TEXTURE_UV_MODE {
TEXTURE_UV_WRAP=0,
TEXTURE_UV_MIRROR=1,
TEXTURE_UV_CLAMP=2,
TEXTURE_UV_BORDER=3,
TEXTURE_UV_MODE_COUNT=4
};
enum ZBUFFER_FUNC {
ZBUFFER_FUNC_NEVER=0,
ZBUFFER_FUNC_LESS=1,
ZBUFFER_FUNC_EQUAL=2,
ZBUFFER_FUNC_LESS_EQUAL=3,
ZBUFFER_FUNC_GREATER=4,
ZBUFFER_FUNC_NOT_EQUAL=5,
ZBUFFER_FUNC_GREATER_EQUAL=6,
ZBUFFER_FUNC_ALWAYS=7,
ZBUFFER_FUNC_COUNT=8
};
enum TEXTURE_OP_MODE {
TEXTURE_OP_DISABLE=0,
TEXTURE_OP_SELECT_ARG1=1,
TEXTURE_OP_SELECT_ARG2=2,
TEXTURE_OP_MODULATE=3,
TEXTURE_OP_MODULATE_2X=4,
TEXTURE_OP_MODULATE_4X=5,
TEXTURE_OP_ADD=6,
TEXTURE_OP_ADD_SIGNED=7,
TEXTURE_OP_ADD_SIGNED2X=8,
TEXTURE_OP_SUBTRACT=9,
TEXTURE_OP_ADD_SMOOTH=10,
TEXTURE_OP_BLEND_DIFFUSE_ALPHA=11,
TEXTURE_OP_BLEND_TEXTURE_ALPHA=12,
TEXTURE_OP_BLEND_FACTOR_ALPHA=13,
TEXTURE_OP_BLEND_TEXTURE_ALPHA_PM=14,
TEXTURE_OP_BLEND_CURRENT_ALPHA=15,
TEXTURE_OP_PREMODULATE=16,
TEXTURE_OP_MODULATE_ALPHA_ADD_COLOR=17,
TEXTURE_OP_MODULATE_COLOR_ADD_ALPHA=18,
TEXTURE_OP_MODULATE_INV_ALPHA_ADD_COLOR=19,
TEXTURE_OP_MODULATE_INV_COLOR_ADD_ALPHA=20,
TEXTURE_OP_BUMP_ENV_MAP=21,
TEXTURE_OP_BUMP_ENV_MAP_LUMINANCE=22,
TEXTURE_OP_DOT_PRODUCT3=23,
TEXTURE_OP_MODE_COUNT=24
};
enum TEXTURE_ARG_MODE {
TEXTURE_ARG_CURRENT=0,
TEXTURE_ARG_DIFFUSE=1,
TEXTURE_ARG_TEXTURE=2,
TEXTURE_ARG_TFACTOR=3,
TEXTURE_ARG_SPECULAR=4,
TEXTURE_ARG_COMPLEMENT_CURRENT=5,
TEXTURE_ARG_COMPLEMENT_DIFFUSE=6,
TEXTURE_ARG_COMPLEMENT_TEXTURE=7,
TEXTURE_ARG_COMPLEMENT_TFACTOR=8,
TEXTURE_ARG_COMPLEMENT_SPECULAR=9,
TEXTURE_ARG_ALPHA_REPLICATE_CURRENT=10,
TEXTURE_ARG_ALPHA_REPLICATE_DIFFUSE=11,
TEXTURE_ARG_ALPHA_REPLICATE_TEXTURE=12,
TEXTURE_ARG_ALPHA_REPLICATE_TFACTOR=13,
TEXTURE_ARG_ALPHA_REPLICATE_SPECULAR=14,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_CURRENT=15,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_DIFFUSE=16,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_TEXTURE=17,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_TFACTOR=18,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_SPECULAR=19,
TEXTURE_ARG_MODE_COUNT=20
};
enum STENCIL_OP_MODE {
STENCIL_OP_MODE_KEEP=0,
STENCIL_OP_MODE_ZERO=1,
STENCIL_OP_MODE_REPLACE=2,
STENCIL_OP_MODE_INCREMENT_SAT=3,
STENCIL_OP_MODE_DECREMENT_SAT=4,
STENCIL_OP_MODE_INVERT=5,
STENCIL_OP_MODE_INCREMENT=6,
STENCIL_OP_MODE_DECREMENT=7,
STENCIL_OP_MODE_COUNT=8
};
enum FOG_MODE {
FOG_MODE_NONE=0,
FOG_MODE_LINEAR=1,
FOG_MODE_EXP=2,
FOG_MODE_EXP2=3,
FOG_MODE_COUNT=4
};
enum ALPHA_BLEND_MODE {
ALPHA_BLEND_ZERO=0,
ALPHA_BLEND_ONE=1,
ALPHA_BLEND_SRC_COLOR=2,
ALPHA_BLEND_INV_SRC_COLOR=3,
ALPHA_BLEND_SRC_ALPHA=4,
ALPHA_BLEND_INV_SRC_ALPHA=5,
ALPHA_BLEND_DEST_ALPHA=6,
ALPHA_BLEND_INV_DEST_ALPHA=7,
ALPHA_BLEND_DEST_COLOR=8,
ALPHA_BLEND_INV_DEST_COLOR=9,
ALPHA_BLEND_SRC_ALPHA_SAT=10,
ALPHA_BLEND_BOTH_SRC_ALPHA=11,
ALPHA_BLEND_BOTH_INV_SRC_ALPHA=12,
ALPHA_BLEND_MODE_COUNT=13
};
enum ALPHA_TEST_FUNC {
ALPHA_TEST_NEVER=0,
ALPHA_TEST_LESS=1,
ALPHA_TEST_EQUAL=2,
ALPHA_TEST_LESS_EQUAL=3,
ALPHA_TEST_GREATER=4,
ALPHA_TEST_NOT_EQUAL=5,
ALPHA_TEST_GREATER_EQUAL=6,
ALPHA_TEST_ALWAYS=7,
ALPHA_TEST_MODE_COUNT=8
};
enum STENCIL_FUNC {
STENCIL_FUNC_NEVER=0,
STENCIL_FUNC_LESS=1,
STENCIL_FUNC_EQUAL=2,
STENCIL_FUNC_LESS_EQUAL=3,
STENCIL_FUNC_GREATER=4,
STENCIL_FUNC_NOT_EQUAL=5,
STENCIL_FUNC_GREATER_EQUAL=6,
STENCIL_FUNC_ALWAYS=7,
STENCIL_FUNC_COUNT=8
};
enum SHADOW_FUNC {
SHADOW_FUNC_NEVER=0,
SHADOW_FUNC_LESS=1,
SHADOW_FUNC_EQUAL=2,
SHADOW_FUNC_LESS_EQUAL=3,
SHADOW_FUNC_GREATER=4,
SHADOW_FUNC_NOT_EQUAL=5,
SHADOW_FUNC_GREATER_EQUAL=6,
SHADOW_FUNC_ALWAYS=7,
SHADOW_FUNC_COUNT=8
};
enum VERTEX_BLEND_MODE {
VERTEX_BLEND_DISABLE=0,
VERTEX_BLEND_1WEIGHT=1,
VERTEX_BLEND_2WEIGHTS=2,
VERTEX_BLEND_3WEIGHTS=3,
VERTEX_BLEND_MODE_COUNT=4
};
enum TEXTURE_TRANSFORM_MODE {
TEXTURE_TRANSFORM_DIABLED=0,
TEXTURE_TRANSFORM_COUNT1=1,
TEXTURE_TRANSFORM_COUNT2=2,
TEXTURE_TRANSFORM_COUNT3=3,
TEXTURE_TRANSFORM_COUNT4=4,
TEXTURE_TRANSFORM_PROJECTED=5,
TEXTURE_TRANSFORM_COUNT1_PROJECTED=6,
TEXTURE_TRANSFORM_COUNT2_PROJECTED=7,
TEXTURE_TRANSFORM_COUNT3_PROJECTED=8,
TEXTURE_TRANSFORM_COUNT4_PROJECTED=9,
TEXTURE_TRANSFORM_MODE_COUNT=10
};
enum MATERIAL_COLOUR_SOURCE {
MATERIAL_COLOUR_SOURCE_MATERIAL=0,
MATERIAL_COLOUR_SOURCE_COLOUR1=1,
MATERIAL_COLOUR_SOURCE_COLOUR2=2,
MATERIAL_COLOUR_SOURCE_COUNT=3
};
enum BLEND_OP {
BLEND_OP_ADD=0,
BLEND_OP_SUBTRACT=1,
BLEND_OP_REVSUBTRACT=2,
BLEND_OP_MIN=3,
BLEND_OP_MAX=4,
BLEND_OP_MODE_COUNT=5
};
enum MULTI_SAMPLE_MODE {
MULTI_SAMPLE_MODE_1X=0,
MULTI_SAMPLE_MODE_2X=1,
MULTI_SAMPLE_MODE_4X=2,
MULTI_SAMPLE_MODE_COUNT=3
};
enum TEXTURE_COLOUR_SIGN {
TEXTURE_COLOUR_SIGN_NONE=0,
TEXTURE_COLOUR_SIGN_R=1,
TEXTURE_COLOUR_SIGN_G=2,
TEXTURE_COLOUR_SIGN_B=3,
TEXTURE_COLOUR_SIGN_A=4,
TEXTURE_COLOUR_SIGN_RGB=5,
TEXTURE_COLOUR_SIGN_RGBA=6,
TEXTURE_COLOUR_SIGN_COUNT=7
};
// Dependency fixtures retain the actual shared state algorithm; the target Apply body follows verbatim.
class CRenderStateManager {public:
 struct CStateInfo {unsigned long CurrentState,DesiredState,BookmarkMask;unsigned long RenderStateParam;bool DirtyListFlag;unsigned char StateType,TextureStage;};
 struct CStateStackElement{CStateInfo* State;unsigned long Value;};
 struct CDeviceState{CStateInfo PolyShadeMode;
CStateInfo PolyFillMode;
CStateInfo ColourWriteEnable;
CStateInfo ZBufferWriteEnable;
CStateInfo ZBufferEnable;
CStateInfo ZBufferFunc;
CStateInfo PolyCullMode;
CStateInfo AlphaTestFunc;
CStateInfo AlphaBlendEnable;
CStateInfo AlphaTestEnable;
CStateInfo AlphaRef;
CStateInfo VertexBlendMode;
CStateInfo SoftwareVertexProcessing;
CStateInfo SourceAlphaBlendMode;
CStateInfo DestAlphaBlendMode;
CStateInfo FogEnable;
CStateInfo FogColour;
CStateInfo FogStart;
CStateInfo FogEnd;
CStateInfo FogDensity;
CStateInfo FogMode;
CStateInfo FogVertexMode;
CStateInfo TextureFactor;
CStateInfo SpecularEnable;
CStateInfo AntialiasingOn;
CStateInfo DitherEnable;
CStateInfo ClippingEnable;
CStateInfo LightingEnable;
CStateInfo NormalizeNormals;
CStateInfo AmbientMaterialColourSource;
CStateInfo DiffuseMaterialColourSource;
CStateInfo SpecularMaterialColourSource;
CStateInfo PointSpriteEnable;
CStateInfo PointScaleEnable;
CStateInfo PointSize;
CStateInfo PointSizeMin;
CStateInfo PointSizeMax;
CStateInfo PointScaleConstantFactor;
CStateInfo PointScaleDistanceFactor;
CStateInfo PointScaleDistanceSqrFactor;
CStateInfo ShadowFunc;
CStateInfo SolidOffsetEnable;
CStateInfo DepthOffset;
CStateInfo DepthOffsetSlope;
CStateInfo StencilEnable;
CStateInfo StencilOpFail;
CStateInfo StencilOpZFail;
CStateInfo StencilOpPass;
CStateInfo StencilFunc;
CStateInfo StencilRef;
CStateInfo StencilMask;
CStateInfo StencilWriteMask;
CStateInfo StippleEnable;
CStateInfo BlendOpMode;
CStateInfo BlendColour;
CStateInfo TextureCoordWrapMode[8];
CStateInfo TextureMaxAnisotropy[8];
CStateInfo TextureMipmapLODBias[8];
CStateInfo TextureMipMapMode[8];
CStateInfo TextureUVModeU[8];
CStateInfo TextureUVModeV[8];
CStateInfo TextureUVModeW[8];
CStateInfo TextureFilterModeMin[8];
CStateInfo TextureFilterModeMag[8];
CStateInfo TextureOperation[8];
CStateInfo TextureAlphaOperation[8];
CStateInfo TextureArgument1[8];
CStateInfo TextureArgument2[8];
CStateInfo TextureAlphaArgument1[8];
CStateInfo TextureAlphaArgument2[8];
CStateInfo TextureCoordIndex[8];
CStateInfo TextureTransformMode[8];
CStateInfo TextureBorderColour[8];
CStateInfo TextureBumpEnvMat00[8];
CStateInfo TextureBumpEnvMat01[8];
CStateInfo TextureBumpEnvMat10[8];
CStateInfo TextureBumpEnvMat11[8];} DeviceState;
 CStateStackElement StateStack[1024];CStateInfo* StateUpdateList[256];long StateStackSize,StateUpdateListSize;unsigned long CurrentBookmarkMask;
 static unsigned long D3DAlphaBlendModeTable[13],D3DAlphaFuncModeTable[8],D3DTextureArgumentModeTable[20],D3DTextureOperationModeTable[24],D3DTextureFilterModeTable[3];
     inline void SetZBufferWriteEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ZBufferWriteEnable); }
    inline void SetZBufferEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ZBufferEnable); }
    inline void SetAlphaBlendEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.AlphaBlendEnable); }
    inline void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(D3DAlphaBlendModeTable[mode], DeviceState.SourceAlphaBlendMode); }
    inline void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(D3DAlphaBlendModeTable[mode], DeviceState.DestAlphaBlendMode); }
    inline void SetAlphaTestEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.AlphaTestEnable); }
    inline void SetAlphaTestFunc(ALPHA_TEST_FUNC func) { SetRenderState(D3DAlphaFuncModeTable[func], DeviceState.AlphaTestFunc); }
    inline void SetTextureFilterModeMin(long stage, TEXTURE_FILTER_MODE mode) { SetRenderState(D3DTextureFilterModeTable[mode], DeviceState.TextureFilterModeMin[stage]); }
    inline void SetTextureFilterModeMag(long stage, TEXTURE_FILTER_MODE mode) { SetRenderState(D3DTextureFilterModeTable[mode], DeviceState.TextureFilterModeMag[stage]); }
    inline void SetTextureOperation(long stage, TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode], DeviceState.TextureOperation[stage]); }
    inline void SetTextureAlphaOperation(long stage, TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode], DeviceState.TextureAlphaOperation[stage]); }
    inline void SetTextureArgument1(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureArgument1[stage]); }
    inline void SetTextureArgument2(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureArgument2[stage]); }
    inline void SetTextureAlphaArgument1(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureAlphaArgument1[stage]); }
    inline void SetTextureAlphaArgument2(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureAlphaArgument2[stage]); }
 void SetRenderState(unsigned long value,CStateInfo& state){
  if(state.DesiredState!=value){
   if((state.BookmarkMask&CurrentBookmarkMask)==0){state.BookmarkMask|=CurrentBookmarkMask;StateStack[StateStackSize].State=&state;StateStack[StateStackSize].Value=state.DesiredState;++StateStackSize;}
   state.DesiredState=value;AddStateToUpdateList(state);
  }
 }
 void AddStateToUpdateList(CStateInfo& state){if(!state.DirtyListFlag){state.DirtyListFlag=true;StateUpdateList[StateUpdateListSize]=&state;++StateUpdateListSize;}}
};
unsigned long CRenderStateManager::D3DAlphaBlendModeTable[13];unsigned long CRenderStateManager::D3DAlphaFuncModeTable[8];unsigned long CRenderStateManager::D3DTextureArgumentModeTable[20];unsigned long CRenderStateManager::D3DTextureOperationModeTable[24];unsigned long CRenderStateManager::D3DTextureFilterModeTable[3];
class CRenderManagerCore:public CRenderStateManager{};class CRenderManager:public CRenderManagerCore{};
static CRenderManager manager;static unsigned get_system_calls,get_display_calls,get_render_calls;
class CDisplayManager{public:CRenderManager& GetRenderManager(){++get_render_calls;return manager;}};
class CSystemManager{public:CDisplayManager* GetDisplayManager(){++get_display_calls;static CDisplayManager display;return &display;}};
CSystemManager* GFGetSystemManager(){++get_system_calls;static CSystemManager system;return &system;}
class CStateBlockFunctionBase{public:virtual void Apply()=0;};class CStateBlockFunctionAlphaBoolean:public CStateBlockFunctionBase{public:virtual void Apply();};
void CStateBlockFunctionAlphaBoolean::Apply()
{
    CRenderManagerCore* rm=&GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    rm->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    rm->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    rm->SetZBufferEnable(true);
    rm->SetZBufferWriteEnable(true);
    rm->SetAlphaBlendEnable(true);
    rm->SetAlphaTestEnable(false);
    rm->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    rm->SetTextureArgument1(0,TEXTURE_ARG_TEXTURE);
    rm->SetTextureArgument2(0,TEXTURE_ARG_DIFFUSE);
    rm->SetTextureAlphaArgument1(0,TEXTURE_ARG_TEXTURE);
    rm->SetTextureAlphaArgument2(0,TEXTURE_ARG_DIFFUSE);
    rm->SetTextureOperation(0,TEXTURE_OP_MODULATE);
    rm->SetTextureAlphaOperation(0,TEXTURE_OP_MODULATE);
    rm->SetTextureOperation(1,TEXTURE_OP_DISABLE);
    rm->SetTextureAlphaOperation(1,TEXTURE_OP_DISABLE);
    rm->SetTextureFilterModeMin(0,TEXTURE_FILTER_POINT);
    rm->SetTextureFilterModeMag(0,TEXTURE_FILTER_POINT);
}

int main(){CStateBlockFunctionAlphaBoolean block;unsigned cases=0;CRenderStateManager::CStateInfo* all[256];int all_count=0;
all[all_count++]=&manager.DeviceState.PolyShadeMode;
all[all_count++]=&manager.DeviceState.PolyFillMode;
all[all_count++]=&manager.DeviceState.ColourWriteEnable;
all[all_count++]=&manager.DeviceState.ZBufferWriteEnable;
all[all_count++]=&manager.DeviceState.ZBufferEnable;
all[all_count++]=&manager.DeviceState.ZBufferFunc;
all[all_count++]=&manager.DeviceState.PolyCullMode;
all[all_count++]=&manager.DeviceState.AlphaTestFunc;
all[all_count++]=&manager.DeviceState.AlphaBlendEnable;
all[all_count++]=&manager.DeviceState.AlphaTestEnable;
all[all_count++]=&manager.DeviceState.AlphaRef;
all[all_count++]=&manager.DeviceState.VertexBlendMode;
all[all_count++]=&manager.DeviceState.SoftwareVertexProcessing;
all[all_count++]=&manager.DeviceState.SourceAlphaBlendMode;
all[all_count++]=&manager.DeviceState.DestAlphaBlendMode;
all[all_count++]=&manager.DeviceState.FogEnable;
all[all_count++]=&manager.DeviceState.FogColour;
all[all_count++]=&manager.DeviceState.FogStart;
all[all_count++]=&manager.DeviceState.FogEnd;
all[all_count++]=&manager.DeviceState.FogDensity;
all[all_count++]=&manager.DeviceState.FogMode;
all[all_count++]=&manager.DeviceState.FogVertexMode;
all[all_count++]=&manager.DeviceState.TextureFactor;
all[all_count++]=&manager.DeviceState.SpecularEnable;
all[all_count++]=&manager.DeviceState.AntialiasingOn;
all[all_count++]=&manager.DeviceState.DitherEnable;
all[all_count++]=&manager.DeviceState.ClippingEnable;
all[all_count++]=&manager.DeviceState.LightingEnable;
all[all_count++]=&manager.DeviceState.NormalizeNormals;
all[all_count++]=&manager.DeviceState.AmbientMaterialColourSource;
all[all_count++]=&manager.DeviceState.DiffuseMaterialColourSource;
all[all_count++]=&manager.DeviceState.SpecularMaterialColourSource;
all[all_count++]=&manager.DeviceState.PointSpriteEnable;
all[all_count++]=&manager.DeviceState.PointScaleEnable;
all[all_count++]=&manager.DeviceState.PointSize;
all[all_count++]=&manager.DeviceState.PointSizeMin;
all[all_count++]=&manager.DeviceState.PointSizeMax;
all[all_count++]=&manager.DeviceState.PointScaleConstantFactor;
all[all_count++]=&manager.DeviceState.PointScaleDistanceFactor;
all[all_count++]=&manager.DeviceState.PointScaleDistanceSqrFactor;
all[all_count++]=&manager.DeviceState.ShadowFunc;
all[all_count++]=&manager.DeviceState.SolidOffsetEnable;
all[all_count++]=&manager.DeviceState.DepthOffset;
all[all_count++]=&manager.DeviceState.DepthOffsetSlope;
all[all_count++]=&manager.DeviceState.StencilEnable;
all[all_count++]=&manager.DeviceState.StencilOpFail;
all[all_count++]=&manager.DeviceState.StencilOpZFail;
all[all_count++]=&manager.DeviceState.StencilOpPass;
all[all_count++]=&manager.DeviceState.StencilFunc;
all[all_count++]=&manager.DeviceState.StencilRef;
all[all_count++]=&manager.DeviceState.StencilMask;
all[all_count++]=&manager.DeviceState.StencilWriteMask;
all[all_count++]=&manager.DeviceState.StippleEnable;
all[all_count++]=&manager.DeviceState.BlendOpMode;
all[all_count++]=&manager.DeviceState.BlendColour;
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureCoordWrapMode[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureMaxAnisotropy[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureMipmapLODBias[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureMipMapMode[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureUVModeU[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureUVModeV[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureUVModeW[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureFilterModeMin[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureFilterModeMag[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureOperation[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureAlphaOperation[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureArgument1[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureArgument2[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureAlphaArgument1[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureAlphaArgument2[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureCoordIndex[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureTransformMode[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureBorderColour[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureBumpEnvMat00[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureBumpEnvMat01[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureBumpEnvMat10[i];
for(int i=0;i<8;++i)all[all_count++]=&manager.DeviceState.TextureBumpEnvMat11[i];
 for(unsigned pattern=0;pattern<512;++pattern)for(unsigned mask=0;mask<4;++mask){
  for(unsigned j=0;j<13;++j)manager.D3DAlphaBlendModeTable[j]=1000+pattern+j;
  for(unsigned j=0;j<8;++j)manager.D3DAlphaFuncModeTable[j]=2000+pattern+j;
  for(unsigned j=0;j<20;++j)manager.D3DTextureArgumentModeTable[j]=3000+pattern+j;
  for(unsigned j=0;j<24;++j)manager.D3DTextureOperationModeTable[j]=4000+pattern+j;
  for(unsigned j=0;j<3;++j)manager.D3DTextureFilterModeTable[j]=5000+pattern+j;
  CRenderStateManager::CStateInfo* order[]={&manager.DeviceState.SourceAlphaBlendMode,&manager.DeviceState.DestAlphaBlendMode,&manager.DeviceState.ZBufferEnable,&manager.DeviceState.ZBufferWriteEnable,&manager.DeviceState.AlphaBlendEnable,&manager.DeviceState.AlphaTestEnable,&manager.DeviceState.AlphaTestFunc,&manager.DeviceState.TextureArgument1[0],&manager.DeviceState.TextureArgument2[0],&manager.DeviceState.TextureAlphaArgument1[0],&manager.DeviceState.TextureAlphaArgument2[0],&manager.DeviceState.TextureOperation[0],&manager.DeviceState.TextureAlphaOperation[0],&manager.DeviceState.TextureOperation[1],&manager.DeviceState.TextureAlphaOperation[1],&manager.DeviceState.TextureFilterModeMin[0],&manager.DeviceState.TextureFilterModeMag[0]};
  const unsigned long values[]={1000+pattern+4,1000+pattern+5,1,1,1,0,2000+pattern+4,3000+pattern+2,3000+pattern+1,3000+pattern+2,3000+pattern+1,4000+pattern+3,4000+pattern+3,4000+pattern,4000+pattern,5000+pattern+0,5000+pattern+0};const int count=sizeof(values)/sizeof(values[0]);
  unsigned long old_value[256],old_mask[256],new_value[256],new_mask[256];bool old_dirty[256],new_dirty[256];
  for(int j=0;j<all_count;++j){all[j]->CurrentState=71+j;all[j]->DesiredState=80000+j;all[j]->BookmarkMask=(j+pattern)%4;all[j]->DirtyListFlag=((pattern+j)%3)==0;all[j]->RenderStateParam=55;all[j]->StateType=6;all[j]->TextureStage=7;}
  for(int j=0;j<count;++j)if((pattern>>((j%9)))&1)order[j]->DesiredState=values[j];
  for(int j=0;j<all_count;++j){new_value[j]=old_value[j]=all[j]->DesiredState;new_mask[j]=old_mask[j]=all[j]->BookmarkMask;new_dirty[j]=old_dirty[j]=all[j]->DirtyListFlag;}
  const unsigned long masks[4]={0,1,2,8};manager.CurrentBookmarkMask=masks[mask];manager.StateStackSize=2;manager.StateUpdateListSize=3;
  manager.StateStack[0].State=all[0];manager.StateStack[0].Value=123;manager.StateStack[1].State=all[1];manager.StateStack[1].Value=456;
  manager.StateUpdateList[0]=all[2];manager.StateUpdateList[1]=all[3];manager.StateUpdateList[2]=all[4];
  CRenderStateManager::CStateInfo* captured[32];unsigned long prior[32];CRenderStateManager::CStateInfo* queued[32];int capture_count=0,queue_count=0;
  for(int j=0;j<count;++j){int idx=0;while(all[idx]!=order[j])++idx;if(old_value[idx]==values[j])continue;new_value[idx]=values[j];
   if(!(old_mask[idx]&masks[mask])){captured[capture_count]=order[j];prior[capture_count++]=old_value[idx];new_mask[idx]|=masks[mask];}
   if(!old_dirty[idx])queued[queue_count++]=order[j];new_dirty[idx]=true;
  }
  get_system_calls=get_display_calls=get_render_calls=0;block.Apply();
  Check(get_system_calls==1&&get_display_calls==1&&get_render_calls==1);Check(manager.StateStackSize==2+capture_count&&manager.StateUpdateListSize==3+queue_count);
  Check(manager.StateStack[0].State==all[0]&&manager.StateStack[0].Value==123&&manager.StateStack[1].Value==456);Check(manager.StateUpdateList[0]==all[2]&&manager.StateUpdateList[2]==all[4]);
  for(int j=0;j<capture_count;++j)Check(manager.StateStack[2+j].State==captured[j]&&manager.StateStack[2+j].Value==prior[j]);
  for(int j=0;j<queue_count;++j)Check(manager.StateUpdateList[3+j]==queued[j]);
  for(int j=0;j<all_count;++j){Check(all[j]->DesiredState==new_value[j]&&all[j]->BookmarkMask==new_mask[j]&&all[j]->DirtyListFlag==new_dirty[j]);Check(all[j]->CurrentState==(unsigned long)(71+j)&&all[j]->RenderStateParam==55&&all[j]->StateType==6&&all[j]->TextureStage==7);}
  block.Apply();Check(manager.StateStackSize==2+capture_count&&manager.StateUpdateListSize==3+queue_count);Check(get_system_calls==2&&get_display_calls==2&&get_render_calls==2);++cases;
 }
 Check(cases==2048);std::puts("009e0410_TEST PASS");return 0;
}