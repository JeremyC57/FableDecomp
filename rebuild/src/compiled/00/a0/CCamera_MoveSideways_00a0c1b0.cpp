// CCamera::MoveSideways(float world_units) -- strafe the camera along its right axis (Forward x Up); with HeightLocked only X and Y move
// Leaf: ?MoveSideways@CCamera@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C1B0, 152 bytes, thiscall `ret 4`, no callees.
// Identity: Ego_r.pdb / FableWin.pdb ?MoveSideways@CCamera@@QAEXM@Z (lib_camera.obj #4 in FableWin, #11 in Ego_r); parameter `world_units`
//   (Ego_d, const float). FableWin 0x030F5780: `if (HeightLocked) { X += RHSet.GetRight().X * world_units; Y += ...GetRight().Y * ...; }
//   else WorldPosition += RHSet.GetRight() * world_units;` (GetRight 0x01B409F0 = Forward ^ Up, operator* (float), operator+=).
// Bracket: the shared opt-in FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22 (operator^ with all three components bracketed, the donor's
//   formal `vector`) is byte-required: it sets the x87 operand order of the free arm's cross products (0x00A0C1C0..0x00A0C210, the hoisted
//   F.Y*U.Z at 0x00A0C1B3). Measured: CROSS_XYZ_GROUP (same brackets) also MATCHes; the unbracketed default, CROSS_ORIGINAL_FORMAL,
//   CROSS_NAMED_RESULT, CROSS_X_GROUP and both CRightHandedSet RIGHT_COMPONENTS spellings give DIFFER(158v152).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCAMERA_MOVESIDEWAYS
#include "engine/CCamera.h"

void CCamera::MoveSideways(const float world_units)
{
    if (HeightLocked) {
        WorldPosition.X += RHSet.GetRight().X * world_units;
        WorldPosition.Y += RHSet.GetRight().Y * world_units;
    } else {
        WorldPosition += RHSet.GetRight() * world_units;
    }
}