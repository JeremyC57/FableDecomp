#pragma optimize("s",on)
// CRetailList_node0xc_dword::Buynode(const long&) -- malloc a 0xc-byte node and placement-copy the dword value at +0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00429FE3, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0xc ; call malloc ; pop ecx ; lea ecx,[eax+0x8] ; test ecx,ecx ; je S ; mov edx,[esp+4] ; mov edx,[edx] ; mov [ecx],edx ; S: ret 4
// Retail std::list node factory for a 4-byte scalar value: one malloc'd node (malloc @
// 0x00BFEA0E) whose value at +0x8 is placement-copied (hence the null test on the value
// address); the node (malloc's result, still in eax) is returned. `this` (the list) is unused.
// One of 10 bodies of this shape.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailList_node0xc_dword {
    struct SNode {
        unsigned char _links[0x8];       // prev/next (and any further link) pointers
        long m_value;                      // +0x8
    };
    SNode* Buynode(const long& value);
};

CRetailList_node0xc_dword::SNode* CRetailList_node0xc_dword::Buynode(const long& value)
{
    SNode* node = (SNode*)malloc(sizeof(SNode));
    new (&node->m_value) long(value);
    return node;
}