#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
// Leaf: ?InitialiseFromMatrix@C3DKeyframe@@QAE_NABVCMatrix3x4@@@Z
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

#include <math.h>
#include <stdio.h>

static bool close_value(float a, float b) { return fabs(a-b)<0.0001; }
static const CMatrix3x4* expected_input;
static bool equals_receiver_correct;

bool CMatrix3x4::Equals(const CMatrix3x4& other) const
{
    equals_receiver_correct = &other == expected_input;
    return close_value(E11, other.E11) && close_value(E12, other.E12) && close_value(E13, other.E13)
        && close_value(E21, other.E21) && close_value(E22, other.E22) && close_value(E23, other.E23)
        && close_value(E31, other.E31) && close_value(E32, other.E32) && close_value(E33, other.E33)
        && close_value(E41, other.E41) && close_value(E42, other.E42) && close_value(E43, other.E43);
}

static bool unit_rotation_input;
void CQuaternion::SetFromRotationMatrix(const CMatrix3x4& matrix)
{
    unit_rotation_input = close_value(matrix.E11*matrix.E11 + matrix.E12*matrix.E12,1)
        && close_value(matrix.E21*matrix.E21 + matrix.E22*matrix.E22,1)
        && close_value(matrix.E33,1) && close_value(matrix.E41,0)
        && close_value(matrix.E42,0) && close_value(matrix.E43,0);
    const double angle=atan2(matrix.E21,matrix.E11);
    X=Y=0;
    Z=static_cast<float>(sin(angle*0.5));
    W=static_cast<float>(cos(angle*0.5));
}

int main()
{
    const float scales[] = {0.25f, 1, 3.5f};
    const float angles[] = {0, 0.35f, -0.9f, 1.57f, 3.1f};
    CMatrix3x4 matrix;
    C3DKeyframe key;
    expected_input = &matrix;
    int cases=0;
    for (unsigned i=0; i<5; ++i) {
        for (unsigned x=0; x<3; ++x) for (unsigned y=0; y<3; ++y)
        for (unsigned z=0; z<3; ++z) for (int sign=-1; sign<=1; sign+=2) {
            float sx=scales[x], sy=scales[y], sz=sign*scales[z];
            matrix.E11=sx*static_cast<float>(cos(angles[i]));
            matrix.E12=-sx*static_cast<float>(sin(angles[i]));
            matrix.E13=0;
            matrix.E21=sy*static_cast<float>(sin(angles[i]));
            matrix.E22=sy*static_cast<float>(cos(angles[i]));
            matrix.E23=0;
            matrix.E31=matrix.E32=0;
            matrix.E33=sz;
            matrix.SetTranslateOnly(12.5f,-3,19);
            equals_receiver_correct=unit_rotation_input=false;
            if (!key.InitialiseFromMatrix(matrix) || !unit_rotation_input || !equals_receiver_correct) return 1;
            if (!close_value(key.Scaling.X,sx) || !close_value(key.Scaling.Y,sy)
                || !close_value(key.Scaling.Z,sz) || key.Scaling.W!=0) return 2;
            if (key.Position.X!=12.5f || key.Position.Y!=-3 || key.Position.Z!=19 || key.Position.W!=0) return 3;
            ++cases;
        }
    }
    matrix.E12+=0.7f;
    if (key.InitialiseFromMatrix(matrix) || !equals_receiver_correct || cases!=270) return 4;
    puts("00aaf1e0_TEST PASS");
    return 0;
}