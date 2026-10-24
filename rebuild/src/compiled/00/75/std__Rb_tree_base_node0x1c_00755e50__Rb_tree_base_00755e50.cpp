// std::_Rb_tree_base<node0x1c_00755e50>::_Rb_tree_base -- STLport _Rb_tree_base(const allocator&)
// Leaf: ??0_Rb_tree_base_node0x1c_00755e50@std@@
// Renamed (fistp2, 2026-10-24) from CRetailBlock0x1c::CRetailBlock0x1c @ 00/75/CRetailBlock0x1c_CRetailBlock0x1c_00755e50.cpp: STLport _Rb_tree_base(const allocator&): nulls the head and malloc's one 0x1c-byte head node (the same node size as the _Rb_tree constructors of that family); called from the two-argument _Rb_tree(comp, alloc) constructors that the map() default constructors use; value type not recoverable, so the name carries the node size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x00755E50, 27 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument (ret 4)
namespace std {
struct _Rb_tree_base_node0x1c_00755e50 {
    unsigned char* head;
    _Rb_tree_base_node0x1c_00755e50(CRetailAllocator allocator);
};
}
std::_Rb_tree_base_node0x1c_00755e50::_Rb_tree_base_node0x1c_00755e50(CRetailAllocator allocator)
    : head(0)
{
    head = (unsigned char*)malloc(0x1c);
}
