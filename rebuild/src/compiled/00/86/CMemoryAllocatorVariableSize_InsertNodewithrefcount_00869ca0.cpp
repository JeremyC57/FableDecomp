// CMemoryAllocatorVariableSize::InsertNode_with_refcount(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w) -- STLport _Rb_tree::_M_insert for an 8-byte {counted pointer key, long} value: link a new 0x18-byte node left or right of y, fix the header's root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00869CA0, length 205 bytes (`ret 14h`).
// Callees: malloc @ 0x00BFEA0E (the node), _Rb_global<bool>::_Rebalance @ 0x0042971D (__cdecl (node, root&)).
// Label correction: the catalog name is propagated; the body is an STLport `_Rb_tree::_M_insert` instantiation (same shape as the
// landed 0x00567AF0), its key and value types identified only by the copy and the compare (unsigned pointer `jb`).
// Shape: the explicit iterator out-pointer form; the node is malloc'd and its value placement-copied under VC's null test; the new node
// goes left when y is the header, or (w null) when x is set or the key orders before y's.
#include <new>
// NF50 (round 7): the rebalance call binds the STLport ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z
// of the shared stlport/fable_map.h; the local global-scope _Rb_tree_node_base / _Rb_global models (which bound a std-less
// spelling) are gone. The row's own name is unchanged (see NF50 NOTES, "self name still a stand-in").
#include "stlport/fable_map.h"
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CRefCounted {                            // intrusive count at +4
    void* __vftable;
    long  RefCount;                             // +0x04
};
struct CRefPointer {                            // compared by address
    CRefCounted* Object;
    CRefPointer(const CRefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
};
struct SValue {                                 // 8 bytes: pair<const CRefPointer, long>
    CRefPointer Key;
    long        Data;
    SValue(const SValue& o) : Key(o.Key), Data(o.Data) { }
};
struct SNode : std::_Rb_tree_node_base {
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
    if (y == m_header || (w == 0 && (x != 0 || (unsigned long)value.Key.Object < (unsigned long)y->Value.Key.Object))) {
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
    std::_Rb_global<bool>::_Rebalance(z, m_header->_M_parent);
    ++m_node_count;
    result->m_node = z;
    return result;
}