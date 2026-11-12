// IS_THING_ALIVE<CThingBuilding*>(CThingBuilding* const& thing) -- true when the building pointer is set and the building is not dead
// Leaf: ??$IS_THING_ALIVE@PAVCThingBuilding@@@@YI_NABQAVCThingBuilding@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065D624, __fastcall (ecx = &pointer), tests p && !(p->+0x91 & 1) (CThing::Dead).
// Flags: none
// Name-fix pass (2026-11-12): Ego_r.pdb 0x0069F330 + FableWin.pdb 0x017A3A14 / 0x01B5E870 ??$IS_THING_ALIVE@PAVCThingBuilding@@@@YI_NABQAVCThingBuilding@@@Z;
//   retail 0x0065AA9A passes ecx = &temp holding the GetWorkBuilding result (`lea ecx,[ebp+8] ; call 0x0065D624`); the landed
//   CTCVillage::IsTimeToGoToTavern 0x0065AA6F calls it by this name. Was the made-up extern "C" FableIsAlive_65d624(Wrap*) (leaf
//   @FableIsAlive_65d624@4) on local CThingBuilding / Wrap stand-ins; now the explicit instantiation of the engine/ThingAlive.h template
//   (the landed IS_THING_ALIVE<CIntelligentPointer<CThingObject> > 0x0078B2A0 spelling) on engine/CThingBuilding.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CThingBuilding*>(CThingBuilding* const&);
