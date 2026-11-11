// NCameraModes::CBase::IsFOVChange() const -- returns true
// Leaf: ?IsFOVChange@CBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E190, thiscall, `ret`.
// Identity: slot 10 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (defined here: the slot holds this body in 17 vtable(s), all of NCameraModes::CBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsFOVChange(void) at this slot; Ego_r/FableWin name set: ?IsFOVChange@CBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CBase.h (opt-ins as in the landed row CGameCameraManager_GetCameraModeType_00697ab0.cpp).
#define FABLE_RETAIL_NCAMERAMODES_CBASE_VSLOTS
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"
bool NCameraModes::CBase::IsFOVChange() const
{
    return true;
}
