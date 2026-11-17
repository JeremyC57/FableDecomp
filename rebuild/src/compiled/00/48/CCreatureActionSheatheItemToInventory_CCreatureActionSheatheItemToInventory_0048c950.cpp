// CCreatureAction_SheatheItemToInventory::~CCreatureAction_SheatheItemToInventory() -- the implicit destructor's own COMDAT: release
// PFollowUpAction (+0xA8, CCountedPointer<CCreatureActionBase>: count block destroy + delete at zero, both fields nulled), then
// ~CCreatureActionBase 0x00693EF0 by tail call (no vptr store)
// Leaf: ??1CCreatureAction_SheatheItemToInventory@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048C950, oracle length 75 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + VC7.1 cl.exe).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0xac] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+0xac] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0xac] ; push eax ; call operator delete ; add esp,4 ; N: mov dword ptr [esi+0xa8],0 ; mov dword ptr [esi+0xac],0 ; mov ecx,esi ; pop esi ; jmp CCreatureActionBase::~CCreatureActionBase
// Identity (agentNF77, q10884): the shared engine/CCreatureAction_SheatheItemToInventory.h names 0x0048C950 as
//   ??1CCreatureAction_SheatheItemToInventory@@UAE@XZ (Ego_r.pdb 0x00496240 + FableWin.pdb 0x01AD07A0; CTCTrophy::TerminateMinigame 0x006DE280
//   destroys its action temporary with it). Its one member, PFollowUpAction (CCountedPointer<CCreatureActionBase>, Ego_r.pdb), sits at retail
//   +0xA8, and the tail call is ??1CCreatureActionBase@@UAE@XZ 0x00693EF0 (Ego_r.pdb + FableWin.pdb; RTTI: CCreatureAction_SheatheItemToInventory
//   : CCreatureActionBase). Was a local CPhysicsMeshInfo / CPhysicsMeshInfo_Derived_0048c950 / CCountedPointer model (manifest label propagated).
// Shape: no vtable store, VC7.1's implicit destructor (hdr/ overlay opt-in FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_IMPLICIT_DTOR
//   leaves out the PDB-generated user declaration; a user-declared one stores the vptr first, GOTCHAS 73); DestroyOne (a destruction by
//   name, ungraded) makes VC7.1 emit the out-of-line implicit ??1 (as the landed ~CTCTargetingBase 0x004DA8B7).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_IMPLICIT_DTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CCreatureAction_SheatheItemToInventory* object)
{
    object->CCreatureAction_SheatheItemToInventory::~CCreatureAction_SheatheItemToInventory();
}
