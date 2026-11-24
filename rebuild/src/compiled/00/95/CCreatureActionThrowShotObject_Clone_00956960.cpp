// CCreatureAction_ThrowShotObject::Clone() const -- new CCreatureAction_ThrowShotObject(*this) with the implicit copy constructor inlined
// Leaf: ?Clone@CCreatureAction_ThrowShotObject@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956960, oracle length 47 bytes.
//
// Retail bytes:  push esi ; push edi ; push 0x114 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_ThrowShotObject::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062F3B0 plus this class's vtable store
// (0x012944D4, named through rtti-vtables.tsv). sizeof = 0x114. One of 196 bodies of this shape.
// NF148 (2026-11-20, q11785, one vtable): moved onto the shared headers. Own leaf now the PDB
//   ?Clone@CCreatureAction_ThrowShotObject@@UBEPAVCCreatureActionBase@@XZ (Ego_r.pdb 0x0097B4B0 + FableWin.pdb 0x02BF6900: public virtual const,
//   returns CCreatureActionBase*; retail vftable 0x012944D4 slot 11), was the non-virtual stand-in ?Clone@...@@QAEPAVCCombatActionBase@@XZ.
//   The local CCombatActionBase / CCreatureAction_ThrowShotObject models (with a placeholder virtual v00) are gone for
//   engine/CCombatActionBase.h (MELEE mode, opt-in FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR: ??0CCombatActionBase@@QAE@ABV0@@Z,
//   0x0062F3B0) and engine/CCreatureAction_ThrowShotObject.h (_CLASS + _CLONE + _INITIALISE), so this object's
//   ??_7CCreatureAction_ThrowShotObject@@6B@ COMDAT is the same as the copies 0x009551A0 and 0x009566C0 emit.
#define FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS  // NF264 (q12750): CCombatActionBase::AllowsCombinationAnims override, so emitted tables bind slot 66 = 0x0062EA10 as retail
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_INITIALISE
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"

CCreatureActionBase* CCreatureAction_ThrowShotObject::Clone() const
{
    return new CCreatureAction_ThrowShotObject(*this);
}
