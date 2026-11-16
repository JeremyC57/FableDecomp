// CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase& creature) -- base CCombatActionBase(creature, 0, a zeroed
// CSoundPair), own vtable (0x0127B1B4), WeaponType (+0x114) = UNSHEATHE_ALL_WEAPONS (2)
// Leaf: ??0CCreatureAction_UnsheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8590, length 64 bytes, `ret 4`.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030.
// Shape: the creature is loaded into edx before the frame; the priority 0 shares the zero register that also builds the sound
// pair (inline constructor, register-zeroed stack stores); the member is stored after the vtable.
// NF39 (2026-11-16): moved off its local CCombatActionBase / CSoundPair / class models (pointer argument) onto the shared
//   engine/CCreatureAction_UnsheatheWeapons.h (opt-in FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS, which names 0x008B8590),
//   engine/CCombatActionBase.h (MELEE + SOUNDCTOR: ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z 0x00858030),
//   engine/CSoundPair.h and engine/EUnsheatheWeaponType.h (Ego_r.pdb enumerators: 2 = UNSHEATHE_ALL_WEAPONS). Evidence: Ego_r.pdb + FableWin.pdb
//   ??0CCreatureAction_UnsheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z (one reference argument, `ret 4`); the landed callers already bind it.
// Defines: the landed 0x00929340 BanditPlayDice::Update preamble (the class header's chain) + SOUNDCTOR.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_BanditPlayDice.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"

CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 0, CSoundPair())
    , WeaponType(UNSHEATHE_ALL_WEAPONS)
{
}
