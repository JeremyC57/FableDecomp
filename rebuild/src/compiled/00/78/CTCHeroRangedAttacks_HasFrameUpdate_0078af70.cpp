// CTCHeroRangedAttacks::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroRangedAttacks@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078AF70, thiscall, `ret`.
// Identity: slot 8 of CTCHeroRangedAttacks's vtable 0x0126BAF4 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroRangedAttacks or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroRangedAttacks@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroRangedAttacks.h (opt-ins as in the landed row CTCHeroRangedAttacks_FireRangedWeapon_0078a390.cpp).
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/NCreatureMode.h"
#include "engine/ThingAlive.h"
bool CTCHeroRangedAttacks::HasFrameUpdate() const
{
    return true;
}
