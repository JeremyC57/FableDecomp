// std::list<T_0x40_008fbe80>::list -- STLport list constructor
// Leaf: ??0list_T_0x40_008fbe80@std@@
// Renamed (fistp2, 2026-10-24) from CRetailList_node0x48::CRetailList_node0x48 @ 00/8f/CRetailListnode0x48_CRetailListnode0x48_008fbe80.cpp: STLport list constructor: sentinel node of 0x48 bytes (= 8 bytes of links + a 0x40-byte element) malloc'd and self-linked, allocator by value (ret 4); T is not recoverable from the bytes, so the name carries the element size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FBE80, 32 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct list_T_0x40_008fbe80 {
    struct node { node* next; node* prev; unsigned char value[0x48 - 8]; };
    node* head;
    list_T_0x40_008fbe80(CRetailAllocator allocator);
};
}
std::list_T_0x40_008fbe80::list_T_0x40_008fbe80(CRetailAllocator allocator)
    : head(0)
{
    node* sentinel = (node*)malloc(sizeof(node));   // linked to itself before it is stored into head
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}
