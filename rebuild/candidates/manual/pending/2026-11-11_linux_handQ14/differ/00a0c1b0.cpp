// CCamera::MoveSideways(float distance) -- strafe the camera along its right axis (Forward x Up); with HeightLocked only X and Y move.
// Grade: DIFFER(ours v retail) -- 158v152 on engine/CCamera.h (opt-in FABLE_RETAIL_CCAMERA_MOVESIDEWAYS) + CRightHandedSet::GetRight: locked arm and the add/scale tail match; the free arm's cross-product operand load order differs (retail loads F.Y*U.Z, U.Y*F.Z, F.Z*U.X, F.X*U.Z, U.Y*F.X, F.Y*U.X; ours U-first on all), so the common F.Y*U.Z is not hoisted above the HeightLocked test. Tried: named right local before the if (136 bytes, worse), named right in the free arm, FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS (parenthesised components).
// Leaf: ?MoveSideways@CCamera@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C1B0, length 152 bytes, __thiscall, no callees.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCAMERA_MOVESIDEWAYS
#include "engine/CCamera.h"

void CCamera::MoveSideways(float distance)
{
    if (HeightLocked) {
        WorldPosition.X += RHSet.GetRight().X * distance;
        WorldPosition.Y += RHSet.GetRight().Y * distance;
    } else {
        WorldPosition += RHSet.GetRight() * distance;
    }
}
