// CCreatureAction_NymphSummonOutOf::Clone() const -- new CCreatureAction_NymphSummonOutOf(*this) with the implicit copy constructor inlined
// Leaf: ?Clone@CCreatureAction_NymphSummonOutOf@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CAA70, oracle length 47 bytes.
//
// Retail bytes:  push esi ; push edi ; push 0x114 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_NymphSummonOutOf::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is inlined as the base copy constructor
// @ 0x0062F3B0 plus this class's vtable store (0x01282354). sizeof = 0x114.
// Names (agentNF255, 2026-11-22, q12721, with its ctor 0x008CA600): own leaf now the PDB
//   ?Clone@CCreatureAction_NymphSummonOutOf@@UBEPAVCCreatureActionBase@@XZ (Ego_r.pdb 0x00902060 + FableWin.pdb 0x02A2D5D0: public virtual
//   const, returns CCreatureActionBase*; retail vftable 0x01282354 slot 11), was the non-virtual stand-in
//   ?Clone@CCreatureAction_NymphSummonOutOf@@QAEPAVCCombatActionBase@@XZ on row-local models; the base copy constructor is
//   ??0CCombatActionBase@@QAE@ABV0@@Z (MELEE_COPYCTOR). The 0x00956960 (CCreatureAction_ThrowShotObject::Clone) recipe.
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

CCreatureActionBase* CCreatureAction_NymphSummonOutOf::Clone() const
{
    return new CCreatureAction_NymphSummonOutOf(*this);
}
