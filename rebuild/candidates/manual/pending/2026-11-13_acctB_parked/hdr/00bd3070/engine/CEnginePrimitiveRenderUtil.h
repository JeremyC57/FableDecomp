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
#include "engine/EEngineOcclusionLayer.h"
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class C3DVector;
class CEngineLocalLightArray;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
class CEngineInternalPrimitiveStaticMesh;
class CEngineInternalPrimitiveAnimatedMesh;
class CMatrix3x4;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ANIMATEDMESH
class CEngineMeshEffect;
class CEngineMeshEffectOutline;
class CRGBColour;
class CPixelShader;
class CMatrix3x4;
class C2DAlphaMaskInfo;
enum ERenderPassType;
enum EEnginePrimitiveBlendMode;
#endif
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
 static float __fastcall CalcDistanceFromCamera(const C3DVector&);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE_SQUARED
 // Nested opt-in (chunk 2032): FableWin.pdb ?CalcDistanceFromCameraSquared@CEnginePrimitiveRenderUtil@@SIMABVC3DVector@@@Z (inline; the row defines the body).
 static float __fastcall CalcDistanceFromCameraSquared(const C3DVector&);
#endif
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
// Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): FableWin.pdb static fastcall
    // getters ?GetCurrentOcclusionLayer@CEnginePrimitiveRenderUtil@@SI?AW4EEngineOcclusionLayer@@XZ, ?GetOcclusionEnable@..@@SI_NXZ,
    // ?GetLODErrorTolerance@..@@SIMXZ, ?GetLODVirtualScreenSizeFOVAdjusted@..@@SIMXZ (no Ego_r.pdb or Fable.exe copy: the row reads the
    // shared data 0x01436E78 +0x78 / +0xBD / +0x9C / +0xA8 in place) and the member ?CalculateSubPrimitiveRenderFlags@..@@QAEK_N00K0@Z
    // (unreferenced COMDAT 0x00B98CC0 in this TU, expanded at 0x00B9AA14..0x00B9AAA0). The including row defines the bodies.
    static EEngineOcclusionLayer __fastcall GetCurrentOcclusionLayer();
    static bool __fastcall GetOcclusionEnable();
    static float __fastcall GetLODErrorTolerance();
    static float __fastcall GetLODVirtualScreenSizeFOVAdjusted();
    unsigned long CalculateSubPrimitiveRenderFlags(bool fading, bool translucent, bool alpha_test, unsigned long primitive_flags, bool shadowed);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHABLENDSTATES
    // Opt-in (chunk 2060 row 0x00BA5260, lane bwf1 row 0x00BD3070: both RenderMeshPrimitive rows expand it): FableWin.pdb
    // ?SetupAlphaBlendingStatesFromRenderFlags@CEnginePrimitiveRenderUtil@@QAEXKW4ERenderPassType@@EE@Z (268 bytes / 71 debug instructions;
    // no Ego_r.pdb or retail out-of-line copy). The including row defines the body.
    void SetupAlphaBlendingStatesFromRenderFlags(unsigned long render_flags, ERenderPassType pass, unsigned char alpha, unsigned char alpha_ref);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
    // Opt-in (chunk 2032, row 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette): FableWin.pdb
    // ?FindLocalLights@CEnginePrimitiveRenderUtil@@SIPAVCEngineLocalLightArray@@ABVC3DVector@@MVCEnginePrimitiveThemeChannelDesc@@@Z (no
    // Ego_r.pdb or Fable.exe copy): static fastcall, the channel BY VALUE -- the caller's default-constructed temporary is copied into the
    // parameter (0x00BA71C7..0x00BA71DB: three zero bytes, then a dword copy into the same frame word) -- and the lights of the sphere
    // (position, radius) from the lighting manager's GetLightsInSphereFast with at most 5 lights. The including row defines the body.
    static CEngineLocalLightArray* __fastcall FindLocalLights(const C3DVector& position, float radius, CEnginePrimitiveThemeChannelDesc channel);
    // FableWin.pdb static fastcall getters ?GetVisibilityRatiosValid@CEnginePrimitiveRenderUtil@@SI_NXZ and
    // ?GetShowSilhouettes@CEnginePrimitiveRenderUtil@@SIJXZ (no Ego_r.pdb or Fable.exe copy: the row reads the shared data 0x01436E78
    // +0x25A / +0x250 in place). The including row defines the bodies.
    static bool __fastcall GetVisibilityRatiosValid();
    static long __fastcall GetShowSilhouettes();
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
    // Overlay opt-in (chunk 2011): Ego_r.pdb / FableWin.pdb ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveStaticMesh@@@Z
    // (retail 0x00BF12A0, `ret 4`) and ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveAnimatedMesh@@ABVCMatrix3x4@@@Z
    // (retail 0x00BF18B0, `ret 8`): non-static members that never read `this` (Ego_r donor order 0x00AA9420 < 0x00AA99E0 = retail order).
    void AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh& primitive);
    void AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh& primitive, const CMatrix3x4& object_transform);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ANIMATEDMESH
    // Opt-in (chunk 4010 scout, row 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive). Out of line in retail
    // (Ego_r.pdb + FableWin.pdb spellings; retail addresses from the manifest labels, the two SetupOutline*States told apart by body:
    // 0x00BF0740 writes ColourWriteEnable / StencilOpPass / StencilWriteMask = Stencil, 0x00BF0B20 alpha-blends = Fill, Ego_r order agrees):
    void SetupStencilMask(const CEngineMeshEffect* effect, unsigned long render_flags, ERenderPassType pass);   // 0x00BF1030 ret 0Ch
    void SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline& effect, float scale);                 // 0x00BEFD90 ret 8
    void SetupOutlineSmoothFillStates(const CEngineMeshEffectOutline& effect, float scale, bool alpha, long);  // 0x00BF2240 ret 10h
    void SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float scale);                       // 0x00BF0740 ret 8
    void SetupOutlineFillStates(const CEngineMeshEffectOutline& effect, float scale);                          // 0x00BF0B20 ret 8
    void SetupOverlayAlphaBlendingStates(unsigned long, CRGBColour colour);                                   // 0x00BF1EE0
    // Inline in retail (FableWin.pdb only; COMDATs listed right after RenderMeshPrimitive; the row defines the bodies over the shared data
    // g_pPrimitiveRendererSharedData 0x01436E78; debug sizes 6i (15/16b) for the getters, 19i/58b for the 2D blend-mode pair):
    static const CPixelShader& __fastcall GetPixelShaderDiffuseOnly();                   // +0xdc  (FableWin.pdb 0x02E74B80)
    static const CPixelShader& __fastcall GetPixelShaderDiffuseColourTextureDiffuseAlpha();   // +0xe4 (0x02E74BA0)
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFill();             // +0x1fc (0x02E74BC0)
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillAlpha();        // +0x204 (0x02E74BE0)
    static const CPixelShader& __fastcall GetPixelShaderDiffuse();                       // +0xf4  (0x02E74C00)
    static const CPixelShader& __fastcall GetPixelShaderDiffuseFogShadow();              // *+0x224 (0x02E74C20)
    static const CPixelShader& __fastcall GetPixelShaderDiffuseFogShadowOutputFactor();  // *+0x228 (0x02E74C40)
    static const CPixelShader& __fastcall GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode mode);   // +0x1ac (0x02E739A0)
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillDP3();          // +0x20c
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillDP3Alpha();     // +0x214
    static const CPixelShader& __fastcall GetPixelShaderDiffuseFog();                    // +0xfc
    static const CPixelShader& __fastcall GetPixelShaderDiffuseEnvFog();                 // +0x104
    static const CPixelShader& __fastcall GetPixelShaderDiffuseEnvFogShadow();           // *+0x22c
    static const CPixelShader& __fastcall GetPixelShaderDiffuseDP3BumpFogShadow();       // *+0x234
    static const CPixelShader& __fastcall GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor();   // *+0x238
    static const CPixelShader& __fastcall GetPixelShader2DDiffuseEnv(EEnginePrimitiveBlendMode mode);
    static const CPixelShader& __fastcall GetPixelShader2DDiffuseDP3Bump(EEnginePrimitiveBlendMode mode);
    static bool __fastcall GetShowPreloadColours();                                      // +0x248
    static void __fastcall SetWorldTransform(const CMatrix3x4& transform);               // 21i/61b: FixedFunctionVertexMode ? core : shader
    static float __fastcall GetMipmapSizeMultiplier();                                   // ?GetMipmapSizeMultiplier@CEnginePrimitiveRenderUtil@@SIMXZ 6i/16b (+0xb0)
    static void __fastcall FailIfXbox();                                                 // 4i/5b (empty)
    const C2DAlphaMaskInfo* GetCurrent2DAlphaMaskInfo();                                 // 16i/46b (2D viewport manager's AlphaMaskInfo)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveRenderUtil) == 0x4);

#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
