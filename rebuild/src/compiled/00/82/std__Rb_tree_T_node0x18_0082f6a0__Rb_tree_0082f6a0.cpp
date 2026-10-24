// std::_Rb_tree<T_node0x18_0082f6a0>::_Rb_tree -- STLport red-black tree constructor
// Leaf: ??0_Rb_tree_T_node0x18_0082f6a0@std@@
// Renamed (fistp2, 2026-10-24) from CRetailTree_node0x18::CRetailTree_node0x18 @ 00/82/CRetailTreenode0x18_CRetailTreenode0x18_0082f6a0.cpp: STLport red-black tree constructor: 0x18-byte head node malloc'd (16 bytes of colour/parent/left/right + a 0x8-byte value), size zeroed, colour/parent cleared, left/right self-linked; the key/value types are not recoverable from the bytes, so the name carries the node size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082F6A0, 54 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument
struct CRetailCompare { };                        // empty by-value comparator argument
namespace std {
struct _Rb_tree_T_node0x18_0082f6a0 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x18 - 16]; };
    node*         head;
    unsigned long size;
    _Rb_tree_T_node0x18_0082f6a0();
};
}
std::_Rb_tree_T_node0x18_0082f6a0::_Rb_tree_T_node0x18_0082f6a0()
    : head(0)
{
    head = (node*)malloc(sizeof(node));
    size = 0;
    head->colour = 0;
    head->parent = 0;
    head->left   = head;
    head->right  = head;
}
