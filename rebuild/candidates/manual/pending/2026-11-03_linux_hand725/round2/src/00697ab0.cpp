// CGameCameraManager::GetCameraModeType() const -- the type of the camera mode in use: CurrentMode (+0x98)->GetModeType(), NCameraModes::CBase vtable slot 2
// Leaf: ?GetCameraModeType@CGameCameraManager@@QBE?AW4EMode@NCameraModes@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697AB0, 11 bytes: `mov ecx,[ecx+98h] ; mov eax,[ecx] ; jmp dword ptr [eax+8]` (a sibling call
//   through the counted pointer's inline operator->).
// Identity: FableWin.pdb ?GetCameraModeType@CGameCameraManager@@QBE?AW4EMode@NCameraModes@@XZ. FableWin orders the TU
//   GetCameraModeType, HasCameraMode, .., IsUsingScriptedCameraMode -- retail 0x00697AB0, 0x00697AC0 (landed HasCameraMode, the
//   same CurrentMode slot-2 call compared with its argument), 0x00697AE0 (landed). Slot 2 is GetModeType (21 FableWin overrides
//   ?GetModeType@<Mode>@NCameraModes@@UBE?AW4EMode@2@XZ; CProjectileWeapon's 0x0087B5F0 returns 0x0F). Caller:
//   CThingPlayerCreature::ControlledMove 0x006AD77A (compares the result with 8). The catalog label "CScriptThing::operator==" is
//   propagated across the byte-twin shape and is wrong.
// Header: engine/CGameCameraManager.h FABLE_RETAIL_CGAMECAMERAMANAGER (CurrentMode as the real CCountedPointer) and the new
//   engine/NCameraModes_CBase.h (overlay hdr/).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

NCameraModes::EMode CGameCameraManager::GetCameraModeType() const
{
    return CurrentMode->GetModeType();
}
