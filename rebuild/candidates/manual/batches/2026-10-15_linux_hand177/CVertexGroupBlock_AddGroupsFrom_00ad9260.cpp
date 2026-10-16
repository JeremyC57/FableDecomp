// CVertexGroupBlock::AddGroupsFrom(const CVertexGroupBlock& other) -- merge another block's groups: every group of `other` not yet in this block's GroupToIndexMap is added
// Leaf: ?AddGroupsFrom@CVertexGroupBlock@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD9260, length 114 bytes (padded to 128), `ret 4`.
// Callees: CVertexGroupBlock::AddGroup(unsigned long) @ 0x00AD91F0.
// Layout (retail 0x18 bytes; PDB 32 with debug containers): Groups CArray<unsigned long> +0x00 (12-byte vector),
// GroupToIndexMap map<unsigned long, long> +0x0c (header +0, count +4).
// Shape: the STLport map find is inlined (lower-bound walk from header->_M_parent, then the `y == header || k < y->key`
// fix-up) and compared with the header re-read from the map; the loop bound `other.Groups.size()` is recomputed each pass.
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
    SGroupMapNode* find(unsigned long key) const   // inline STLport _Rb_tree::find, returning the node
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
struct CVertexGroupBlock {                         // retail 0x18 bytes (PDB 32: debug containers)
    CGroupArray      Groups;                       // +0x00
    CGroupToIndexMap GroupToIndexMap;              // +0x0c
    void AddGroup(unsigned long group);            // 0x00AD91F0
    void AddGroupsFrom(const CVertexGroupBlock& other);
};
void CVertexGroupBlock::AddGroupsFrom(const CVertexGroupBlock& other)
{
    for (unsigned long i = 0; i < other.Groups.size(); ++i) {
        unsigned long group = other.Groups[i];
        if (GroupToIndexMap.find(group) == GroupToIndexMap.Header)
            AddGroup(group);
    }
}
