// CCreatureAction_StabbedToDeath::Clone() -- new CCreatureAction_StabbedToDeath(*this) with the implicit copy constructor inlined
// Leaf: ?Clone@CCreatureAction_StabbedToDeath@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008800C0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xa8 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_StabbedToDeath::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062EBC0 plus this class's vtable store
// (0x01277B74, named through rtti-vtables.tsv). sizeof = 0xa8. One of 196 bodies of this shape.
// NF339 (2026-11-24): moved off the local stand-in models onto engine/CCreatureAction_StabbedToDeath.h (the NF330 opt-in model,
//   defined by landed 0x0087E830 / 0x0087FE50) as the PDB member virtual Clone() const (Ego_r + FableWin UBE; RTTI slot 11).
#define FABLE_RETAIL_CCREATUREACTION_STABBEDTODEATH_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#include "engine/CCreatureAction_StabbedToDeath.h"

CCreatureActionBase* CCreatureAction_StabbedToDeath::Clone() const
{
    return new CCreatureAction_StabbedToDeath(*this);
}
