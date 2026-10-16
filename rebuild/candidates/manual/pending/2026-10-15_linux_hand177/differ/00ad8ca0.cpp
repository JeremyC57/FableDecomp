// CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) -- every group of this block is in `other` and `other` has strictly more groups
// Grade: DIFFER(117v108) -- register allocation: retail loads `other` into ebp and its begin into ebx (reused as the loop index), this's begin in edi; ours keeps this's begin in ebx and other's in ebp, index in edi, with a longer prologue; loop body (key homed in the dead arg slot via the __forceinline HasGroup) matches; tried index form, pointer walk, if-block vs early return, operand swap
// Leaf: ?IsProperSubsetOf@CVertexGroupBlock@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD8CA0, length 105 bytes (padded to 112), `ret 4`.
// Callees: map<unsigned long, long>::find(iterator* result, const unsigned long& key) @ 0x006699C0 (landed as
// CRetailTree<ulong key>::find; header at the map's +0, end() re-read after the call).
// Layout (retail; PDB 32 bytes with debug containers): Groups CArray<unsigned long> +0x00 (12-byte vector), GroupToIndexMap +0x0c.
// Shape: sizes compared unsigned (`sar 2` then `ja`/`jae`); the group key is a named local homed in the dead `other`
// argument slot and passed to find by address; the miss compares the iterator with the header.
struct SGroupMapNode {                             // STLport _Rb_tree_node<pair<const unsigned long, long> >
    long           Color;                          // +0x00
    SGroupMapNode* Parent;                         // +0x04
    SGroupMapNode* Left;                           // +0x08
    SGroupMapNode* Right;                          // +0x0c
    unsigned long  Key;                            // +0x10
    long           Value;                          // +0x14
};
struct SGroupMapIterator {
    SGroupMapNode* Node;
};
struct CGroupToIndexMap {                          // map<unsigned long, long> (retail 8 bytes: header, count)
    SGroupMapNode* Header;                         // +0x00
    unsigned long  NodeCount;                      // +0x04
    SGroupMapIterator* Find(SGroupMapIterator* result, const unsigned long& key) const;   // 0x006699C0
};
struct CGroupArray {                               // CArray<unsigned long> (retail std::vector, 12 bytes)
    unsigned long* m_first;
    unsigned long* m_last;
    unsigned long* m_end;
    unsigned long* begin() const { return m_first; }
    unsigned long size() const { return m_last - m_first; }
    const unsigned long& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CVertexGroupBlock {                         // retail 0x14 bytes (PDB 32: debug containers)
    CGroupArray      Groups;                       // +0x00
    CGroupToIndexMap GroupToIndexMap;              // +0x0c
    __forceinline bool HasGroup(unsigned long group) const
    {
        SGroupMapIterator it;
        GroupToIndexMap.Find(&it, group);
        return it.Node != GroupToIndexMap.Header;
    }
    bool IsSubsetOf(const CVertexGroupBlock& other) const;
    bool IsProperSubsetOf(const CVertexGroupBlock& other) const;
};
bool CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) const
{
    unsigned long count = Groups.size();
    if (count < other.Groups.size()) {
        const unsigned long* group = Groups.begin();
        for (unsigned long i = 0; i < count; ++i, ++group) {
            if (!other.HasGroup(*group))
                return false;
        }
        return true;
    }
    return false;
}
