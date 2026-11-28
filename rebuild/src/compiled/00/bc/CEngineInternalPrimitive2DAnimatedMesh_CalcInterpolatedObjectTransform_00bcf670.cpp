// 00bcf670 CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform
// Leaf: ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DAnimatedMesh@@UBEXAAVCMatrix3x4@@M_N@Z
// Flags:
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_MESH_2D_OBJECT_MATRIX_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DANIMATEDMESH
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"

void CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform(CMatrix3x4& dest, float interp, bool use_scaling) const
{
    C3DVector world_pos = WorldPos.GetInterpolated(interp);
    CRightHandedSet rhs = Orientation.GetInterpolated(interp);
    rhs.MakePerpendicular();
    Calc2DObjectMatrix(dest, ObjectBoundingSphere.GetCenter(), world_pos, rhs, use_scaling ? Scale : 1.0f);
}
