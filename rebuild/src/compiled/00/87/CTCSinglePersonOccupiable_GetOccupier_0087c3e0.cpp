// CTCSinglePersonOccupiable::GetOccupier() const -- the creature currently occupying this single-person occupiable (bed, seat)
// Leaf: ?GetOccupier@CTCSinglePersonOccupiable@@QBEPAVCThingCreatureBase@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C3E0, 8 bytes: `add ecx,0Ch ; jmp 0x00A01B50` (POccupier.Get(), tail call).
// Identity (two sources): (1) Ego_r.pdb ?GetOccupier@CTCSinglePersonOccupiable@@QBEPAVCThingCreatureBase@@XZ, in donor order between
//   SetOccupier and IsOccupied exactly as retail places it (SetOccupier 0x0087C390 stores into +0x0C, IsOccupied 0x0087C3F0, IsOfType
//   0x0087C420), with POccupier the Ego_r member at +0x0C; (2) both callers (CAIStateGroup_GoToBed::OnReplace 0x0092C84B,
//   CAIStateGroup_SleepOnFloor::OnReplace 0x00969B37) fetch the thing's TC of interface type 172 (TCI_SINGLE_PERSON_OCCUPIABLE), compare
//   this result with the brain's creature and then call SetOccupier(0). The catalog's "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50 (through CIntelligentPointer<CThingCreatureBase>::Get()).
// Header: engine/CTCSinglePersonOccupiable.h (new), engine/CThingCreatureBase.h (the retail chain, so Get()'s cast is a no-op).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSinglePersonOccupiable.h"
CThingCreatureBase* CTCSinglePersonOccupiable::GetOccupier() const
{
    return POccupier.Get();
}