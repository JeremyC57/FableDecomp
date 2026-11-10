// CRightHandedSet::GetAngleXZ() const -- signed roll about the forward axis, in turns.
// Leaf: ?GetAngleXZ@CRightHandedSet@@QBEMXZ
// Flags:
// Retail: 0x00a22ca0, 368 bytes, const member returning float.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_GETANGLEXZ
#include "engine/CRightHandedSet.h"
#ifdef _MSC_VER
#pragma intrinsic(atan2, fmod, sin, cos, acos)
#endif
float CRightHandedSet::GetAngleXZ() const
{
    C3DVector forward(GetForward().X, GetForward().Y, GetForward().Z);
    C3DVector right = GetRight();
    right.AccurateNormalise();
    C3DVector heading;
    heading.InitialiseFromAngles(forward.GetAngleXY(), 0.0f, 1.0f);
    C3DVector flatRight = heading.CrossWithUp();
    float sign = (float)(right.Z < 0.0f ? -1 : 1);
    flatRight.AccurateNormalise();
    float dot = flatRight.Dot(right);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915494f;
    return angle * sign;
}