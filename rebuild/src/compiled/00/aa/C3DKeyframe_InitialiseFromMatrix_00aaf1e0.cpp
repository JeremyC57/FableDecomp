// 00aaf1e0
// Leaf: ?InitialiseFromMatrix@C3DKeyframe@@QAE_NABVCMatrix3x4@@@Z
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_KEYFRAME_MATRIX_ROWS_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_MULTIPLYNOTRANSLATION
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
#define FABLE_RETAIL_C3DKEYFRAME_FROMMATRIX
#define FABLE_RETAIL_C3DKEYFRAME_SETPOSITION
#define FABLE_RETAIL_C3DKEYFRAME_TOMATRIX_INLINE
#define FABLE_RETAIL_CQUATERNION_TOMATRIX_FORCEINLINE
#include "engine/C3DKeyframe.h"
#ifdef _DEBUG
void FABLE_FASTCALL GFOutputDebugString(const char*);
#endif
bool C3DKeyframe::InitialiseFromMatrix(const CMatrix3x4& src)
{
    CMatrix3x4 matrix = src;
    SetPosition(matrix.GetTranslation());
    C3DVector xunit = matrix.GetRowX();
    C3DVector yunit = matrix.GetRowY();
    C3DVector zunit = matrix.GetRowZ();
    float xscale = xunit.GetMagnitude();
    float yscale = yunit.GetMagnitude();
    float zscale = zunit.GetMagnitude();
    if ((xunit ^ yunit).Dot(zunit) < 0.0f)
        zscale = -zscale;
    SetScaling(C3DVector(xscale, yscale, zscale));
    matrix.PreScale(1.0f/xscale, 1.0f/yscale, 1.0f/zscale);
    matrix.SetTranslateOnly(0, 0, 0);
#ifdef _DEBUG
    xunit = matrix.GetRowX();
    yunit = matrix.GetRowY();
    zunit = matrix.GetRowZ();
    ((xunit^yunit)-zunit).IsNonZero();
    if ((xunit^yunit).Dot(zunit)<0.0f) {
        GFOutputDebugString("FLIPPED!");
        GFOutputDebugString("\n");
    }
#endif
    Rotation.SetFromRotationMatrix(matrix);
    bool status = true;
    CMatrix3x4 fromkf;
    ToMatrix(fromkf, true);
    if (!fromkf.Equals(src))
        status = false;
    return status;
}