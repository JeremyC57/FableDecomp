// CRightHandedSet::Rotate -- rotate the two basis vectors about an axis.
// Inline-budget: ToMatrix stays a call at S278 (253 bytes); S279 expands it as native (523 bytes).
// Flags: /d2inlS279
// Retail: 00a22f80,523 bytes,thiscall with axis reference and angle.
// Leaf: ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#include "engine/CRightHandedSet.h"
#include "engine/CQuaternion.h"
void CRightHandedSet::Rotate(const C3DVector& rotate_axis,float angle)
{
 CQuaternion quaternion;
 quaternion.SetFromAxisAngle(rotate_axis,angle);
 CMatrix3x4 rotation_mat;
 quaternion.ToMatrix(rotation_mat,C3DVector(0.0f,0.0f,0.0f));
 rotation_mat.TransformVector(Up);
 rotation_mat.TransformVector(Forward);
}