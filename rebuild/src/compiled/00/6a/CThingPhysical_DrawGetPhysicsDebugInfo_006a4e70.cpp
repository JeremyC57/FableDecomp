// CThingPhysical::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const -- fills the debug info from the physics component (CTCPhysicsBase::DrawGetPhysicsDebugInfo, slot 88) and returns true
// Leaf: ?DrawGetPhysicsDebugInfo@CThingPhysical@@UBE_NAAVCThingPhysicalDebugInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A4E70, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 35 of CThingTrackNode's vtable 0x01272CE4 (the slot holds this body in 14 vtable(s), owner CThingPhysical); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawGetPhysicsDebugInfo@CThingPhysical@@UBE_NAAVCThingPhysicalDebugInfo@@@Z; `ret` matches the signature.
// Was: CThingPhysical::vfunc_35 (rebuild/src/compiled/00/6a/CThingPhysical_vfunc35_006a4e70.cpp).
#define FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS2
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
bool CThingPhysical::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo& info) const
{
    PhysicsTC->DrawGetPhysicsDebugInfo(info);
    return true;
}
