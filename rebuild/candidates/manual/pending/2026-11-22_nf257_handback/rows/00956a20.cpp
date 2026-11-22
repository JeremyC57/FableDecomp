// CCombatAction_WillHealInto::Clone() const -- new CCombatAction_WillHealInto(*this) with the implicit copy constructor inlined
//   (base copy + SpellLevel at +0x114)
// Leaf: ?Clone@CCombatAction_WillHealInto@@EBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956A20, oracle length 59 bytes.
//
// Retail bytes:  push esi ; push edi ; push 0x118 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; mov dword ptr [esi],offset CCombatAction_WillHealInto::`vftable' ; mov eax,[edi+0x114] ; mov [esi+0x114],eax ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds one long (SpellLevel, +0x114) to its base: the implicit copy constructor is inlined as the base
// copy constructor @ 0x0062F3B0, this class's vtable store (0x012948AC) and the member copy. sizeof = 0x118.
// Names (agentNF257, 2026-11-22, q12723 / CODEXP27 L100, with its constructor 0x00956870): own leaf now the PDB
//   ?Clone@CCombatAction_WillHealInto@@EBEPAVCCreatureActionBase@@XZ (Ego_r.pdb 0x0097B570 + FableWin.pdb 0x02BF9AD0: private virtual
//   const, returns CCreatureActionBase*; retail vftable 0x012948AC slot 11, its only reference), was the non-virtual stand-in
//   ?Clone@CCombatAction_WillHealInto@@QAEPAUCBase_0062f3b0@@XZ on row-local models; the base copy constructor is
//   ??0CCombatActionBase@@QAE@ABV0@@Z (MELEE_COPYCTOR). The 0x00956960 (CCreatureAction_ThrowShotObject::Clone) recipe.
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

CCreatureActionBase* CCombatAction_WillHealInto::Clone() const
{
    return new CCombatAction_WillHealInto(*this);
}
