// std::list<T_0x20_004b8960>::list -- STLport list constructor
// Leaf: ??0list_T_0x20_004b8960@std@@
// Renamed (fistp2, 2026-10-24) from CRetailList_node0x28::CRetailList_node0x28 @ 00/4b/CRetailListnode0x28_CRetailListnode0x28_004b8960.cpp: STLport list constructor: sentinel node of 0x28 bytes (= 8 bytes of links + a 0x20-byte element) malloc'd and self-linked, allocator by value (ret 4); T is not recoverable from the bytes, so the name carries the element size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8960, 32 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct list_T_0x20_004b8960 {
    struct node { node* next; node* prev; unsigned char value[0x28 - 8]; };
    node* head;
    list_T_0x20_004b8960(CRetailAllocator allocator);
};
}
std::list_T_0x20_004b8960::list_T_0x20_004b8960(CRetailAllocator allocator)
    : head(0)
{
    node* sentinel = (node*)malloc(sizeof(node));   // linked to itself before it is stored into head
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}
