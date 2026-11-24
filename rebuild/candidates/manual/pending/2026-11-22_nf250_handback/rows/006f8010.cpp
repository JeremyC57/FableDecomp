// CCreatureAction_CombatIdle::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_CombatIdle expanded,
//   then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_GCCreatureAction_CombatIdle@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F8010, 30 bytes. Identity: slot 0 of vtable 0x012632B4, RTTI .?AVCCreatureAction_CombatIdle@@ (manifest rtti-vtables.tsv; the
//   catalog name agrees).
// Callees: CCombatActionBase::~CCombatActionBase (the class adds no member), operator delete 0x00BFE9BC. Model: the shared
//   engine/CCreatureAction_CombatIdle.h (the MAZE branch with its native overrides, as 0x00955620
//   builds it).
// Names (agentNF250, 2026-11-22, CODEXP27 L50): was a free __fastcall stand-in (no Leaf line) over a row-local model; now the member the
//   compiler emits for the shared header class, so the leaf is the ??_G spelling (no landed code referencer: the address is reached only
//   through the vtable slot).
// Prelude: the #define / #include block of the landed 0x00955620 (its object emits this class's vtable at HEAD), so this row's vtable
//   COMDAT has the same slots as that copy (vtcheck.py).
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
#define FABLE_RETAIL_CMAZEBATTLEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCBOSS_GETPHASESTARTPOS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_COMBATIDLE_MAZE
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ECombatZone.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCAICreatureWillPowerIndicator.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatAction_WillShieldInto.h"
#include "engine/CCombatAction_WillHealInto.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include "engine/CCreatureAction_CombatIdle.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable and its scalar deleting destructor (the
// row's leaf). Internal to this TU (anonymous namespace, so no two rows define the same harness symbol); never called.
CCreatureActionBase* EmitCombatIdleDeletingDestructor(CThingCreatureBase& creature, CThingCreatureBase& target)
{
    return new CCreatureAction_CombatIdle(creature, target);
}
}
