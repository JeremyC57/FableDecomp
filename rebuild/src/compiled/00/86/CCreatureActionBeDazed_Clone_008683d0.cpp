// CCreatureAction_BeDazed::Clone() -- new CCreatureAction_BeDazed(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008683D0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xa8 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_BeDazed::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062EBC0 plus this class's vtable store
// (0x01276F54, named through rtti-vtables.tsv). sizeof = 0xa8. One of 196 bodies of this shape.
// Leaf: ?Clone@CCreatureAction_BeDazed@@UBEPAVCCreatureActionBase@@XZ
// Clone port (NF323, 2026-11-24; NF317 Q3 / NF320 Q1): defined on the real class of engine/CCreatureAction_BeDazed.h over engine/CCreatureActionBase.h
//   instead of the local one-slot stand-in model (was ?Clone@CCreatureAction_BeDazed@@QAEPAVCCreatureActionBase@@XZ). The inline implicit copy constructor stores
//   the class vftable, so this TU emits ??_7CCreatureAction_BeDazed@@6B@ from the header model, slot for slot the retail table.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCREATUREACTION_BEDAZED_CLASS
#define FABLE_RETAIL_CCREATUREACTION_BEDAZED_RTTI_DECLS
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_BeDazed.h"

CCreatureActionBase* CCreatureAction_BeDazed::Clone() const
{
    return new CCreatureAction_BeDazed(*this);
}
