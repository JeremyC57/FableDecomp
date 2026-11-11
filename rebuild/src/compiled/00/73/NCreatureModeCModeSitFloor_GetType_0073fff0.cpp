// NCreatureMode::CModeSitFloor::GetType() const -- returns MODE_SIT_FLOOR
// Leaf: ?GetType@CModeSitFloor@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FFF0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 1 of NCreatureMode::CModeSitFloor's vtable 0x01267BBC (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeSitFloor); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetType@CModeSitFloor@NCreatureMode@@UBEKXZ; `ret` matches the signature.
// Was: NCreatureMode::CModeSitFloor::vfunc_1 (rebuild/src/compiled/00/73/gapscan_sub0073fff0_0073fff0.cpp).
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
#include "engine/NCreatureMode.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
unsigned long NCreatureMode::CModeSitFloor::GetType() const
{
    return MODE_SIT_FLOOR;
}
