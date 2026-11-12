// CThingPlayerCreature::DrawIsDisplayingInformation() const -- returns false
// Leaf: ?DrawIsDisplayingInformation@CThingPlayerCreature@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A9560, thiscall, `ret`.
// Identity: slot 33 of CThingPlayerCreature's vtable 0x012457FC (defined here: the slot holds this body in 1 vtable(s), all of CThingPlayerCreature or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts DrawIsDisplayingInformation(void) at this slot; Ego_r/FableWin name set: ?DrawIsDisplayingInformation@CThingPlayerCreature@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingPlayerCreature.h (opt-ins as in the landed row CTCEnemy_AreThingsEnemies_0076d2f0.cpp).
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ThingAlive.h"
bool CThingPlayerCreature::DrawIsDisplayingInformation() const
{
    return false;
}
