// CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) -- every group of this block is in `other` and `other` has strictly more groups
// Grade: RELOCATION_MATCH -- same levers as IsSubsetOf 0x00AD8C30: named iterator local + Groups[i] index form.
// Leaf: ?IsProperSubsetOf@CVertexGroupBlock@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD8CA0, length 105 bytes (padded to 112), `ret 4`.
// Callees: map<unsigned long, long>::find(iterator* result, const unsigned long& key) @ 0x006699C0 (landed as
// CRetailTree<ulong key>::find; header at the map's +0, end() re-read after the call).
// Layout (retail; PDB 32 bytes with debug containers): Groups CArray<unsigned long> +0x00 (12-byte vector), GroupToIndexMap +0x0c.
// Shape: sizes compared unsigned (`sar 2` then `ja`/`jae`); the group key is a named local homed in the dead `other`
// argument slot and passed to find by address; the result is a named iterator (its storage is the hidden-result slot,
// re-read after the call: `mov eax,[ebp] ; cmp [esp+10h],eax`) compared with the header; the loop indexes Groups[i].
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
    SGroupMapIterator(SGroupMapNode* n) : Node(n) {}
};
struct CGroupToIndexMap {                          // map<unsigned long, long> (retail 8 bytes: header, count)
    SGroupMapNode* Header;                         // +0x00
    unsigned long  NodeCount;                      // +0x04
    SGroupMapIterator find(const unsigned long& key) const;   // 0x006699C0
};
#pragma auto_inline(off)
// Same-TU body of the map<unsigned long, long>::find instantiation 0x006699C0 (landed text): lower-bound walk from
// the root, then end() unless the key matches.
__declspec(noinline) SGroupMapIterator CGroupToIndexMap::find(const unsigned long& key) const
{
    SGroupMapNode* result = Header;
    SGroupMapNode* n = Header->Parent;
    while (n) {
        if (!(n->Key < key)) {
            result = n;
            n = n->Left;
        }
        else
            n = n->Right;
    }
    SGroupMapNode* found = result;
    if (found == Header || key < found->Key)
        found = Header;
    return SGroupMapIterator(found);
}
#pragma auto_inline(on)
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
        SGroupMapIterator it = GroupToIndexMap.find(group);
        return it.Node != GroupToIndexMap.Header;
    }
    bool IsSubsetOf(const CVertexGroupBlock& other) const;
    bool IsProperSubsetOf(const CVertexGroupBlock& other) const;
};
bool CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) const
{
    unsigned long count = Groups.size();
    if (count < other.Groups.size()) {
        for (unsigned long i = 0; i < count; ++i) {
            if (!other.HasGroup(Groups[i]))
                return false;
        }
        return true;
    }
    return false;
}