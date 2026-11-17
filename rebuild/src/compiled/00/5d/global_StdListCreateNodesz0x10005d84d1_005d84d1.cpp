#pragma optimize("s",on)
// Leaf: Buynode
// CRetailList_node0x10::Buynode(const T&) -- malloc a 0x10-byte node and copy-construct the 8-byte value at +0x8 in place
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D84D1, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; call malloc ; mov esi,eax ; pop ecx ; lea ecx,[esi+0x8] ; test ecx,ecx ; je S ; push [esp+8] ; call T::T(const T&) ; S: mov eax,esi ; pop esi ; ret 4
// Retail std::list node factory: one malloc'd node (malloc @ 0x00BFEA0E) whose value is
// copy-constructed in place with placement new (hence the null test on the value address) by the
// out-of-line copy constructor @ 0x005B12F4; the node is returned. `this` (the list) is unused.
// One of 28 bodies.
// Name fix (agentNF54, nf44:52): the 8-byte value is the shared STLport std::pair<CWideString, long> (stlport/fable_containers.h +
//   fable_string.h; the value_type_005b12f4 stand-in is gone): its copy constructor is the landed 0x005B12F4
//   ??0?$pair@VCWideString@@J@std@@QAE@ABU01@@Z, called out of line in this size-optimised TU. The list itself keeps its descriptive
//   stand-in (owner unproven).
#include <new>
#include "fable_string.h"
#include "stlport/fable_containers.h"
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailList_node0x10 {
    typedef std::pair<CWideString, long> value_type;
    struct node { node* next; node* prev; value_type value; };
    node* head;
    node* Buynode(const value_type& value);
};

CRetailList_node0x10::node* CRetailList_node0x10::Buynode(const value_type& value)
{
    node* n = (node*)malloc(sizeof(node));
    new (&n->value) value_type(value);
    return n;
}
