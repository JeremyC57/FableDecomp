#ifndef FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
#define FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
// Hand-written (chunk 723): CEnginePrimitiveRenderUtil : CBaseClassNonCopyable : CBaseClass (Fable.exe RTTI), no data members
// (Ego_r.pdb). vtable 0x012A3BD0; ctor inline (out-of-line copy 0x00B8B5E0), virtual dtor slot 0 0x00B8B600. Its many helpers are
// static fastcall (SI) functions in the PDBs and are declared where a row needs them.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB
enum EVertexBufferPool;                                                  // MS forward enum
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICIB
enum EIndexBufferPool;                                                   // MS forward enum (engine/CIndexBuffer.h)
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
class CEngineInternalPrimitiveStaticMesh;
class CEngineInternalPrimitiveAnimatedMesh;
class CMatrix3x4;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#include "engine/EEngineOcclusionLayer.h"
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
class C2DAlphaMaskInfo;
enum EEnginePrimitiveBlendMode;                                          // MS forward enum (engine/CEnginePrimitive2DSprite.h)
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
class CEngineMeshEffect;
class CMatrix3x4;
enum ERenderPassType;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
class CEngineMeshEffectOutline;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class C3DVector;
class CEngineLocalLightArray;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER
class C2DAlphaMaskInfo;
#endif
#ifdef FABLE_RETAIL_PASS22_RENDERUTIL_COMPLETE_ORIGINAL_DECLARATION
// User declarations reconstructed in original PDB field-list order; generated special members remain implicit.
class C2DAlphaMaskInfo;
class C2DBoxF;
class C3DTextureStages;
class C3DVector;
class CEngineCamera;
class CEngineInternalPrimitiveAnimatedMesh;
class CEngineInternalPrimitiveStaticMesh;
class CEngineLocalLightArray;
class CEngineMeshEffect;
class CEngineMeshEffectOutline;
class CEnginePrimitiveThemeChannelDesc;
class CMatrix3x4;
class CPixelShader;
class CRGBColour;
class CRGBFloatColour;
class CStateBlockFunctionBase;
class CVertexShader;
enum EEngineOcclusionLayer;
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
enum EIndexBufferPool;
enum ERenderPassType;
enum EVertexBufferPool;
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
    static CEngineCamera & __fastcall GetCurrentCamera();
    static EEngineOcclusionLayer __fastcall GetCurrentOcclusionLayer();
    static float __fastcall GetLODErrorTolerance();
    static float __fastcall GetCharacterLODErrorTolerance();
    static float __fastcall GetLODVirtualScreenSize();
    static float __fastcall GetLODVirtualScreenSizeFOVAdjusted();
    static float __fastcall GetMipmapSizeMultiplier();
    static long __fastcall GetPreloadMipmapTolerance();
    static long __fastcall GetPreloadMipmapMaxSize();
    static bool __fastcall GetOcclusionEnable();
    static bool __fastcall GetOcclusionHierarchyEnable();
    static bool __fastcall GetBumpMappingEnable();
    static bool __fastcall GetMirroredTextureHighlightEnabled();
    static long __fastcall GetLODForced();
    static unsigned long __fastcall GetGlobalPrimitiveInitFlags();
    static unsigned long __fastcall GetDynamicVBUsage();
    static EVertexBufferPool __fastcall GetDynamicVBPool();
    static unsigned long __fastcall GetDynamicVBLockFlags();
    static unsigned long __fastcall GetDynamicIBUsage();
    static EIndexBufferPool __fastcall GetDynamicIBPool();
    static unsigned long __fastcall GetDynamicIBLockFlags();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseOnly();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseColourTextureDiffuseAlpha();
    static const CPixelShader & __fastcall GetPixelShaderOutlineSmoothFill();
    static const CPixelShader & __fastcall GetPixelShaderOutlineSmoothFillAlpha();
    static const CPixelShader & __fastcall GetPixelShaderOutlineSmoothFillDP3();
    static const CPixelShader & __fastcall GetPixelShaderOutlineSmoothFillDP3Alpha();
    static const CPixelShader & __fastcall GetPixelShaderTextureOnly();
    static const CPixelShader & __fastcall GetPixelShaderDiffuse();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseFog();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseEnvFog();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseEnvBumpFog();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseDP3BumpFog();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseFogShadow();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseFogShadowOutputFactor();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseEnvFogShadow();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseEnvBumpFogShadow();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseDP3BumpFogShadow();
    static const CPixelShader & __fastcall GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor();
    static const CPixelShader & __fastcall GetPixelShaderShadowBufferRender();
    static const CPixelShader & __fastcall GetPixelShaderShadowBufferRenderAlpha();
    static const CPixelShader & __fastcall GetPixelShader2DDiffuseOnly(EEnginePrimitiveBlendMode);
    static const CPixelShader & __fastcall GetPixelShader2DTextureOnly(EEnginePrimitiveBlendMode);
    static const CPixelShader & __fastcall GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode);
    static const CPixelShader & __fastcall GetPixelShader2DDiffuseEnv(EEnginePrimitiveBlendMode);
    static const CPixelShader & __fastcall GetPixelShader2DDiffuseEnvBump(EEnginePrimitiveBlendMode);
    static const CPixelShader & __fastcall GetPixelShader2DDiffuseDP3Bump(EEnginePrimitiveBlendMode);
    static float __fastcall CalcDistanceFromCamera(const CMatrix3x4 &);
    static float __fastcall CalcDistanceFromCamera(const C3DVector &);
    static float __fastcall CalcDistanceFromCameraSquared(const CMatrix3x4 &);
    static float __fastcall CalcDistanceFromCameraSquared(const C3DVector &);
    static bool __fastcall GetBoundingSphereViewEnabled();
    static bool __fastcall GetBoundingBoxViewEnabled();
    static long __fastcall GetShowMeshInfo();
    static bool __fastcall GetShowPreloadColours();
    static long __fastcall GetShowVisibilityInfo();
    static long __fastcall GetShowSilhouettes();
    static bool __fastcall GetBoneViewEnabled();
    static bool __fastcall GetShow2DBoundingRect();
    static bool __fastcall GetShow2DBoundingSphere();
    static bool __fastcall GetShowUser2DBoundingSphere();
    static bool __fastcall GetShow2DRotationPos();
    static bool __fastcall GetVisibilityRatiosUpdate();
    static bool __fastcall GetVisibilityRatiosValid();
    void AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh &, const CMatrix3x4 &);
    void AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh &);
    static CEngineLocalLightArray * __fastcall FindLocalLights(const C3DVector &, float, CEnginePrimitiveThemeChannelDesc);
    static void __fastcall SetupLocalLights(const CEngineLocalLightArray &);
    static void __fastcall SetWorldTransform(const CMatrix3x4 &);
    static void __fastcall ApplyVertexShader(const CVertexShader &);
    static void __fastcall ApplyPixelShader(const CPixelShader &);
    static void __fastcall ApplyPixelShaderIfXbox(const CPixelShader &);
    static void __fastcall ApplyStateBlockIfPC(CStateBlockFunctionBase *);
    static void __fastcall FailIfXbox();
    static void __fastcall FailIfPC();
    static void __fastcall SetTextureFactor(const CRGBFloatColour &);
    unsigned long CalculateSubPrimitiveRenderFlags(bool, bool, bool, unsigned long, bool);
    unsigned long CalculateSubPrimitiveRenderFlags2D(bool, bool, unsigned long);
    void SetupAlphaBlendingStatesFromRenderFlags(unsigned long, ERenderPassType, unsigned char, unsigned char);
    bool SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode);
    void SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode, unsigned long, unsigned long &);
    void SetupOverlayAlphaBlendingStates(unsigned long, CRGBColour);
    void SetupBlendOpState(EEnginePrimitiveBlendOp);
    void SetupOutlineStencilMask(unsigned long &, unsigned long &, const CEngineMeshEffectOutline &, unsigned long, ERenderPassType);
    void SetupColourFilterStencilMask(unsigned long &, unsigned long &);
    void SetupBlankTextures(C3DTextureStages &, bool);
    void SetupDiffuseShading(bool, const CRGBColour &);
    void SetupDiffuseShading(bool, unsigned long, unsigned long);
    void Setup2DDiffuseShading(bool, unsigned long, unsigned long, EEnginePrimitiveBlendMode);
    void Set2DAlphaMaskTextureMode(unsigned long, bool, bool);
    void DisableTextureFiltering(unsigned long);
    const C2DAlphaMaskInfo * GetCurrent2DAlphaMaskInfo();
    bool ClipScreenSpaceRectToViewport(C2DBoxF &);
    void SetupStencilMask(const CEngineMeshEffect *, unsigned long, ERenderPassType);
    void SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline &, float);
    void SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline &);
    void SetupOutlineSmoothFillStates(const CEngineMeshEffectOutline &, float, bool, long);
    void SetupOutlineStencilStates(const CEngineMeshEffectOutline &, float);
    void SetupOutlineFillStates(const CEngineMeshEffectOutline &, float);
};

#else
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB
    // Opt-in (chunk 2061, row 0x00BCC800 RenderWeaponTrail): FableWin.pdb ?GetDynamicVBUsage@CEnginePrimitiveRenderUtil@@SIKXZ,
    // ?GetDynamicVBPool@CEnginePrimitiveRenderUtil@@SI?AW4EVertexBufferPool@@XZ, ?GetDynamicVBLockFlags@CEnginePrimitiveRenderUtil@@SIKXZ
    // (static fastcall; no out-of-line copy in retail). The including row defines the bodies (the primitive renderer's shared
    // data DynamicVBUsage +0xC4 / DynamicVBPool +0xC8 / DynamicVBLockFlags +0xCC).
    static unsigned long __fastcall GetDynamicVBUsage();
    static EVertexBufferPool __fastcall GetDynamicVBPool();
    static unsigned long __fastcall GetDynamicVBLockFlags();
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICIB
    // Overlay opt-in (wfA4, row 0x00BB2540 CEngineClothRenderer::RenderPolygons): FableWin.pdb ?GetDynamicIBUsage@CEnginePrimitiveRenderUtil@@SIKXZ,
    // ?GetDynamicIBPool@CEnginePrimitiveRenderUtil@@SI?AW4EIndexBufferPool@@XZ, ?GetDynamicIBLockFlags@CEnginePrimitiveRenderUtil@@SIKXZ (static
    // fastcall, no out-of-line copy in retail; shared data DynamicIBUsage +0xD0 / DynamicIBPool +0xD4 / DynamicIBLockFlags +0xD8, read at
    // 0x00BB2BBF..0x00BB2BED). The including row defines the bodies.
    static unsigned long __fastcall GetDynamicIBUsage();
    static EIndexBufferPool __fastcall GetDynamicIBPool();
    static unsigned long __fastcall GetDynamicIBLockFlags();
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER
    // Original FableWin bodies02e74030/90/e0 and both PDB class records.
    const C2DAlphaMaskInfo* GetCurrent2DAlphaMaskInfo();
    void DisableTextureFiltering(unsigned long);
    void Set2DAlphaMaskTextureMode(unsigned long,bool,bool);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
 static float __fastcall CalcDistanceFromCamera(const C3DVector&);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE_SQUARED
 // Nested opt-in (chunk 2032): FableWin.pdb ?CalcDistanceFromCameraSquared@CEnginePrimitiveRenderUtil@@SIMABVC3DVector@@@Z (inline; the row defines the body).
 static float __fastcall CalcDistanceFromCameraSquared(const C3DVector&);
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_PASS22_SMOOTHSTENCIL
    // Both PDBs; donor02EEE7C0; native00BEFD90. Original public non-static thiscall.
    void SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline& effect, float mesh_scale);
#endif
    CEnginePrimitiveRenderUtil() { }                                     // ??0CEnginePrimitiveRenderUtil@@QAE@XZ
    virtual ~CEnginePrimitiveRenderUtil();                               // slot 0  ??1CEnginePrimitiveRenderUtil@@UAE@XZ
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHABLENDSTATES
    // Opt-in (chunk 2060 row 0x00BA5260, lane bwf1 row 0x00BD3070: both RenderMeshPrimitive rows expand it): FableWin.pdb
    // ?SetupAlphaBlendingStatesFromRenderFlags@CEnginePrimitiveRenderUtil@@QAEXKW4ERenderPassType@@EE@Z (268 bytes / 71 debug instructions;
    // no Ego_r.pdb or retail out-of-line copy). The including row defines the body.
    void SetupAlphaBlendingStatesFromRenderFlags(unsigned long render_flags, ERenderPassType pass, unsigned char alpha, unsigned char alpha_ref);
#endif
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPDIFFUSESHADING
    // Nested opt-in (wfE5 14th author, row 0x00BC8A60 RenderLine): FableWin.pdb ?SetupDiffuseShading@CEnginePrimitiveRenderUtil@@QAEX_NKK@Z
    // (0x02EC0760, 159b/49i), a debug COMDAT placed right after RenderLine (first used there); no Ego_r.pdb / retail out-of-line copy.
    // Parameter names descriptive. The including row defines the inline body. Declaration only.
    void SetupDiffuseShading(bool textured, unsigned long brightness, unsigned long alpha);
#endif
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
    // Nested opt-in (chunk Q25 row 0x00BF0B20; w24C row 0x00BF0740): Ego_r.pdb / FableWin.pdb, the two (effect, float) outline setters
    // (`this` unused; retail `ret 8`). By render pass (0x00BB3E80 / 0x00BA2350): 0x00BF0740 = SetupOutlineStencilStates (pass 12),
    // 0x00BF0B20 = SetupOutlineFillStates (pass 13); landing gj respelled 0x00BF0B20, which this block first bound to the stencil name.
    void SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float scale);   // 0x00BF0740
    void SetupOutlineFillStates(const CEngineMeshEffectOutline& effect, float scale);      // 0x00BF0B20
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
    // Opt-in (chunk 2060 row 0x00BA5260 RenderMeshPrimitive, there spelled _MESHRENDER before that name was taken by chunk 2028; agentC2 row
    // 0x00BB3E80): Ego_r.pdb / FableWin.pdb ?SetupStencilMask@CEnginePrimitiveRenderUtil@@QAEXPBVCEngineMeshEffect@@KW4ERenderPassType@@@Z
    // (out of line, 0x00BF1030, landed), the FableWin.pdb inline statics ?SetWorldTransform@CEnginePrimitiveRenderUtil@@SIXABVCMatrix3x4@@@Z
    // and ?GetMipmapSizeMultiplier@CEnginePrimitiveRenderUtil@@SIMXZ, and ?SetupAlphaBlendingStatesFromRenderFlags@CEnginePrimitiveRenderUtil@@
    // QAEXKW4ERenderPassType@@EE@Z (no retail out-of-line copy). The including row defines the inline bodies.
    void SetupStencilMask(const CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass);   // 0x00BF1030
    static void __fastcall SetWorldTransform(const CMatrix3x4& transform);
    static float __fastcall GetMipmapSizeMultiplier();
    void SetupAlphaBlendingStatesFromRenderFlags(unsigned long render_flags, ERenderPassType pass, unsigned char alpha, unsigned char alpha_ref);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHSHADERS
    // Opt-in (agentC2, row 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive): the FableWin.pdb static shader getters listed right after
    // that row (its first user): ?GetPixelShaderDiffuseOnly@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@XZ (15b / 6 debug instructions
    // each = the shared data's member address), ..DiffuseColourTextureDiffuseAlpha, ..OutlineSmoothFill, ..OutlineSmoothFillAlpha, ..Diffuse,
    // and ..DiffuseFogShadow / ..DiffuseFogShadowOutputFactor (16b / 6: the pointer member dereferenced). No retail copy (read in place).
    // The including row defines the bodies (they reach the shared data global).
    static const CPixelShader& __fastcall GetPixelShaderDiffuseOnly();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseColourTextureDiffuseAlpha();
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFill();
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillAlpha();
    static const CPixelShader& __fastcall GetPixelShaderDiffuse();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseFogShadow();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseFogShadowOutputFactor();
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
    // Opt-in (agentC6, row 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive): the 2D alpha-mask helpers that row expands (FableWin.pdb
    // names, listed right after that row: ?GetPixelShader2DDiffuse@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@W4EEnginePrimitiveBlendMode@@@Z,
    // ?Set2DAlphaMaskTextureMode@..@@QAEXK_N0@Z, ?DisableTextureFiltering@..@@QAEXK@Z, ?GetCurrent2DAlphaMaskInfo@..@@QAEPBVC2DAlphaMaskInfo@@XZ).
    // No retail copy (all expanded); the including row defines the bodies.
    static const CPixelShader& __fastcall GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode blend_mode);
    void Set2DAlphaMaskTextureMode(unsigned long stage, bool filter, bool clamp);
    void DisableTextureFiltering(unsigned long stage);
    const C2DAlphaMaskInfo* GetCurrent2DAlphaMaskInfo();
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SHADOWBUFFERSHADERS
    // Opt-in (wfA6b, row 0x00BA6240 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer): FableWin.pdb inline statics
    // ?GetPixelShaderShadowBufferRender@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@XZ and ?GetPixelShaderShadowBufferRenderAlpha@..@@SIABVCPixelShader@@XZ
    // (no Ego_r.pdb or retail copy: the row reads [g_pPrimitiveRendererSharedData+21Ch] / [+220h] in place). The including row defines the bodies.
    static const CPixelShader& __fastcall GetPixelShaderShadowBufferRender();
    static const CPixelShader& __fastcall GetPixelShaderShadowBufferRenderAlpha();
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_STATICMESHSHADERS
    // Opt-in (agentC20, row 0x00BA2350 CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants): the remaining FableWin.pdb static
    // shader getters that row reads (?GetPixelShaderDiffuseFog@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@XZ and its siblings, 6 debug
    // instructions each; the 2D ones take the blend mode, 19 instructions, like GetPixelShader2DDiffuse). No retail copy (read in place); the
    // including row defines the bodies (they reach the shared data global).
    static const CPixelShader& __fastcall GetPixelShaderDiffuseFog();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseEnvFog();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseEnvFogShadow();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseDP3BumpFogShadow();
    static const CPixelShader& __fastcall GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor();
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillDP3();
    static const CPixelShader& __fastcall GetPixelShaderOutlineSmoothFillDP3Alpha();
    static const CPixelShader& __fastcall GetPixelShader2DDiffuseOnly(EEnginePrimitiveBlendMode blend_mode);
    static const CPixelShader& __fastcall GetPixelShader2DDiffuseEnv(EEnginePrimitiveBlendMode blend_mode);
    static const CPixelShader& __fastcall GetPixelShader2DDiffuseDP3Bump(EEnginePrimitiveBlendMode blend_mode);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPBLANKTEXTURES
    // Opt-in (bwf2 / agentC16, row 0x00BA2350): FableWin.pdb inline ?SetupBlankTextures@CEnginePrimitiveRenderUtil@@QAEXAAVC3DTextureStages@@_N@Z
    // (no Ego_r.pdb / retail out-of-line copy). The including row defines the body.
    void SetupBlankTextures(class C3DTextureStages& texture_stages, bool dp3_bump);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
    // Opt-in (bwf1 / agentC2, row 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive): the outline set-ups the cloth renderer calls out of
    // line (Ego_r.pdb / FableWin.pdb names; `this` unused). Identities by pass: 0x00BF0740 = SetupOutlineStencilStates (pass 12), 0x00BF0B20 =
    // SetupOutlineFillStates (pass 13); the _OUTLINESTENCILSTATES opt-in declares both (landing gj fixed its 0x00BF0B20 binding).
    void SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline& effect, float scale);                       // 0x00BEFD90
#ifndef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
    void SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline& effect);                          // 0x00BF01E0
#endif
    void SetupOutlineSmoothFillStates(const CEngineMeshEffectOutline& effect, float scale, bool alpha, long pass_index);   // 0x00BF2240
    void SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float scale);                            // 0x00BF0740
    void SetupOutlineFillStates(const CEngineMeshEffectOutline& effect, float scale);                               // 0x00BF0B20
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_2DSPRITES
    // Opt-in (wfE5, row 0x00BAE2D0 CEnginePrimitiveManager2DSprites::RenderSubPrimitive): the three helpers FableWin.pdb lists right after
    // that row (02e97220 ?SetupAlphaBlendingStatesFromBlendMode@CEnginePrimitiveRenderUtil@@QAEXW4EEnginePrimitiveBlendMode@@KAAK@Z,
    // 02e972e0 ?Setup2DDiffuseShading@CEnginePrimitiveRenderUtil@@QAEX_NKKW4EEnginePrimitiveBlendMode@@@Z, 02e973b0
    // ?GetPixelShader2DDiffuseOnly@CEnginePrimitiveRenderUtil@@SIABVCPixelShader@@W4EEnginePrimitiveBlendMode@@@Z). No retail copy
    // (expanded twice, 0x00BAEA60 / 0x00BAF4B9 and 0x00BAECD7 / 0x00BAF6EE); the including row defines the bodies.
    void SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode, unsigned long alpha, unsigned long& colour_scale);
    void Setup2DDiffuseShading(bool textured, unsigned long colour_scale, unsigned long alpha, EEnginePrimitiveBlendMode mode);
    static const CPixelShader& __fastcall GetPixelShader2DDiffuseOnly(EEnginePrimitiveBlendMode blend_mode);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
    // Overlay opt-in (chunk 2011): Ego_r.pdb / FableWin.pdb ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveStaticMesh@@@Z
    // (retail 0x00BF12A0, `ret 4`) and ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveAnimatedMesh@@ABVCMatrix3x4@@@Z
    // (retail 0x00BF18B0, `ret 8`): non-static members that never read `this` (Ego_r donor order 0x00AA9420 < 0x00AA99E0 = retail order).

#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Original two PDBs, donor02E82300 and native00BD71B0; evidence in00bd71cc.
 static bool FABLE_FASTCALL GetBoneViewEnabled();
#endif
    void AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh& primitive);
    void AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh& primitive, const CMatrix3x4& object_transform);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
// Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): FableWin.pdb static fastcall
    // getters ?GetCurrentOcclusionLayer@CEnginePrimitiveRenderUtil@@SI?AW4EEngineOcclusionLayer@@XZ, ?GetOcclusionEnable@..@@SI_NXZ,
    // ?GetLODErrorTolerance@..@@SIMXZ, ?GetLODVirtualScreenSizeFOVAdjusted@..@@SIMXZ (no Ego_r.pdb or Fable.exe copy: the row reads the
    // shared data 0x01436E78 +0x78 / +0xBD / +0x9C / +0xA8 in place) and the member ?CalculateSubPrimitiveRenderFlags@..@@QAEK_N00K0@Z
    // (unreferenced COMDAT 0x00B98CC0 in this TU, expanded at 0x00B9AA14..0x00B9AAA0). The including row defines the bodies.
    static EEngineOcclusionLayer __fastcall GetCurrentOcclusionLayer();
    static bool __fastcall GetOcclusionEnable();
#ifdef FABLE_RETAIL_ANIMATED_MESH_PRELOAD_PASS30
    static float FABLE_FASTCALL GetCharacterLODErrorTolerance();
#endif
    static float __fastcall GetLODErrorTolerance();
    static float __fastcall GetLODVirtualScreenSizeFOVAdjusted();
    unsigned long CalculateSubPrimitiveRenderFlags(bool fading, bool translucent, bool alpha_test, unsigned long primitive_flags, bool shadowed);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
    // Opt-in (chunk 2032, row 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette): FableWin.pdb
    // ?FindLocalLights@CEnginePrimitiveRenderUtil@@SIPAVCEngineLocalLightArray@@ABVC3DVector@@MVCEnginePrimitiveThemeChannelDesc@@@Z (no
    // Ego_r.pdb or Fable.exe copy): static fastcall, the channel BY VALUE -- the caller's default-constructed temporary is copied into the
    // parameter (0x00BA71C7..0x00BA71DB: three zero bytes, then a dword copy into the same frame word) -- and the lights of the sphere
    // (position, radius) from the lighting manager's GetLightsInSphereFast with at most 5 lights. The including row defines the body.
    static CEngineLocalLightArray* __fastcall FindLocalLights(const C3DVector& position, float radius, CEnginePrimitiveThemeChannelDesc channel);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPLOCALLIGHTS
    // Nested opt-in (wfE5, row 0x00BC04F0 RenderSingleSprite): FableWin.pdb ?SetupLocalLights@CEnginePrimitiveRenderUtil@@SIXABVCEngineLocalLightArray@@@Z
    // (static fastcall; no Ego_r.pdb or Fable.exe copy: the row calls SetupShaderPointLights 0x00B44F20 directly). Body in the including row.
    static void __fastcall SetupLocalLights(const CEngineLocalLightArray& lights);
#endif
    // FableWin.pdb static fastcall getters ?GetVisibilityRatiosValid@CEnginePrimitiveRenderUtil@@SI_NXZ and
    // ?GetShowSilhouettes@CEnginePrimitiveRenderUtil@@SIJXZ (no Ego_r.pdb or Fable.exe copy: the row reads the shared data 0x01436E78
    // +0x25A / +0x250 in place). The including row defines the bodies.
    static bool __fastcall GetVisibilityRatiosValid();
    static long __fastcall GetShowSilhouettes();
#endif
};
#endif

FABLE_STATIC_ASSERT_AT(110, sizeof(CEnginePrimitiveRenderUtil) == 0x4);

#ifdef FABLE_RETAIL_MESH_MIPMAP_PASS30
#include "engine/CEnginePrimitiveRendererSharedData.h"
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }
// FableWin GetMipmapSizeMultiplier and the shared-data member at +0xB0 in retail.
inline float FABLE_FASTCALL CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier()
{
    return NEngineInternal::GPPrimitiveRendererSharedData->MipmapSizeMultiplier;
}
#endif
#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
