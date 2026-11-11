// CThingCreatureBase::DrawUpdate(CInterpolationInfo const *) -- does nothing
// Leaf: ?DrawUpdate@CThingCreatureBase@@UAEXPBVCInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661A40, thiscall, `ret 4`.
// Identity: slot 26 of CThingAICreature's vtable 0x0127293C (defined here: the slot holds this body in 2 vtable(s), all of CThingCreatureBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts DrawUpdate(CInterpolationInfo const *) at this slot; Ego_r/FableWin name set: ?DrawUpdate@CThingCreatureBase@@UAEXPBVCInterpolationInfo@@@Z; `ret` matches the signature.
// Header: engine/CThingCreatureBase.h (opt-ins as in the landed row anonymousnamespace_GetBestAttackerForPlacementVector_006e8ce0.cpp).
class CInterpolationInfo;
#define FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
void CThingCreatureBase::DrawUpdate(CInterpolationInfo const *)
{
}
