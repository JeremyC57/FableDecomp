// CThingSwitch::GetTriggerRadius() const -- returns TriggerRadius
// Leaf: ?GetTriggerRadius@CThingSwitch@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529B80, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 62 of CThingSwitch's vtable 0x012453FC (the slot holds this body in 1 vtable(s), owner CThingSwitch); the Xbox 360 Test PDB virtual order (third source) aligned on 28 TLC-named slots of that vtable puts GetTriggerRadius(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetTriggerRadius@CThingSwitch@@UBEMXZ; `ret` matches the signature.
// Was: CThingSwitch::vfunc_62 (rebuild/src/compiled/00/52/gapscan_sub00529b80_00529b80.cpp).
#define FABLE_RETAIL_CTHINGSWITCH_VSLOTS2
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
#include "engine/CThingSwitch.h"
float CThingSwitch::GetTriggerRadius() const
{
    return TriggerRadius;
}
