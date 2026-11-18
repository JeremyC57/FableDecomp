// CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle) -- build an axis-angle rotation and zero the translation
// Fidelity: the axis reference preserves the x87 operand order at 0x00A55FC9 and 0x00A55FD2.
// Leaf: ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A55F90, 259 bytes, thiscall, `ret 8`.
// The normal and angle parameters and the scalar product locals follow Ego_d.
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#include "engine/CMatrix3x4.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"

void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle)
{
    float cos = GFCos(angle);
    float sin = GFSin(angle);
    const C3DVector& axis = normal;
    float vx2 = axis.X * axis.X;
    float vy2 = axis.Y * axis.Y;
    float vz2 = axis.Z * axis.Z;
    float vxvy = axis.X * axis.Y;
    float vxvz = axis.X * axis.Z;
    float vyvz = axis.Y * axis.Z;
    float stvx = sin * axis.X;
    float stvy = sin * axis.Y;
    float stvz = sin * axis.Z;
    E11 = (vx2 + cos) - vx2 * cos;
    E21 = vxvy - vxvy * cos + stvz;
    E31 = vxvz - vxvz * cos - stvy;
    E12 = vxvy - vxvy * cos - stvz;
    E22 = (vy2 + cos) - vy2 * cos;
    E32 = vyvz - vyvz * cos + stvx;
    E13 = vxvz - vxvz * cos + stvy;
    E23 = vyvz - vyvz * cos - stvx;
    E33 = vz2 + cos - vz2 * cos;
    E41 = 0.0f;
    E42 = 0.0f;
    E43 = 0.0f;
}