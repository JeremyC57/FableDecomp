#ifndef FABLE_ENGINE_CMATRIX4X4_H
#define FABLE_ENGINE_CMATRIX4X4_H
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_SHADER_LIGHTING
#include "engine/C4DVector.h"
#endif
// Original Ego_r TPI0xa231: all16 named float entries overlap public Data[4][4].
// Native0098acf0 stores four diagonal ones and12 off-diagonal zeroes in each used matrix.
class CMatrix4x4 {
public:
    union {
        struct { float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44; };
        float Data[4][4];
    };
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    float SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4&);
    __forceinline void TransformTransposedAssumeW(const C4DVector& src,C4DVector& dest,float w) const {
        C4DVector vect=src;
        dest.X=E11*vect.X+E12*vect.Y+E13*vect.Z+E14*w;
        dest.Y=E21*vect.X+(E22*vect.Y+E23*vect.Z)+E24*w;
        dest.Z=E31*vect.X+E32*vect.Y+E33*vect.Z+E34*w;
        dest.W=w;
    }
    void TransformTransposedAssumeW(C4DVector& vect,float w) const;

#endif
    void SetIdentity() {
        E11=1.0f; E22=1.0f; E33=1.0f; E44=1.0f;
        E12=0.0f; E13=0.0f; E14=0.0f; E21=0.0f; E23=0.0f; E24=0.0f; E31=0.0f; E32=0.0f; E34=0.0f; E41=0.0f; E42=0.0f; E43=0.0f;
    }
};
FABLE_STATIC_ASSERT(sizeof(CMatrix4x4)==64);
#endif
