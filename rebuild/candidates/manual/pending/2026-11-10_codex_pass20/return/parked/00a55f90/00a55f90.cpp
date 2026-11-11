// 00a55f90 CMatrix3x4::SetFromVectorAndAngle
// Grade: DIFFER(259v259) -- xy/xz commutative loads reversed; terminal three zero stores have different interleaving with final x87 subtract/store/pop
// Retail: 00a55f90,259 bytes.
// Flags: none
// Leaf: ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
#include "engine/GFCos.h"
#include "engine/GFSin.h"
#ifdef _MSC_VER
#pragma intrinsic(sin, cos)
#endif
#include "engine/CMatrix3x4.h"

void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& normal, float angle)
{
    float cos = (float)GFCos(angle);
    float sin = (float)GFSin(angle);
    float vx2 = normal.X * normal.X;
    float vy2 = normal.Y * normal.Y;
    float vz2 = normal.Z * normal.Z;
    float vxvy = normal.X * normal.Y;
    float vxvz = normal.X * normal.Z;
    float vyvz = normal.Y * normal.Z;
    float stvx = sin * normal.X;
    float stvy = sin * normal.Y;
    float stvz = sin * normal.Z;
    E11 = vx2 + cos - vx2 * cos;
    E21 = vxvy - vxvy * cos + stvz;
    E31 = vxvz - vxvz * cos - stvy;
    E12 = vxvy - vxvy * cos - stvz;
    E22 = vy2 + cos - vy2 * cos;
    E32 = vyvz - vyvz * cos + stvx;
    E13 = vxvz - vxvz * cos + stvy;
    E23 = vyvz - vyvz * cos - stvx;
    E33 = vz2 + cos - vz2 * cos;
    E41 = 0.0f;
    E42 = 0.0f;
    E43 = 0.0f;
}
