#pragma optimize("s",on)
// CRetailList_node0x28::Buynode(const T&) -- malloc a 0x28-byte node and copy-construct the 24-byte value at +0x10 in place
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9763, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x28 ; call malloc ; mov esi,eax ; pop ecx ; lea ecx,[esi+0x10] ; test ecx,ecx ; je S ; push [esp+8] ; call T::T(const T&) ; S: mov eax,esi ; pop esi ; ret 4
// Retail std::list node factory: one malloc'd node (malloc @ 0x00BFEA0E) whose value is
// copy-constructed in place with placement new (hence the null test on the value address) by the
// out-of-line copy constructor @ 0x004F9784; the node is returned. `this` (the list) is unused.
// The element type is not recoverable beyond its 24-byte size. One of 28 bodies.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailList_node0x28 {
    struct value_type { unsigned char _body[0x18]; value_type(const value_type& other); };   // copy ctor 0x004F9784
    struct node { node* next; node* prev; unsigned char _gap[0x8]; value_type value; };
    node* head;
    node* Buynode(const value_type& value);
};

CRetailList_node0x28::node* CRetailList_node0x28::Buynode(const value_type& value)
{
    node* n = (node*)malloc(sizeof(node));
    new (&n->value) value_type(value);
    return n;
}