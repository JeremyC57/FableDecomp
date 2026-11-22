// CCreatureAction_NymphSummonOutOf::FinishAction() -- turn the creature's physics off, then the base CCombatActionBase::FinishAction
// Leaf: ?FinishAction@CCreatureAction_NymphSummonOutOf@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9E40, 32 bytes: PCreature.GetPItem() (0x00A01B50), its PhysicsTC (+0x60), virtual slot 36
//   (+0x90) EnablePhysics(false), then a tail `jmp` to CCombatActionBase::FinishAction 0x00859A20.
// Identity: retail vftable 0x01282354 slot 4 (its only reference; CCombatActionBase's slot 4 is 0x00859A20). CTCPhysicsBase slot 36 is
//   EnablePhysics(bool) (engine/CTCPhysicsBase.h; the landed 0x006B10B0 row), CThing +0x60 PhysicsTC (engine/CThing.h, GetPhysicsTC()).
// Names (agentNF255, 2026-11-22, q12721, with its ctor 0x008CA600): own leaf now the PDB ?FinishAction@CCreatureAction_NymphSummonOutOf@@UAEXXZ
//   (Ego_r.pdb 0x009014A0 + FableWin.pdb 0x02A2D6F0: public virtual), was the free __fastcall stand-in
//   ?CCreatureAction_NymphSummonOutOf_FinishAction@@YIXPAVCCreatureAction_NymphSummonOutOf@@@Z over row-local Target / Inner models (36
//   placeholder virtuals) and the free stand-in base_FinishAction_859a20; now the protected ?FinishAction@CCombatActionBase@@MAEXXZ.
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

void CCreatureAction_NymphSummonOutOf::FinishAction()
{
    PCreature->GetPhysicsTC()->EnablePhysics(false);
    CCombatActionBase::FinishAction();
}
