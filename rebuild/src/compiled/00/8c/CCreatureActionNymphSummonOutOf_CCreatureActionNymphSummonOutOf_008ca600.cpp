// CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase& creature) -- combat action with an unnamed sound
//   pair (priority 150), animation "SUMMON_CREATURES_OUTOF"
// Leaf: ??0CCreatureAction_NymphSummonOutOf@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA600, length 116 bytes, `ret 4`. Vtable 0x01282354.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0.
// Shape: the sound name is a NULL pointer converted implicitly (`push -1 ; push 0`), the hidden-pointer CSoundPair
//   feeds the base constructor, the string dies after it; then a scoped named CCharString for the animation (its
//   constructor pushes are scheduled above the vtable store, GOTCHAS family wave) and the setter call.
// Identity: the stored vtable 0x01282354 is CCreatureAction_NymphSummonOutOf (rtti-vtables.tsv). No member of its own is touched.
// Names (agentNF255, 2026-11-22, q12721 / CODEXP27 L74; NF250's draft): was ??0CCreatureAction_NymphSummonOutOf@@QAE@PAVCThingCreatureBase@@@Z
//   from row-local models. Ego_r.pdb 0x00901D60 and FableWin.pdb 0x02A2D4F0 both spell ??0CCreatureAction_NymphSummonOutOf@@QAE@AAVCThingCreatureBase@@@Z
//   (`ret 4`: one argument); the landed caller 0x0091C6A0 already binds it. Now on engine/CCreatureAction_NymphSummonOutOf.h
//   (FABLE_RETAIL_NYMPH_ATTACK_PASS27, the caller's opt-in) and engine/CCombatActionBase.h.
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

CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 150, GetSoundPair(creature, (const char*)0))
{
    {
        CCharString name("SUMMON_CREATURES_OUTOF", -1);
        SetCombatAnimation(name);
    }
}
