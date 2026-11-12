// CTCCreatureModeManager::CModeBase::GetAnimFilter() const -- returns the file-static CAnimFilterNoGroup (the default filter: no animation group)
// Leaf: ?GetAnimFilter@CModeBase@CTCCreatureModeManager@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739E50, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeHoverBase's vtable 0x01287D14 (the slot holds this body in 9 vtable(s), owner CTCCreatureModeManager::CModeBase); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeBase@CTCCreatureModeManager@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: CTCCreatureModeManager::CModeBase::vfunc_4 (rebuild/src/compiled/00/73/gapscan_sub00739e50_00739e50.cpp).
// The object at 0x01382438 is a file-static of the retail TU (its atexit pair $E1 0x0121ED80 / $E2 0x01229A10 is landed in
// global_dynamicatexitdestructorforsNoGroupAnimFilter0138_01229a10.cpp, where its spelling is s_NoGroupAnimFilter_01382438;
// the original name is in no PDB). Declared extern here so this row reaches the same object.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CANIMFILTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterNoGroup.h"
extern CAnimFilterNoGroup s_NoGroupAnimFilter_01382438;                  // 0x01382438
const CAnimFilterBase& CTCCreatureModeManager::CModeBase::GetAnimFilter() const
{
    return s_NoGroupAnimFilter_01382438;
}
