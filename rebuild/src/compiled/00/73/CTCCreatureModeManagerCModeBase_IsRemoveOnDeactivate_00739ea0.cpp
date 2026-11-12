// CTCCreatureModeManager::CModeBase::IsRemoveOnDeactivate() const -- the base mode answers false
// Leaf: ?IsRemoveOnDeactivate@CModeBase@CTCCreatureModeManager@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739EA0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 11 of NCreatureMode::CModeHoverBase's vtable 0x01287D14 (the slot holds this body in 98 vtable(s), owner CTCCreatureModeManager::CModeBase); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsRemoveOnDeactivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsRemoveOnDeactivate@CModeBase@CTCCreatureModeManager@@UBE_NXZ; `ret` matches the signature.
// Was: CTCCreatureModeManager::CModeBase::vfunc_11 (rebuild/src/compiled/00/73/gapscan_sub00739ea0_00739ea0.cpp).
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
bool CTCCreatureModeManager::CModeBase::IsRemoveOnDeactivate() const
{
    return false;
}
