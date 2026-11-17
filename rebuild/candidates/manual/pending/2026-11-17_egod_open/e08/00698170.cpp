// CGameCameraManager::IsVolumeInViewingFrustrum(const C3DVector& origin, float radius) const -- true when the four points origin +- radius along two axes perpendicular to the view line all lie in the camera's viewing cone
// Grade: DIFFER(693v693) -- one scheduling slot at the first CTools::IsPosInViewingFrustrum call: retail issues `fld perp.X ; fmul radius` before `mov esi,[origin]` / `mov ebp,[fov]` (0x00698295..0x006982CF), ours loads esi one slot earlier
// Leaf: ?IsVolumeInViewingFrustrum@CGameCameraManager@@UBE_NABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00698170, 693 bytes (`ret 8`; virtual const).
// Callees: CCamera::GetFOV 0x00A0BEB0 (the draft's GetHorizontalFOV was the wrong name: 0x00A0BE90), C3DVector::AccurateNormalise 0x00A14440 (x2),
//   NCameraModes::CTools::IsPosInViewingFrustrum 0x00875120 (x4; new opt-in FABLE_RETAIL_NCAMERAMODES_CTOOLS_ISPOSINVIEWINGFRUSTRUM); DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Ego_d.pdb locals (all function scope): origin, radius, cam_pos, perp, one_line_in, other_perp, fov, other_line_in, line, cam_look_at.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ISPOSINVIEWINGFRUSTRUM
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKATPHASE_INLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCamera.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CBase.h"
#include <math.h>
extern const float DELTA_NOTIONAL_ZERO;

bool CGameCameraManager::IsVolumeInViewingFrustrum(const C3DVector& origin, float radius) const
{
    float fov = Camera.GetFOV();
    C3DVector cam_pos = CurrentMode->GetPosition();
    C3DVector cam_look_at = CurrentMode->GetLookAtPhase().Position;
    C3DVector line = cam_look_at - cam_pos;
    C3DVector perp;
    if (fabsf(line.Y) > DELTA_NOTIONAL_ZERO) {
        perp.X = 1.0f;
        perp.Z = 1.0f;
        perp.Y = -((line.X + line.Z) / line.Y);
    } else if (fabsf(line.Z) > DELTA_NOTIONAL_ZERO) {
        perp.X = 1.0f;
        perp.Y = 1.0f;
        perp.Z = -((line.X + line.Y) / line.Z);
    } else {
        return false;
    }
    perp.AccurateNormalise();
    C3DVector other_perp = perp ^ line;
    other_perp.AccurateNormalise();
    bool one_line_in = CamTools.IsPosInViewingFrustrum(origin + perp * radius, cam_look_at, cam_pos, fov) && CamTools.IsPosInViewingFrustrum(origin - perp * radius, cam_look_at, cam_pos, fov);
    bool other_line_in = CamTools.IsPosInViewingFrustrum(origin + other_perp * radius, cam_look_at, cam_pos, fov) && CamTools.IsPosInViewingFrustrum(origin - other_perp * radius, cam_look_at, cam_pos, fov);
    return one_line_in && other_line_in;
}
