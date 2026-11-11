// NCameraModes::CBase::OnCameraInsideObservedThing() -- does nothing
// Leaf: ?OnCameraInsideObservedThing@CBase@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E150, thiscall, `ret`.
// Identity: slot 3 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (defined here: the slot holds this body in 18 vtable(s), all of NCameraModes::CBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts OnCameraInsideObservedThing(void) at this slot; Ego_r/FableWin name set: ?OnCameraInsideObservedThing@CBase@NCameraModes@@UAEXXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CBase.h (opt-ins as in the landed row CGameCameraManager_GetCameraModeType_00697ab0.cpp).
#define FABLE_RETAIL_NCAMERAMODES_CBASE_VSLOTS
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"
void NCameraModes::CBase::OnCameraInsideObservedThing()
{
}
