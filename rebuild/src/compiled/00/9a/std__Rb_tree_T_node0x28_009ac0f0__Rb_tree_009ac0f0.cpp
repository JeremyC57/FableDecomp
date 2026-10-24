// std::_Rb_tree<T_node0x28_009ac0f0>::_Rb_tree -- STLport red-black tree constructor
// Leaf: ??0_Rb_tree_T_node0x28_009ac0f0@std@@
// Renamed (fistp2, 2026-10-24) from CRetailTree_node0x28::CRetailTree_node0x28 @ 00/9a/CRetailTreenode0x28_CRetailTreenode0x28_009ac0f0.cpp: STLport red-black tree constructor: 0x28-byte head node malloc'd (16 bytes of colour/parent/left/right + a 0x18-byte value), size zeroed, colour/parent cleared, left/right self-linked; the key/value types are not recoverable from the bytes, so the name carries the node size and the address
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC0F0, 54 bytes.
extern "C" void* __cdecl malloc(unsigned int size);
struct CRetailAllocator { };                      // empty by-value allocator argument
struct CRetailCompare { };                        // empty by-value comparator argument
namespace std {
struct _Rb_tree_T_node0x28_009ac0f0 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x28 - 16]; };
    node*         head;
    unsigned long size;
    _Rb_tree_T_node0x28_009ac0f0();
};
}
std::_Rb_tree_T_node0x28_009ac0f0::_Rb_tree_T_node0x28_009ac0f0()
    : head(0)
{
    head = (node*)malloc(sizeof(node));
    size = 0;
    head->colour = 0;
    head->parent = 0;
    head->left   = head;
    head->right  = head;
}
