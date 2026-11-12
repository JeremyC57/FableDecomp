// CThingObject::CanBlockNavigatingCreature() const -- the CThingPhysical answer (tail call to CThingPhysical::CanBlockNavigatingCreature 0x006A4D90)
// Leaf: ?CanBlockNavigatingCreature@CThingObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702CC0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 55 of CThingObject's vtable 0x01264DCC (the slot holds this body in 1 vtable(s), owner CThingObject); the Xbox 360 Test PDB virtual order (third source) aligned on 30 TLC-named slots of that vtable puts CanBlockNavigatingCreature(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CanBlockNavigatingCreature@CThingObject@@UBE_NXZ; no `ret` to check (NORET).
// Was: CThingObject::vfunc_55 (rebuild/src/compiled/00/70/gapscan_sub00702cc0_00702cc0.cpp).
#define FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS2
#define FABLE_RETAIL_CTHINGOBJECT_VSLOTS2
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingObject.h"
bool CThingObject::CanBlockNavigatingCreature() const
{
    return CThingPhysical::CanBlockNavigatingCreature();
}
