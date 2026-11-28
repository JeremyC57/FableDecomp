// 00b9d220 CEngineInternalPrimitive2DStaticMesh::CalcInterpolatedObjectTransform
// Leaf: ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DStaticMesh@@UAEXM_N@Z
// Flags:
// Inline-budget: native 00B9D220 expands both matrix multiplies and their SetMultiply helper. This uses the same shared inline forms as the verified 00BEB6C0 transform.
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_STATIC_2D_TRANSFORM_PASS30
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C4DVECTOR
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"

void CEngineInternalPrimitive2DStaticMesh::CalcInterpolatedObjectTransform(float interp, bool use_scaling)
{
    float scale = use_scaling ? Scale : 1.0f;
    CMatrix3x4 trans_centre_to_o(MATRIX34_INIT_IDENTITY);
    trans_centre_to_o.SetTranslateOnly(-BoundingSphere.GetCenter());
    CMatrix3x4 trans_to_pos(MATRIX34_INIT_IDENTITY);
    trans_to_pos.SetTranslateOnly(WorldPos);
    CRightHandedSet rhs = Orientation.GetInterpolated(interp);
    C3DVector forward = rhs.GetForward() * scale;
    C3DVector right = rhs.GetRight() * scale;
    C3DVector up = rhs.GetUp() * scale;
    ObjectTransform.Set(-right.X, -right.Y, -right.Z,
                        -forward.X, -forward.Y, -forward.Z,
                        up.X, up.Y, up.Z, 0.0f, 0.0f, 0.0f);
    ObjectTransform.PreMultiply(trans_centre_to_o);
    ObjectTransform.PostMultiply(trans_to_pos);
}