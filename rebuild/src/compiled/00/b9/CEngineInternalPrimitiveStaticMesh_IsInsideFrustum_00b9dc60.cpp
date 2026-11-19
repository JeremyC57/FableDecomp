// CEngineInternalPrimitiveStaticMesh::IsInsideFrustum(const CEngineCamera& frustum, bool distance_test_only) -- the mesh's world-space
//   bounding sphere centre (the local centre through ObjectTransform) must lie within the primitive's max draw distance of the camera;
//   unless distance_test_only, the sphere (radius scaled by Scale) must also pass the camera's ClipSphere (near / far along the look
//   vector, then the four side planes)
// Leaf: ?IsInsideFrustum@CEngineInternalPrimitiveStaticMesh@@UAE_NABVCEngineCamera@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9DC60, 547 bytes, __thiscall virtual, `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?IsInsideFrustum@CEngineInternalPrimitiveStaticMesh@@UAE_NABVCEngineCamera@@_N@Z.
// Callees: CEngineInternalPrimitiveMeshBase::GetMaxDrawDistance 0x00BEB660 (const). CMatrix3x4::TransformPoint and
//   CEngineCamera::ClipSphere are expanded in place (FableWin.pdb debug copies only).
// Port (NF101, 2026-11-19, q11277): moved off the flattened header branch (a free __fastcall stand-in over byte casts) onto the retail
//   class chain, the 0x00BCF040 (CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum) define set; the callee 0x00BEB660 now binds
//   the PDB ?GetMaxDrawDistance@CEngineInternalPrimitiveMeshBase@@QBEMXZ (was the free stand-in CEnginePrimitive_ComputeFadeDistance).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_GETMAXDRAWDISTANCE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"

bool CEngineInternalPrimitiveStaticMesh::IsInsideFrustum(const CEngineCamera& frustum, bool distance_test_only)
{
    C3DVector center;
    ObjectTransform.TransformPoint(MeshInfo->GetBoundingSphere().Center, center);
    C3DVector delta = center - frustum.PeekCameraPos();
    float max_draw_dist = GetMaxDrawDistance();
    if (delta.GetSquaredMagnitude() > max_draw_dist * max_draw_dist)
        return false;
    if (distance_test_only)
        return true;
    float radius = MeshInfo->GetBoundingSphere().Radius * Scale;
    return frustum.ClipSphere(center, radius);
}
