#ifndef FABLE_ENGINE_CENGINESHADOWRENDERER_H
#define FABLE_ENGINE_CENGINESHADOWRENDERER_H
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineShadowRenderer@@ (vtable 0x012A2374) : CEngineComponent (+0x0); sizeof 0x340 from
// CEngine::Initialise 0x00B26360 (`new` of 0x340 bytes before the constructor call). Constructor ??0CEngineShadowRenderer@@QAE@XZ 0x00B50370 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineShadowRenderer@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
class CEngineInit;
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
class CEngineCamera;
struct CEngineLightingManagerSpotlightParams;
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
// Retail opt-in (chunk 2024, row 0x00B4FC40 SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&)):
// the own members in Ego_r.pdb order, names and types (struct_layouts_egor.tsv; no container member and C3DVector is 1-aligned, so
// the Ego_r offsets are retail's: the row reads SceneData.WorldToShadowBufferMatrixTextureSpace +0x8C..+0xC8, ShadowFadeStart /
// ShadowFadeEnd +0xFC / +0x100, ShadowBufferTexture +0x2E8, DepthPackingLookupTexture +0x32C and RenderingTechnique +0x334). The nested
// CColorBufferDepthPackParameters (Ego_r.pdb, 12 bytes, U-keyed: ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABUCColorBufferDepthPackParameters@1@@Z)
// and EShadowRenderingTechnique (nested: ?PeekRenderingTechnique@CEngineShadowRenderer@@QBE?AW4EShadowRenderingTechnique@1@XZ; the
// enumerator names are descriptive, the PDB keeps only the tag; 0 / 1 / 2 select the ice pass's plain / shadow-buffer / colour-buffer
// shaders at 0x00B75B04, hand-back 2026-11-03_linux_hand727). Inline accessors: FableWin.pdb only (no Ego_r.pdb or Fable.exe copy).
#ifndef FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE
#endif
#ifndef FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE
#endif
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
class CEngineShadowRenderer : public CEngineComponent {
public:
    struct CColorBufferDepthPackParameters {                             // 12 bytes (Ego_r.pdb)
        CColorBufferDepthPackParameters(long stage, long carry_constant, long repeat_constant)   // FableWin ??0CColorBufferDepthPackParameters@CEngineShadowRenderer@@QAE@JJJ@Z
            : DepthToColourLookupTextureStage(stage), LowByteCarryPixelShaderConstant(carry_constant),
              LowRepeatFactorUserVertexShaderConstant(repeat_constant) { }
        long DepthToColourLookupTextureStage;                            // +0x00
        long LowByteCarryPixelShaderConstant;                            // +0x04
        long LowRepeatFactorUserVertexShaderConstant;                    // +0x08
    };
    enum EShadowRenderingTechnique {
#ifdef FABLE_RETAIL_SHADOW_BUF_ENUM_PASS27
        SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS,
        SHADOW_RENDERING_TECHNIQUE_DEPTH_BUFFER,
#else
        SHADOW_RENDERING_TECHNIQUE_NONE,                                 // descriptive (0: unshadowed shaders)
        SHADOW_RENDERING_TECHNIQUE_SHADOW_BUFFER,                        // descriptive (1: the "Shadowed" shaders)
#endif
        SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER                         // descriptive (2: the "ColbuffShadowed" shaders)
    };
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
    // Both PDB method types and native00B4F390 call arguments; declaration only.
    void UpdateShadowScene(const CEngineCamera&,bool,const CEngineLightingManagerSpotlightParams&);
#endif
    CEngineShadowRenderer();                   // 0x00B50370 ??0CEngineShadowRenderer@@QAE@XZ
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_ONRELEASEDEFAULTPOOLRESOURCES
    // NF152 opt-in (q5011, row 0x00B4FC10): the CEngineComponent override, Ego_r.pdb 0x009EBC80 / FableWin.pdb 0x02D78040
    // ?OnReleaseDefaultPoolResources@CEngineShadowRenderer@@UAEXXZ (retail 0x00B4B830; called directly, so callers qualify it). The slot is
    // the base's: declaring the override changes no layout. Declaration only.
    virtual void OnReleaseDefaultPoolResources();                        // 0x00B4B830
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_CHANGEVIDEOOPTIONSHADOWQUALITY
    // NF152 opt-in (q5011, row 0x00B4FC10): Ego_r.pdb 0x009EED40 / FableWin.pdb 0x02D73600
    // ?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z (public; the target of CEngine 0x00B24500). Declaration only.
    bool ChangeVideoOptionShadowQuality(long shadow_buffer_size, float shadow_distance_scale);   // 0x00B4FC10
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXDEALLOCATESHADOWBUFFER
    // NF152 opt-in (q4597, row 0x00B4B890): Ego_r.pdb 0x009EBCD0 / FableWin.pdb 0x02D78060 ?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ
    // (public; tail-jump target of CEngine::DeallocateShadowBuffer 0x00B23F70). Declaration only.
    bool XBoxDeallocateShadowBuffer();                                   // 0x00B4B890
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXREALLOCATESHADOWBUFFER
    // NF152 opt-in (q4598, row 0x00B4B8A0): Ego_r.pdb 0x009EBCE0 / FableWin.pdb 0x02D78070 ?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ
    // (public; tail-jump target of CEngine::ReallocateShadowBuffer 0x00B23F80). Declaration only.
    void XBoxReallocateShadowBuffer();                                   // 0x00B4B8A0
#endif
    virtual unsigned long PeekSceneFilterFlags() const;
    void SetupShadowFuncRenderStates(long stage, const CMatrix3x4& texture_transform,
                                     const CColorBufferDepthPackParameters& pack_parameters);   // 0x00B4FC40
    void SetupShadowFuncRenderStates(long stage, const CColorBufferDepthPackParameters& pack_parameters);   // 0x00B50510
    EShadowRenderingTechnique PeekRenderingTechnique() const { return RenderingTechnique; }
    CTexture& GetShadowBufferTexture() { return ShadowBufferTexture; }
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_BUILDDEPTHPACKING
    // NF63 (round 7, row 0x00B4F420): Ego_r.pdb 0x009EDE70 / FableWin.pdb 0x02D77E40 ?BuildDepthPackingLookupTexture@CEngineShadowRenderer@@AAEXXZ
    // (private). Declaration only.
private:
    void BuildDepthPackingLookupTexture();                               // 0x00B4F420
public:
#endif
    CTexture& GetDepthPackingTexture() { return DepthPackingLookupTexture; }
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKMINWCLAMP
    // Retail opt-in (wfA6b, row 0x00BA6240): FableWin.pdb ?PeekColourBufferShadowMinWClamp@CEngineShadowRenderer@@QBEMXZ (0x02E822A0,
    // 9i/20b = the member read; no retail out-of-line copy). Read at 0x00BA6909 (+0x25C).
    float PeekColourBufferShadowMinWClamp() const { return ColourBufferShadowMinWClamp; }
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
    // Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): Ego_r.pdb / FableWin.pdb
    // ?PeekSceneData@CEngineShadowRenderer@@QAE?BVCShadowRendererSceneData@@XZ (inline; the caller copies SceneData +0x0C through the
    // out-of-line copy constructor 0x00B38600 into its own frame, 0x00B9A71D..0x00B9A72B).
    const CShadowRendererSceneData PeekSceneData() { return SceneData; }
#endif
private:
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_INITIALISETEXTURES
    // Retail opt-in (Codex pass 24): both PDB names and call at 0x00B504FB.
    bool InitialiseTextures();
#ifdef FABLE_RETAIL_SHADOW_BUFFERS_PASS27
    void UninitialiseTextures();
#endif
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESPACE
    // Opt-in (NF53, round 7, row 0x00B4F390 UpdateShadowScene; needs _UPDATESCENE for the forward declarations): Ego_r.pdb private members
    // ?SetShadowClipVolumesToNeverShadow@CEngineShadowRenderer@@AAEXXZ (0x00B4CE20), ?UpdateSpotLightShadowSpace@CEngineShadowRenderer@@AAEXABUCEngineLightingManagerSpotlightParams@@@Z
    // (0x00B4EF80) and ?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z (0x00B4D370; FableWin.pdb 0x02D74FB0). Declarations only.
    void SetShadowClipVolumesToNeverShadow();                            // 0x00B4CE20
    void UpdateSpotLightShadowSpace(const CEngineLightingManagerSpotlightParams& spot_light);   // 0x00B4EF80
    void UpdateShadowSpace(const CEngineCamera& camera);                 // 0x00B4D370
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_CALCLOCALWORLD
    // Opt-in (egod e03, row 0x00B4B8B0): Ego_r.pdb / FableWin.pdb ?CalcLocalWorldToBaseShadowSpace@CEngineShadowRenderer@@AAE?AVCMatrix3x4@@ABVC3DVector@@MM@Z
    // (private); Ego_d parameter names light_vector / shadow_box_width / shadow_box_length. Declaration only.
    CMatrix3x4 CalcLocalWorldToBaseShadowSpace(const C3DVector& light_vector, float shadow_box_width, float shadow_box_length);   // 0x00B4B8B0
#endif
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_CLIPVOLUMES_PASS22
    // Original public records and class descriptors in both PDBs; native00b4bcc0/00b4c550.
    void CalcClipVolume(const class CEngineCamera&);
    void CalcSpotLightClipVolume(const struct CEngineLightingManagerSpotlightParams&);
#endif
#ifdef FABLE_RETAIL_SPOTLIGHT_MATRIX_BUILD_PASS30
    // FableWin and Ego_r retain this private member with the original spelling.
    void BuildDerivedShadowMatriciesFromWorldToShadowMatrix();
#endif
    CShadowRendererSceneData  SceneData;                                 // +0x00c
    CRenderStateCaptureBlock  ShadowBufferRenderStateCaptureBlock;       // +0x24c
    float                     ShadowProjectionZOffset;                   // +0x250
    float                     ShadowProjectionZSlopeOffset;              // +0x254
    float                     ColourBufferShadowTextureZBias;            // +0x258
    float                     ColourBufferShadowMinWClamp;               // +0x25c
    bool                      EnableShadowBufferDraw;                    // +0x260
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_PACKED_VECTORS
    // Opt-in (NF53, round 7, row 0x00B4F390): Ego_r.pdb places the two C3DVector members byte-packed after the bool (StaticDataOrigin +0x261,
    // CameraLookVector +0x26D; retail 0x00B4F3B6 `lea esi,[eax+26Dh]`), while CameraViewMatrix keeps its 4-byte alignment (+0x27C). Without it
    // the default packing puts them at +0x264 / +0x270. No size change (0x340).
#pragma pack(push, 1)
#endif
    C3DVector                 StaticDataOrigin;                          // +0x261
    C3DVector                 CameraLookVector;                          // +0x26d
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_PACKED_VECTORS
#pragma pack(pop)
#endif
    CMatrix3x4                CameraViewMatrix;                          // +0x27c
    bool                      PerspectiveShadowsEnabled;                 // +0x2ac
    bool                      SceneOriginLocked;                         // +0x2ad
    bool                      EnableFrustrumDebugDisplay;                // +0x2ae
    bool                      TexturesAllocated;                         // +0x2af
    float                     PerspectiveInterpStart;                    // +0x2b0
    float                     PerspectiveInterpEnd;                      // +0x2b4
    float                     ForegroundBufferSpan;                      // +0x2b8
    float                     MaxShadowDistance;                         // +0x2bc
    float                     ShadowFadeRatio;                           // +0x2c0
    CRenderTarget             RenderTarget;                              // +0x2c4
    CTexture                  ShadowBufferTexture;                       // +0x2e8
    CTexture                  RenderSurface;                             // +0x2f0
    CSurface                  ShadowBufferDepthSurface;                  // +0x2f8
    CSurface                  ShadowBufferColourSurface;                 // +0x308
    CTexture                  ShadowBufferColourTexture;                 // +0x318
    C3DVector                 ShadowBufferTextureUVScaler;               // +0x320
    CTexture                  DepthPackingLookupTexture;                 // +0x32c
    EShadowRenderingTechnique RenderingTechnique;                        // +0x334
    long                      UserVideoOptionShadowBufferSize;           // +0x338
    float                     UserVideoOptionShadowDistanceScale;        // +0x33c
};
#else
class CEngineShadowRenderer : public CEngineComponent {
public:
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
    // Both PDB method types and native00B4F390 call arguments; declaration only.
    void UpdateShadowScene(const CEngineCamera&,bool,const CEngineLightingManagerSpotlightParams&);
#endif
    CEngineShadowRenderer();                   // 0x00B50370 ??0CEngineShadowRenderer@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
private:
    unsigned char _pad_0xc[0x334];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
#endif
FABLE_STATIC_ASSERT_AT(107, sizeof(CEngineShadowRenderer) == 0x340);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineShadowRenderer = 0x340.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWBUFFERRENDERSTATECAPTUREBLOCK
#include "engine/CRenderStateCaptureBlock.h"
#endif

struct IDirect3DTexture9;

#pragma pack(push, 1)
class CEngineShadowRenderer {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0x8];                           // +0x04 flattened base-class subobject(s)
    unsigned char SceneData[0x240];                          // +0x0c CShadowRendererSceneData
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWBUFFERRENDERSTATECAPTUREBLOCK
    // NF45 opt-in (row 0x00B4F510 PreLayerRender): the member typed as the Ego_r.pdb class (engine/CRenderStateCaptureBlock.h,
    // Open at +0x24C; the rest of the Ego_r 4 bytes stays padding), as the retail branch above.
    CRenderStateCaptureBlock ShadowBufferRenderStateCaptureBlock;        // +0x24c
    unsigned char _pad_0x24d[0x3];                                       // +0x24d
#else
    unsigned char ShadowBufferRenderStateCaptureBlock[0x4];  // +0x24c CRenderStateCaptureBlock
#endif
    float         ShadowProjectionZOffset;                   // +0x250
    float         ShadowProjectionZSlopeOffset;              // +0x254
    float         ColourBufferShadowTextureZBias;            // +0x258
    float         ColourBufferShadowMinWClamp;               // +0x25c
    bool          EnableShadowBufferDraw;                    // +0x260
    float         StaticDataOrigin_X;                        // +0x261 C3DVector::X
    float         StaticDataOrigin_Y;                        // +0x265 C3DVector::Y
    float         StaticDataOrigin_Z;                        // +0x269 C3DVector::Z
    unsigned char CameraLookVector[0xf];                     // +0x26d C3DVector
    unsigned char CameraViewMatrix[0x30];                    // +0x27c CMatrix3x4
    bool          PerspectiveShadowsEnabled;                 // +0x2ac
    bool          SceneOriginLocked;                         // +0x2ad
    bool          EnableFrustrumDebugDisplay;                // +0x2ae
    bool          TexturesAllocated;                         // +0x2af
    float         PerspectiveInterpStart;                    // +0x2b0
    float         PerspectiveInterpEnd;                      // +0x2b4
    float         ForegroundBufferSpan;                      // +0x2b8
    float         MaxShadowDistance;                         // +0x2bc
    float         ShadowFadeRatio;                           // +0x2c0
    unsigned char RenderTarget[0x24];                        // +0x2c4 CRenderTarget
    unsigned char ShadowBufferTexture[0x8];                  // +0x2e8 CTexture
    unsigned char RenderSurface[0x8];                        // +0x2f0 CTexture
    unsigned char ShadowBufferDepthSurface[0x10];            // +0x2f8 CSurface
    unsigned char ShadowBufferColourSurface[0x10];           // +0x308 CSurface
    unsigned char ShadowBufferColourTexture[0x8];            // +0x318 CTexture
    float         ShadowBufferTextureUVScaler_X;             // +0x320 C3DVector::X
    float         ShadowBufferTextureUVScaler_Y;             // +0x324 C3DVector::Y
    float         ShadowBufferTextureUVScaler_Z;             // +0x328 C3DVector::Z
    unsigned char DepthPackingLookupTexture[0x8];            // +0x32c CTexture
    unsigned char RenderingTechnique[0x4];                   // +0x334 EShadowRenderingTechnique
    long          UserVideoOptionShadowBufferSize;           // +0x338
    float         UserVideoOptionShadowDistanceScale;        // +0x33c
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
    // Opt-in (bwf1 / agentC2, row 0x00BB3E80): Ego_r.pdb CEngineShadowRenderer::CColorBufferDepthPackParameters (12 bytes) and
    // ?SetupShadowFuncRenderStates@CEngineShadowRenderer@@QAEXJABVCMatrix3x4@@ABUCColorBufferDepthPackParameters@1@@Z (0x00B4FC40, out of line).
    struct CColorBufferDepthPackParameters {
        long DepthToColourLookupTextureStage;
        long LowByteCarryPixelShaderConstant;
        long LowRepeatFactorUserVertexShaderConstant;
#ifdef FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC_CTOR
        // Nested opt-in (agentC16, row 0x00BA2350): FableWin.pdb ??0CColorBufferDepthPackParameters@CEngineShadowRenderer@@QAE@JJJ@Z (inline).
        CColorBufferDepthPackParameters(long stage, long carry, long repeat)
            : DepthToColourLookupTextureStage(stage), LowByteCarryPixelShaderConstant(carry), LowRepeatFactorUserVertexShaderConstant(repeat) { }
#endif
    };
    void SetupShadowFuncRenderStates(long stage, const class CMatrix3x4& transform, const CColorBufferDepthPackParameters& params);   // 0x00B4FC40
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(161, sizeof(CEngineShadowRenderer) == 0x340);
FABLE_STATIC_ASSERT_AT(162, offsetof(CEngineShadowRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(163, offsetof(CEngineShadowRenderer, SceneData) == 0xc);
FABLE_STATIC_ASSERT_AT(164, offsetof(CEngineShadowRenderer, ShadowBufferRenderStateCaptureBlock) == 0x24c);
FABLE_STATIC_ASSERT_AT(165, offsetof(CEngineShadowRenderer, ShadowProjectionZOffset) == 0x250);
FABLE_STATIC_ASSERT_AT(166, offsetof(CEngineShadowRenderer, ShadowProjectionZSlopeOffset) == 0x254);
FABLE_STATIC_ASSERT_AT(167, offsetof(CEngineShadowRenderer, ColourBufferShadowTextureZBias) == 0x258);
FABLE_STATIC_ASSERT_AT(168, offsetof(CEngineShadowRenderer, ColourBufferShadowMinWClamp) == 0x25c);
FABLE_STATIC_ASSERT_AT(169, offsetof(CEngineShadowRenderer, EnableShadowBufferDraw) == 0x260);
FABLE_STATIC_ASSERT_AT(170, offsetof(CEngineShadowRenderer, StaticDataOrigin_X) == 0x261);
FABLE_STATIC_ASSERT_AT(171, offsetof(CEngineShadowRenderer, StaticDataOrigin_Y) == 0x265);
FABLE_STATIC_ASSERT_AT(172, offsetof(CEngineShadowRenderer, StaticDataOrigin_Z) == 0x269);
FABLE_STATIC_ASSERT_AT(173, offsetof(CEngineShadowRenderer, CameraLookVector) == 0x26d);
FABLE_STATIC_ASSERT_AT(174, offsetof(CEngineShadowRenderer, CameraViewMatrix) == 0x27c);
FABLE_STATIC_ASSERT_AT(175, offsetof(CEngineShadowRenderer, PerspectiveShadowsEnabled) == 0x2ac);
FABLE_STATIC_ASSERT_AT(176, offsetof(CEngineShadowRenderer, SceneOriginLocked) == 0x2ad);
FABLE_STATIC_ASSERT_AT(177, offsetof(CEngineShadowRenderer, EnableFrustrumDebugDisplay) == 0x2ae);
FABLE_STATIC_ASSERT_AT(178, offsetof(CEngineShadowRenderer, TexturesAllocated) == 0x2af);
FABLE_STATIC_ASSERT_AT(179, offsetof(CEngineShadowRenderer, PerspectiveInterpStart) == 0x2b0);
FABLE_STATIC_ASSERT_AT(180, offsetof(CEngineShadowRenderer, PerspectiveInterpEnd) == 0x2b4);
FABLE_STATIC_ASSERT_AT(181, offsetof(CEngineShadowRenderer, ForegroundBufferSpan) == 0x2b8);
FABLE_STATIC_ASSERT_AT(182, offsetof(CEngineShadowRenderer, MaxShadowDistance) == 0x2bc);
FABLE_STATIC_ASSERT_AT(183, offsetof(CEngineShadowRenderer, ShadowFadeRatio) == 0x2c0);
FABLE_STATIC_ASSERT_AT(184, offsetof(CEngineShadowRenderer, RenderTarget) == 0x2c4);
FABLE_STATIC_ASSERT_AT(185, offsetof(CEngineShadowRenderer, ShadowBufferTexture) == 0x2e8);
FABLE_STATIC_ASSERT_AT(186, offsetof(CEngineShadowRenderer, RenderSurface) == 0x2f0);
FABLE_STATIC_ASSERT_AT(187, offsetof(CEngineShadowRenderer, ShadowBufferDepthSurface) == 0x2f8);
FABLE_STATIC_ASSERT_AT(188, offsetof(CEngineShadowRenderer, ShadowBufferColourSurface) == 0x308);
FABLE_STATIC_ASSERT_AT(189, offsetof(CEngineShadowRenderer, ShadowBufferColourTexture) == 0x318);
FABLE_STATIC_ASSERT_AT(190, offsetof(CEngineShadowRenderer, ShadowBufferTextureUVScaler_X) == 0x320);
FABLE_STATIC_ASSERT_AT(191, offsetof(CEngineShadowRenderer, ShadowBufferTextureUVScaler_Y) == 0x324);
FABLE_STATIC_ASSERT_AT(192, offsetof(CEngineShadowRenderer, ShadowBufferTextureUVScaler_Z) == 0x328);
FABLE_STATIC_ASSERT_AT(193, offsetof(CEngineShadowRenderer, DepthPackingLookupTexture) == 0x32c);
FABLE_STATIC_ASSERT_AT(194, offsetof(CEngineShadowRenderer, RenderingTechnique) == 0x334);
FABLE_STATIC_ASSERT_AT(195, offsetof(CEngineShadowRenderer, UserVideoOptionShadowBufferSize) == 0x338);
FABLE_STATIC_ASSERT_AT(196, offsetof(CEngineShadowRenderer, UserVideoOptionShadowDistanceScale) == 0x33c);

#endif // FABLE_RETAIL_CENGINESHADOWRENDERER
#endif // FABLE_ENGINE_CENGINESHADOWRENDERER_H
