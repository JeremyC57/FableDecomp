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
inline void* operator new(unsigned int, void* where) { return where; }

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
    struct node {                                           // 0x28 bytes
        unsigned char color;
        unsigned char _pad[3];
        node*         parent;                               // +0x04
        node*         left;                                 // +0x08
        node*         right;                                // +0x0c
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

void __cdecl Rebalance_0042971d(CUIBinaryTree::node* newnode, CUIBinaryTree::node** root);   // 0x0042971D

CUIBinaryTree::iterator CUIBinaryTree::Insert(node* trynode, node* wherenode, const SUITreePair& value, long forceRight)
{
    node* newnode;
    if (wherenode == head || (!forceRight && (trynode != 0 || value.first < wherenode->value.first))) {
        newnode = _Buynode(value);
        wherenode->left = newnode;
        if (wherenode == head) {
            head->parent = newnode;
            head->right = newnode;
        }
        else if (wherenode == head->left)
            head->left = newnode;
    }
    else {
        newnode = _Buynode(value);
        wherenode->right = newnode;
        if (wherenode == head->right)
            head->right = newnode;
    }
    newnode->left = 0;
    newnode->right = 0;
    newnode->parent = wherenode;
    Rebalance_0042971d(newnode, &head->parent);
    ++size;
    return iterator(newnode);
}
