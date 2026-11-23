// CCreatureAction_DropGenericBox::Clone() -- new CCreatureAction_DropGenericBox(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C6C0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xa8 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_DropGenericBox::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062EBC0 plus this class's vtable store
// (0x012731E4, named through rtti-vtables.tsv). sizeof = 0xa8. One of 196 bodies of this shape.
// Leaf: ?Clone@CCreatureAction_DropGenericBox@@UBEPAVCCreatureActionBase@@XZ
// Clone port PILOT (NF320, 2026-11-23, NF317 Q3): defined on the real class of engine/CCreatureAction_DropGenericBox.h
//   (FABLE_RETAIL_CCREATUREACTION_GENERICBOX over engine/CCreatureActionBase.h, FABLE_RETAIL_CCREATUREACTIONBASE) instead of the local
//   one-slot stand-in model (was ?Clone@CCreatureAction_DropGenericBox@@QAEPAVCCreatureActionBase@@XZ). The inline implicit copy
//   constructor stores the class vftable, so this TU emits ??_7CCreatureAction_DropGenericBox@@6B@ from the header's full model.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_DropGenericBox.h"

CCreatureActionBase* CCreatureAction_DropGenericBox::Clone() const
{
    return new CCreatureAction_DropGenericBox(*this);
}
