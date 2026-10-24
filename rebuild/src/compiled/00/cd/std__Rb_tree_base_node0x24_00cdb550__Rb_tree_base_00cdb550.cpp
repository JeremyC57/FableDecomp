// std::_Rb_tree_base<node0x24_00cdb550>::_Rb_tree_base -- STLport _Rb_tree_base(const allocator&)
// Leaf: ??0_Rb_tree_base_node0x24_00cdb550@std@@
// Renamed (fistp2, 2026-10-24) from CRetailBlock0x24::CRetailBlock0x24 @ 00/cd/CRetailBlock0x24_CRetailBlock0x24_00cdb550.cpp: STLport _Rb_tree_base(const allocator&): nulls the head and malloc's one 0x24-byte head node (the same node size as the _Rb_tree constructors of that family); called from the two-argument _Rb_tree(comp, alloc) constructors that the map() default constructors use; value type not recoverable, so the name carries the node size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDB550, 27 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct _Rb_tree_base_node0x24_00cdb550 {
    unsigned char* head;
    _Rb_tree_base_node0x24_00cdb550(CRetailAllocator allocator);
};
}
std::_Rb_tree_base_node0x24_00cdb550::_Rb_tree_base_node0x24_00cdb550(CRetailAllocator allocator)
    : head(0)
{
    head = (unsigned char*)malloc(0x24);
}
