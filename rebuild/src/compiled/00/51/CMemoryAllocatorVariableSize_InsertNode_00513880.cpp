// CMemoryAllocatorVariableSize::InsertNode(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w) -- STLport _Rb_tree::_M_insert for a 1-byte value keyed by a signed char: link a new 0x14-byte node left or right of y, fix the header's root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513880, length 178 bytes (`ret 14h`).
// Callees: malloc @ 0x00BFEA0E (the node), _Rb_global<bool>::_Rebalance @ 0x0042971D (__cdecl (node, root&)).
// Label correction: the catalog name is propagated; the body is an STLport `_Rb_tree::_M_insert` instantiation (same shape as the
// landed 0x00567AF0), its key and value types identified only by the copy and the compare (signed byte `jl`).
// Shape: the explicit iterator out-pointer form; the node is malloc'd and its value placement-copied under VC's null test; the new node
// goes left when y is the header, or (w null) when x is set or the key orders before y's.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
struct SValue {                                 // 1 byte
    char Key;
};
struct SNode : _Rb_tree_node_base {
    SValue Value;                               // +0x10
};
struct SIterator {
    SNode* m_node;
};
struct CRbTree {                                 // STLport _Rb_tree: header, count
    SNode*        m_header;                     // +0x00
    unsigned long m_node_count;                 // +0x04
    static SNode* CreateNode(const SValue& value)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->Value) SValue(value);
        return node;
    }
    SIterator* _M_insert(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w);
};

SIterator* CRbTree::_M_insert(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w)
{
    SNode* z;
    if (y == m_header || (w == 0 && (x != 0 || value.Key < y->Value.Key))) {
        z = CreateNode(value);
        y->_M_left = z;
        if (y == m_header) {
            m_header->_M_parent = z;
            m_header->_M_right = z;
        }
        else if (y == m_header->_M_left)
            m_header->_M_left = z;
    }
    else {
        z = CreateNode(value);
        y->_M_right = z;
        if (y == m_header->_M_right)
            m_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, m_header->_M_parent);
    ++m_node_count;
    result->m_node = z;
    return result;
}