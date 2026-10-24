// std::list<T_0xc_00780040>::list -- STLport list constructor
// Leaf: ??0list_T_0xc_00780040@std@@
// Renamed (fistp2, 2026-10-24) from CRetailList_node0x14::CRetailList_node0x14 @ 00/78/CRetailListnode0x14_CRetailListnode0x14_00780040.cpp: STLport list constructor: sentinel node of 0x14 bytes (= 8 bytes of links + a 0xc-byte element) malloc'd and self-linked, allocator by value (ret 4); T is not recoverable from the bytes, so the name carries the element size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x00780040, 32 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct list_T_0xc_00780040 {
    struct node { node* next; node* prev; unsigned char value[0x14 - 8]; };
    node* head;
    list_T_0xc_00780040(CRetailAllocator allocator);
};
}
std::list_T_0xc_00780040::list_T_0xc_00780040(CRetailAllocator allocator)
    : head(0)
{
    node* sentinel = (node*)malloc(sizeof(node));   // linked to itself before it is stored into head
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}
