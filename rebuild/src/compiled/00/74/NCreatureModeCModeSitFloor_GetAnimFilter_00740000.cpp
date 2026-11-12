// NCreatureMode::CModeSitFloor::GetAnimFilter() const -- returns the mode's own animation filter (Filter, a CAnimFilterOfGroup, opaque in the shared header)
// Leaf: ?GetAnimFilter@CModeSitFloor@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740000, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeSitFloor's vtable 0x01267BBC (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeSitFloor); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeSitFloor@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeSitFloor::vfunc_4 (rebuild/src/compiled/00/74/gapscan_sub00740000_00740000.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITFLOOR_GETANIMFILTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
const CAnimFilterBase& NCreatureMode::CModeSitFloor::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(Filter);
}
