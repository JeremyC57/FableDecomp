// CVertexGroupBlockList::HasBlock(const CVertexGroupBlock& block) -- true when some block in the list (searched from the back) holds exactly the same group count and every group of `block`
// Leaf: ?HasBlock@CVertexGroupBlockList@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD8D10, 181 bytes (padded to 192), `ret 4`.
//   PDB name ?HasBlock@CVertexGroupBlockList@@QBE_NABVCVertexGroupBlock@@@Z.
// Callees: none (the STLport map find is inline).
// Layout (retail; the PDB sizes carry debug containers): CVertexGroupBlockList::Blocks is a 12-byte CArray of 0x18-byte
//   CVertexGroupBlock {Groups CArray<unsigned long> +0, GroupToIndexMap map<unsigned long, long> +0xc (header +0)}
//   (landed CVertexGroupBlock::AddGroupsFrom 0x00AD9260 model).
// Shape: `block`'s group pointer and count are locals read before the walk (kept in edi/ebx, spilled across the
//   blocks); a const_reverse_iterator walk (the cursor is spilled to the dead `block` argument slot, `*it` is
//   `*(current - 1)`); the count test, then a `__forceinline` ContainsAll walking the group pointer by value
//   (`++i, ++groups`: the pointer, not an index, is what retail steps) and looking every group up with the inline
//   lower-bound find (a miss returns false straight to the next block).
struct SGroupMapNode {                             // STLport _Rb_tree_node<pair<const unsigned long, long> >
    long           Color;                          // +0x00
    SGroupMapNode* Parent;                         // +0x04
    SGroupMapNode* Left;                           // +0x08
    SGroupMapNode* Right;                          // +0x0c
    unsigned long  Key;                            // +0x10
    long           Value;                          // +0x14
};
struct CGroupToIndexMap {                          // map<unsigned long, long> (retail 12 bytes)
    SGroupMapNode* Header;                         // +0x00
    unsigned long  NodeCount;                      // +0x04
    unsigned long  Compare;                        // +0x08
    __forceinline SGroupMapNode* find(unsigned long key) const   // inline STLport _Rb_tree::find, returning the node
    {
        SGroupMapNode* y = Header;
        SGroupMapNode* x = Header->Parent;
        while (x != 0) {
            if (!(x->Key < key)) {
                y = x;
                x = x->Left;
            }
            else
                x = x->Right;
        }
        if (y == Header || key < y->Key)
            y = Header;
        return y;
    }
};
struct CGroupArray {                               // CArray<unsigned long> (retail std::vector, 12 bytes)
    unsigned long* m_first;
    unsigned long* m_last;
    unsigned long* m_end;
    unsigned long* begin() const { return m_first; }
    unsigned long size() const { return m_last - m_first; }
    const unsigned long& operator[](unsigned long n) const { return *(begin() + n); }
};
class CVertexGroupBlock { public:                         // retail 0x18 bytes (PDB 32: debug containers)
    CGroupArray      Groups;                       // +0x00
    CGroupToIndexMap GroupToIndexMap;              // +0x0c
    __forceinline bool ContainsAll(const unsigned long* groups, unsigned long count) const
    {
        for (unsigned long i = 0; i < count; ++i, ++groups)
            if (GroupToIndexMap.find(*groups) == GroupToIndexMap.Header)
                return false;
        return true;
    }
};
template <class T> struct const_reverse_iterator {
    const T* current;
    const_reverse_iterator(const T* p) : current(p) { }
    const T& operator*() const { const T* tmp = current; return *--tmp; }
    const T* operator->() const { return &(operator*()); }
    const_reverse_iterator& operator++() { --current; return *this; }
    bool operator!=(const const_reverse_iterator& other) const { return current != other.current; }
};
struct CBlockArray {                               // CArray<CVertexGroupBlock> (retail std::vector, 12 bytes)
    CVertexGroupBlock* m_first;
    CVertexGroupBlock* m_last;
    CVertexGroupBlock* m_end;
    const_reverse_iterator<CVertexGroupBlock> rbegin() const { return const_reverse_iterator<CVertexGroupBlock>(m_last); }
    const_reverse_iterator<CVertexGroupBlock> rend() const { return const_reverse_iterator<CVertexGroupBlock>(m_first); }
};
class CVertexGroupBlockList { public:
    CBlockArray Blocks;                            // +0x00
    bool HasBlock(const CVertexGroupBlock& block) const;
};
bool CVertexGroupBlockList::HasBlock(const CVertexGroupBlock& block) const
{
    unsigned long count = block.Groups.size();
    const unsigned long* groups = block.Groups.begin();
    for (const_reverse_iterator<CVertexGroupBlock> it = Blocks.rbegin(); it != Blocks.rend(); ++it)
        if (count == it->Groups.size() && it->ContainsAll(groups, count))
            return true;
    return false;
}