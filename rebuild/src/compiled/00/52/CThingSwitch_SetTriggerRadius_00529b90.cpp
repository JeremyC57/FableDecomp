// CThingSwitch::SetTriggerRadius(float) -- sets TriggerRadius
// Leaf: ?SetTriggerRadius@CThingSwitch@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529B90, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 63 of CThingSwitch's vtable 0x012453FC (the slot holds this body in 1 vtable(s), owner CThingSwitch); the Xbox 360 Test PDB virtual order (third source) aligned on 28 TLC-named slots of that vtable puts SetTriggerRadius(float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetTriggerRadius@CThingSwitch@@UAEXM@Z; `ret` matches the signature.
// Was: CThingSwitch::vfunc_63 (rebuild/src/compiled/00/52/gapscan_sub00529b90_00529b90.cpp).
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
void CThingSwitch::SetTriggerRadius(float radius)
{
    TriggerRadius = radius;
}
