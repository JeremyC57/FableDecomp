// NCreatureMode::CModeDrained::GetAnimFilter() const -- returns the mode's own animation filter (Filter)
// Leaf: ?GetAnimFilter@CModeDrained@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741470, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeDrained's vtable 0x01268234 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeDrained); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeDrained@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeDrained::vfunc_4 (rebuild/src/compiled/00/74/gapscan_sub00741470_00741470.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NCreatureMode_CModeDrained.h"
const CAnimFilterBase& NCreatureMode::CModeDrained::GetAnimFilter() const
{
    return Filter;
}
