// 00beb6c0 CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix
// Leaf: ?Calc2DObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@1ABVCRightHandedSet@@M@Z
// Flags:
// Inline-budget: the shared matrix SetMultiply and both wrappers expand at 00BEB6C0. Plain inline leaves calls and a 550-byte leaf; the established shared opt-ins produce 978 bytes.
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_MESH_2D_OBJECT_MATRIX_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"

void FABLE_FASTCALL CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix(CMatrix3x4& matrix,
    const C3DVector& offset, const C3DVector& world_pos, const CRightHandedSet& rhset, float scale)
{
    CMatrix3x4 trans_centre_to_o(MATRIX34_INIT_IDENTITY);
    trans_centre_to_o.SetTranslateOnly(C3DVector(0.0f, 0.0f, 0.0f) - offset);
    CMatrix3x4 trans_to_pos(MATRIX34_INIT_IDENTITY);
    trans_to_pos.SetTranslateOnly(world_pos);
    C3DVector forward = rhset.GetForward() * scale;
    C3DVector right = rhset.GetRight() * scale;
    C3DVector up = rhset.GetUp() * scale;
    matrix.Set(-right.X, -right.Y, -right.Z,
               -forward.X, -forward.Y, -forward.Z,
               up.X, up.Y, up.Z, 0.0f, 0.0f, 0.0f);
    matrix.PreMultiply(trans_centre_to_o);
    matrix.PostMultiply(trans_to_pos);
}