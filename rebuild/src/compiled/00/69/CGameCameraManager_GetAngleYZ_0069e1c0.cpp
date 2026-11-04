// CGameCameraManager::GetAngleYZ() const -- the current game camera's YZ angle (virtual; forwards to the camera member)
// Leaf: ?GetAngleYZ@CGameCameraManager@@UBEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E1C0, 8 bytes: `add ecx,4 ; jmp 0x00A0C3E0` (Camera.GetAngleYZ(), tail call).
// Identity (two sources): slot 26 of CGameCameraManager's vtable 0x0125C754 (rtti-vtables.tsv; the catalog names it GetAngleYZ), and
//   Ego_r.pdb / FableWin.pdb ?GetAngleYZ@CGameCameraManager@@UBEMXZ (Ego_r 0x006C36F0, FableWin 0x021FC9A0; GetAngleXY then GetAngleYZ, as retail
//   0x0069E1B0 then 0x0069E1C0); the jump target 0x00A0C3E0 is ?GetAngleYZ@CCamera@@QBEMXZ and Camera is the CCamera at +0x04
//   (engine/CGameCameraManager.h).
// Callees: ?GetAngleYZ@CCamera@@QBEMXZ 0x00A0C3E0.
// Header: engine/CGameCameraManager.h (FABLE_RETAIL_CGAMECAMERAMANAGER + FABLE_RETAIL_CGAMECAMERAMANAGER_ANGLES), engine/CCamera.h
//   (FABLE_RETAIL_CCAMERA_ANGLES).
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_ANGLES
#define FABLE_RETAIL_CCAMERA_ANGLES
#include "engine/CGameCameraManager.h"
float CGameCameraManager::GetAngleYZ() const
{
    return Camera.GetAngleYZ();
}