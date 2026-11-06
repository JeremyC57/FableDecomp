#ifndef FABLE_ENGINE_CRENDERSTATEMANAGER_H
#define FABLE_ENGINE_CRENDERSTATEMANAGER_H
#ifdef FABLE_RETAIL_CRENDERSTATEMANAGER
// Retail branch: one model (merged 2026-11-03 from chunk 723b/723c, the water / sprite render passes, and Codex pass 13, row
// 0x009DA9F0 CRenderManager2D::Render2DDrawList; Codex's FABLE_RETAIL_RENDER2D_TYPES model and its FABLE_RETAIL_RENDERSTATE_EXPAND_SET
// opt-in are folded in). The polymorphic retail class with its nested PDB types and the inline state setters every render
// pass expands. RTTI CRenderStateManager : CBaseClassNonCopyable : CBaseClass (vtable 0x0129C978; ~CRenderStateManager 0x00A044B0
// stores it, clears CRenderStateCaptureBlock::StateManager 0x013CB508 and PD3DDevice, then chains to ~CBaseClassNonCopyable
// 0x0099A300). Members: Ego_r.pdb names and offsets (sizeof 0x3A3C, the same in retail: the CRenderManagerCore constructor
// 0x00A0AFE0 stores its own first member at +0x3A3C); the nested CStateInfo (0x14, U-keyed:
// ?SetRenderState@CRenderStateManager@@AAEXKAAUCStateInfo@1@@Z), CStateStackElement (8) and CDeviceState (0x120C) are typed
// here (Ego_r.pdb struct layouts); data private as in the PDB declarations.
// Functions: Ego_r.pdb / FableWin.pdb spellings; the inline ones are __forceinline (plain inline runs out of VC7.1's inline
// budget in a pass that expands a dozen of them, 0x00B70230 and 0x009DA9F0). Out of line in retail: RestoreCaptureBlock()
// 0x00A05840, RealiseRenderState() 0x00A058C0. Inline everywhere (no retail copy): StartCaptureBlock, SetRenderState (private),
// AddStateToUpdateList (private) and the public Set* state setters below, with the PDB parameter types; each enum setter maps the
// engine enum (engine/RenderStateEnums.h) through its private static D3D table. Table names: the PDB DATA records Codex pass 13
// cited (verify with cvdump; chunk 723's descriptive g_textureUVModeToD3D / D3DBlendOpTable / D3DAlphaTestFuncTable are these
// tables). SetRenderState reads CurrentBookmarkMask directly: a local copy schedules the mask load before the state's mask for a
// constant value (0x00B75A0D). Lane gap: the bookmark test / or operand order varies block by block in retail and no
// spelling moves it (0x00B70230 3 of 7 blocks, 0x00B761D0 6 of 8, 0x00B751F0 6 of 7 come out the other way).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/RenderStateEnums.h"
struct IDirect3DDevice9;
enum _D3DRENDERSTATETYPE;                                                // d3d9types.h (MS forward enums)
enum _D3DTEXTURESTAGESTATETYPE;
enum _D3DSAMPLERSTATETYPE;

class CRenderStateManager : public CBaseClassNonCopyable {
public:
    struct CStateInfo {                                                  // 0x14 bytes (Ego_r.pdb)
        CStateInfo();                                                    // ??0CStateInfo@CRenderStateManager@@QAE@XZ
        unsigned long CurrentState;                                      // +0x00
        unsigned long DesiredState;                                      // +0x04
        unsigned long BookmarkMask;                                      // +0x08
        union {                                                          // +0x0c CStateInfo_u_12: the D3D render/stage/sampler state id
            _D3DTEXTURESTAGESTATETYPE TextureStageStateParam;
            _D3DRENDERSTATETYPE       RenderStateParam;
            _D3DSAMPLERSTATETYPE      SamplerStageStateParam;
        };
        bool          DirtyListFlag;                                     // +0x10
        unsigned char StateType;                                         // +0x11
        unsigned char TextureStage;                                      // +0x12
    };
    struct CStateStackElement {                                          // 8 bytes (Ego_r.pdb)
        CStateInfo*   State;                                             // +0x00
        unsigned long Value;                                             // +0x04
    };
    class CDeviceState {                                                 // 0x120C bytes (Ego_r.pdb)
    public:
        CDeviceState();                                                  // ??0CDeviceState@CRenderStateManager@@QAE@XZ
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

    CRenderStateManager(unsigned long initFlags);                        // 0x00A05970 ??0CRenderStateManager@@QAE@K@Z
    virtual ~CRenderStateManager();                                      // slot 0  0x00A044B0 ??1CRenderStateManager@@UAE@XZ
    void Init(IDirect3DDevice9* device);                                 // ?Init@CRenderStateManager@@QAEXPAUIDirect3DDevice9@@@Z
    void ForceResendRenderStatesToHardware();                            // 0x00A044E0
    void SetDefaultRenderStates();                                       // 0x00A059E0 ?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ
    void RealiseRenderState();                                           // 0x00A058C0 ?RealiseRenderState@CRenderStateManager@@QAEXXZ
    void RestoreCaptureBlock();                                          // 0x00A05840 ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ
    // ?StartCaptureBlock@CRenderStateManager@@QAEXPAX@Z: push the bookmark marker and open a new bookmark bit (the block
    // argument is not stored in retail: 0x00B70230 writes only StateStack[n].State).
    __forceinline void StartCaptureBlock(void* block)
    {
        StateStack[StateStackSize++].State = &BookmarkDummyState;
        CurrentBookmarkMask <<= 1;
    }

    // Public state setters (Ego_r.pdb / FableWin.pdb spellings and parameter types), inline in retail. A bool setter passes
    // `enable ? 1 : 0` (0x009DA9F0 SetZBufferEnable(zbuffer): `xor ecx,ecx ; test dl,dl ; setne cl`, not movzx).
    __forceinline void SetPolyCullMode(POLY_CULL_MODE mode) { SetRenderState(D3DPolyCullModeTable[mode], DeviceState.PolyCullMode); }
    __forceinline void SetPolyFillMode(POLY_FILL_MODE mode) { SetRenderState(D3DPolyFillModeTable[mode], DeviceState.PolyFillMode); }
    __forceinline void SetPolyShadeMode(POLY_SHADE_MODE mode) { SetRenderState(D3DPolyShadeModeTable[mode], DeviceState.PolyShadeMode); }
    __forceinline void SetZBufferWriteEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ZBufferWriteEnable); }
    __forceinline void SetZBufferEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ZBufferEnable); }
    __forceinline void SetZBufferTestFunc(ZBUFFER_FUNC func) { SetRenderState(D3DZBufferFuncModeTable[func], DeviceState.ZBufferFunc); }
    __forceinline void SetAlphaBlendEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.AlphaBlendEnable); }
    __forceinline void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(D3DAlphaBlendModeTable[mode], DeviceState.SourceAlphaBlendMode); }
    __forceinline void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(D3DAlphaBlendModeTable[mode], DeviceState.DestAlphaBlendMode); }
    __forceinline void SetAlphaTestEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.AlphaTestEnable); }
    __forceinline void SetAlphaTestFunc(ALPHA_TEST_FUNC func) { SetRenderState(D3DAlphaFuncModeTable[func], DeviceState.AlphaTestFunc); }
    __forceinline void SetAlphaRef(unsigned long ref) { SetRenderState(ref, DeviceState.AlphaRef); }
    __forceinline void SetBlendOp(BLEND_OP op) { SetRenderState(D3DBlendOpModeTable[op], DeviceState.BlendOpMode); }
    __forceinline void SetFogEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.FogEnable); }
    __forceinline void SetSpecularEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.SpecularEnable); }
    __forceinline void SetClippingEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.ClippingEnable); }
    __forceinline void SetTextureFilterModeMin(long stage, TEXTURE_FILTER_MODE mode) { SetRenderState(D3DTextureFilterModeTable[mode], DeviceState.TextureFilterModeMin[stage]); }
    __forceinline void SetTextureFilterModeMag(long stage, TEXTURE_FILTER_MODE mode) { SetRenderState(D3DTextureFilterModeTable[mode], DeviceState.TextureFilterModeMag[stage]); }
    __forceinline void SetTextureMipMapMode(long stage, TEXTURE_MIPMAP_MODE mode) { SetRenderState(D3DTextureMipMapModeTable[mode], DeviceState.TextureMipMapMode[stage]); }
    __forceinline void SetTextureUVModeU(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], DeviceState.TextureUVModeU[stage]); }
    __forceinline void SetTextureUVModeV(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], DeviceState.TextureUVModeV[stage]); }
    __forceinline void SetTextureUVModeW(long stage, TEXTURE_UV_MODE mode) { SetRenderState(D3DTextureUVModeTable[mode], DeviceState.TextureUVModeW[stage]); }
    __forceinline void SetTextureCoordIndex(long stage, long index) { SetRenderState(index, DeviceState.TextureCoordIndex[stage]); }
    // Texture stage colour / alpha operation and argument setters (?SetTextureOperation@CRenderStateManager@@QAEXJW4TEXTURE_OP_MODE@@@Z,
    // ?SetTextureArgument1@CRenderStateManager@@QAEXJW4TEXTURE_ARG_MODE@@@Z, ...): FableWin.pdb has the out-of-line debug copies, Ego_r.pdb
    // none (inline in release); the CEngineStateBlock* Apply bodies 0x00B573A0.. expand them through D3DTextureOperationModeTable
    // 0x01396E70 / D3DTextureArgumentModeTable 0x01396EE8.
    __forceinline void SetTextureOperation(long stage, TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode], DeviceState.TextureOperation[stage]); }
    __forceinline void SetTextureAlphaOperation(long stage, TEXTURE_OP_MODE mode) { SetRenderState(D3DTextureOperationModeTable[mode], DeviceState.TextureAlphaOperation[stage]); }
    __forceinline void SetTextureArgument1(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureArgument1[stage]); }
    __forceinline void SetTextureArgument2(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureArgument2[stage]); }
    __forceinline void SetTextureAlphaArgument1(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureAlphaArgument1[stage]); }
    __forceinline void SetTextureAlphaArgument2(long stage, TEXTURE_ARG_MODE mode) { SetRenderState(D3DTextureArgumentModeTable[mode], DeviceState.TextureAlphaArgument2[stage]); }
    // More public setters with no out-of-line copy in Ego_r.pdb (FableWin.pdb has the debug bodies), expanded by
    // CEngine::SetDefaultRenderStates 0x00B24850: fog colour / range (a float's bits are the D3D value: retail spills the constant
    // to a stack slot and reloads it as a dword, 0x00B24E7E), dither, lighting, anisotropy and the texture transform flags
    // (D3DTextureTransformModeTable 0x01396FCC).
    __forceinline void SetFogColour(unsigned long colour) { SetRenderState(colour, DeviceState.FogColour); }
    __forceinline void SetFogStart(float start) { SetRenderState(*(unsigned long*)&start, DeviceState.FogStart); }
    __forceinline void SetFogEnd(float end) { SetRenderState(*(unsigned long*)&end, DeviceState.FogEnd); }
    __forceinline void SetDitherEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.DitherEnable); }
    __forceinline void SetLightingEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.LightingEnable); }
    __forceinline void SetTextureMaxAnisotropy(long stage, long anisotropy) { SetRenderState(anisotropy, DeviceState.TextureMaxAnisotropy[stage]); }
    __forceinline void SetTextureTransformMode(long stage, TEXTURE_TRANSFORM_MODE mode) { SetRenderState(D3DTextureTransformModeTable[mode], DeviceState.TextureTransformMode[stage]); }
    // ?SetTextureMipmapLODBias@CRenderStateManager@@QAEXJM@Z: the float's bits are the D3D value (0x00B705DE: the bias is copied
    // to a stack slot and reloaded as a dword).
    __forceinline void SetTextureMipmapLODBias(long stage, float bias) { SetRenderState(*(unsigned long*)&bias, DeviceState.TextureMipmapLODBias[stage]); }

    // The rest of the PDB interface (out of line).
#ifdef FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
    // Chunk 773 (row 0x00F38A10 CSurfaceCompositor::SetBlendMode): ?SetColourWriteEnable@CRenderStateManager@@QAEX_N000@Z is in
    // FableWin.pdb only (debug copy); Ego_r.pdb and retail Fable.exe have no out-of-line copy, and 0x00F38F71 expands it with the
    // folded D3DRS_COLORWRITEENABLE mask 7 (red | green | blue) straight into SetRenderState on ColourWriteEnable (+0x28).
    __forceinline void SetColourWriteEnable(bool red, bool green, bool blue, bool alpha)
    {
        SetRenderState((red ? 1 : 0) | (green ? 2 : 0) | (blue ? 4 : 0) | (alpha ? 8 : 0), DeviceState.ColourWriteEnable);
    }
#else
    void SetColourWriteEnable(bool,bool,bool,bool);
#endif
    void SetColourWriteEnable(bool);
    void SetVertexBlending(VERTEX_BLEND_MODE);
    void SetFogDensity(float);
    void SetFogMode(FOG_MODE);
    void SetFogVertexMode(FOG_MODE);
    void SetTextureFactor(unsigned long);
    void SetNormalizeNormals(bool);
    void SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE);
    void SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE);
    void SetSpecularMaterialColourSource(MATERIAL_COLOUR_SOURCE);
    void SetPointSpriteEnable(bool);
    void SetPointScaleEnable(bool);
    void SetPointSize(float);
    void SetPointSizeMin(float);
    void SetPointSizeMax(float);
    void SetPointScaleConstantFactor(float);
    void SetPointScaleDistanceFactor(float);
    void SetPointScaleDistanceSqrFactor(float);
    void SetTextureCoordWrapUVW(long,bool,bool,bool);
    void SetTextureBorderColour(long,unsigned long);
    void SetTextureBumpEnvMatrix(long,float,float,float,float);
#ifdef FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
    // Retail opt-in (chunk 792, rows 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter / 0x00B5AF80 RenderColorFilteredQuad):
    // ?SetStencilEnable@CRenderStateManager@@QAEX_N@Z, ?SetStencilFunc@CRenderStateManager@@QAEXW4STENCIL_FUNC@@@Z,
    // ?SetStencilRef@CRenderStateManager@@QAEXK@Z, ?SetStencilMask@CRenderStateManager@@QAEXK@Z (Ego_r.pdb / FableWin.pdb name sets) have
    // no out-of-line copy in retail Fable.exe: both rows expand them into SetRenderState on StencilEnable +0x370 (0x00B5AB01, value 1),
    // StencilFunc +0x3C0 (0x00B5AB6D, D3DStencilFuncModeTable 0x01396E18 [STENCIL_FUNC_EQUAL] = D3DCMP_EQUAL), StencilMask +0x3E8
    // (0x00B5ABDA, immediate 2) and StencilRef +0x3D4 (0x00B5AC46, immediate 0). The default branch keeps the declarations.
    __forceinline void SetStencilEnable(bool enable) { SetRenderState(enable ? 1 : 0, DeviceState.StencilEnable); }
    __forceinline void SetStencilFunc(STENCIL_FUNC func) { SetRenderState(D3DStencilFuncModeTable[func], DeviceState.StencilFunc); }
    __forceinline void SetStencilRef(unsigned long ref) { SetRenderState(ref, DeviceState.StencilRef); }
    __forceinline void SetStencilMask(unsigned long mask) { SetRenderState(mask, DeviceState.StencilMask); }
    void SetStencilOpFail(STENCIL_OP_MODE);
    void SetStencilOpZFail(STENCIL_OP_MODE);
    void SetStencilOpPass(STENCIL_OP_MODE);
#else
    void SetStencilEnable(bool);
    void SetStencilOpFail(STENCIL_OP_MODE);
    void SetStencilOpZFail(STENCIL_OP_MODE);
    void SetStencilOpPass(STENCIL_OP_MODE);
    void SetStencilFunc(STENCIL_FUNC);
    void SetStencilRef(unsigned long);
    void SetStencilMask(unsigned long);
#endif
    void SetStencilWriteMask(unsigned long);
    void SetDepthOffset(float);
    void SetSlopeScaledDepthOffset(float);
    void SetSoftwareVertexProcessing(bool);
    void SetShadowFunc(SHADOW_FUNC);
    void SetSolidOffsetEnable(bool);
    bool IsSoftwareProcessingEnabled();
    void SetStippleEnable(bool);
    void SetStipplePattern(unsigned long *);
    void SetMultiSampleMode(MULTI_SAMPLE_MODE);
    void SetMultiSampleModeRenderTarget(MULTI_SAMPLE_MODE);
    void SetTextureColourSign(long,TEXTURE_COLOUR_SIGN);
    void SetBlendColour(unsigned long);

private:
    // The engine-enum -> D3D value tables the enum setters index (.data), private statics (?<Name>@CRenderStateManager@@0PAKA).
    static unsigned long D3DTextureUVModeTable[4];                       // 0x01396DC8 {1..4} = D3DTADDRESS_WRAP, MIRROR, CLAMP, BORDER
    static unsigned long D3DTextureFilterModeTable[3];                   // 0x01396E58
    static unsigned long D3DTextureMipMapModeTable[3];                   // 0x01396E64
    static unsigned long D3DTextureOperationModeTable[24];                // 0x01396E70 {1..24} = D3DTOP_DISABLE .. DOTPRODUCT3
    static unsigned long D3DTextureArgumentModeTable[20];                 // 0x01396EE8 {1, 0, 2, 3, 4, 0x11..} = D3DTA_CURRENT, DIFFUSE, TEXTURE ..
    static unsigned long D3DStencilOperationModeTable[8];
    static unsigned long D3DPolyShadeModeTable[2];                       // 0x01396F9C
    static unsigned long D3DPolyFillModeTable[3];                        // 0x01396FA4
    static unsigned long D3DPolyCullModeTable[3];                        // 0x01396FB0 {3, 2, 1} = D3DCULL_CCW, CW, NONE
    static unsigned long D3DAlphaBlendModeTable[13];                     // 0x01396F68 {1..13} = D3DBLEND_ZERO .. BOTHINVSRCALPHA
    static unsigned long D3DAlphaFuncModeTable[8];                       // 0x01396DD8 {1..8} = D3DCMP_NEVER .. ALWAYS (723: D3DAlphaTestFuncTable)
    static unsigned long D3DZBufferFuncModeTable[8];                     // 0x01396DF8
    static unsigned long D3DStencilFuncModeTable[8];                     // 0x01396E18 {1..8} = D3DCMP_NEVER .. ALWAYS (chunk 792)
    static unsigned long D3DVertexBlendModeTable[4];
    static unsigned long D3DShadowFuncModeTable[8];
    static unsigned long D3DTextureTransformModeTable[10];               // 0x01396FCC {0..4, 0x100..0x104} = D3DTTFF_DISABLE .. COUNT4|PROJECTED
    static unsigned long D3DMaterialColourSourceTable[3];
    static unsigned long D3DFogTableMode[4];
    static unsigned long D3DBlendOpModeTable[5];                         // 0x01396ED0 {1..5} = D3DBLENDOP_ADD .. MAX (723: D3DBlendOpTable)
    static _D3DRENDERSTATETYPE D3DTextureCoordWrapStageTable[8];
    // ?SetRenderState@CRenderStateManager@@AAEXKAAUCStateInfo@1@@Z: skip an unchanged value; otherwise bookmark the old value once per
    // capture block (the state's mask bit), store the new one and queue the state for RealiseRenderState.
    __forceinline void SetRenderState(unsigned long value, CStateInfo& state)
    {
        if (state.DesiredState != value) {
            if ((state.BookmarkMask & CurrentBookmarkMask) == 0) {
                state.BookmarkMask |= CurrentBookmarkMask;
                StateStack[StateStackSize].State = &state;
                StateStack[StateStackSize].Value = state.DesiredState;
                ++StateStackSize;
            }
            state.DesiredState = value;
            AddStateToUpdateList(state);
        }
    }
    // ?AddStateToUpdateList@CRenderStateManager@@AAEXAAUCStateInfo@1@@Z
    __forceinline void AddStateToUpdateList(CStateInfo& state)
    {
        if (!state.DirtyListFlag) {
            state.DirtyListFlag = true;
            StateUpdateList[StateUpdateListSize] = &state;
            ++StateUpdateListSize;
        }
    }
    void SetupRenderStates();
    void SetupRenderState(CStateInfo &,_D3DRENDERSTATETYPE);
    void SetupTextureStageState(CStateInfo &,_D3DTEXTURESTAGESTATETYPE,long);
    void SetupSamplerStageState(CStateInfo &,_D3DSAMPLERSTATETYPE,long);
    void CheckDeviceStateMatchesCache();

    IDirect3DDevice9*  PD3DDevice;                                       // +0x04
    CStateStackElement StateStack[1024];                                 // +0x08
    CStateInfo*        StateUpdateList[256];                             // +0x2008
    CStateInfo*        AllStatesList[256];                               // +0x2408
    long               StateStackSize;                                   // +0x2808
    long               StateUpdateListSize;                              // +0x280c
    long               AllStatesListSize;                                // +0x2810
    unsigned long      CurrentBookmarkMask;                              // +0x2814
    CDeviceState       DeviceState;                                      // +0x2818
    CStateInfo         BookmarkDummyState;                               // +0x3a24
    unsigned long      InitFlags;                                        // +0x3a38
};
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager) == 0x3a3c);
#else // generated flattened Ego_r layout (the landed default, unchanged)
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

#endif // FABLE_RETAIL_CRENDERSTATEMANAGER
#endif // FABLE_ENGINE_CRENDERSTATEMANAGER_H
