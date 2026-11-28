// 00a35b70 C3DMeshObject::GetRotationMatrixForTransformFromTo
// Leaf: ?GetRotationMatrixForTransformFromTo@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@0@Z
// Flags: /d2inlT150
// Inline-budget: all calls to Set, SetInverse and PostMultiply visible in the editor expand throughout the retail leaf.
#define FABLE_RETAIL_CARRYABLE_POSE_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_ADD_SET
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DONOR_PASS26
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DMeshObject.h"

CMatrix3x4 FABLE_FASTCALL C3DMeshObject::GetRotationMatrixForTransformFromTo(
    const CRightHandedSet& transform_from_rhs, const CRightHandedSet& transform_to_rhs)
{
    CMatrix3x4 creature_carry_matrix;
    creature_carry_matrix.Set(transform_to_rhs, C3DVector(0.0f, 0.0f, 0.0f));
    CMatrix3x4 object_matrix;
    object_matrix.Set(transform_from_rhs.GetRight(), transform_from_rhs.GetUp(),
                      transform_from_rhs.GetForward(), C3DVector(0.0f, 0.0f, 0.0f));
    CMatrix3x4 transform_to;
    transform_to.SetInverse(object_matrix);
    transform_to.PostMultiply(creature_carry_matrix);
    return transform_to;
}