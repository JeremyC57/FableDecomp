// std::list<T_0x14_0072d480>::list -- STLport list constructor
// Leaf: ??0list_T_0x14_0072d480@std@@
// Renamed (fistp2, 2026-10-24) from CRetailList_node0x1c::CRetailList_node0x1c @ 00/72/CRetailListnode0x1c_CRetailListnode0x1c_0072d480.cpp: STLport list constructor: sentinel node of 0x1c bytes (= 8 bytes of links + a 0x14-byte element) malloc'd and self-linked, allocator by value (ret 4); T is not recoverable from the bytes, so the name carries the element size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072D480, 32 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct list_T_0x14_0072d480 {
    struct node { node* next; node* prev; unsigned char value[0x1c - 8]; };
    node* head;
    list_T_0x14_0072d480(CRetailAllocator allocator);
};
}
std::list_T_0x14_0072d480::list_T_0x14_0072d480(CRetailAllocator allocator)
    : head(0)
{
    node* sentinel = (node*)malloc(sizeof(node));   // linked to itself before it is stored into head
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}
