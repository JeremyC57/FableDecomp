#pragma optimize("s",on)
// Leaf: Buynode
// CRetailList_node0x10::Buynode(const T&) -- malloc a 0x10-byte node and copy-construct the 8-byte value at +0x8 in place
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F39B, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; call malloc ; mov esi,eax ; pop ecx ; lea ecx,[esi+0x8] ; test ecx,ecx ; je S ; push [esp+8] ; call T::T(const T&) ; S: mov eax,esi ; pop esi ; ret 4
// Retail std::list node factory: one malloc'd node (malloc @ 0x00BFEA0E) whose value is
// copy-constructed in place with placement new (hence the null test on the value address) by the
// out-of-line copy constructor @ 0x0065D806; the node is returned. `this` (the list) is unused.
// The element type is not recoverable beyond its 8-byte size. One of 28 bodies.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailList_node0x10 {
    struct value_type_0065d806 { unsigned char _body[0x8]; value_type_0065d806(const value_type_0065d806& other); };   // copy ctor 0x0065D806
    struct node { node* next; node* prev; value_type_0065d806 value; };
    node* head;
    node* Buynode(const value_type_0065d806& value);
};

CRetailList_node0x10::node* CRetailList_node0x10::Buynode(const value_type_0065d806& value)
{
    node* n = (node*)malloc(sizeof(node));
    new (&n->value) value_type_0065d806(value);
    return n;
}