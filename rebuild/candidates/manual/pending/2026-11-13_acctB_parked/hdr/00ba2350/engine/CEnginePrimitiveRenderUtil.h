#ifndef FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
#define FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
// Hand-written (chunk 723): CEnginePrimitiveRenderUtil : CBaseClassNonCopyable : CBaseClass (Fable.exe RTTI), no data members
// (Ego_r.pdb). vtable 0x012A3BD0; ctor inline (out-of-line copy 0x00B8B5E0), virtual dtor slot 0 0x00B8B600. Its many helpers are
// static fastcall (SI) functions in the PDBs and are declared where a row needs them.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
class CVertexShader;
class CPixelShader;
class CEngineCamera;
enum EEnginePrimitiveBlendOp;                                            // MS forward enums
enum EEnginePrimitiveBlendMode;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
class CRGBFloatColour;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
class CEngineMeshEffectOutline;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
class C3DVector;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
class CEngineMeshEffect;
class CMatrix3x4;
enum ERenderPassType;
#endif
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPRENDERMODE
    // Opt-in (bwf2, row 0x00BA2350): Ego_r.pdb / FableWin.pdb outline state setters (non-static members, out of line in retail).
    void SetupOutlineSmoothStencilStates(const class CEngineMeshEffectOutline& effect, float scale);   // 0x00BEFD90
    void SetupOutlineSmoothAlphaStencilMaskStates(const class CEngineMeshEffectOutline& effect);     // 0x00BF01E0
    void SetupOutlineSmoothFillStates(const class CEngineMeshEffectOutline& effect, float scale, bool textured, long stage_count);   // 0x00BF2240
    void SetupOutlineStencilStates(const class CEngineMeshEffectOutline& effect, float scale);       // 0x00BF0740
    void SetupOutlineFillStates(const class CEngineMeshEffectOutline& effect, float scale);          // 0x00BF0B20
    // FableWin.pdb inline helpers (no Ego_r.pdb / retail out-of-line copy; the row defines the bodies, which reach the engine globals).
    void SetupBlankTextures(class C3DTextureStages& texture_stages, bool dp3_bump);   // ?SetupBlankTextures@CEnginePrimitiveRenderUtil@@QAEXAAVC3DTextureStages@@_N@Z
    void DisableTextureFiltering(unsigned long stage);                   // ?DisableTextureFiltering@CEnginePrimitiveRenderUtil@@QAEXK@Z
    void Set2DAlphaMaskTextureMode(unsigned long stage, bool filter, bool clamp);   // ?Set2DAlphaMaskTextureMode@CEnginePrimitiveRenderUtil@@QAEXK_N0@Z
    const class C2DAlphaMaskInfo* GetCurrent2DAlphaMaskInfo();           // ?GetCurrent2DAlphaMaskInfo@CEnginePrimitiveRenderUtil@@QAEPBVC2DAlphaMaskInfo@@XZ
    static void __fastcall ApplyPixelShader(const class CPixelShader& shader);   // ?ApplyPixelShader@CEnginePrimitiveRenderUtil@@SIXABVCPixelShader@@@Z
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_PIXELSHADERGETTERS
    // FableWin.pdb static inline getters ?GetPixelShader<Name>@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@XZ (15/16 debug bytes: the
    // renderer's member / the member pointer's target); the row defines the bodies.
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseOnly();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseColourTextureDiffuseAlpha();
    static const class CPixelShader& __fastcall GetPixelShaderOutlineSmoothFill();
    static const class CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillAlpha();
    static const class CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillDP3();
    static const class CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillDP3Alpha();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuse();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseFog();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseEnvFog();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseFogShadow();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseFogShadowOutputFactor();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseEnvFogShadow();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseDP3BumpFogShadow();
    static const class CPixelShader& __fastcall GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor();
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
 static float __fastcall CalcDistanceFromCamera(const C3DVector&);
#endif
    CEnginePrimitiveRenderUtil() { }                                     // ??0CEnginePrimitiveRenderUtil@@QAE@XZ
    virtual ~CEnginePrimitiveRenderUtil();                               // slot 0  ??1CEnginePrimitiveRenderUtil@@UAE@XZ
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
    // Retail opt-in (chunk 742, the particle manager render passes 0x00BBE090 / 0x00BC25F0 / 0x00BC3F30; chunk 723c's RenderSingleSprite
    // draft declares the same): Ego_r.pdb / FableWin.pdb members with no out-of-line copy in retail. The including row defines the
    // inline bodies (they reach the engine globals): ApplyVertexShader / ApplyPixelShader forward to the shader render manager, GetCurrentCamera returns *g_pEngineCamera,
    // SetupBlendOpState maps the primitive blend op (1, 2, default) to the D3D blend op and SetupAlphaBlendingStatesFromBlendMode the
    // blend mode (3, 4, default) to the source / dest blend factors, true when the colour must be premultiplied by alpha.
    static void __fastcall ApplyVertexShader(const CVertexShader& shader);   // ?ApplyVertexShader@CEnginePrimitiveRenderUtil@@SIXABVCVertexShader@@@Z
    static void __fastcall ApplyPixelShader(const CPixelShader& shader);     // ?ApplyPixelShader@CEnginePrimitiveRenderUtil@@SIXABVCPixelShader@@@Z
    static CEngineCamera& __fastcall GetCurrentCamera();                 // ?GetCurrentCamera@CEnginePrimitiveRenderUtil@@SIAAVCEngineCamera@@XZ
    void SetupBlendOpState(EEnginePrimitiveBlendOp op);                   // ?SetupBlendOpState@CEnginePrimitiveRenderUtil@@QAEXW4EEnginePrimitiveBlendOp@@@Z
    bool SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode);   // ?SetupAlphaBlendingStatesFromBlendMode@CEnginePrimitiveRenderUtil@@QAE_NW4EEnginePrimitiveBlendMode@@@Z
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
    // Opt-in (chunk 851, row 0x00BA1B00 DisplayMirroredTextures): Ego_r.pdb / FableWin.pdb
    // ?SetTextureFactor@CEnginePrimitiveRenderUtil@@SIXABVCRGBFloatColour@@@Z, no out-of-line copy in retail (0x00BA1BDC / 0x00BA1F56
    // expand it: pixel-shader constant 2 when pixel shaders are on, else the TEXTUREFACTOR render state). The row defines the body.
    static void __fastcall SetTextureFactor(const CRGBFloatColour& colour);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
    // Opt-in (chunk 892, row 0x00BF01E0): Ego_r.pdb / FableWin.pdb
    // ?SetupOutlineSmoothAlphaStencilMaskStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@@Z (a non-static member
    // that never reads `this`; retail `ret 4`).
    void SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline& effect);   // 0x00BF01E0
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCILSTATES
    // Nested opt-in (chunk Q25, row 0x00BF0B20): Ego_r.pdb / FableWin.pdb
    // ?SetupOutlineStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z (`this` unused; retail `ret 8`).
    void SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float scale);   // 0x00BF0B20
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
    // Opt-in (chunk 2060, row 0x00BA5260 RenderMeshPrimitive): Ego_r.pdb / FableWin.pdb
    // ?SetupStencilMask@CEnginePrimitiveRenderUtil@@QAEXPBVCEngineMeshEffect@@KW4ERenderPassType@@@Z (out of line, 0x00BF1030, landed),
    // and the FableWin.pdb inline statics ?SetWorldTransform@CEnginePrimitiveRenderUtil@@SIXABVCMatrix3x4@@@Z (21 debug instructions)
    // and ?GetMipmapSizeMultiplier@CEnginePrimitiveRenderUtil@@SIMXZ (6); the including row defines their bodies (they reach the
    // engine globals).
    void SetupStencilMask(const CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass);   // 0x00BF1030
    static void __fastcall SetWorldTransform(const CMatrix3x4& transform);
    static float __fastcall GetMipmapSizeMultiplier();
    // FableWin.pdb ?SetupAlphaBlendingStatesFromRenderFlags@CEnginePrimitiveRenderUtil@@QAEXKW4ERenderPassType@@EE@Z (268 bytes / 71 debug
    // instructions; no Ego_r.pdb or retail out-of-line copy: RenderMeshPrimitive expands it, 0x00BA557F..0x00BA5BB2). Body in the row.
    void SetupAlphaBlendingStatesFromRenderFlags(unsigned long render_flags, ERenderPassType pass, unsigned char alpha, unsigned char alpha_ref);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveRenderUtil) == 0x4);

#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
