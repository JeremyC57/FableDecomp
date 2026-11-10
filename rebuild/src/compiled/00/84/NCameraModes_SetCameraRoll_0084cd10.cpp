// NCameraModes::SetCameraRoll(CCamera&, float) -- rebuilds the camera's up vector for its current view direction (the perpendicular with z = 1, normalised), rolls it about the view direction by the angle, and re-applies the view
// Leaf: ?SetCameraRoll@NCameraModes@@YIXAAVCCamera@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084CD10, 194 bytes, __fastcall (ecx = camera, the roll angle on the stack), `ret 4`.
// Identity: Ego_r.pdb ?SetCameraRoll@NCameraModes@@YIXAAVCCamera@@M@Z; callers 0x0084DF75 and 0x0087BB77.
// Callees: C3DVector::AccurateNormalise() 0x00A14440 (in place), C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60,
//   CCamera::SetView(const C3DVector& forward, const C3DVector& up) 0x00A0BE30 (Ego_r ?SetView@CCamera@@QAEXABVC3DVector@@0@Z,
//   declared by the new opt-in FABLE_RETAIL_CCAMERA_SETVIEW of engine/CCamera.h, hdr/README.md).
// Layout: engine/CCamera.h (Ego_r CCamera: RHSet +0x0C, Forward +0x18) read through the inline PeekViewDirection(); engine/C3DVector.h.
// Shape: forward copied into a local (copy ctor), up = (?, ?, 1); the horizontal components solve dot(up, forward) = 0 by the larger
//   horizontal axis of forward (`fabs` compare, `test ah,5 ; jp`); forward.Z / len2 is computed once ahead of the branch; each arm
//   writes one component as -(k * axis) and derives the other by ratio. The squared horizontal length is written with its two
//   squares parenthesised: the two PAREN tuples move the scheduling so the forward.Z copy (`mov edx,[esi+20h]`) issues after the
//   first square as retail (0x0084CD28); without them VC7.1 loads it with X and Y.
#define FABLE_RETAIL_CCAMERA_SETVIEW
#include "engine/CCamera.h"

namespace NCameraModes {
void __fastcall SetCameraRoll(CCamera& camera, float roll)
{
    C3DVector forward(camera.PeekViewDirection());
    C3DVector up;
    up.Z = 1.0f;
    float len2 = (forward.X * forward.X) + (forward.Y * forward.Y);
    if (fabs(forward.X) > fabs(forward.Y)) {
        up.X = -(forward.Z / len2 * forward.X);
        up.Y = up.X / forward.X * forward.Y;
    } else {
        up.Y = -(forward.Z / len2 * forward.Y);
        up.X = up.Y / forward.Y * forward.X;
    }
    up.AccurateNormalise();
    up.Rotate(forward, roll);
    camera.SetView(forward, up);
}
}