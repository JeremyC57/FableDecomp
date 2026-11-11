// NCameraModes::CBase::IsFinished() const -- returns false
// Leaf: ?IsFinished@CBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E1B0, thiscall, `ret`.
// Identity: slot 13 of NCameraModes::CPCMouseLook's vtable 0x01277784 (defined here: the slot holds this body in 17 vtable(s), all of NCameraModes::CBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts IsFinished(void) at this slot; Ego_r/FableWin name set: ?IsFinished@CBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CBase.h (opt-ins as in the landed row CGameCameraManager_GetCameraModeType_00697ab0.cpp).
#define FABLE_RETAIL_NCAMERAMODES_CBASE_VSLOTS
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"
bool NCameraModes::CBase::IsFinished() const
{
    return false;
}
