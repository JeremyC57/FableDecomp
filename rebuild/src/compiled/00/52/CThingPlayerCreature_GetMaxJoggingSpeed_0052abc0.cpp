// CThingPlayerCreature::GetMaxJoggingSpeed(C3DVector const&) const -- returns MaxJoggingSpeed (the direction is unused)
// Leaf: ?GetMaxJoggingSpeed@CThingPlayerCreature@@UBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052ABC0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 80 of CThingPlayerCreature's vtable 0x012457FC (the slot holds this body in 1 vtable(s), owner CThingPlayerCreature); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts GetMaxJoggingSpeed(C3DVector const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMaxJoggingSpeed@CThingPlayerCreature@@UBEMABVC3DVector@@@Z; `ret` matches the signature.
// Was: CThingPlayerCreature::vfunc_80 (rebuild/src/compiled/00/52/gapscan_sub0052abc0_0052abc0.cpp).
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS2

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingPlayerCreature.h"
float CThingPlayerCreature::GetMaxJoggingSpeed(const C3DVector&) const
{
    return MaxJoggingSpeed;
}
