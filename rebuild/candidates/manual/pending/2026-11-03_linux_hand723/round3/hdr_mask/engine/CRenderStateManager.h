#ifndef FABLE_ENGINE_CRENDERSTATEMANAGER_H
#define FABLE_ENGINE_CRENDERSTATEMANAGER_H
#ifdef FABLE_RETAIL_CRENDERSTATEMANAGER
// Retail opt-in (chunk 723b, the water render passes): the polymorphic retail class with its nested PDB types and the inline
// state setters every render pass expands. RTTI CRenderStateManager : CBaseClassNonCopyable : CBaseClass (vtable 0x0129C978;
// ~CRenderStateManager 0x00A044B0 stores it, clears CRenderStateCaptureBlock::StateManager 0x013CB508 and PD3DDevice, then
// chains to ~CBaseClassNonCopyable 0x0099A300). Members: Ego_r.pdb names and offsets (sizeof 0x3A3C, the same in retail: the
// CRenderManagerCore constructor 0x00A0AFE0 stores its own first member at +0x3A3C); the nested CStateInfo (0x14, U-keyed:
// ?SetRenderState@CRenderStateManager@@AAEXKAAUCStateInfo@1@@Z), CStateStackElement (8) and CDeviceState (0x120C) are typed
// here (Ego_r.pdb struct layouts).
// Functions: Ego_r.pdb / FableWin.pdb spellings; the inline ones are __forceinline (plain inline runs out of VC7.1's inline
// budget in a pass that expands a dozen of them, 0x00B70230). Out of line in retail: RestoreCaptureBlock() 0x00A05840, RealiseRenderState()
// 0x00A058C0. Inline everywhere (no retail copy): StartCaptureBlock, SetRenderState (private), AddStateToUpdateList (private)
// and the public Set* state setters, with the PDB parameter types: SetTextureUVModeU/V/W(long, TEXTURE_UV_MODE) map the engine
// enum through the D3D address-mode table at 0x01396DC8 ({1,2,3,4} = D3DTADDRESS_WRAP, MIRROR, CLAMP, BORDER; read as
// [0x01396DC8 + 4*mode] by 0x00B70230 / 0x00B761D0 / 0x00B677D0; the table has no PDB name, g_textureUVModeToD3D is
// descriptive and the enumerator names are descriptive too, the PDB keeps only the W4TEXTURE_UV_MODE tag). The other enum-typed
// setters (ALPHA_BLEND_MODE, POLY_CULL_MODE, TEXTURE_FILTER_MODE, TEXTURE_MIPMAP_MODE, TEXTURE_OP_MODE, TEXTURE_ARG_MODE,
// TEXTURE_TRANSFORM_MODE) map through tables no row has pinned yet and are left out until one does. SetRenderState reads
// CurrentBookmarkMask directly: a `mask` local schedules the mask load before the state's mask for a constant value
// (0x00B75A0D, SetSpecularEnable(true) in 0x00B751F0 loads the state first). Lane gap: the bookmark `test` / `or` operand order
// (`test esi,eax ; or eax,esi` v `test eax,esi ; or esi,eax`) varies block by block in retail and no spelling moves it
// (local / no local / either operand order / `x = mask | x` compile identically): 0x00B70230 3 of 7 blocks, 0x00B761D0 6 of 8,
// 0x00B751F0 6 of 7 come out the other way.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
struct IDirect3DDevice9;

// W4TEXTURE_UV_MODE (Ego_r.pdb / FableWin.pdb ?SetTextureUVModeU@CRenderStateManager@@QAEXJW4TEXTURE_UV_MODE@@@Z); the
// enumerator order follows the D3D table it indexes (descriptive names).
enum TEXTURE_UV_MODE {
    TEXTURE_UV_MODE_WRAP,                                                // D3DTADDRESS_WRAP (1)
    TEXTURE_UV_MODE_MIRROR,                                              // D3DTADDRESS_MIRROR (2)
    TEXTURE_UV_MODE_CLAMP,                                               // D3DTADDRESS_CLAMP (3)
    TEXTURE_UV_MODE_BORDER                                               // D3DTADDRESS_BORDER (4)
};
extern const unsigned long g_textureUVModeToD3D[4];                      // 0x01396DC8 (descriptive; no PDB name)
// W4ALPHA_BLEND_MODE (Ego_r.pdb / FableWin.pdb ?SetSourceAlphaBlendMode@CRenderStateManager@@QAEXW4ALPHA_BLEND_MODE@@@Z and
// ?SetDestAlphaBlendMode@...): the setters map the engine enum through the D3DBLEND table at 0x01396F68 ({1..13} = D3DBLEND_ZERO ..
// D3DBLEND_BOTHINVSRCALPHA in D3D order; chunk 723c: RenderEnvironmentMap 0x00B70A90 reads [0x01396F78] / [0x01396F7C] = SRCALPHA /
// INVSRCALPHA, the landed state blocks 0x009DF060 / 0x00B58DE0 read [0x01396F6C] / [0x01396F68] = ONE / ZERO). Enumerator names are
// descriptive (the PDB keeps only the tag), in the table's D3D order.
enum ALPHA_BLEND_MODE {
    ALPHA_BLEND_MODE_ZERO,                                               // D3DBLEND_ZERO (1)
    ALPHA_BLEND_MODE_ONE,                                                // D3DBLEND_ONE (2)
    ALPHA_BLEND_MODE_SRC_COLOUR,                                         // D3DBLEND_SRCCOLOR (3)
    ALPHA_BLEND_MODE_INV_SRC_COLOUR,                                     // D3DBLEND_INVSRCCOLOR (4)
    ALPHA_BLEND_MODE_SRC_ALPHA,                                          // D3DBLEND_SRCALPHA (5)
    ALPHA_BLEND_MODE_INV_SRC_ALPHA,                                      // D3DBLEND_INVSRCALPHA (6)
    ALPHA_BLEND_MODE_DEST_ALPHA,                                         // D3DBLEND_DESTALPHA (7)
    ALPHA_BLEND_MODE_INV_DEST_ALPHA,                                     // D3DBLEND_INVDESTALPHA (8)
    ALPHA_BLEND_MODE_DEST_COLOUR,                                        // D3DBLEND_DESTCOLOR (9)
    ALPHA_BLEND_MODE_INV_DEST_COLOUR,                                    // D3DBLEND_INVDESTCOLOR (10)
    ALPHA_BLEND_MODE_SRC_ALPHA_SAT,                                      // D3DBLEND_SRCALPHASAT (11)
    ALPHA_BLEND_MODE_BOTH_SRC_ALPHA,                                     // D3DBLEND_BOTHSRCALPHA (12)
    ALPHA_BLEND_MODE_BOTH_INV_SRC_ALPHA                                  // D3DBLEND_BOTHINVSRCALPHA (13)
};
extern const unsigned long g_alphaBlendModeToD3D[13];                    // 0x01396F68 (descriptive; no PDB name)

class CRenderStateManager : public CBaseClassNonCopyable {
public:
    struct CStateInfo {                                                  // 0x14 bytes (Ego_r.pdb)
        unsigned long CurrentState;                                      // +0x00
        unsigned long DesiredState;                                      // +0x04
        unsigned long BookmarkMask;                                      // +0x08
        unsigned long D3DState;                                          // +0x0c CStateInfo_u_12 (the D3D render/stage/sampler state id)
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

    // Public state setters (Ego_r.pdb / FableWin.pdb spellings and parameter types).
    __forceinline void SetZBufferWriteEnable(bool enable) { SetRenderState(enable, DeviceState.ZBufferWriteEnable); }
    __forceinline void SetZBufferEnable(bool enable) { SetRenderState(enable, DeviceState.ZBufferEnable); }
    __forceinline void SetAlphaBlendEnable(bool enable) { SetRenderState(enable, DeviceState.AlphaBlendEnable); }
    __forceinline void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(g_alphaBlendModeToD3D[mode], DeviceState.SourceAlphaBlendMode); }
    __forceinline void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode) { SetRenderState(g_alphaBlendModeToD3D[mode], DeviceState.DestAlphaBlendMode); }
    __forceinline void SetAlphaTestEnable(bool enable) { SetRenderState(enable, DeviceState.AlphaTestEnable); }
    __forceinline void SetAlphaRef(unsigned long ref) { SetRenderState(ref, DeviceState.AlphaRef); }
    __forceinline void SetFogEnable(bool enable) { SetRenderState(enable, DeviceState.FogEnable); }
    __forceinline void SetSpecularEnable(bool enable) { SetRenderState(enable, DeviceState.SpecularEnable); }
    __forceinline void SetTextureUVModeU(long stage, TEXTURE_UV_MODE mode) { SetRenderState(g_textureUVModeToD3D[mode], DeviceState.TextureUVModeU[stage]); }
    __forceinline void SetTextureUVModeV(long stage, TEXTURE_UV_MODE mode) { SetRenderState(g_textureUVModeToD3D[mode], DeviceState.TextureUVModeV[stage]); }
    __forceinline void SetTextureUVModeW(long stage, TEXTURE_UV_MODE mode) { SetRenderState(g_textureUVModeToD3D[mode], DeviceState.TextureUVModeW[stage]); }
    // ?SetTextureMipmapLODBias@CRenderStateManager@@QAEXJM@Z: the float's bits are the D3D value (0x00B705DE: the bias is copied
    // to a stack slot and reloaded as a dword).
    __forceinline void SetTextureMipmapLODBias(long stage, float bias) { SetRenderState(*(unsigned long*)&bias, DeviceState.TextureMipmapLODBias[stage]); }
    __forceinline void SetTextureCoordIndex(long stage, long index) { SetRenderState(index, DeviceState.TextureCoordIndex[stage]); }

private:
    // ?SetRenderState@CRenderStateManager@@AAEXKAAUCStateInfo@1@@Z: skip an unchanged value; otherwise bookmark the old value once per
    // capture block (the state's mask bit), store the new one and queue the state for RealiseRenderState.
    __forceinline void SetRenderState(unsigned long value, CStateInfo& state)
    {
        if (state.DesiredState != value) {
            unsigned long mask = CurrentBookmarkMask;
            if ((state.BookmarkMask & mask) == 0) {
                state.BookmarkMask |= mask;
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

public:
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
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager::CStateInfo) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager::CDeviceState) == 0x120c);
FABLE_STATIC_ASSERT(sizeof(CRenderStateManager) == 0x3a3c);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateStack) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateUpdateList) == 0x2008);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, StateStackSize) == 0x2808);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, CurrentBookmarkMask) == 0x2814);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, DeviceState) == 0x2818);
FABLE_STATIC_ASSERT(offsetof(CRenderStateManager, BookmarkDummyState) == 0x3a24);

// ?StateManager@CRenderStateCaptureBlock@@0PAVCRenderStateManager@@A (0x013CB508, the binding of the landed 0x00BC9800) and the
// RAII capture block of Ego_r.pdb (size 1: bool Open; ctor / dtor / Capture / Restore / static SetStateManager), inline everywhere.
class CRenderStateCaptureBlock {
    bool Open;                                                           // +0x00
    static CRenderStateManager* StateManager;                            // 0x013CB508
public:
    __forceinline CRenderStateCaptureBlock() : Open(false) { Capture(); }             // ??0CRenderStateCaptureBlock@@QAE@XZ
    __forceinline ~CRenderStateCaptureBlock() { Restore(); }                           // ??1CRenderStateCaptureBlock@@QAE@XZ
    __forceinline void Capture() { if (!Open) { StateManager->StartCaptureBlock(this); Open = true; } }   // ?Capture@CRenderStateCaptureBlock@@QAEXXZ
    __forceinline void Restore() { if (Open) { StateManager->RestoreCaptureBlock(); Open = false; } }    // ?Restore@CRenderStateCaptureBlock@@QAEXXZ
    static void __fastcall SetStateManager(CRenderStateManager* manager);   // ?SetStateManager@CRenderStateCaptureBlock@@SIXPAVCRenderStateManager@@@Z
};
#else // flattened Ego_r layout (the landed default, unchanged)
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
