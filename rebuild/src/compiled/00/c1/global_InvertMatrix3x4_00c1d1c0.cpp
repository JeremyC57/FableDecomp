// InvertMatrix3x4 -- invert an affine matrix with a signed determinant floor.
// Retail: 0x00C1D1C0, 347 bytes; fastcall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?InvertMatrix3x4@@YIXPAVCMatrix3x4@@ABV1@@Z
// Flags: none
#include <math.h>
#pragma intrinsic(fabs)
#include "engine/CMatrix3x4.h"
#include "engine/MatrixInverseEpsilon.h"
void FABLE_FASTCALL InvertMatrix3x4(CMatrix3x4* out, const CMatrix3x4& m)
{
    float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
    if (fabs(det) < g_fMatrixInverseEpsilon)
        det = (det < 0.0f) ? -g_fMatrixInverseEpsilon : g_fMatrixInverseEpsilon;
    float inv = 1.0f / det;
    out->E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
    out->E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
    out->E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    out->E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
    out->E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
    out->E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    const float& x=m.E41;
    out->E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
    out->E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
    out->E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    float t = m.E43 * out->E31;
    t += x * out->E11;
    t += m.E42 * out->E21;
    out->E41 = -t;
    t = m.E42 * out->E22;
    t += x * out->E12;
    t += out->E32 * m.E43;
    out->E42 = -t;
    t = m.E43 * out->E33;
    t += m.E42 * out->E23;
    t += out->E13 * x;
    out->E43 = -t;
}