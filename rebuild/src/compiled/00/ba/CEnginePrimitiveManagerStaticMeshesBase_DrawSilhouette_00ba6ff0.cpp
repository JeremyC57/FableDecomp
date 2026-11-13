// CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette(CEngineInternalPrimitiveStaticMesh& primitive) -- under a scoped render-state capture block, when the visibility ratios are valid and the primitive has a silhouette colour: take the camera distance of the shadow mesh's world-space sphere centre and the mesh's world-space radius, pick the LOD (GetAppropriateLOD), fetch that LOD mesh (return if not resident), record mesh / LOD / local lights in the render info, then either draw the mesh into the stencil (ShowSilhouettes == 1: stencil replace 1, colour and z off, DrawImmediately, then colour on, stencil equal 1, op zero) or set z test on / z write off, and draw the alpha bounding box in the colour
// Leaf: ?DrawSilhouette@CEnginePrimitiveManagerStaticMeshesBase@@AAEXAAVCEngineInternalPrimitiveStaticMesh@@@Z
// Inline-budget: FABLE_FORCEINLINE on CEnginePrimitiveManagerMeshBase::GetAppropriateLOD (c2 inline score 182 = c1 91 at f 0.25 > T 74; retail
//   expands it here, 0x00BA7119..0x00BA717B, as at every other caller) and on the FableWin-only CEnginePrimitiveRenderUtil::FindLocalLights
//   (score 98 at f 0.125; expanded 0x00BA71C7..0x00BA720F); no /d2inlT fits because ~CCountedPointer<const C3DMesh2> (94 at f 0.125) must stay
//   called at both returns (0x00BA719C / 0x00BA7749). CalcDistanceFromCamera (44) expands by itself. Trace: tools/inl.sh (probe/q0i.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA6FF0, 1907 bytes, thiscall `ret 4`, no EH frame, /O2. Identity: Ego_r.pdb (donor 0x00A5EE60) / FableWin.pdb
//   ?DrawSilhouette@CEnginePrimitiveManagerStaticMeshesBase@@AAEXAAVCEngineInternalPrimitiveStaticMesh@@@Z (private; next to DrawImmediately 0x00BA6DC0 in both PDB orders).
// Callees: CEnginePrimitiveManagerOcclusionBase::GetSilhouetteColour 0x00B90090 (PC body `xor al,al ; ret 8`) / DrawAlphaBoundingBox 0x00B90CD0,
//   C3DMeshInfo::GetMesh 0x00A25E70, ~CCountedPointer<const C3DMesh2> 0x00A36EC0, CEngineLightingManager::GetLightsInSphereFast 0x00B46B90,
//   DrawImmediately 0x00BA6DC0, CRenderStateManager::RestoreCaptureBlock 0x00A05840 (scoped capture block), the primitive's virtual
//   GetBoundingBoxTransform (slot 16). Globals: CRenderStateCaptureBlock::StateManager 0x013CB508, NEngineInternal::GPRenderManager 0x01436E18,
//   g_pPrimitiveRendererSharedData 0x01436E78 (VisibilityRatiosValid +0x25A, ShowSilhouettes +0x250, LODErrorTolerance +0x9C, FOV-adjusted
//   virtual screen size +0xA8), g_pEngineLightingManager 0x01436E9C, g_pEngineCamera 0x01436EA0, DELTA_NOTIONAL_ZERO 0x0129BA3C, the stencil
//   op / func tables 0x01396F38 / 0x01396E18.
// Inline bodies proven against retail's unreferenced out-of-line COMDAT copies (RepeatedStaticMeshes TU; a file-scope address-taking probe
//   compiles to the same bytes): GetAppropriateLOD = 0x00B9C210 (ternary clamp of the distance to the surface, a separate lod_error local that
//   lands in radius's dead home), CalcDistanceFromCamera = 0x00B9C280 (one-line GetMagnitude of the camera offset, as the matched 0x00B9A4A0).
//   Shape: the ternary clamp is what schedules the radius chain after the fsqrt (0x00BA7119; an if-clamp schedules it before the squares); the
//   named render_info pointer for the three render-info stores is what makes GetMesh re-read primitive.MeshInfo (0x00BA7184) so the LOD walk
//   reuses ecx (`add ecx,44h`, 0x00BA714A) -- writing primitive.GetRenderInfo()-> three times keeps MeshInfo CSE'd in ecx and walks edx. The
//   FindLocalLights channel is a by-value parameter (default temporary copied into the same frame word, 0x00BA71C7..0x00BA71DB).
//   Header opt-ins: hdr/README.md.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_GETRENDERINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_RADIUS_INLINE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_SILHOUETTE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SILHOUETTE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_compiler.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/CCountedPointer.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderManager.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;                                  // 0x01436E18
}
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData; // 0x01436E78
extern CEngineLightingManager*             g_pEngineLightingManager;       // 0x01436E9C
extern CEngineCamera*                      g_pEngineCamera;                // 0x01436EA0

inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *g_pEngineCamera; }
inline float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& position)
{
    return (position - GetCurrentCamera().PeekCameraPos()).GetMagnitude();
}
inline bool __fastcall CEnginePrimitiveRenderUtil::GetVisibilityRatiosValid() { return g_pPrimitiveRendererSharedData->VisibilityRatiosValid; }
inline long __fastcall CEnginePrimitiveRenderUtil::GetShowSilhouettes() { return g_pPrimitiveRendererSharedData->ShowSilhouettes; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODErrorTolerance() { return g_pPrimitiveRendererSharedData->LODErrorTolerance; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted() { return g_pPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted; }
FABLE_FORCEINLINE CEngineLocalLightArray* __fastcall CEnginePrimitiveRenderUtil::FindLocalLights(const C3DVector& position, float radius,
                                                                                      CEnginePrimitiveThemeChannelDesc channel)
{
    return g_pEngineLightingManager->GetLightsInSphereFast(C3DBoundingSphere(position, radius), 5, channel);
}

FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateLOD(const C3DMeshInfo* mesh_info, float distance, float radius, float tolerance)
{
    float nearest = distance - radius > DELTA_NOTIONAL_ZERO ? distance - radius : DELTA_NOTIONAL_ZERO;
    float lod_error = nearest / (radius * CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted()) * tolerance;
    unsigned long lod = 0;
    while (mesh_info->HasLOD(lod + 1) && !(mesh_info->GetLODError(lod + 1) > lod_error))
        ++lod;
    return lod;
}

void CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette(CEngineInternalPrimitiveStaticMesh& primitive)
{
    CRenderStateScopedCaptureBlock capture_block;
    if (!GetVisibilityRatiosValid())
        return;
    CRGBColour colour;
    if (!GetSilhouetteColour(primitive, colour))
        return;
    C3DVector centre = primitive.ShadowMeshInfo->GetBoundingSphere().Center;
    primitive.ObjectTransform.TransformPoint(centre);
    float distance = CalcDistanceFromCamera(centre);
    float radius = primitive.GetBoundingSphereRadiusWorldSpace();
    unsigned long lod = GetAppropriateLOD(primitive.MeshInfo, distance, radius, GetLODErrorTolerance());
    CCountedPointer<const C3DMesh2> mesh;
    if (!primitive.MeshInfo->GetMesh(mesh, lod, 0))
        return;
    CEngineRenderInfoStaticMesh* render_info = primitive.GetRenderInfo();
    render_info->Mesh = mesh.Get();
    render_info->LOD = lod;
    render_info->Lights = FindLocalLights(centre, radius, CEnginePrimitiveThemeChannelDesc());
    if (GetShowSilhouettes() == 1) {
        NEngineInternal::GPRenderManager->SetStencilEnable(true);
        NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
        NEngineInternal::GPRenderManager->SetStencilRef(1);
        NEngineInternal::GPRenderManager->SetStencilWriteMask(1);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        DrawImmediately(primitive, (ERenderMode)2, RENDER_PASS_Z_WRITE);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(true);
        NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_EQUAL);
        NEngineInternal::GPRenderManager->SetStencilMask(1);
        NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_ZERO);
    } else {
        NEngineInternal::GPRenderManager->SetZBufferEnable(true);
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    }
    CMatrix3x4 box;
    primitive.GetBoundingBoxTransform(box);
    DrawAlphaBoundingBox(box, colour.GetAsInt());
}