// CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum(const CEngineCamera& frustum, bool distance_test_only) -- false beyond the primitive's max draw distance from the camera (the interpolated world position); with distance_test_only that is all; otherwise the mesh's safe bounding sphere (centre through the interpolated object transform, radius times Scale) must pass the camera's ClipSphere test
// Leaf: ?IsInsideFrustum@CEngineInternalPrimitiveAnimatedMesh@@UAE_NABVCEngineCamera@@_N@Z
// Retail: 0x00BCF040, 658 bytes (ret 8).
// Names: Ego_r / Ego_d; Ego_d parameters frustum / distance_test_only and locals world_pos, max_draw_dist, interp, radius, distance_squared,
//        obj_transform, center. Callees: GetMaxDrawDistance 0x00BEB660 (Ego_r), CalcInterpolatedObjectTransform (virtual slot 33).
// FITTED (chosen by grading): the centre through the two-argument TransformPoint(source, destination) (FableWin
//        ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z) with the existing FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS body
//        (every one-argument TransformPoint text left row 1's x*E11 operand order wrong: `fld [E11] ; fmul st(4)` v retail `fld st(3) ;
//        fmul [E11]`, 0x00BCF147); distance_squared computed before the max-draw-distance call (retail stores the difference first).
// Inline-budget: TransformPoint is FABLE_FORCEINLINE by the shared CMatrix3x4.h default (FABLE_MATRIX_INLINE_PASS23; with
//        FABLE_RETAIL_CMATRIX3X4_PLAIN_INLINE_PASS23 VC7.1 calls it: 610v658); ClipSphere is forced by its existing opt-in
//        FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE (owner 2026-11-01 helper rule: no out-of-line copy in retail; as landed 0x00B8DF50).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_GETMAXDRAWDISTANCE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ISINSIDEFRUSTUM
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DMESHINFO_SAFEBOUNDINGRADIUS
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"

bool CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum(const CEngineCamera& frustum, bool distance_test_only)
{
    float interp = GetCurrentInterpolate();
    C3DVector world_pos = WorldPos.GetInterpolated(interp);
    float distance_squared = (world_pos - frustum.PeekCameraPos()).GetSquaredMagnitude();
    float max_draw_dist = GetMaxDrawDistance();
    if (distance_squared > max_draw_dist * max_draw_dist)
        return false;
    if (distance_test_only)
        return true;
    CMatrix3x4 obj_transform;
    CalcInterpolatedObjectTransform(obj_transform, interp, true);
    C3DVector center;
    obj_transform.TransformPoint(MeshInfo->GetBoundingSphere().Center, center);
    float radius = MeshInfo->GetSafeBoundingRadius() * Scale;
    return frustum.ClipSphere(center, radius);
}
