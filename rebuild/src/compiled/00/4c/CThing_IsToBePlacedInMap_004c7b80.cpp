// CThing::IsToBePlacedInMap() const -- placed in the map when it has a base def, is included in thing scans (activated, alive, not in limbo) and the def is InMapSearches
// Leaf: ?IsToBePlacedInMap@CThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7B80, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 17 of CThingNoise's vtable 0x01272EE4 (the slot holds this body in 17 vtable(s), owner CThing); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts IsToBePlacedInMap(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsToBePlacedInMap@CThing@@UBE_NXZ; `ret` matches the signature.
// Was: CThing::vfunc_17 (rebuild/src/compiled/00/4c/CThing_vfunc17_004c7b80.cpp).
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "engine/CThingBaseDef.h"
bool CThing::IsToBePlacedInMap() const
{
    const CThingBaseDef* def = PBaseDef.Get();
    if (!def)
        return false;
    return IncludeInThingScans && Activated && IsAlive() && !InLimbo && def->InMapSearches;
}
