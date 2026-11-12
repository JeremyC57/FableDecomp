// NCameraModes::CBase::IsTrackMeleeOpponentsAutoFOVChange() const -- returns the camera manager's AutoFOVZoomInCombat
// Leaf: ?IsTrackMeleeOpponentsAutoFOVChange@CBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084CDE0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 12 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (the slot holds this body in 19 vtable(s), owner NCameraModes::CBase); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsTrackMeleeOpponentsAutoFOVChange(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsTrackMeleeOpponentsAutoFOVChange@CBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_12 (rebuild/src/compiled/00/84/unattributed_vfunc12_0084cde0.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#include "engine/CGameCameraManager.h"
namespace NCameraModes {
bool CBase::IsTrackMeleeOpponentsAutoFOVChange() const
{
    return GetCameraManager().AutoFOVZoomInCombat;
}
}
