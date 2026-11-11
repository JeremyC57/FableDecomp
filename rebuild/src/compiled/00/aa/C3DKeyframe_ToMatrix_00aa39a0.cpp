// C3DKeyframe::ToMatrix -- write the rotation and position as a homogeneous row-vector matrix.
// Inline-budget: /d2inlT308 expands the original quaternion conversion; T307 keeps its call.
// Leaf: ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z
// Retail: 0x00AA39A0,332 bytes,thiscall.
// Flags: /d2inlT308
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CQUATERNION_MATRIX4
#include "engine/C3DKeyframe.h"
void C3DKeyframe::ToMatrix(CMatrix4x4& matrix,bool applyScaling)const
{
 Rotation.ToMatrix(matrix,GetPosition());
}