// CCreatureAction_SummonCreatures::Clone() const -- new CCreatureAction_SummonCreatures(*this) with the implicit copy constructor inlined
//   (base copy + SpawnOnTarget at +0x114)
// Leaf: ?Clone@CCreatureAction_SummonCreatures@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B89A0, oracle length 59 bytes.
//
// Retail bytes:  push esi ; push edi ; push 0x118 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; mov dword ptr [esi],offset CCreatureAction_SummonCreatures::`vftable' ; mov al,[edi+0x114] ; mov [esi+0x114],al ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds one bool (SpawnOnTarget, +0x114) to its base: the implicit copy constructor is inlined as the base
// copy constructor @ 0x0062F3B0, this class's vtable store (0x0127ADDC) and the member copy. sizeof = 0x118.
// Names (agentNF257, 2026-11-22, q12722 / CODEXP27 L72, with its constructors): own leaf now the PDB
//   ?Clone@CCreatureAction_SummonCreatures@@UBEPAVCCreatureActionBase@@XZ (Ego_r.pdb 0x008F1520 + FableWin.pdb 0x029FF1A0: public virtual
//   const, returns CCreatureActionBase*; retail vftable 0x0127ADDC slot 11, its only reference), was the non-virtual stand-in
//   ?Clone@CCreatureAction_SummonCreatures@@QAEPAUCBase_0062f3b0@@XZ on row-local models; the base copy constructor is
//   ??0CCombatActionBase@@QAE@ABV0@@Z (MELEE_COPYCTOR). The 0x00956960 (CCreatureAction_ThrowShotObject::Clone) recipe.
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

CCreatureActionBase* CCreatureAction_SummonCreatures::Clone() const
{
    return new CCreatureAction_SummonCreatures(*this);
}
