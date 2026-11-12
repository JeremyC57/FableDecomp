// CThingBuilding::GetProperties() const -- returns 0
// Leaf: ?GetProperties@CThingBuilding@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D0B0, thiscall, `ret`.
// Identity: slot 48 of CThingBuilding's vtable 0x0124509C (defined here: the slot holds this body in 1 vtable(s), all of CThingBuilding or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 31 TLC-named slots of that vtable puts GetProperties(void) at this slot; Ego_r/FableWin name set: ?GetProperties@CThingBuilding@@UBEKXZ; `ret` matches the signature.
// Header: engine/CThingBuilding.h (opt-ins as in the landed row CThingManager_InitThingTypes_00522a20.cpp).
#define FABLE_RETAIL_CTHINGBUILDING_VSLOTS
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
unsigned long CThingBuilding::GetProperties() const
{
    return 0;
}
