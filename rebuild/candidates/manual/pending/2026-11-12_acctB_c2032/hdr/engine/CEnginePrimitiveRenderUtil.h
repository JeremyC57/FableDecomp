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
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveRenderUtil) == 0x4);

#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
