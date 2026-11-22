// CCreatureAction_UnsheatheItemFromInventory::Clone() const -- new CCreatureAction_UnsheatheItemFromInventory(*this)
// Leaf: ?Clone@CCreatureAction_UnsheatheItemFromInventory@@EBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A0E50, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb8 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_UnsheatheItemFromInventory) = 0xb8, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x006A0E80; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
// NF273 (2026-11-22): moved off its local CCreatureActionBase / derived-class models onto engine/CCreatureAction_UnsheatheItemFromInventory.h
//   with the define / include set of the landed sibling 0x006A0150 (same class, same header) plus the add-only opt-in
//   FABLE_RETAIL_CCREATUREACTION_UNSHEATHE_COPYCTOR (the copy constructor 0x006A0E80, both PDBs). Own leaf now the PDB
//   ?Clone@CCreatureAction_UnsheatheItemFromInventory@@EBEPAVCCreatureActionBase@@XZ (Ego_r.pdb 0x006C59B0 + FableWin.pdb 0x02211670:
//   private virtual const; retail vtable 0x0125CCBC slot 11, its only reference), was the non-virtual stand-in
//   ?Clone@CCreatureAction_UnsheatheItemFromInventory@@QAEPAVCCreatureActionBase@@XZ.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CANIMATIONENTRY_TRANSITION_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHE_COPYCTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"

CCreatureActionBase* CCreatureAction_UnsheatheItemFromInventory::Clone() const
{
    return new CCreatureAction_UnsheatheItemFromInventory(*this);
}
