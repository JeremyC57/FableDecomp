// IS_THING_ALIVE<CThingObject*>(CThingObject* const& thing) -- true when the pointer is set and the thing's dead bit (+0x91 bit 0) is clear
// Leaf: ??$IS_THING_ALIVE@PAVCThingObject@@@@YI_NABQAVCThingObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654AF5 (__fastcall, the reference in ecx).
// Port (NF40, round 4): was the extern "C" FableIsAlive_654af5 stand-in over a local CThingBuilding view; now the explicit
//   instantiation of engine/ThingAlive.h's IS_THING_ALIVE<T> (FABLE_RETAIL_IS_THING_ALIVE) over engine/CThingObject.h (CThing::IsDead).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/ThingAlive.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CThingObject*>(CThingObject* const& thing);
