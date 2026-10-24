// std::_Rb_tree_base<node0x14_009d7850>::_Rb_tree_base -- STLport _Rb_tree_base(const allocator&)
// Leaf: ??0_Rb_tree_base_node0x14_009d7850@std@@
// Renamed (fistp2, 2026-10-24) from CRetailBlock0x14::CRetailBlock0x14 @ 00/9d/CRetailBlock0x14_CRetailBlock0x14_009d7850.cpp: STLport _Rb_tree_base(const allocator&): nulls the head and malloc's one 0x14-byte head node (the same node size as the _Rb_tree constructors of that family); called from the two-argument _Rb_tree(comp, alloc) constructors that the map() default constructors use; value type not recoverable, so the name carries the node size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D7850, 27 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct _Rb_tree_base_node0x14_009d7850 {
    unsigned char* head;
    _Rb_tree_base_node0x14_009d7850(CRetailAllocator allocator);
};
}
std::_Rb_tree_base_node0x14_009d7850::_Rb_tree_base_node0x14_009d7850(CRetailAllocator allocator)
    : head(0)
{
    head = (unsigned char*)malloc(0x14);
}
