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
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
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
#include <cstdio>
#include <cstdlib>
static void verify(float x, float y) { if (x != y) std::abort(); }

int main()
{
    const float axes[][3] = {{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-2,0},{1,-1,2},{0,0,0}};
    const float scales[] = {-2, -0.5f, 0, 0.25f, 1, 3};
    for (unsigned int i=0; i<7; ++i) for (unsigned int j=0; j<7; ++j) {
        CRightHandedSet rhs;
        rhs.SetUp(C3DVector(axes[i][0],axes[i][1],axes[i][2]));
        rhs.SetForward(C3DVector(axes[j][0],axes[j][1],axes[j][2]));
        for (unsigned int k=0; k<6; ++k) for (unsigned int c=0; c<7; ++c)
        for (unsigned int v=0; v<7; ++v) {
            C3DVector offset(axes[c][0],axes[c][1],axes[c][2]);
            C3DVector world_pos(axes[v][0],axes[v][1],axes[v][2]);
            CMatrix3x4 matrix(MATRIX34_INIT_ZERO);
            CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix(matrix,offset,world_pos,rhs,scales[k]);
            const float entries[12]={matrix.E11,matrix.E12,matrix.E13,matrix.E21,matrix.E22,matrix.E23,
                                      matrix.E31,matrix.E32,matrix.E33,matrix.E41,matrix.E42,matrix.E43};
            for (unsigned int axis=0; axis<3; ++axis) {
                unsigned int b=(axis+1)%3, d=(axis+2)%3;
                const float right=(axes[j][b]*axes[i][d]-axes[j][d]*axes[i][b])*scales[k];
                const float forward=axes[j][axis]*scales[k], up=axes[i][axis]*scales[k];
                verify(entries[axis],-right);
                verify(entries[3+axis],-forward);
                verify(entries[6+axis],up);
                verify(entries[9+axis],axes[v][axis]+axes[c][0]*right+axes[c][1]*forward-axes[c][2]*up);
            }
        }
    }
    std::puts("00beb6c0_TEST PASS");
    return 0;
}