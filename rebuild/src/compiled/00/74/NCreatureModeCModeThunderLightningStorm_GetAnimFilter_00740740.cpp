// NCreatureMode::CModeThunderLightningStorm::GetAnimFilter() const -- returns the mode's own animation filter (Filter)
// Leaf: ?GetAnimFilter@CModeThunderLightningStorm@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740740, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeThunderLightningStorm's vtable 0x01267DF4 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeThunderLightningStorm); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeThunderLightningStorm@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeThunderLightningStorm::vfunc_4 (rebuild/src/compiled/00/74/gapscan_sub00740740_00740740.cpp).
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
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"
const CAnimFilterBase& NCreatureMode::CModeThunderLightningStorm::GetAnimFilter() const
{
    return Filter;
}
