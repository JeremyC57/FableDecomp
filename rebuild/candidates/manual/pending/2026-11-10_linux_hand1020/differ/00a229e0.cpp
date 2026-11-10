// CRightHandedSet::CRightHandedSet(const C3DAngles& angles) -- build the forward vector from the XY/YZ angles, rotate the matching up vector about forward by the XZ angle (an inline axis-angle rotation matrix), then fast-normalise both
// Grade: DIFFER(691v689) -- on shared headers (1 grade), unchanged from the local-model draft: frame 0x50 v 0x4c (retail homes up.X in the dead angles argument slot one word lower) and the x87 order of the inline axis-angle block (retail evaluates s*Forward.Z before Forward.Y*Forward.X - ..*c). A precomputed-terms SetFromVectorAndAngle spelling + TransformVector probe is worse (743).
// Leaf: ??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A229E0, 689 bytes, thiscall `ret 4`.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/C3DAngles.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#ifdef _MSC_VER
#pragma intrinsic(sin, cos)
#endif

CRightHandedSet::CRightHandedSet(const C3DAngles& angles)
{
    float sinYZ = (float)sin(angles.AngleYZ * (double)6.2831855f);
    float cosYZ = (float)cos(angles.AngleYZ * (double)6.2831855f);
    float sinXY = (float)sin(angles.AngleXY * (double)6.2831855f);
    float cosXY = (float)cos(angles.AngleXY * (double)6.2831855f);
    float s = (float)sin(angles.AngleXZ * (double)6.2831855f);
    float c = (float)cos(angles.AngleXZ * (double)6.2831855f);
    Forward.X = sinXY * cosYZ;
    Forward.Y = cosXY * cosYZ;
    Forward.Z = sinYZ;
    CMatrix3x4 rotation;
    rotation.E11 = Forward.X * Forward.X + c - Forward.X * Forward.X * c;
    rotation.E21 = Forward.Y * Forward.X - Forward.Y * Forward.X * c + s * Forward.Z;
    rotation.E31 = Forward.Z * Forward.X - Forward.Z * Forward.X * c - s * Forward.Y;
    rotation.E12 = Forward.Y * Forward.X - Forward.Y * Forward.X * c - s * Forward.Z;
    rotation.E22 = Forward.Y * Forward.Y + c - Forward.Y * Forward.Y * c;
    rotation.E32 = Forward.Z * Forward.Y - Forward.Z * Forward.Y * c + s * Forward.X;
    rotation.E13 = Forward.Z * Forward.X - Forward.Z * Forward.X * c + s * Forward.Y;
    rotation.E23 = Forward.Z * Forward.Y - Forward.Z * Forward.Y * c - s * Forward.X;
    rotation.E33 = Forward.Z * Forward.Z + c - Forward.Z * Forward.Z * c;
    rotation.E41 = 0.0f;
    rotation.E42 = 0.0f;
    rotation.E43 = 0.0f;
    C3DVector up(-sinXY * sinYZ, -cosXY * sinYZ, cosYZ);
    Up.X = up.X * rotation.E11 + up.Y * rotation.E21 + up.Z * rotation.E31;
    Up.Y = up.X * rotation.E12 + up.Y * rotation.E22 + up.Z * rotation.E32;
    Up.Z = up.X * rotation.E13 + up.Y * rotation.E23 + up.Z * rotation.E33;
    Forward.Normalise();
    Up.Normalise();
}
