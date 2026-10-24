// std::_Rb_tree_base<node0x30_00b96570>::_Rb_tree_base -- STLport _Rb_tree_base(const allocator&)
// Leaf: ??0_Rb_tree_base_node0x30_00b96570@std@@
// Renamed (fistp2, 2026-10-24) from CRetailBlock0x30::CRetailBlock0x30 @ 00/b9/CRetailBlock0x30_CRetailBlock0x30_00b96570.cpp: STLport _Rb_tree_base(const allocator&): nulls the head and malloc's one 0x30-byte head node (the same node size as the _Rb_tree constructors of that family); called from the two-argument _Rb_tree(comp, alloc) constructors that the map() default constructors use; value type not recoverable, so the name carries the node size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B96570, 27 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct _Rb_tree_base_node0x30_00b96570 {
    unsigned char* head;
    _Rb_tree_base_node0x30_00b96570(CRetailAllocator allocator);
};
}
std::_Rb_tree_base_node0x30_00b96570::_Rb_tree_base_node0x30_00b96570(CRetailAllocator allocator)
    : head(0)
{
    head = (unsigned char*)malloc(0x30);
}
