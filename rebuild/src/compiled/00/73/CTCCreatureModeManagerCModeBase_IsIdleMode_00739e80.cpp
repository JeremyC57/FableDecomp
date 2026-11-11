// CTCCreatureModeManager::CModeBase::IsIdleMode() const -- the base mode answers false
// Leaf: ?IsIdleMode@CModeBase@CTCCreatureModeManager@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739E80, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 8 of NCreatureMode::CModeSitTavernStool's vtable 0x01292FF8 (the slot holds this body in 98 vtable(s), owner CTCCreatureModeManager::CModeBase); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsIdleMode(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsIdleMode@CModeBase@CTCCreatureModeManager@@UBE_NXZ; `ret` matches the signature.
// Was: CTCCreatureModeManager::CModeBase::vfunc_8 (rebuild/src/compiled/00/73/gapscan_sub00739e80_00739e80.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
bool CTCCreatureModeManager::CModeBase::IsIdleMode() const
{
    return false;
}
