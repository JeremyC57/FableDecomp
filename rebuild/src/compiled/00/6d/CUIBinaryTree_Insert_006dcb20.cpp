// CUIBinaryTree::Insert(node* trynode, node* wherenode, const value_type& value, long forceRight) -- the VC6-era Dinkumware `_Tree::_Insert` of a map<long, {map, long, long}>: link a new node left or right of wherenode, fix the head links, rebalance and count
// Leaf: Insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DCB20, length 198 bytes (`ret 14h`: hidden iterator + four arguments;
// 208 with padding).
// Callees: malloc @ 0x00BFEA0E (the 0x28-byte node), the inner map's copy constructor @ 0x004F97B3 (manifest
// "CUIBinaryTree::CopyConstructor"), the out-of-line _Buynode copy @ 0x004F9763 (manifest "Alloc"), the __cdecl
// red-black fix-up @ 0x0042971D (node, &root).
// Shape: the landed CRetailTree _Insert (0x005BB220, a size TU) compiled for speed: the left/right split of
// `wherenode == head || (!forceRight && (trynode != 0 || key < wherenode->key))`; the in-class _Buynode (malloc +
// placement copy of the pair: key, inner map copy, two longs) is inlined at its first call and called out of line at the
// second, so the left arm builds the node in place while the right arm calls 0x004F9763.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
// NF50 (round 7): the rebalance call binds the STLport ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z
// of the shared stlport/fable_map.h (was a local `Rebalance_0042971d` stand-in); the local node now derives from the header's
// std::_Rb_tree_node_base. The row's own name is unchanged (see NF50 NOTES, "self name still a stand-in").
#include "stlport/fable_map.h"

struct CInnerMap {                                          // VC6 map (12 bytes: allocator byte, head, size)
    void*         m_head;
    unsigned long m_size;
    unsigned long m_compare;
    CInnerMap(const CInnerMap& other);                      // 0x004F97B3
};
struct SUITreeValue {                                       // the mapped value: 0x14 bytes
    CInnerMap m_children;                                   // +0x00
    long      m_a;                                          // +0x0c
    long      m_b;                                          // +0x10
};
struct SUITreePair {                                        // pair<const long, SUITreeValue>
    long         first;                                     // +0x00
    SUITreeValue second;                                    // +0x04
};

struct CUIBinaryTree {
    struct node : std::_Rb_tree_node_base {                 // 0x28 bytes: colour +0, parent +4, left +8, right +0xC
        SUITreePair   value;                                // +0x10
    };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node*         head;                                     // +0x00
    unsigned long size;                                     // +0x04
    node* _Buynode(const SUITreePair& v)
    {
        node* n = (node*)malloc(sizeof(node));
        new (&n->value) SUITreePair(v);
        return n;
    }
    iterator Insert(node* trynode, node* wherenode, const SUITreePair& value, long forceRight);
};

CUIBinaryTree::iterator CUIBinaryTree::Insert(node* trynode, node* wherenode, const SUITreePair& value, long forceRight)
{
    node* newnode;
    if (wherenode == head || (!forceRight && (trynode != 0 || value.first < wherenode->value.first))) {
        newnode = _Buynode(value);
        wherenode->_M_left = newnode;
        if (wherenode == head) {
            head->_M_parent = newnode;
            head->_M_right = newnode;
        }
        else if (wherenode == head->_M_left)
            head->_M_left = newnode;
    }
    else {
        newnode = _Buynode(value);
        wherenode->_M_right = newnode;
        if (wherenode == head->_M_right)
            head->_M_right = newnode;
    }
    newnode->_M_left = 0;
    newnode->_M_right = 0;
    newnode->_M_parent = wherenode;
    std::_Rb_global<bool>::_Rebalance(newnode, head->_M_parent);
    ++size;
    return iterator(newnode);
}