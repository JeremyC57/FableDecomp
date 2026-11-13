#ifndef FABLE_ENGINE_CENGINESCREENEFFECTOUTLINEGLOW_H
#define FABLE_ENGINE_CENGINESCREENEFFECTOUTLINEGLOW_H
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineScreenEffectOutlineGlow@@ (vtable 0x012A3B6C) : CEngineComponent (+0x0); sizeof 0x1E4 from
// CEngine::Initialise 0x00B26360 (`new` of 0x1E4 bytes before the constructor call). Constructor ??0CEngineScreenEffectOutlineGlow@@QAE@ABVCEngineInit@@@Z 0x00B891B0 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineScreenEffectOutlineGlow@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
class CEngineInit;
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
// Retail opt-in (chunk 2024, row 0x00B891B0 the constructor): the own members in Ego_r.pdb order, names and types
// (struct_layouts_egor.tsv; no STLport member, so the Ego_r offsets are retail's: the constructor stores the three texture
// pointers +0x0C..+0x14, the three texture handles +0x18..+0x20 and DynamicTexturePageHandle +0x24, the three CTVertexColTex1[4]
// quads from +0x2C, builds OldRenderTarget +0x14C in place (CBaseClass() 0x0099A2D0, vtable 0x0129C800, two CSurface), the six
// shader wrappers +0x180..+0x1A8 (CVertexShader() 0x00A62FB0 / CPixelShader() 0x00A5EC00, then assigned), OutlineGlowScaleFactor
// +0x1BC, OuterStencilExtraNormalExtrusion +0x1B8, the overridden parameters +0x1C0..+0x1D4, BlurStepCount +0x1D8, DepthBufferBias
// +0x1DC, the capture block +0x1E0 and EnableDrawDebugTextures +0x1E1; the landed destructor 0x00B898D0 releases the shaders and
// the render target at the same offsets). Methods: Ego_r.pdb / FableWin.pdb spellings.
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#ifndef FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE
#endif
#ifndef FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#endif
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CTVertexColTex1.h"
#include "engine/CRenderTarget.h"
#include "engine/C2DBoxI.h"
#include "engine/CRGBColour.h"
#include "engine/CRenderStateCaptureBlock.h"
class CTexture;
class CMatrix3x4;
class CEngineComponentRenderParams;
class CEngineScreenEffectOutlineGlow : public CEngineComponent {
public:
    CEngineScreenEffectOutlineGlow(const CEngineInit& init);             // 0x00B891B0 ??0CEngineScreenEffectOutlineGlow@@QAE@ABVCEngineInit@@@Z
    virtual ~CEngineScreenEffectOutlineGlow();                            // 0x00B898D0 ??1CEngineScreenEffectOutlineGlow@@UAE@XZ
    virtual void Render(const CEngineComponentRenderParams& params);      // 0x00B8B550 ?Render@CEngineScreenEffectOutlineGlow@@UAEXABVCEngineComponentRenderParams@@@Z
    virtual CCharString GetConsoleEnableFunctionName();                   // 0x00B8B580
    virtual unsigned long PeekSceneFilterFlags() const;                   // 0x00B8B5A0
    void BeginOutlineRendering();                                         // ?BeginOutlineRendering@CEngineScreenEffectOutlineGlow@@QAEXXZ
    void FinishOutlineRendering();                                        // ?FinishOutlineRendering@CEngineScreenEffectOutlineGlow@@QAEXXZ
    void SetupDepthBufferTransform(const CMatrix3x4& transform);          // ?SetupDepthBufferTransform@CEngineScreenEffectOutlineGlow@@QAEXABVCMatrix3x4@@@Z
    float PeekOuterStencilExtraNormalExtrusion() { return OuterStencilExtraNormalExtrusion; }   // FableWin.pdb (inline)
    float PeekOutlineGlowScaleFactor() { return OutlineGlowScaleFactor; }                       // FableWin.pdb (inline)
private:
    void FinishWithDynamicTextures();                                     // 0x00B89080 ?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ
    void ApplyOutline();                                                  // ?ApplyOutline@CEngineScreenEffectOutlineGlow@@AAEXXZ
    void DrawDebugTextures();                                             // ?DrawDebugTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ
    CTexture*       OutlineTexture;                                       // +0x0c
    CTexture*       Blur1Texture;                                         // +0x10
    CTexture*       Blur2Texture;                                         // +0x14
    long            OutlineTextureHandle;                                 // +0x18
    long            Blur1TextureHandle;                                   // +0x1c
    long            Blur2TextureHandle;                                   // +0x20
    long            DynamicTexturePageHandle;                             // +0x24
    bool            HasContent;                                           // +0x28
    CTVertexColTex1 OutlineToBlurVerts[4];                                // +0x2c
    CTVertexColTex1 BlurToBlurVerts[4];                                   // +0x8c
    CTVertexColTex1 BlurToBackBufferVerts[4];                             // +0xec
    CRenderTarget   OldRenderTarget;                                      // +0x14c
    C2DBoxI         OldViewport;                                          // +0x170
    CVertexShader   OutlineToBlurVertexShader;                            // +0x180
    CPixelShader    OutlineToBlurPixelShader;                             // +0x188
    CVertexShader   BlurToBlurVertexShader;                               // +0x190
    CPixelShader    BlurToBlurPixelShader;                                // +0x198
    CVertexShader   BlurToBackBufferVertexShader;                         // +0x1a0
    CPixelShader    BlurToBackBufferPixelShader;                          // +0x1a8
    float           BlurFactor;                                           // +0x1b0
    float           SharpenFactor;                                        // +0x1b4
    float           OuterStencilExtraNormalExtrusion;                     // +0x1b8
    float           OutlineGlowScaleFactor;                               // +0x1bc
    bool            ParametersOverridden;                                 // +0x1c0
    float           OverriddenParamsWidthWorldSpace;                      // +0x1c4
    float           OverriddenParamsWidthScreenSpace;                     // +0x1c8
    CRGBColour      OverriddenParamsColour;                               // +0x1cc
    long            OverriddenParamsTextureAlphaRef;                      // +0x1d0
    long            OverriddenParamsTextureAlphaBias;                     // +0x1d4
    long            BlurStepCount;                                        // +0x1d8
    float           DepthBufferBias;                                      // +0x1dc
    CRenderStateCaptureBlock OutlineRenderingCaptureBlock;                // +0x1e0
    bool            EnableDrawDebugTextures;                              // +0x1e1
};
#else
class CEngineScreenEffectOutlineGlow : public CEngineComponent {
public:
    CEngineScreenEffectOutlineGlow(const CEngineInit&); // 0x00B891B0 ??0CEngineScreenEffectOutlineGlow@@QAE@ABVCEngineInit@@@Z
    virtual unsigned long PeekSceneFilterFlags() const;
private:
    unsigned char _pad_0xc[0x1d8];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
#endif
FABLE_STATIC_ASSERT(sizeof(CEngineScreenEffectOutlineGlow) == 0x1e4);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineScreenEffectOutlineGlow = 0x1e4.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CTexture;

#pragma pack(push, 1)
class CEngineScreenEffectOutlineGlow {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    CTexture*     OutlineTexture;                            // +0x0c
    CTexture*     Blur1Texture;                              // +0x10
    CTexture*     Blur2Texture;                              // +0x14
    long          OutlineTextureHandle;                      // +0x18
    long          Blur1TextureHandle;                        // +0x1c
    long          Blur2TextureHandle;                        // +0x20
    long          DynamicTexturePageHandle;                  // +0x24
    bool          HasContent;                                // +0x28
    unsigned char _pad_0x29[0x1];                            // +0x29
    unsigned char _pad_0x2a[0x2];                            // +0x2a
    unsigned char OutlineToBlurVerts[0x60];                  // +0x2c CTVertexColTex1[4]
    unsigned char BlurToBlurVerts[0x60];                     // +0x8c CTVertexColTex1[4]
    unsigned char BlurToBackBufferVerts[0x60];               // +0xec CTVertexColTex1[4]
    unsigned char OldRenderTarget[0x24];                     // +0x14c CRenderTarget
    long          OldViewport_TLX;                           // +0x170 C2DBoxI::TLX
    long          OldViewport_TLY;                           // +0x174 C2DBoxI::TLY
    long          OldViewport_BRX;                           // +0x178 C2DBoxI::BRX
    long          OldViewport_BRY;                           // +0x17c C2DBoxI::BRY
    unsigned char OutlineToBlurVertexShader[0x8];            // +0x180 CVertexShader
    unsigned char OutlineToBlurPixelShader[0x8];             // +0x188 CPixelShader
    unsigned char BlurToBlurVertexShader[0x8];               // +0x190 CVertexShader
    unsigned char BlurToBlurPixelShader[0x8];                // +0x198 CPixelShader
    unsigned char BlurToBackBufferVertexShader[0x8];         // +0x1a0 CVertexShader
    unsigned char BlurToBackBufferPixelShader[0x8];          // +0x1a8 CPixelShader
    float         BlurFactor;                                // +0x1b0
    float         SharpenFactor;                             // +0x1b4
    float         OuterStencilExtraNormalExtrusion;          // +0x1b8
    float         OutlineGlowScaleFactor;                    // +0x1bc
    bool          ParametersOverridden;                      // +0x1c0
    unsigned char _pad_0x1c1[0x3];                           // +0x1c1
    float         OverriddenParamsWidthWorldSpace;           // +0x1c4
    float         OverriddenParamsWidthScreenSpace;          // +0x1c8
    unsigned char OverriddenParamsColour[0x4];               // +0x1cc CRGBColour
    long          OverriddenParamsTextureAlphaRef;           // +0x1d0
    long          OverriddenParamsTextureAlphaBias;          // +0x1d4
    long          BlurStepCount;                             // +0x1d8
    float         DepthBufferBias;                           // +0x1dc
    bool          OutlineRenderingCaptureBlock_Open;         // +0x1e0 CRenderStateCaptureBlock::Open
    bool          EnableDrawDebugTextures;                   // +0x1e1
    unsigned char _pad_0x1e2[0x2];                           // +0x1e2
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineScreenEffectOutlineGlow) == 0x1e4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineTexture) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, Blur1Texture) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, Blur2Texture) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineTextureHandle) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, Blur1TextureHandle) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, Blur2TextureHandle) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, DynamicTexturePageHandle) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, HasContent) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineToBlurVerts) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurToBlurVerts) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurToBackBufferVerts) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OldRenderTarget) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OldViewport_TLX) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OldViewport_TLY) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OldViewport_BRX) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OldViewport_BRY) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineToBlurVertexShader) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineToBlurPixelShader) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurToBlurVertexShader) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurToBlurPixelShader) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurToBackBufferVertexShader) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurToBackBufferPixelShader) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurFactor) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, SharpenFactor) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OuterStencilExtraNormalExtrusion) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineGlowScaleFactor) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, ParametersOverridden) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OverriddenParamsWidthWorldSpace) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OverriddenParamsWidthScreenSpace) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OverriddenParamsColour) == 0x1cc);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OverriddenParamsTextureAlphaRef) == 0x1d0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OverriddenParamsTextureAlphaBias) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, BlurStepCount) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, DepthBufferBias) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, OutlineRenderingCaptureBlock_Open) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, EnableDrawDebugTextures) == 0x1e1);

#endif // FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#endif // FABLE_ENGINE_CENGINESCREENEFFECTOUTLINEGLOW_H
