// CRightHandedSet::CRightHandedSet(const C3DAngles& angles) -- forward from the XY/YZ angles, up rotated about forward by the XZ angle, both fast-normalised
// Grade: DIFFER(689v689) -- frame and every x87 op equal; only the first Normalise's `mov esi,17Ch` is hoisted to 0x00A22ADF (retail issues it after the table load, 0x00A22BB8)
// Leaf: ??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A229E0, 689 bytes, thiscall `ret 4`.
// Ego_d.pdb (4th record of the name, debug length 545 = FableWin 545b/182i): sin_yz cos_yz sin_xy cos_xy sin_xz cos_xz, vx2 vy2 vz2 vxvy vyvz vxvz,
// stvx stvy stvz, sin_up_yz cos_up_yz sin_up_xy cos_up_xy, rotation (all function scope). This text is 518b/173i at /Od (FableWin 545b/182i).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/C3DAngles.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#ifdef _MSC_VER
#pragma intrinsic(sin, cos)
#endif

CRightHandedSet::CRightHandedSet(const C3DAngles& angles)
{
    float sin_yz = GFSin(angles.AngleYZ);
    float cos_yz = GFCos(angles.AngleYZ);
    float sin_xy = GFSin(angles.AngleXY);
    float cos_xy = GFCos(angles.AngleXY);
    float sin_xz = GFSin(angles.AngleXZ);
    float cos_xz = GFCos(angles.AngleXZ);
    Forward.X = sin_xy * cos_yz;
    Forward.Y = cos_xy * cos_yz;
    Forward.Z = sin_yz;
    CMatrix3x4 rotation;
    float vx2 = Forward.X * Forward.X;
    rotation.E11 = vx2 + cos_xz - vx2 * cos_xz;
    float vxvy = Forward.X * Forward.Y;
    float stvz = sin_xz * Forward.Z;
    rotation.E21 = vxvy - vxvy * cos_xz + stvz;
    float vxvz = Forward.X * Forward.Z;
    float stvy = sin_xz * Forward.Y;
    rotation.E31 = vxvz - vxvz * cos_xz - stvy;
    float vyvz = Forward.Y * Forward.Z;
    rotation.E12 = vxvy - vxvy * cos_xz - stvz;
    float vy2 = Forward.Y * Forward.Y;
    rotation.E22 = vy2 + cos_xz - vy2 * cos_xz;
    float stvx = sin_xz * Forward.X;
    rotation.E32 = vyvz - vyvz * cos_xz + stvx;
    rotation.E13 = vxvz - vxvz * cos_xz + stvy;
    rotation.E23 = vyvz - vyvz * cos_xz - stvx;
    float vz2 = Forward.Z * Forward.Z;
    rotation.E33 = vz2 + cos_xz - vz2 * cos_xz;
    float sin_up_yz = cos_yz;
    float cos_up_yz = sin_yz;
    float sin_up_xy = -sin_xy;
    float cos_up_xy = -cos_xy;
    Up.X = sin_up_xy * cos_up_yz;
    Up.Y = cos_up_xy * cos_up_yz;
    Up.Z = sin_up_yz;
    rotation.TransformVector(Up);
    Forward.Normalise();
    Up.Normalise();
}
