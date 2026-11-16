// 00b202d0 NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformPosToMaintainShape
// Flags: none
// Leaf: ?TransformPosToMaintainShape@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@AAEXAAVC3DVector@@ABV4@111@Z
// Identity: both original PDB signatures and original FableWin02f5ed10 call graph.
// Original parameter and local names recovered from both module records.
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_SQUARE_GROUPS_PASS23
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#define FABLE_RETAIL_CMATRIX3X4_PRETRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#include "engine/CPSCSplineBase.h"
#include "engine/CMatrix3x4.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
void NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformPosToMaintainShape(
    C3DVector& dest,const C3DVector& old_start_pos,const C3DVector& new_start_pos,
    const C3DVector& old_end_pos,const C3DVector& new_end_pos)
{
    C3DVector new_start_end_vector=new_end_pos-new_start_pos;
    C3DVector old_start_end_vector=old_end_pos-old_start_pos;
    const float new_start_end_vector_mag=new_start_end_vector.GetAccurateMagnitude();
    const float old_start_end_vector_mag=old_start_end_vector.GetAccurateMagnitude();
    const float scale_factor=new_start_end_vector_mag/old_start_end_vector_mag;
    new_start_end_vector*=1.0f/new_start_end_vector_mag;
    old_start_end_vector*=1.0f/old_start_end_vector_mag;
    CMatrix3x4 transform(MATRIX34_INIT_IDENTITY);
    C3DVector normal=new_start_end_vector.Cross(old_start_end_vector);
    if(normal.IsNonZero()) {
        normal.AccurateNormalise();
        transform.SetFromVectorAndAngle(normal,(float)acos(new_start_end_vector.Dot(old_start_end_vector))*(1.0f/(2.0f*3.14159265f)));
    }
    transform.PreScale(scale_factor);
    transform.PreTranslate(-old_start_pos);
    transform.PostTranslate(new_start_pos);
    transform.TransformPoint(dest);
}