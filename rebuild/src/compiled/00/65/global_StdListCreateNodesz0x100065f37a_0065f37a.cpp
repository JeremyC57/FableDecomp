#pragma optimize("s",on)
// Leaf: ?Buynode@CRetailList_node0x10@@QAEPAUnode@1@ABV?$CCountedPointer@VCVillageTaskHorn@@@@@Z
// CRetailList_node0x10::Buynode(const T&) -- malloc a 0x10-byte node and copy-construct the 8-byte value at +0x8 in place
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F37A, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; call malloc ; mov esi,eax ; pop ecx ; lea ecx,[esi+0x8] ; test ecx,ecx ; je S ; push [esp+8] ; call T::T(const T&) ; S: mov eax,esi ; pop esi ; ret 4
// Retail std::list node factory: one malloc'd node (malloc @ 0x00BFEA0E) whose value is
// copy-constructed in place with placement new (hence the null test on the value address) by the
// out-of-line copy constructor @ 0x0065D89B; the node is returned. `this` (the list) is unused.
// Naming (NF184, 2026-11-20, q6143): the 8-byte value is CCountedPointer<CVillageTaskHorn> (engine/CCountedPointer.h): its
//   out-of-line copy constructor 0x0065D89B is ??0?$CCountedPointer@VCVillageTaskHorn@@@@QAE@ABV0@@Z (FableWin.pdb name set + the
//   self row's Leaf; landed 0x0065F96D binds it). The header's copy constructor is declaration-only here
//   (FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY): retail calls the COMDAT. The list stand-in CRetailList_node0x10 (no shared header)
//   stays; its own leaf follows the value type (was ...ABUvalue_type_0065d89b@1@@Z; no landed row binds either spelling: the one
//   referencer 0x0065FD39 binds its own stand-in).
#define FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY
#include <new>
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);
class CVillageTaskHorn;

struct CRetailList_node0x10 {
    struct node { node* next; node* prev; CCountedPointer<CVillageTaskHorn> value; };
    node* head;
    node* Buynode(const CCountedPointer<CVillageTaskHorn>& value);
};

CRetailList_node0x10::node* CRetailList_node0x10::Buynode(const CCountedPointer<CVillageTaskHorn>& value)
{
    node* n = (node*)malloc(sizeof(node));
    new (&n->value) CCountedPointer<CVillageTaskHorn>(value);
    return n;
}
