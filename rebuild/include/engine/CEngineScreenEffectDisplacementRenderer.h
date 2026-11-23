#ifndef FABLE_ENGINE_CENGINESCREENEFFECTDISPLACEMENTRENDERER_H
#define FABLE_ENGINE_CENGINESCREENEFFECTDISPLACEMENTRENDERER_H
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineScreenEffectDisplacementRenderer@@ (vtable 0x012A3A94) : CEngineComponent (+0x0); sizeof 0x1AC from
// CEngine::Initialise 0x00B26360 (`new` of 0x1AC bytes before the constructor call). Constructor ??0CEngineScreenEffectDisplacementRenderer@@QAE@ABVCEngineInit@@@Z 0x00B86C00 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineScreenEffectDisplacementRenderer@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
class CEngineInit;
class CMatrix3x4;
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
// Retail opt-in (row 0x00B87870 ApplyDisplacement): the own members in Ego_r.pdb order and names (struct_layouts_egor.tsv; no STLport
// member, so the Ego_r offsets are retail's: 0x00B87870 reads BackBufferCopyTexture +0xC, DisplacementTexture +0x10, DisplaceAmount +0xE8,
// TextureToBackBufferVerts +0x80, applies DisplacementVertexShader +0x134 / DisplacementPixelShader +0x13C). CRenderTarget has no header:
// OldRenderTarget stays opaque. Method: ?ApplyDisplacement@CEngineScreenEffectDisplacementRenderer@@AAEXXZ (Ego_r.pdb / FableWin.pdb, private).
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CTVertexColTex1.h"
class CTexture;
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_FINISHSTENCIL
#include "engine/CRenderStateCaptureBlock.h"
#endif
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_OLDRENDERTARGET
#include "engine/CRenderTarget.h"
#endif
class CEngineScreenEffectDisplacementRenderer : public CEngineComponent {
public:
    CEngineScreenEffectDisplacementRenderer(const CEngineInit&); // 0x00B86C00 ??0CEngineScreenEffectDisplacementRenderer@@QAE@ABVCEngineInit@@@Z
    virtual unsigned long PeekSceneFilterFlags() const;
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_SETEFFECTQUALITY
    // NF57 opt-in (row 0x00B86B00): Ego_r.pdb 0x00A2D680 / FableWin.pdb ?SetEffectQuality@CEngineScreenEffectDisplacementRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z;
    // retail vtable 0x012A3A94 slot 13, the first slot after CEngineComponent's thirteen. No layout change.
    virtual bool SetEffectQuality(const class CEngineVideoOptionsDef& options);
#endif
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_FINISHSTENCIL
    // NF56 opt-in (row 0x00B86BB0): Ego_r.pdb 0x00A2D730 / FableWin.pdb 0x02DBA380
    // ?FinishDisplacementStencilRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ (public), out of line at 0x00B86BB0; its body
    // restores StencilRenderingCaptureBlock (+0x144), which this opt-in types as the CRenderStateCaptureBlock it is (Ego_r.pdb; one bool,
    // engine/CRenderStateCaptureBlock.h) in place of the opaque 2 bytes (same offset; the next member stays at +0x148).
    void FinishDisplacementStencilRendering();                          // 0x00B86BB0
#endif
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_GETENABLE2DDISPLACEMENT
    // Nested opt-in (wfE5, rows 0x00BB0970 / 0x00BAFA00): FableWin.pdb ?GetEnable2DDisplacement@CEngineScreenEffectDisplacementRenderer@@QBE_NXZ
    // (02e96c40, listed right after RenderSpritePrim), inline in retail (`mov cl,[GPScreenEffectDisplacementRenderer + 0xEE]`, 0x00BB1473).
    bool GetEnable2DDisplacement() const { return Enable2DDisplacement; }
#endif
private:
    void ApplyDisplacement();                                // 0x00B87870 ?ApplyDisplacement@CEngineScreenEffectDisplacementRenderer@@AAEXXZ
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_INITIALISETEXTURES
    // NF57 opt-in (q10423, row 0x00B86A70): Ego_r.pdb 0x00A2D5F0 / FableWin.pdb 0x02DBA070
    // ?InitialiseTextures@CEngineScreenEffectDisplacementRenderer@@AAE_NXZ (private; retail order InitialiseTextures, SetEffectQuality = PDB order).
    bool InitialiseTextures();                               // 0x00B86A70
#endif
    CTexture*       BackBufferCopyTexture;                   // +0x0c
    CTexture*       DisplacementTexture;                     // +0x10
    long            DisplacementTextureHandle;               // +0x14
    long            DynamicTexturePageHandle;                // +0x18
    bool            HasContent;                              // +0x1c
    CTVertexColTex1 BackBufferToTextureVerts[4];             // +0x20
    CTVertexColTex1 TextureToBackBufferVerts[4];             // +0x80
    long            ClearX;                                  // +0xe0
    long            ClearY;                                  // +0xe4
    float           DisplaceAmount;                          // +0xe8
    bool            DisplacementUsesShadowZBuffer;           // +0xec
    bool            EffectSupported;                         // +0xed
    bool            Enable2DDisplacement;                    // +0xee
    bool            Enable3DDisplacement;                    // +0xef
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_OLDRENDERTARGET
    // NF65 overlay opt-in (row 0x00B86C00, the constructor builds it: CBaseClass + two CSurface): Ego_r.pdb OldRenderTarget, typed on
    // engine/CRenderTarget.h (0x24 bytes). Same size and offsets.
    CRenderTarget   OldRenderTarget;                         // +0xf0
#else
    unsigned char   OldRenderTarget[0x24];                   // +0xf0 CRenderTarget (no header)
#endif
    long            OldViewport_TLX;                         // +0x114 C2DBoxI OldViewport
    long            OldViewport_TLY;                         // +0x118
    long            OldViewport_BRX;                         // +0x11c
    long            OldViewport_BRY;                         // +0x120
    CVertexShader   ScreenCopyVertexShader;                  // +0x124
    CPixelShader    ScreenCopyPixelShader;                   // +0x12c
    CVertexShader   DisplacementVertexShader;                // +0x134
    CPixelShader    DisplacementPixelShader;                 // +0x13c
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_FINISHSTENCIL
    CRenderStateCaptureBlock StencilRenderingCaptureBlock;   // +0x144
#else
    unsigned char   StencilRenderingCaptureBlock[2];         // +0x144 CRenderStateCaptureBlock
#endif
    CTVertexColTex1 DisplacementToBackBufferVerts[4];        // +0x148
    bool            EnableDisplacementTextureView;           // +0x1a8
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_RTTI_DECLS
    // Opt-in (NF319, 2026-11-23): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base CEngineComponent chain declares at the same slot (no new slot).
    // 0x00B86970 vtable 0x012A3A94 slot 11: ?OnReleaseDefaultPoolResources@CEngineScreenEffectDisplacementRenderer@@UAEXXZ (Ego_r.pdb + FableWin.pdb)
public:
    virtual void OnReleaseDefaultPoolResources();                     // 0x00B86970
private:
#endif
};
#else
class CEngineScreenEffectDisplacementRenderer : public CEngineComponent {
public:
    CEngineScreenEffectDisplacementRenderer(const CEngineInit&); // 0x00B86C00 ??0CEngineScreenEffectDisplacementRenderer@@QAE@ABVCEngineInit@@@Z
    virtual unsigned long PeekSceneFilterFlags() const;
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_SHADOWHW
    // Opt-in (chunk 2059, row 0x00BC25F0): Ego_r.pdb ?SetupShadowHWAsDepthBuffer@CEngineScreenEffectDisplacementRenderer@@QAEXPBVCMatrix3x4@@@Z
    // (donor 0x00A2D550 between FinishDisplacementRendering 0x00A2D4F0 and CaptureBackBuffer 0x00A2D560 = retail 0x00B869A0 / 0x00B86A00 /
    // 0x00B86A10): the PC body is empty (`ret 4`); its one caller 0x00BC2695 passes the mesh group's WorldTransform.
    void SetupShadowHWAsDepthBuffer(const CMatrix3x4* transform);        // 0x00B86A00
#endif
private:
    unsigned char _pad_0xc[0x1a0];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
#endif
FABLE_STATIC_ASSERT_AT(75, sizeof(CEngineScreenEffectDisplacementRenderer) == 0x1ac);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineScreenEffectDisplacementRenderer = 0x1ac.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_STENCILRENDERINGCAPTUREBLOCK
#include "engine/CRenderStateCaptureBlock.h"
#endif

class CTexture;

#pragma pack(push, 1)
class CEngineScreenEffectDisplacementRenderer {
public:
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_DISPLACEMENTPASSES
    // NF45 opt-in (row 0x00B33010 CEnginePrimitiveRenderer::Render): the four public pass brackets, Ego_r.pdb names
    // ?BeginDisplacementRendering@CEngineScreenEffectDisplacementRenderer@@QAE_NXZ, ?FinishDisplacementRendering@..@@QAEXXZ,
    // ?BeginDisplacementStencilRendering@..@@QAEXXZ, ?FinishDisplacementStencilRendering@..@@QAEXXZ. The first three are the landed
    // self rows 0x00B87680 / 0x00B869A0 / 0x00B871B0 (catalog names); the fourth is called right after the stencil list as the pair of
    // BeginDisplacementStencilRendering and sits after SetEffectQuality in the Ego_r donor order (0x00A2D730), retail 0x00B86BB0
    // (catalog label "AddToInterface" is a propagated label). Declarations only.
    bool BeginDisplacementRendering();                                   // 0x00B87680
    void FinishDisplacementRendering();                                  // 0x00B869A0
    void BeginDisplacementStencilRendering();                            // 0x00B871B0
    void FinishDisplacementStencilRendering();                           // 0x00B86BB0
#endif
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    CTexture*     BackBufferCopyTexture;                     // +0x0c
    CTexture*     DisplacementTexture;                       // +0x10
    long          DisplacementTextureHandle;                 // +0x14
    long          DynamicTexturePageHandle;                  // +0x18
    bool          HasContent;                                // +0x1c
    unsigned char _pad_0x1d[0x1];                            // +0x1d
    unsigned char _pad_0x1e[0x2];                            // +0x1e
    unsigned char BackBufferToTextureVerts[0x60];            // +0x20 CTVertexColTex1[4]
    unsigned char TextureToBackBufferVerts[0x60];            // +0x80 CTVertexColTex1[4]
    long          ClearX;                                    // +0xe0
    long          ClearY;                                    // +0xe4
    float         DisplaceAmount;                            // +0xe8
    bool          DisplacementUsesShadowZBuffer;             // +0xec
    bool          EffectSupported;                           // +0xed
    bool          Enable2DDisplacement;                      // +0xee
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_GETENABLE3D
    // Opt-in (row 0x00BBF4A0): FableWin.pdb ?GetEnable3DDisplacement@CEngineScreenEffectDisplacementRenderer@@QBE_NXZ (20b), inline in retail
    // (0x00BBF5F2 `mov cl,[edx+0EFh]`).
    bool GetEnable3DDisplacement() const { return Enable3DDisplacement; }
#endif
    bool          Enable3DDisplacement;                      // +0xef
    unsigned char OldRenderTarget[0x24];                     // +0xf0 CRenderTarget
    long          OldViewport_TLX;                           // +0x114 C2DBoxI::TLX
    long          OldViewport_TLY;                           // +0x118 C2DBoxI::TLY
    long          OldViewport_BRX;                           // +0x11c C2DBoxI::BRX
    long          OldViewport_BRY;                           // +0x120 C2DBoxI::BRY
    unsigned char ScreenCopyVertexShader[0x8];               // +0x124 CVertexShader
    unsigned char ScreenCopyPixelShader[0x8];                // +0x12c CPixelShader
    unsigned char DisplacementVertexShader[0x8];             // +0x134 CVertexShader
    unsigned char DisplacementPixelShader[0x8];              // +0x13c CPixelShader
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_STENCILRENDERINGCAPTUREBLOCK
    // NF45 opt-in (row 0x00B871B0 BeginDisplacementStencilRendering): the member typed as the Ego_r.pdb class
    // (engine/CRenderStateCaptureBlock.h, Open at +0x144); the second Ego_r byte stays padding.
    CRenderStateCaptureBlock StencilRenderingCaptureBlock;               // +0x144
    unsigned char _pad_0x145[0x1];                                       // +0x145
#else
    unsigned char StencilRenderingCaptureBlock[0x2];         // +0x144 CRenderStateCaptureBlock
#endif
    unsigned char _pad_0x146[0x2];                           // +0x146
    unsigned char DisplacementToBackBufferVerts[0x60];       // +0x148 CTVertexColTex1[4]
    bool          EnableDisplacementTextureView;             // +0x1a8
    unsigned char _pad_0x1a9[0x3];                           // +0x1a9
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(131, sizeof(CEngineScreenEffectDisplacementRenderer) == 0x1ac);
FABLE_STATIC_ASSERT_AT(132, offsetof(CEngineScreenEffectDisplacementRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(133, offsetof(CEngineScreenEffectDisplacementRenderer, BackBufferCopyTexture) == 0xc);
FABLE_STATIC_ASSERT_AT(134, offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementTexture) == 0x10);
FABLE_STATIC_ASSERT_AT(135, offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementTextureHandle) == 0x14);
FABLE_STATIC_ASSERT_AT(136, offsetof(CEngineScreenEffectDisplacementRenderer, DynamicTexturePageHandle) == 0x18);
FABLE_STATIC_ASSERT_AT(137, offsetof(CEngineScreenEffectDisplacementRenderer, HasContent) == 0x1c);
FABLE_STATIC_ASSERT_AT(138, offsetof(CEngineScreenEffectDisplacementRenderer, BackBufferToTextureVerts) == 0x20);
FABLE_STATIC_ASSERT_AT(139, offsetof(CEngineScreenEffectDisplacementRenderer, TextureToBackBufferVerts) == 0x80);
FABLE_STATIC_ASSERT_AT(140, offsetof(CEngineScreenEffectDisplacementRenderer, ClearX) == 0xe0);
FABLE_STATIC_ASSERT_AT(141, offsetof(CEngineScreenEffectDisplacementRenderer, ClearY) == 0xe4);
FABLE_STATIC_ASSERT_AT(142, offsetof(CEngineScreenEffectDisplacementRenderer, DisplaceAmount) == 0xe8);
FABLE_STATIC_ASSERT_AT(143, offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementUsesShadowZBuffer) == 0xec);
FABLE_STATIC_ASSERT_AT(144, offsetof(CEngineScreenEffectDisplacementRenderer, EffectSupported) == 0xed);
FABLE_STATIC_ASSERT_AT(145, offsetof(CEngineScreenEffectDisplacementRenderer, Enable2DDisplacement) == 0xee);
FABLE_STATIC_ASSERT_AT(146, offsetof(CEngineScreenEffectDisplacementRenderer, Enable3DDisplacement) == 0xef);
FABLE_STATIC_ASSERT_AT(147, offsetof(CEngineScreenEffectDisplacementRenderer, OldRenderTarget) == 0xf0);
FABLE_STATIC_ASSERT_AT(148, offsetof(CEngineScreenEffectDisplacementRenderer, OldViewport_TLX) == 0x114);
FABLE_STATIC_ASSERT_AT(149, offsetof(CEngineScreenEffectDisplacementRenderer, OldViewport_TLY) == 0x118);
FABLE_STATIC_ASSERT_AT(150, offsetof(CEngineScreenEffectDisplacementRenderer, OldViewport_BRX) == 0x11c);
FABLE_STATIC_ASSERT_AT(151, offsetof(CEngineScreenEffectDisplacementRenderer, OldViewport_BRY) == 0x120);
FABLE_STATIC_ASSERT_AT(152, offsetof(CEngineScreenEffectDisplacementRenderer, ScreenCopyVertexShader) == 0x124);
FABLE_STATIC_ASSERT_AT(153, offsetof(CEngineScreenEffectDisplacementRenderer, ScreenCopyPixelShader) == 0x12c);
FABLE_STATIC_ASSERT_AT(154, offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementVertexShader) == 0x134);
FABLE_STATIC_ASSERT_AT(155, offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementPixelShader) == 0x13c);
FABLE_STATIC_ASSERT_AT(156, offsetof(CEngineScreenEffectDisplacementRenderer, StencilRenderingCaptureBlock) == 0x144);
FABLE_STATIC_ASSERT_AT(157, offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementToBackBufferVerts) == 0x148);
FABLE_STATIC_ASSERT_AT(158, offsetof(CEngineScreenEffectDisplacementRenderer, EnableDisplacementTextureView) == 0x1a8);

#endif // FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#endif // FABLE_ENGINE_CENGINESCREENEFFECTDISPLACEMENTRENDERER_H
