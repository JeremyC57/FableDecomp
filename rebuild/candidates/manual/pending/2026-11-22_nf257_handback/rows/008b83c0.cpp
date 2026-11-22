// CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures() -- base CCombatActionBase(150), own vtable (0x0127ADDC), SpawnOnTarget cleared
// Leaf: ??0CCreatureAction_SummonCreatures@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B83C0, length 32 bytes. Vtable 0x0127ADDC.
// Callees: CCombatActionBase::CCombatActionBase(long) @ 0x00857E40 (protected).
// Shape: the default-constructor shell of the landed family (0x008C9D20) plus the byte member cleared after the vtable store.
// Names (agentNF257, 2026-11-22, q12722 / CODEXP27 L72): own leaf unchanged (Ego_r.pdb 0x008F1140 + FableWin.pdb 0x029FF160
//   ??0CCreatureAction_SummonCreatures@@QAE@XZ; the landed caller 0x006F5C10 binds it); now on engine/CCreatureAction_SummonCreatures.h
//   (SpawnOnTarget +0x114, size 0x118) and engine/CCombatActionBase.h, off the row-local CCombatActionBase / class models. The base
//   constructor is the PDB ??0CCombatActionBase@@IAE@J@Z (Ego_r 0x00838AC0 + FableWin 0x02823960, protected; the 0x006F5C10 spelling), was the
//   local public QAE stand-in.
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

CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures()
    : CCombatActionBase(150)
    , SpawnOnTarget(false)
{
}
