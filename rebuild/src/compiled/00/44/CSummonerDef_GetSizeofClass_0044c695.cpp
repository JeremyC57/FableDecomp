// Leaf: ?GetSizeofClass@CSummonerDef@@UAEKXZ
#define FABLE_RETAIL_CSUMMONERDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSummonerDef.h"
#pragma optimize("s",on)
typedef unsigned long ulong;

unsigned long CSummonerDef::GetSizeofClass()
{
    return 0x5c;
}
// Flatten route (flatten_pilot v5, NF330): CSummonerDef_GetSizeofClass -> CSummonerDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CSummonerDef.h opt-in FABLE_RETAIL_CSUMMONERDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
