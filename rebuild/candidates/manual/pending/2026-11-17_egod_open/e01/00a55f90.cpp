// CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle) -- make this the rotation by `angle` (a fraction of a full turn) about the unit axis `normal`, with a zero translation
// Grade: DIFFER(259v259) -- x87 operand order of two products: vxvy / vxvz load [X] first (fld [eax] ; fmul [eax+4] / [eax+8]) where retail loads [Y] / [Z] first (0x00A55FC9 / 0x00A55FD2); all else identical
// Leaf: ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A55F90, 259 bytes, thiscall, `ret 8`.
// Locals: Ego_d.pdb names (normal, angle; cos, sin, vx2, vy2, vz2, vxvy, vxvz, vyvz, stvx, stvy, stvz).
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#include "engine/CMatrix3x4.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"

void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle)
{
    float cos = GFCos(angle);
    float sin = GFSin(angle);
    float vx2 = normal.X * normal.X;
    float vy2 = normal.Y * normal.Y;
    float vz2 = normal.Z * normal.Z;
    float vxvy = normal.X * normal.Y;
    float vxvz = normal.X * normal.Z;
    float vyvz = normal.Y * normal.Z;
    float stvx = sin * normal.X;
    float stvy = sin * normal.Y;
    float stvz = sin * normal.Z;
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
