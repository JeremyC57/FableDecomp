// NCameraModes::CBase::IsShortenOnOcclusion() const -- returns false
// Leaf: ?IsShortenOnOcclusion@CBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E170, thiscall, `ret`.
// Identity: slot 6 of NCameraModes::CLookAtFront's vtable 0x01277714 (defined here: the slot holds this body in 10 vtable(s), all of NCameraModes::CBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CBase.h (opt-ins as in the landed row CGameCameraManager_GetCameraModeType_00697ab0.cpp).
#define FABLE_RETAIL_NCAMERAMODES_CBASE_VSLOTS
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"
bool NCameraModes::CBase::IsShortenOnOcclusion() const
{
    return false;
}
