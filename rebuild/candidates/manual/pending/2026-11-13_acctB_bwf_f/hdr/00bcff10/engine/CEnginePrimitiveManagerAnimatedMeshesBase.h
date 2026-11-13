#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_H
// NEW header (chunk 4010 scout, 2026-11-12; row 0x00BD3070 RenderMeshPrimitive). No repo header existed (the landed same-class rows
// 0x00BD7110 / 0x00BD7EF0 / 0x00BCEA60 / 0x00BCE900 use local models). RTTI chain (landed ~CEnginePrimitiveManagerAnimatedMeshesBase
// 0x00BD7EF0 tail-jumps ~CEnginePrimitiveManagerOcclusionBase 0x00B38590): AnimatedMeshesBase : MeshBase : OcclusionBase : CEnginePrimitiveManager.
// Members: Ego_r.pdb names / types / offsets (ghidra_out/struct_layouts_egor.tsv, sizeof 0x690); no STLport container in the class, so
// retail = Ego_r: PointLightShaderIndex +0x20 and the selector pointers +0x2e8..+0x308 are read in place by the landed FrameStart
// 0x00BCEA60 and ApplyPalSkinVertexShaderSpotShadow 0x00BCE900; RenderMeshPrimitive reads +0x38/+0x58 (arrays), +0x2a8..+0x2c8 (singles),
// +0x2ec..+0x300 (selectors), +0x30c (Spotlight array), +0x32c (TextureOverlayFadeDistandeFactor, PDB spelling).
// Needs FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (the retail branch of the base headers), like engine/CEnginePrimitiveManagerStaticMeshesBase.h.
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CVertexShader.h"
#include "engine/CPreTransposedBoneMatrix.h"
class C3DPrimitive2;
class CMatrix4x4;
class CEngineSubPrimitiveAnimatedMesh;
class CEngineInternalPrimitiveAnimatedMesh;
class CShaderRenderManager;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }   // 0x01436E14 (reloc_bindings ?GPShaderRenderManager@NEngineInternal@@3PAVCShaderRenderManager@@A)
#endif
class CEnginePrimitiveManagerAnimatedMeshesBase : public CEnginePrimitiveManagerMeshBase {
public:
    // Overrides (Ego_r.pdb / FableWin.pdb UAE spellings; slots from the base). RenderSubPrimitive slot 5 = landed 0x00BD7110.
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);                 // 0x00BD6DF0
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList&) const;                                     // 0x00BD27F0
    virtual void FrameStart();                                                                           // 0x00BCEA60
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType);                          // 0x00BD7110
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&);
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&);
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&, unsigned long);
    virtual ~CEnginePrimitiveManagerAnimatedMeshesBase();                                                // 0x00BD7EF0
    CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>&);
    // ?RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@QAEXAAVCEngineSubPrimitiveAnimatedMesh@@W4ERenderPassType@@@Z
    // (Ego_r.pdb 0x00A87E30 / FableWin.pdb 0x02E7E830; retail 0x00BD3070, `ret 8`; called by RenderSubPrimitive 0x00BD7110).
    void RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh& sub_primitive, ERenderPassType pass);
    // FableWin.pdb ?RenderPrimitiveBlocks@..@@QAEXAAVC3DPrimitive2@@PBVCMatrix4x4@@@Z (13 debug instr; inline, forwards to the IndexBuffer form).
    void RenderPrimitiveBlocks(C3DPrimitive2& primitive, const CMatrix4x4* bone_transforms);
private:
    // ?RenderPrimitiveBlocksIndexBuffer@..@@AAEXAAVC3DPrimitive2@@PBVCMatrix4x4@@@Z (FableWin.pdb; manifest label at retail 0x00BCFB00, `ret 8`).
    void RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* bone_transforms);    // 0x00BCFB00
    void RenderPrimitiveBlocksPushBuffer(C3DPrimitive2& primitive, const CMatrix4x4* bone_transforms);     // FableWin 7 instr (Xbox stub)
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS
    // Opt-in (chunk 4010 scout): the private inline shader selectors of the PDB (FableWin.pdb COMDATs listed right after
    // RenderMeshPrimitive = first instantiated there; FableWin debug sizes in brackets, check with fwsize). Text = the landed
    // ApplyPalSkinVertexShaderSpotShadow 0x00BCE900 (clamp the light count to 5, index PointLightShaderIndex), routed through the
    // private ApplyVertexShader (FableWin ?ApplyVertexShader@CEnginePrimitiveManagerAnimatedMeshesBase@@AAEXABVCVertexShader@@@Z,
    // 12i/30b): GPShaderRenderManager->ApplyVertexShader(shader, true) (every site pushes 1, e.g. 0x00BD3B6A).
    void ApplyVertexShader(const CVertexShader& shader);
    void ApplyPalSkinVertexShader(unsigned long num_point_lights, unsigned long);             // [23i/68b]  +0x38  (0x00BD3B59)
    void ApplyPalSkinVertexShaderEnv(unsigned long num_point_lights);                         // [20i/60b]  +0x58  (0x00BD3D21)
    void ApplyPalSkinVertexShaderShadow(unsigned long num_point_lights, unsigned long);       // [23i/71b]  *+0x2ec (0x00BD3E89)
    void ApplyPalSkinVertexShaderEnvShadow(unsigned long num_point_lights);                   // [21i/65b]  *+0x2f0 (0x00BD3FB1)
    void ApplyPalSkinVertexShaderDP3BumpShadow(unsigned long num_point_lights);               // [21i/65b]  *+0x2f8 (0x00BD4064; OOL copy 0x00BCE8A0)
    void ApplyPalSkinVertexShaderSpotLight(unsigned long num_point_lights);                   // [20i/63b]  +0x30c (0x00BD3BC7)
    void ApplyPalSkinVertexShaderSpotShadow(unsigned long num_point_lights, unsigned long);   // [23i/71b]  *+0x2fc (0x00BD4112; OOL 0x00BCE900 landed)
    void ApplyPalSkinVertexShaderEnvSpotShadow(unsigned long num_point_lights);               // [21i/65b]  *+0x300 (0x00BD422E; OOL copy 0x00BCE940)
    void Apply2DPalSkinVertexShader();                                                        // [12i/28b]  +0x2c8 (0x00BD4543)
    void Apply2DPalSkinVertexShaderEnv();                                                     // [12i/28b]  +0x2d0
    void Apply2DPalSkinVertexShaderDP3Bump();                                                 // [12i/28b]  +0x2e0
#endif
    unsigned long  PointLightShaderIndex[6];                                 // +0x020
    CVertexShader  VertexShadersPalSkin[4];                                  // +0x038
    CVertexShader  VertexShadersPalSkinEnv[4];                               // +0x058
    CVertexShader  VertexShadersPalSkinEnvBump[4];                           // +0x078
    CVertexShader  VertexShadersPalSkinDepthBufferShadow[4];                 // +0x098
    CVertexShader  VertexShadersPalSkinEnvDepthBufferShadow[4];              // +0x0b8
    CVertexShader  VertexShadersPalSkinEnvBumpDepthBufferShadow[4];          // +0x0d8
    CVertexShader  VertexShadersPalSkinDP3BumpDepthBufferShadow[4];          // +0x0f8
    CVertexShader  VertexShadersPalSkinSpotDepthBufferShadow[4];             // +0x118
    CVertexShader  VertexShadersPalSkinEnvSpotDepthBufferShadow[4];          // +0x138
    CVertexShader  VertexShadersPalSkinEnvBumpSpotDepthBufferShadow[4];      // +0x158
    CVertexShader  VertexShadersPalSkinDP3BumpSpotDepthBufferShadow[4];      // +0x178
    CVertexShader  VertexShadersPalSkinDepthShadowBufferRender;              // +0x198
    CVertexShader  VertexShadersPalSkinColourShadowBufferRender;             // +0x1a0
    CVertexShader  VertexShadersPalSkinColourBufferShadow[4];                // +0x1a8
    CVertexShader  VertexShadersPalSkinEnvColourBufferShadow[4];             // +0x1c8
    CVertexShader  VertexShadersPalSkinEnvBumpColourBufferShadow[4];         // +0x1e8
    CVertexShader  VertexShadersPalSkinDP3BumpColourBufferShadow[4];         // +0x208
    CVertexShader  VertexShadersPalSkinSpotColourBufferShadow[4];            // +0x228
    CVertexShader  VertexShadersPalSkinEnvSpotColourBufferShadow[4];         // +0x248
    CVertexShader  VertexShadersPalSkinEnvBumpSpotColourBufferShadow[4];     // +0x268
    CVertexShader  VertexShadersPalSkinDP3BumpSpotColourBufferShadow[4];     // +0x288
    CVertexShader  VertexShaderPalSkinOutline;                               // +0x2a8
    CVertexShader  VertexShaderPalSkinDP3BumpOutline;                        // +0x2b0
    CVertexShader  VertexShaderPalSkinOutlineSmoothFill;                     // +0x2b8
    CVertexShader  VertexShaderPalSkinDP3BumpOutlineSmoothFill;              // +0x2c0
    CVertexShader  VertexShaders2DPalSkin;                                   // +0x2c8
    CVertexShader  VertexShaders2DPalSkinEnv;                                // +0x2d0
    CVertexShader  VertexShaders2DPalSkinEnvBump;                            // +0x2d8
    CVertexShader  VertexShaders2DPalSkinDP3Bump;                            // +0x2e0
    CVertexShader* VertexShadersPalSkinShadowBufferRender;                   // +0x2e8
    CVertexShader* VertexShadersPalSkinShadow;                               // +0x2ec
    CVertexShader* VertexShadersPalSkinEnvShadow;                            // +0x2f0
    CVertexShader* VertexShadersPalSkinEnvBumpShadow;                        // +0x2f4
    CVertexShader* VertexShadersPalSkinDP3BumpShadow;                        // +0x2f8
    CVertexShader* VertexShadersPalSkinSpotShadow;                           // +0x2fc
    CVertexShader* VertexShadersPalSkinEnvSpotShadow;                        // +0x300
    CVertexShader* VertexShadersPalSkinEnvBumpSpotShadow;                    // +0x304
    CVertexShader* VertexShadersPalSkinDP3BumpSpotShadow;                    // +0x308
    CVertexShader  VertexShadersPalSkinSpotlight[4];                         // +0x30c
    float          TextureOverlayFadeDistandeFactor;                         // +0x32c (sic, PDB spelling)
    CPreTransposedBoneMatrix BoneMatrixBuffer[18];                           // +0x330
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerAnimatedMeshesBase) == 0x690);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS
// Bodies (chunk 4010 scout): texts FITTED to the FableWin.pdb debug sizes with plain VC7.1 /Od (scout/fw_palskin.cpp, fw_helpers.cpp):
// SAME = ApplyVertexShader 12/30, ApplyPalSkinVertexShader 23/68 (ternary local), Env 20/60 + SpotLight 20/63 (local copy, if-clamp),
// EnvShadow / DP3BumpShadow / EnvSpotShadow 21/65 (if-clamp the parameter, index local), Apply2D* 12/28. NOT fitted (DIFF): Shadow /
// SpotShadow (KK, FableWin 23/71): the ternary-local text gives 24/73, the if-clamp + index-local text 21/65; landed 0x00BCE900's text
// (if-clamp, no local) is kept below for them. The two retail out-of-line copies 0x00BCE8A0 / 0x00BCE940 can judge DP3BumpShadow /
// EnvSpotShadow exactly (compile a member-address-taking probe against them).
#include "engine/CShaderRenderManager.h"
// Nested opt-in FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS_FORCEINLINE (chunk 4010, row 0x00BD3070): retail
// expands every helper at all 21 sites of RenderMeshPrimitive; compiled alone, VC7.1 calls 17 of them (the per-caller inline budget of
// a callee not yet compiled in the unit runs out; in the original unit FrameStart 0x00BCEA60 and the out-of-line copies 0x00BCE8A0 /
// 0x00BCE900 / 0x00BCE940 come first, so the helpers were already compiled and the cost rule expanded them).
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS_FORCEINLINE
#define FABLE_ANIMATEDMESHESBASE_HELPER_INLINE FABLE_FORCEINLINE
#else
#define FABLE_ANIMATEDMESHESBASE_HELPER_INLINE inline
#endif
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, true);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShader(unsigned long num_point_lights, unsigned long)
{
    unsigned long lights = num_point_lights > 5 ? 5 : num_point_lights;
    ApplyVertexShader(VertexShadersPalSkin[PointLightShaderIndex[lights]]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderEnv(unsigned long num_point_lights)
{
    unsigned long lights = num_point_lights;
    if (lights > 5)
        lights = 5;
    ApplyVertexShader(VertexShadersPalSkinEnv[PointLightShaderIndex[lights]]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderShadow(unsigned long num_point_lights, unsigned long)
{
    if (num_point_lights > 5)
        num_point_lights = 5;
    ApplyVertexShader(VertexShadersPalSkinShadow[PointLightShaderIndex[num_point_lights]]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderEnvShadow(unsigned long num_point_lights)
{
    if (num_point_lights > 5)
        num_point_lights = 5;
    unsigned long index = PointLightShaderIndex[num_point_lights];
    ApplyVertexShader(VertexShadersPalSkinEnvShadow[index]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderDP3BumpShadow(unsigned long num_point_lights)
{
    if (num_point_lights > 5)
        num_point_lights = 5;
    unsigned long index = PointLightShaderIndex[num_point_lights];
    ApplyVertexShader(VertexShadersPalSkinDP3BumpShadow[index]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderSpotLight(unsigned long num_point_lights)
{
    unsigned long lights = num_point_lights;
    if (lights > 5)
        lights = 5;
    ApplyVertexShader(VertexShadersPalSkinSpotlight[PointLightShaderIndex[lights]]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderSpotShadow(unsigned long num_point_lights, unsigned long)
{
    if (num_point_lights > 5)
        num_point_lights = 5;
    ApplyVertexShader(VertexShadersPalSkinSpotShadow[PointLightShaderIndex[num_point_lights]]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderEnvSpotShadow(unsigned long num_point_lights)
{
    if (num_point_lights > 5)
        num_point_lights = 5;
    unsigned long index = PointLightShaderIndex[num_point_lights];
    ApplyVertexShader(VertexShadersPalSkinEnvSpotShadow[index]);
}
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::Apply2DPalSkinVertexShader() { ApplyVertexShader(VertexShaders2DPalSkin); }
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::Apply2DPalSkinVertexShaderEnv() { ApplyVertexShader(VertexShaders2DPalSkinEnv); }
FABLE_ANIMATEDMESHESBASE_HELPER_INLINE void CEnginePrimitiveManagerAnimatedMeshesBase::Apply2DPalSkinVertexShaderDP3Bump() { ApplyVertexShader(VertexShaders2DPalSkinDP3Bump); }
#endif
#endif // FABLE_ENGINE_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_H
