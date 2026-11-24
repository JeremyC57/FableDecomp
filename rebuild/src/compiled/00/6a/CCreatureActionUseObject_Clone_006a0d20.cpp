// CCreatureAction_UseObject::Clone() const -- new CCreatureAction_UseObject(*this)
// Leaf: ?Clone@CCreatureAction_UseObject@@EBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A0D20, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xe0 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_UseObject::CCreatureAction_UseObject(const CCreatureAction_UseObject&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_UseObject) = 0xe0, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x006A0D50; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
// NF273 (2026-11-22): moved off its local CCreatureActionBase / CCreatureAction_UseObject models onto engine/CCreatureAction_UseObject.h
//   (FABLE_RETAIL_CCREATUREACTION_USEOBJECT, which declares the private Clone and the copy constructor 0x006A0D50), with the define /
//   include set of the landed copy constructor row 0x006A0D50. Own leaf now the PDB ?Clone@CCreatureAction_UseObject@@EBEPAVCCreatureActionBase@@XZ
//   (Ego_r.pdb 0x006C5980 + FableWin.pdb 0x022129F0: private virtual const; retail vtable 0x0125CEFC slot 11, its only reference), was
//   the non-virtual stand-in ?Clone@CCreatureAction_UseObject@@QAEPAVCCreatureActionBase@@XZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UseObject.h"

CCreatureActionBase* CCreatureAction_UseObject::Clone() const
{
    return new CCreatureAction_UseObject(*this);
}
