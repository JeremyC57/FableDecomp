// Full selected CEngine body. Dependency layout is portable; native offsets are graded separately.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static void Check(bool ok,int line){if(!ok){printf("render failure %d\n",line);exit(1);}}
#define CHECK(c) Check((c),__LINE__)
static unsigned int FloatBits(float f){unsigned int u;memcpy(&u,&f,4);return u;}
static char events[128];static int eventCount;
static void Event(char e){events[eventCount++]=e;events[eventCount]=0;}

// The CRenderStateManager setter enums (Codex pass 13: Ego_r.pdb LF_ENUM field lists, evidence/009da9f0_Ego_r_final_enums.json),
// one model for every render row. The enumerator order is the order of the D3D tables the setters index (chunk 723b/723c read
// the same tables: TEXTURE_UV_MODE 0x01396DC8 {WRAP, MIRROR, CLAMP, BORDER}, POLY_CULL_MODE 0x01396FB0 {CCW 3, CW 2, NONE 1},
// ALPHA_BLEND_MODE 0x01396F68 {D3DBLEND_ZERO..}, ALPHA_TEST_FUNC 0x01396DD8 {D3DCMP_NEVER..}, BLEND_OP 0x01396ED0 {ADD..}).
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
class CRenderStateManager {public:
struct CStateInfo {unsigned int CurrentState,DesiredState,BookmarkMask;bool DirtyListFlag;CStateInfo():CurrentState(0x1234),DesiredState(0xdead),BookmarkMask(0),DirtyListFlag(false){} };
struct CStateStackElement {CStateInfo* State;unsigned int Value;};
class CDeviceState {public:

        CStateInfo PolyShadeMode;               // +0x000
        CStateInfo PolyFillMode;                // +0x014
        CStateInfo ColourWriteEnable;           // +0x028
        CStateInfo ZBufferWriteEnable;          // +0x03c
        CStateInfo ZBufferEnable;               // +0x050
        CStateInfo ZBufferFunc;                 // +0x064
        CStateInfo PolyCullMode;                // +0x078
        CStateInfo AlphaTestFunc;               // +0x08c
        CStateInfo AlphaBlendEnable;            // +0x0a0
        CStateInfo AlphaTestEnable;             // +0x0b4
        CStateInfo AlphaRef;                    // +0x0c8
        CStateInfo VertexBlendMode;             // +0x0dc
        CStateInfo SoftwareVertexProcessing;    // +0x0f0
        CStateInfo SourceAlphaBlendMode;        // +0x104
        CStateInfo DestAlphaBlendMode;          // +0x118
        CStateInfo FogEnable;                   // +0x12c
        CStateInfo FogColour;                   // +0x140
        CStateInfo FogStart;                    // +0x154
        CStateInfo FogEnd;                      // +0x168
        CStateInfo FogDensity;                  // +0x17c
        CStateInfo FogMode;                     // +0x190
        CStateInfo FogVertexMode;               // +0x1a4
        CStateInfo TextureFactor;               // +0x1b8
        CStateInfo SpecularEnable;              // +0x1cc
        CStateInfo AntialiasingOn;              // +0x1e0
        CStateInfo DitherEnable;                // +0x1f4
        CStateInfo ClippingEnable;              // +0x208
        CStateInfo LightingEnable;              // +0x21c
        CStateInfo NormalizeNormals;            // +0x230
        CStateInfo AmbientMaterialColourSource; // +0x244
        CStateInfo DiffuseMaterialColourSource; // +0x258
        CStateInfo SpecularMaterialColourSource; // +0x26c
        CStateInfo PointSpriteEnable;           // +0x280
        CStateInfo PointScaleEnable;            // +0x294
        CStateInfo PointSize;                   // +0x2a8
        CStateInfo PointSizeMin;                // +0x2bc
        CStateInfo PointSizeMax;                // +0x2d0
        CStateInfo PointScaleConstantFactor;    // +0x2e4
        CStateInfo PointScaleDistanceFactor;    // +0x2f8
        CStateInfo PointScaleDistanceSqrFactor; // +0x30c
        CStateInfo ShadowFunc;                  // +0x320
        CStateInfo SolidOffsetEnable;           // +0x334
        CStateInfo DepthOffset;                 // +0x348
        CStateInfo DepthOffsetSlope;            // +0x35c
        CStateInfo StencilEnable;               // +0x370
        CStateInfo StencilOpFail;               // +0x384
        CStateInfo StencilOpZFail;              // +0x398
        CStateInfo StencilOpPass;               // +0x3ac
        CStateInfo StencilFunc;                 // +0x3c0
        CStateInfo StencilRef;                  // +0x3d4
        CStateInfo StencilMask;                 // +0x3e8
        CStateInfo StencilWriteMask;            // +0x3fc
        CStateInfo StippleEnable;               // +0x410
        CStateInfo BlendOpMode;                 // +0x424
        CStateInfo BlendColour;                 // +0x438
        CStateInfo TextureCoordWrapMode[8];     // +0x44c
        CStateInfo TextureMaxAnisotropy[8];     // +0x4ec
        CStateInfo TextureMipmapLODBias[8];     // +0x58c
        CStateInfo TextureMipMapMode[8];        // +0x62c
        CStateInfo TextureUVModeU[8];           // +0x6cc
        CStateInfo TextureUVModeV[8];           // +0x76c
        CStateInfo TextureUVModeW[8];           // +0x80c
        CStateInfo TextureFilterModeMin[8];     // +0x8ac
        CStateInfo TextureFilterModeMag[8];     // +0x94c
        CStateInfo TextureOperation[8];         // +0x9ec
        CStateInfo TextureAlphaOperation[8];    // +0xa8c
        CStateInfo TextureArgument1[8];         // +0xb2c
        CStateInfo TextureArgument2[8];         // +0xbcc
        CStateInfo TextureAlphaArgument1[8];    // +0xc6c
        CStateInfo TextureAlphaArgument2[8];    // +0xd0c
        CStateInfo TextureCoordIndex[8];        // +0xdac
        CStateInfo TextureTransformMode[8];     // +0xe4c
        CStateInfo TextureBorderColour[8];      // +0xeec
        CStateInfo TextureBumpEnvMat00[8];      // +0xf8c
        CStateInfo TextureBumpEnvMat01[8];      // +0x102c
        CStateInfo TextureBumpEnvMat10[8];      // +0x10cc
        CStateInfo TextureBumpEnvMat11[8];      // +0x116c
};
CDeviceState DeviceState;CStateStackElement StateStack[1024];CStateInfo* StateUpdateList[256];
long StateStackSize,StateUpdateListSize;unsigned int CurrentBookmarkMask;
CRenderStateManager():StateStackSize(0),StateUpdateListSize(0),CurrentBookmarkMask(1){}
static unsigned int D3DAlphaBlendModeTable[32];
static unsigned int D3DAlphaFuncModeTable[32];
static unsigned int D3DPolyCullModeTable[32];
static unsigned int D3DPolyFillModeTable[32];
static unsigned int D3DPolyShadeModeTable[32];
static unsigned int D3DTextureArgumentModeTable[32];
static unsigned int D3DTextureFilterModeTable[32];
static unsigned int D3DTextureMipMapModeTable[32];
static unsigned int D3DTextureOperationModeTable[32];
static unsigned int D3DTextureTransformModeTable[32];
static unsigned int D3DTextureUVModeTable[32];
static unsigned int D3DZBufferFuncModeTable[32];
void SetAlphaBlendEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.AlphaBlendEnable); }
void SetAlphaRef(unsigned int ref) { SetRenderState(ref, DeviceState.AlphaRef); }
void SetAlphaTestFunc(ALPHA_TEST_FUNC func) { SetRenderState(D3DAlphaFuncModeTable[func], DeviceState.AlphaTestFunc); }
void SetClippingEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ClippingEnable); }
void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(D3DAlphaBlendModeTable[mode], DeviceState.DestAlphaBlendMode); }
void SetDitherEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.DitherEnable); }
void SetFogColour(unsigned int colour) { SetRenderState(colour, DeviceState.FogColour); }
void SetFogEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.FogEnable); }
void SetFogEnd(float end) { SetRenderState(FloatBits(end), DeviceState.FogEnd); }
void SetFogStart(float start) { SetRenderState(FloatBits(start), DeviceState.FogStart); }
void SetLightingEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.LightingEnable); }
void SetPolyCullMode(POLY_CULL_MODE mode) { SetRenderState(D3DPolyCullModeTable[mode], DeviceState.PolyCullMode); }
void SetPolyFillMode(POLY_FILL_MODE mode) { SetRenderState(D3DPolyFillModeTable[mode], DeviceState.PolyFillMode); }
void SetPolyShadeMode(POLY_SHADE_MODE mode) { SetRenderState(D3DPolyShadeModeTable[mode], DeviceState.PolyShadeMode); }
void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(D3DAlphaBlendModeTable[mode], DeviceState.SourceAlphaBlendMode); }
void SetSpecularEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.SpecularEnable); }
void SetTextureAlphaArgument1(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureAlphaArgument1[stage]); }
void SetTextureAlphaArgument2(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureAlphaArgument2[stage]); }
void SetTextureAlphaOperation(long stage, TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode], DeviceState.TextureAlphaOperation[stage]); }
void SetTextureArgument1(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureArgument1[stage]); }
void SetTextureArgument2(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureArgument2[stage]); }
void SetTextureFilterModeMag(long stage, TEXTURE_FILTER_MODE mode) { SetRenderState(D3DTextureFilterModeTable[mode], DeviceState.TextureFilterModeMag[stage]); }
void SetTextureFilterModeMin(long stage, TEXTURE_FILTER_MODE mode) { SetRenderState(D3DTextureFilterModeTable[mode], DeviceState.TextureFilterModeMin[stage]); }
void SetTextureMaxAnisotropy(long stage, long anisotropy) { SetRenderState(anisotropy, DeviceState.TextureMaxAnisotropy[stage]); }
void SetTextureMipMapMode(long stage, TEXTURE_MIPMAP_MODE mode) { SetRenderState(D3DTextureMipMapModeTable[mode], DeviceState.TextureMipMapMode[stage]); }
void SetTextureMipmapLODBias(long stage, float bias) { SetRenderState(FloatBits(bias), DeviceState.TextureMipmapLODBias[stage]); }
void SetTextureOperation(long stage, TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode], DeviceState.TextureOperation[stage]); }
void SetTextureTransformMode(long stage, TEXTURE_TRANSFORM_MODE mode) { SetRenderState(D3DTextureTransformModeTable[mode], (DeviceState.TextureTransformMode[stage])); }
void SetTextureUVModeU(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], DeviceState.TextureUVModeU[stage]); }
void SetTextureUVModeV(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], DeviceState.TextureUVModeV[stage]); }
void SetTextureUVModeW(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], DeviceState.TextureUVModeW[stage]); }
void SetZBufferEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ZBufferEnable); }
void SetZBufferTestFunc(ZBUFFER_FUNC func) { SetRenderState(D3DZBufferFuncModeTable[func], DeviceState.ZBufferFunc); }
void SetZBufferWriteEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ZBufferWriteEnable); }
    void SetRenderState(unsigned int value, CStateInfo& state_info)
    {
        if (state_info.DesiredState == value) return;
        {
            if ((state_info.BookmarkMask & CurrentBookmarkMask) == 0) {
                state_info.BookmarkMask |= CurrentBookmarkMask;
                StateStack[StateStackSize].State = &state_info;
                StateStack[StateStackSize].Value = state_info.DesiredState;
                ++StateStackSize;
            }
            state_info.DesiredState = value;
            AddStateToUpdateList(state_info);
        }
    }
    void AddStateToUpdateList(CStateInfo& state)
    {
        if (!state.DirtyListFlag) {
            state.DirtyListFlag = true;
            StateUpdateList[StateUpdateListSize] = &state;
            ++StateUpdateListSize;
        }
    }
};
unsigned int CRenderStateManager::D3DAlphaBlendModeTable[32];
unsigned int CRenderStateManager::D3DAlphaFuncModeTable[32];
unsigned int CRenderStateManager::D3DPolyCullModeTable[32];
unsigned int CRenderStateManager::D3DPolyFillModeTable[32];
unsigned int CRenderStateManager::D3DPolyShadeModeTable[32];
unsigned int CRenderStateManager::D3DTextureArgumentModeTable[32];
unsigned int CRenderStateManager::D3DTextureFilterModeTable[32];
unsigned int CRenderStateManager::D3DTextureMipMapModeTable[32];
unsigned int CRenderStateManager::D3DTextureOperationModeTable[32];
unsigned int CRenderStateManager::D3DTextureTransformModeTable[32];
unsigned int CRenderStateManager::D3DTextureUVModeTable[32];
unsigned int CRenderStateManager::D3DZBufferFuncModeTable[32];
class CRenderManager:public CRenderStateManager {public:bool textures,xbox;void SetTextureEnable(bool b){textures=b;Event('T');}void EnableXBoxShaderConstants(bool b){xbox=b;Event('X');}};
class CEngineLightingManager {public:void SetupDefaultFogStates(){Event('F');}};
class CDisplayManager {public:unsigned int GetVertexShaderVersion()const{Event('V');return 0x200;}};
class CShaderRenderManager {public:long layout,count;float ux,uy,bx,by;
void SetVertexShaderConstantLayout(long n){layout=n;Event('L');}
void SetTextureTransformCount(long n){count=n;Event('C');}
void SetUVCompressionVertexShaderConstant(float a,float b,float c,float d){ux=a;uy=b;bx=c;by=d;Event('U');}};
class CEngine {public:long AnisotropicFilteringLevel;bool EnableTextures,EnableWireFrame;void SetDefaultRenderStates();};
namespace NEngineInternal {CRenderManager* GPRenderManager;CEngineLightingManager* GPLightingManager;CDisplayManager* GPDisplayManager;CShaderRenderManager* GPShaderRenderManager;}
using namespace NEngineInternal;

void CEngine::SetDefaultRenderStates()
{
    long stage;
    for (stage = 0; stage < 8; ++stage) {
        GPRenderManager->SetTextureAlphaOperation(stage, TEXTURE_OP_DISABLE);
        GPRenderManager->SetTextureArgument1(stage, TEXTURE_ARG_TEXTURE);
        GPRenderManager->SetTextureArgument2(stage, TEXTURE_ARG_DIFFUSE);
        GPRenderManager->SetTextureAlphaArgument1(stage, TEXTURE_ARG_TEXTURE);
        GPRenderManager->SetTextureAlphaArgument2(stage, TEXTURE_ARG_DIFFUSE);
    }
    GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_ALWAYS);
    GPRenderManager->SetPolyShadeMode(POLY_SHADE_GOURAUD);
    GPRenderManager->SetPolyFillMode(POLY_FILL_SOLID);
    GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    GPRenderManager->SetZBufferWriteEnable(true);
    GPRenderManager->SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    GPRenderManager->SetZBufferEnable(true);
    GPRenderManager->SetFogEnable(false);
    GPRenderManager->SetFogColour(0xffffff);
    GPRenderManager->SetFogStart(0.0f);
    GPRenderManager->SetFogEnd(1.0f);
    GPRenderManager->SetAlphaRef(0x80);
    GPRenderManager->SetSpecularEnable(false);
    GPRenderManager->SetDitherEnable(true);
    for (stage = 0; stage < 8; ++stage) {
        GPRenderManager->SetTextureMaxAnisotropy(stage, AnisotropicFilteringLevel);
        GPRenderManager->SetTextureMipmapLODBias(stage, 0.0f);
        GPRenderManager->SetTextureMipMapMode(stage, TEXTURE_MIPMAP_LINEAR);
        GPRenderManager->SetTextureFilterModeMin(stage, TEXTURE_FILTER_LINEAR);
        GPRenderManager->SetTextureFilterModeMag(stage, TEXTURE_FILTER_LINEAR);
        GPRenderManager->SetTextureUVModeU(stage, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureUVModeV(stage, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureUVModeW(stage, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureTransformMode(0, TEXTURE_TRANSFORM_DIABLED);
    }
    GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);
    GPRenderManager->SetTextureOperation(1, TEXTURE_OP_DISABLE);
    GPRenderManager->SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    GPRenderManager->SetTextureOperation(0, TEXTURE_OP_MODULATE);
    GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    GPRenderManager->SetTextureEnable(EnableTextures);
    GPRenderManager->SetAlphaBlendEnable(false);
    GPRenderManager->SetClippingEnable(true);
    GPRenderManager->SetLightingEnable(false);
    NEngineInternal::GPLightingManager->SetupDefaultFogStates();
    GPShaderRenderManager->SetVertexShaderConstantLayout(3);
    GPShaderRenderManager->SetTextureTransformCount(0);
    GPRenderManager->EnableXBoxShaderConstants(true);
    NEngineInternal::GPDisplayManager->GetVertexShaderVersion();
    GPShaderRenderManager->SetUVCompressionVertexShaderConstant(1.0f / 2048.0f, 1.0f / 2048.0f, -8.0f, -8.0f);
    if (EnableWireFrame)
        GPRenderManager->SetPolyFillMode(POLY_FILL_WIRE);
}

static void Expected(CRenderStateManager::CStateInfo& st,unsigned int value,bool dirty,unsigned int mask){CHECK(st.DesiredState==value);CHECK(st.DirtyListFlag==dirty);CHECK(st.BookmarkMask==mask);CHECK(st.CurrentState==0x1234);}
int main(){
for(int i=0;i<32;++i) CRenderStateManager::D3DAlphaBlendModeTable[i]=100+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DAlphaFuncModeTable[i]=200+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DPolyCullModeTable[i]=300+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DPolyFillModeTable[i]=400+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DPolyShadeModeTable[i]=500+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DTextureArgumentModeTable[i]=600+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DTextureFilterModeTable[i]=700+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DTextureMipMapModeTable[i]=800+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DTextureOperationModeTable[i]=900+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DTextureTransformModeTable[i]=1000+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DTextureUVModeTable[i]=1100+i;
for(int i=0;i<32;++i) CRenderStateManager::D3DZBufferFuncModeTable[i]=1200+i;
for(int wire=0;wire!=2;++wire)for(int texture=0;texture!=2;++texture){
 CRenderManager manager;CEngineLightingManager lighting;CDisplayManager display;CShaderRenderManager shader;CEngine engine;
 GPRenderManager=&manager;GPLightingManager=&lighting;GPDisplayManager=&display;GPShaderRenderManager=&shader;
 engine.AnisotropicFilteringLevel=4;engine.EnableTextures=texture!=0;engine.EnableWireFrame=wire!=0;
 eventCount=0;events[0]=0;engine.SetDefaultRenderStates();
 CHECK(strcmp(events,"TFLCXVU")==0);CHECK(manager.textures==(texture!=0));CHECK(manager.xbox);
 CHECK(shader.layout==3&&shader.count==0);CHECK(shader.ux==1.0f/2048.0f&&shader.uy==1.0f/2048.0f&&shader.bx==-8.0f&&shader.by==-8.0f);
 CRenderStateManager::CDeviceState& d=manager.DeviceState;
#define E(field,value) Expected(d.field,(unsigned int)(value),true,1)
 E(PolyShadeMode,CRenderStateManager::D3DPolyShadeModeTable[POLY_SHADE_GOURAUD]);
 E(PolyFillMode,CRenderStateManager::D3DPolyFillModeTable[wire?POLY_FILL_WIRE:POLY_FILL_SOLID]);
 E(PolyCullMode,CRenderStateManager::D3DPolyCullModeTable[POLY_CULL_CCW]);
 E(AlphaTestFunc,CRenderStateManager::D3DAlphaFuncModeTable[ALPHA_TEST_ALWAYS]);
 E(ZBufferFunc,CRenderStateManager::D3DZBufferFuncModeTable[ZBUFFER_FUNC_LESS_EQUAL]);
 E(ZBufferEnable,1);E(ZBufferWriteEnable,1);E(FogEnable,0);E(FogColour,0xffffff);E(FogStart,0);E(FogEnd,0x3f800000);
 E(AlphaRef,0x80);E(SpecularEnable,0);E(DitherEnable,1);E(AlphaBlendEnable,0);E(ClippingEnable,1);E(LightingEnable,0);
 E(SourceAlphaBlendMode,CRenderStateManager::D3DAlphaBlendModeTable[ALPHA_BLEND_ONE]);
 E(DestAlphaBlendMode,CRenderStateManager::D3DAlphaBlendModeTable[ALPHA_BLEND_ZERO]);
 for(int i=0;i!=8;++i){
  E(TextureAlphaOperation[i],CRenderStateManager::D3DTextureOperationModeTable[i==0?TEXTURE_OP_MODULATE:TEXTURE_OP_DISABLE]);
  E(TextureArgument1[i],CRenderStateManager::D3DTextureArgumentModeTable[TEXTURE_ARG_TEXTURE]);
  E(TextureArgument2[i],CRenderStateManager::D3DTextureArgumentModeTable[TEXTURE_ARG_DIFFUSE]);
  E(TextureAlphaArgument1[i],CRenderStateManager::D3DTextureArgumentModeTable[TEXTURE_ARG_TEXTURE]);
  E(TextureAlphaArgument2[i],CRenderStateManager::D3DTextureArgumentModeTable[TEXTURE_ARG_DIFFUSE]);
  E(TextureMaxAnisotropy[i],4);E(TextureMipmapLODBias[i],0);
  E(TextureMipMapMode[i],CRenderStateManager::D3DTextureMipMapModeTable[TEXTURE_MIPMAP_LINEAR]);
  E(TextureFilterModeMin[i],CRenderStateManager::D3DTextureFilterModeTable[TEXTURE_FILTER_LINEAR]);
  E(TextureFilterModeMag[i],CRenderStateManager::D3DTextureFilterModeTable[TEXTURE_FILTER_LINEAR]);
  E(TextureUVModeU[i],CRenderStateManager::D3DTextureUVModeTable[TEXTURE_UV_CLAMP]);
  E(TextureUVModeV[i],CRenderStateManager::D3DTextureUVModeTable[TEXTURE_UV_CLAMP]);
  E(TextureUVModeW[i],CRenderStateManager::D3DTextureUVModeTable[TEXTURE_UV_CLAMP]);
  if(i==0){E(TextureTransformMode[i],CRenderStateManager::D3DTextureTransformModeTable[TEXTURE_TRANSFORM_DIABLED]);}
  else Expected(d.TextureTransformMode[i],0xdead,false,0);
  if(i<2){E(TextureOperation[i],CRenderStateManager::D3DTextureOperationModeTable[i==0?TEXTURE_OP_MODULATE:TEXTURE_OP_DISABLE]);}
  else Expected(d.TextureOperation[i],0xdead,false,0);
  Expected(d.TextureCoordIndex[i],0xdead,false,0);
 }
 Expected(d.ColourWriteEnable,0xdead,false,0);Expected(d.AlphaTestEnable,0xdead,false,0);Expected(d.StencilEnable,0xdead,false,0);
 CHECK(manager.StateStackSize==manager.StateUpdateListSize);CHECK(manager.StateStackSize==126);
 for(long j=0;j<manager.StateStackSize;++j){CHECK(manager.StateStack[j].Value==0xdead);CHECK(manager.StateStack[j].State==manager.StateUpdateList[j]);for(long k=0;k<j;++k)CHECK(manager.StateStack[j].State!=manager.StateStack[k].State);}
 long saved=manager.StateStackSize;eventCount=0;engine.SetDefaultRenderStates();CHECK(manager.StateStackSize==saved&&manager.StateUpdateListSize==saved);CHECK(strcmp(events,"TFLCXVU")==0);
 // A changed state in a new capture preserves its old value once, while the dirty list stays unique.
 manager.CurrentBookmarkMask=2;engine.AnisotropicFilteringLevel=8;eventCount=0;engine.SetDefaultRenderStates();
 for(int n=0;n<8;++n)Expected(d.TextureMaxAnisotropy[n],8,true,3);
 CHECK(manager.StateUpdateListSize==saved);CHECK(manager.StateStackSize==saved+9+wire);
 int count=0;for(long q=saved;q<manager.StateStackSize;++q)for(int n=0;n<8;++n)if(manager.StateStack[q].State==&d.TextureMaxAnisotropy[n]){CHECK(manager.StateStack[q].Value==4);++count;}CHECK(count==8);
#undef E
}
printf("00b24850_TEST PASS\n");return 0;
}
