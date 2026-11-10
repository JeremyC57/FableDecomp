// CCamera::SetAngleXY(float angle) -- set the camera's heading (in turns): rebuild the forward and up vectors from the new heading and their own current pitches, then normalise both
// Leaf: ?SetAngleXY@CCamera@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0C570, 371 bytes, thiscall `ret 4`. Caller: CPlayer::UpdateFreeCamera (0x00488602).
// Identity: Ego_r.pdb / FableWin.pdb ?SetAngleXY@CCamera@@QAEXM@Z (Ego_r 0x00B4D1B0); the CCamera TU follows the PDB member order
//   (GetAngleXY 0x00A0C380, GetAngleYZ 0x00A0C3E0, GetAngleXZ 0x00A0C450, SetAngleXY here, SetAngleYZ 0x00A0C6F0, SetAngleXZ 0x00A0C830).
// Layout: Ego_r.pdb CCamera (0x38): WorldPosition +0x00, RHSet +0x0C (Up +0x0C, Forward +0x18) -- engine/CCamera.h.
// Inline helpers (FableWin.pdb only, expanded in retail): C3DVector::GetAngleYZ (0x024712F0; atan2(Z, |XY|) in turns wrapped by
//   _CIfmod 0x00BFEB28 into [0, 1)), C3DVector::InitialiseFromAngles (0x01DCB530; the 1.0f length folds away), CRightHandedSet::Set
//   and CRightHandedSet::Normalise (0x02E881D0: Up then Forward through C3DVector::AccurateNormalise 0x00A14440).
// Shape: sin/cos of the shared heading are computed once and spilled as qwords (0x00A0C5F8, 0x00A0C608); the up vector's pitch lives
//   in the dead angle slot (0x00A0C644). Set(up, forward) keeps &RHSet in esi (0x00A0C5E3 `add esi,0Ch`) and forms &Forward late
//   (0x00A0C6AD `lea edx,[esi+0Ch]`); SetUp + SetForward CSEs this+0x18 into edi at entry instead.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_GETANGLEYZ
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_CRIGHTHANDEDSET_NORMALISE
#define FABLE_RETAIL_CCAMERA_SETANGLEXY
#include "engine/CCamera.h"

void CCamera::SetAngleXY(float angle)
{
    C3DVector forward;
    forward.InitialiseFromAngles(angle, RHSet.GetForward().GetAngleYZ(), 1.0f);
    C3DVector up;
    up.InitialiseFromAngles(angle, RHSet.GetUp().GetAngleYZ(), 1.0f);
    RHSet.Set(up, forward);
    RHSet.Normalise();
}