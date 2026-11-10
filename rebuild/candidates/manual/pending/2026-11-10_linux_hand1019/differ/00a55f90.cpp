// CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle) -- make this the rotation by `angle` (a fraction of a full turn) about the unit `axis`, with a zero translation
// Grade: DIFFER(259v259) -- (agent1019 regrade, unchanged) the xy / xz products load [X] first where retail loads [Y] / [Z] first (yz matches: [Z] first), and the three translation-zero stores sit 2 instructions early. c2prio: the Y and Z addresses are CSE'd integer temps (#19 axis+4, #21 axis+8) while X is addressed straight off the axis candidate, which is where the X-first order comes from. 20 free probes inert: swapped operands, parenthesised / compound-assignment products, a float* view, GFSqr squares, x/y/z locals (263 bytes), products first (253), SetTranslateOnly (temporary, scoped member-wise local, first statement), chained zero store, parenthesised E33 / E23.
// Leaf: ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A55F90, 259 bytes, thiscall, `ret 8`.
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin, cos)
#endif
#include "engine/CMatrix3x4.h"

void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle)
{
    float a = (float)(angle * 6.2831854820251465);
    float c = (float)cos(a);
    float s = (float)sin(a);
    float xx = axis.X * axis.X;
    float yy = axis.Y * axis.Y;
    float zz = axis.Z * axis.Z;
    float xy = axis.X * axis.Y;
    float xz = axis.X * axis.Z;
    float yz = axis.Y * axis.Z;
    float sx = s * axis.X;
    float sy = s * axis.Y;
    float sz = s * axis.Z;
    E11 = xx + c - xx * c;
    E21 = xy - xy * c + sz;
    E31 = xz - xz * c - sy;
    E12 = xy - xy * c - sz;
    E22 = yy + c - yy * c;
    E32 = yz - yz * c + sx;
    E13 = xz - xz * c + sy;
    E23 = yz - yz * c - sx;
    E33 = zz + c - zz * c;
    E41 = 0.0f;
    E42 = 0.0f;
    E43 = 0.0f;
}
