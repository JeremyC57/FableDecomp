#ifndef FABLE_ENGINE_CMATRIX3X4_H
#define FABLE_ENGINE_CMATRIX3X4_H
// CMatrix3x4 (Ego_r TPI: a union whose struct arm is float E11..E43, rows of three, size 0x30; struct_layouts_egor.tsv
// CMatrix3x4_u_0_s_0). The flat members below are that struct arm (identical layout). TransformPoint overloads: FableWin
// ?TransformPoint@CMatrix3x4@@QBEXAAVC3DVector@@@Z (0x024543E0) and ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z
// (0x01D129F0), inline in retail; the bodies (in-place copy to xx/yy/zz locals, X as one expression, Y / Z accumulated) are the
// forms 0x00AE0070 ApplyTransform expands (Codex pass 12, FableWin locals xx/yy/zz).
#include "engine/C3DVector.h"
class CMatrix3x4 {
public:
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
    CMatrix3x4() { }
#ifdef FABLE_RETAIL_CMATRIX3X4
    // Retail opt-in (chunk 723b, the water passes): Ego_r.pdb ??0CMatrix3x4@@QAE@MMMMMMMMMMMM@Z, ?SetIdentity@CMatrix3x4@@QAEXXZ and
    // ?SetTranslateOnly@CMatrix3x4@@QAEXABVC3DVector@@@Z, inline everywhere. SetIdentity's store order is retail's (0x00B707E9: the
    // nine zeros from E43 down to E12, then the ones E33, E22, E11): two chained assignments.
    CMatrix3x4(float e11, float e12, float e13, float e21, float e22, float e23,
               float e31, float e32, float e33, float e41, float e42, float e43)
        : E11(e11), E12(e12), E13(e13), E21(e21), E22(e22), E23(e23),
          E31(e31), E32(e32), E33(e33), E41(e41), E42(e42), E43(e43) { }
    void SetIdentity()
    {
        E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f;
        E11 = E22 = E33 = 1.0f;
    }
    void SetTranslateOnly(const C3DVector& translation)
    {
        E41 = translation.X;
        E42 = translation.Y;
        E43 = translation.Z;
    }
#endif
    void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E11*xx+(E21*yy+E31*zz)+E41;
        point.Y=E12*xx;
        point.Y+=E32*zz;
        point.Y+=E22*yy;
        point.Y+=E42;
        point.Z=E13*xx;
        point.Z+=E33*zz;
        point.Z+=E23*yy;
        point.Z+=E43;
    }
    void TransformPoint(const C3DVector& pSource,C3DVector& destination) const {
        destination.X=E11*pSource.X+(E21*pSource.Y+E31*pSource.Z)+E41;
        destination.Y=E12*pSource.X;
        destination.Y+=E32*pSource.Z;
        destination.Y+=E22*pSource.Y;
        destination.Y+=E42;
        destination.Z=E13*pSource.X;
        destination.Z+=E33*pSource.Z;
        destination.Z+=E23*pSource.Y;
        destination.Z+=E43;
    }
};
#endif
