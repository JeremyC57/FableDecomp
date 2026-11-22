// CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures(CThingCreatureBase& creature, CThingCreatureBase& target, bool spawn_on_target)
//   -- base CCombatActionBase(creature, target, 150, GetSoundPair(creature, null name)), own vtable (0x0127ADDC), SpawnOnTarget at +0x114
// Leaf: ??0CCreatureAction_SummonCreatures@@QAE@AAVCThingCreatureBase@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B83E0, length 96 bytes, `ret 0Ch`. Vtable 0x0127ADDC.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (null text, default length -1: an implicit conversion of the
// null pointer, re-addressed with `lea`), CCombatActionBase::GetSoundPair(const CThingCreatureBase&, const CCharString&) @ 0x008561A0
// (__fastcall, 8-byte class result through the hidden pointer in ecx), CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&,
// long, const CSoundPair&) @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed 10-body GetSoundPair constructor family (0x008C4F50) with a null name; the byte argument is read after the
// string temporary dies and stored after the vtable.
// Names (agentNF250 draft, agentNF257 2026-11-22, q12722 / CODEXP27 L72): was ??0CCreatureAction_SummonCreatures@@QAE@PAVCThingCreatureBase@@PAVCThing@@_N@Z
//   from row-local CCharString / CSoundPair / CCombatActionBase / class models. Ego_r.pdb 0x008F1160 and FableWin.pdb 0x029FF2E0 both spell
//   ??0CCreatureAction_SummonCreatures@@QAE@AAVCThingCreatureBase@@0_N@Z (two creature references and a bool; `ret 0Ch` = three
//   arguments, so not the one-argument (CThingCreatureBase&) sibling 0x008B8440); the landed caller 0x0091C6A0 already binds it. Now on the shared
//   engine/CCreatureAction_SummonCreatures.h (SpawnOnTarget, size 0x118) and engine/CCombatActionBase.h.
// Vtable: this object emits ??_7CCreatureAction_SummonCreatures@@6B@ (81 slots, retail 0x0127ADDC). The prelude is the CCombatActionBase
//   MELEE branch with the override opt-ins of the landed 0x00955620 (MAZE_DEFENSIVE_MELEE_PASS27, FRAMEUPDATE, INITIALISE, FINISHACTION,
//   ONANIMATIONLOADED, MELEE_OWN_VIRTUALS) plus MELEE_PRIORITYCTOR and the class's own declarations (SUMMONCREATURES_OVERRIDES: the default and
//   one-argument constructors, Clone, Initialise), so each slot names the retail owner of its target (0 / 11 / 12 this class). The same prelude
//   text is in the five rows of the class that emit the table (constructors 0x008B83C0 / 0x008B83E0 / 0x008B8440, Clone 0x008B89A0, deleting
//   destructor 0x008B87A0), so the COMDAT copies are one.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_PRIORITYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_SUMMONCREATURES_OVERRIDES
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_SummonCreatures.h"

CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures(CThingCreatureBase& creature, CThingCreatureBase& target, bool spawn_on_target)
    : CCombatActionBase(creature, target, 150, GetSoundPair(creature, (const char*)0))
    , SpawnOnTarget(spawn_on_target)
{
}
