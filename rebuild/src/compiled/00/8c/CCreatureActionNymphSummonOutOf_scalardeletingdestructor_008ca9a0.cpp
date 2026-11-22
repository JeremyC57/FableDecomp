// CCreatureAction_NymphSummonOutOf::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (a direct call to
//   ~CCombatActionBase 0x0062E900, no own vtable store), then free the object when bit 0 is set; returns this
// Leaf: ??_GCCreatureAction_NymphSummonOutOf@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CA9A0, 30 bytes, thiscall, `ret 4`; vftable 0x01282354 slot 0 (RTTI .?AVCCreatureAction_NymphSummonOutOf@@),
//   its only reference.
// Name (agentNF255, 2026-11-22, q12721, with its ctor 0x008CA600): the member the compiler emits for the shared header class (FableWin.pdb
//   0x02A2D690 ??_E is its vtable alias); was the free __fastcall stand-in CCreatureAction_NymphSummonOutOf_scalar_deleting_destructor
//   (no Leaf line) on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, the 0x009568E0 recipe): the deleting destructor is a compiler-emitted COMDAT
//   that VC7.1 emits with the vtable. The class's constructor is out of line (row 0x008CA600), so the anonymous-namespace helper below
//   expands the implicit copy constructor instead (its vtable store); this object then emits ??_7 and ??_G pick-any, as the other three
//   rows of the class do (the same prelude text). Internal to this TU; never called.
// Vtable: this object emits ??_7CCreatureAction_NymphSummonOutOf@@6B@ (81 slots, retail 0x01282354). The prelude is the CCombatActionBase
//   MELEE branch with the override opt-ins of the landed 0x00955620 (MAZE_DEFENSIVE_MELEE_PASS27, FRAMEUPDATE, INITIALISE, FINISHACTION,
//   ONANIMATIONLOADED, MELEE_OWN_VIRTUALS) plus the class's own overrides (NYMPHSUMMONOUTOF_OVERRIDES: Clone, FinishAction), so each slot
//   names the retail owner of its target (0 / 4 / 11 this class). The same prelude text is in the four rows of the class (ctor 0x008CA600,
//   Clone 0x008CAA70, FinishAction 0x008C9E40, deleting destructor 0x008CA9A0), so the COMDAT copies are one.
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
#define FABLE_RETAIL_NYMPH_ATTACK_PASS27
#define FABLE_RETAIL_CCREATUREACTION_NYMPHSUMMONOUTOF_OVERRIDES
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
#include "engine/CCreatureAction_NymphSummonOutOf.h"
#include <new>

namespace {
void ReferenceNymphSummonOutOfVtable(void* where, const CCreatureAction_NymphSummonOutOf& other)
{
    new (where) CCreatureAction_NymphSummonOutOf(other);
}
}
