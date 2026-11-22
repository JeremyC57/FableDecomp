// CCombatAction_WillHealInto::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (a direct call to
//   ~CCombatActionBase 0x0062E900, no own vtable store), then free the object when bit 0 is set; returns this
// Leaf: ??_GCCombatAction_WillHealInto@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956940, 30 bytes, thiscall, `ret 4`; vftable 0x012948AC slot 0 (RTTI .?AVCCombatAction_WillHealInto@@),
//   its only reference.
// Name (agentNF257, 2026-11-22, q12723, with its constructor 0x00956870): the member the compiler emits for the shared header class
//   (FableWin.pdb 0x02BF9BB0 ??_E is its vtable alias); was the free __fastcall stand-in CCombatAction_WillHealInto_scalar_deleting_destructor
//   (no Leaf line) on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, the 0x009568E0 recipe): the deleting destructor is a compiler-emitted COMDAT
//   that VC7.1 emits with the vtable. The class's constructor is out of line (row 0x00956870), so the anonymous-namespace helper below
//   expands the implicit copy constructor instead (its vtable store); this object then emits ??_7 and ??_G pick-any, as the other two
//   rows of the class do (the same prelude text). Internal to this TU; never called.
// Vtable: this object emits ??_7CCombatAction_WillHealInto@@6B@ (81 slots, retail 0x012948AC). The prelude is the CCombatActionBase
//   MELEE branch with the override opt-ins of the landed 0x00955620 (MAZE_DEFENSIVE_MELEE_PASS27, FRAMEUPDATE, INITIALISE, FINISHACTION,
//   ONANIMATIONLOADED, MELEE_OWN_VIRTUALS) plus the class's own overrides (WILLHEALINTO_OVERRIDES: FinishAction, Initialise, Clone), so each
//   slot names the retail owner of its target (0 / 4 / 11 / 12 this class). The same prelude text is in the three rows of the class that
//   emit the table (constructor 0x00956870, Clone 0x00956A20, deleting destructor 0x00956940), so the COMDAT copies are one.
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
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTION_WILLHEALINTO_OVERRIDES
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
#include "engine/CCombatAction_WillHealInto.h"
#include <new>

namespace {
void ReferenceWillHealIntoVtable(void* where, const CCombatAction_WillHealInto& other)
{
    new (where) CCombatAction_WillHealInto(other);
}
}
