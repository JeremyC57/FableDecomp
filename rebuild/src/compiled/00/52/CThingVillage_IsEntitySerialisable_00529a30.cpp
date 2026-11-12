// CThingVillage::IsEntitySerialisable() const -- returns true
// Leaf: ?IsEntitySerialisable@CThingVillage@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529A30, thiscall, `ret`.
// Identity: slot 22 of CThingVillage's vtable 0x012451BC (defined here: the slot holds this body in 1 vtable(s), all of CThingVillage or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsEntitySerialisable(void) at this slot; Ego_r/FableWin name set: ?IsEntitySerialisable@CThingVillage@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingVillage.h (opt-ins as in the landed row CThingManager_InitThingTypes_00522a20.cpp).
#define FABLE_RETAIL_CTHINGVILLAGE_VSLOTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGMARKER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCHARSTRING
#include "rebuild_abi.h"
#include "engine/CThingManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingVillage.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingObject.h"
#include "engine/CThingSoundEmitter.h"
#include "engine/CThingHolySite.h"
#include "engine/CThingSwitch.h"
#include "engine/CThingTrackNode.h"
#include "engine/CThingMarker.h"
#include "engine/CThingPhysicalSwitch.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CThingNoise.h"
#include "engine/CThing.h"
bool CThingVillage::IsEntitySerialisable() const
{
    return true;
}
