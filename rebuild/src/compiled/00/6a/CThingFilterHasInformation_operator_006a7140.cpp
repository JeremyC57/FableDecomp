// CThingFilter_HasInformation::operator()(const CThingAICreature& creature) const -- the creature's HasInformation flag
// Leaf: ??RCThingFilter_HasInformation@@QBE_NABVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A7140, 12 bytes, `ret 4`: `mov ecx,[esp+4]` (the creature), `call 0x008300D0`
// (CThingAICreature::GetHasInformation, FableWin.pdb ?GetHasInformation@CThingAICreature@@QBE_NXZ; landed binders 0x00862780 bind it).
// Identity (NF42, round 5): Ego_r.pdb 0x005A2B70 / FableWin.pdb 0x01F937E0 ??RCThingFilter_HasInformation@@QBE_NABVCThingAICreature@@@Z;
// engine/CThingFilter_HasInformation.h declares it at this address. Was the placeholder `DummyAllocator::Cons_val_pair_morph_a`
// (a wrong reading of the same bytes as a constructor forwarder); now on the shared headers (the CThingAICreature opt-in set of the
// landed 0x0090D030 plus FABLE_RETAIL_CTHINGAICREATURE_INFORMATION).
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_INFORMATION
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingFilter_HasInformation.h"

bool CThingFilter_HasInformation::operator()(const CThingAICreature& creature) const
{
    return creature.GetHasInformation();
}
