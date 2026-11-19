// CGameCameraManager::IsPosInViewingFrustrum(const C3DVector& pos) const -- whether pos is inside the current camera mode's view cone: the camera tools' frustum test from the mode's position towards its observed position with the camera's field of view
// Leaf: ?IsPosInViewingFrustrum@CGameCameraManager@@UBE_NABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697F50, length 48 bytes, virtual const thiscall `ret 4`.
// Callees: CCamera::GetFOV() @ 0x00A0BEB0 (`fld [ecx+2Ch]` on the Camera at +4; sweep3 RENAMES, Ego_r/FableWin CCamera getter order),
//   NCameraModes::CTools::IsPosInViewingFrustrum(pos, look_at, camera, fov) const @ 0x00875120 (Ego_r.pdb 0x0085B8F0 / FableWin.pdb
//   0x029C2520 ?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z) on CamTools +0x78.
// Layout: engine/CGameCameraManager.h (CamTools +0x78, CurrentMode +0x98); the mode's inline GetObservedPos (LookAt.Position +0x10) and
//   GetPosition (GoTo.Position +0x54) are read in place, as in the sibling Is2DAreaInViewingFrustrum 0x00697F80.
// Shape: the float pushed through `push ecx ; fstp [esp]`, the mode in edi, the bool returned in al. Was a local
//   CGameCameraManager / CTools_00697f50 / SViewRecord_00697f50 model (q11337 moved it onto the shared headers).
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_FRUSTRUM_POS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDPOS_INLINE
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CBase.h"

bool CGameCameraManager::IsPosInViewingFrustrum(const C3DVector& pos) const
{
    return CamTools.IsPosInViewingFrustrum(pos, CurrentMode->GetObservedPos(), CurrentMode->GetPosition(), Camera.GetFOV());
}
