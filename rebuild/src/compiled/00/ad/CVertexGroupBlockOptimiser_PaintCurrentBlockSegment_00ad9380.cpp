// CVertexGroupBlockOptimiser::PaintCurrentBlockSegment(unsigned long index) -- flood-fill one segment: input block `index` joins the current segment (its index recorded, its groups merged into the segment's last block), then every still-remaining, unassigned input block that intersects the segment is painted recursively
// Leaf: ?PaintCurrentBlockSegment@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD9380, length 130 bytes, `ret 4`.
// Callees: CVertexGroupBlock::AddGroupsFrom(const CVertexGroupBlock&) @ 0x00AD9260, CVertexGroupBlock::HasIntersection(const
// CVertexGroupBlock&) const @ 0x00AD8AF0 (catalog "TreeNodes_HaveIntersection"), itself (recursion).
// Layout (retail): InputList +0x00 (0x18-byte blocks), RemainingBlocks +0x18 (bytes), CurrentBlockSegmentGroups +0x24 (end +0x28),
// CurrentBlockSegmentIndices +0x30/+0x34, CurrentBlockSegmentCount +0x3c.
// Shape: the segment's back() block is taken once into a local (`end - 0x18`); the loop bound is recomputed after each call.
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
    bool HasIntersection(const CVertexGroupBlock& other) const;   // 0x00AD8AF0
    void AddGroup(unsigned long group);            // 0x00AD91F0
    void AddGroupsFrom(const CVertexGroupBlock& other);           // 0x00AD9260
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    T* begin() const { return m_first; }
    T* end() const { return m_last; }
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    T& back() const { return *(end() - 1); }
};
struct CVertexGroupBlockOptimiser {                // retail layout (PDB 128 bytes with debug containers)
    CRetailVector<CVertexGroupBlock> InputList;    // +0x00 (CVertexGroupBlockList)
    void*                     OptimalBlockList;    // +0x0c (STLport list, 4 bytes)
    void*                     Blocks;              // +0x10 (STLport list, 4 bytes)
    CVertexGroupBlock*        CurrentBlock;        // +0x14
    CRetailVector<bool>       RemainingBlocks;     // +0x18 (one byte per block in retail)
    CRetailVector<CVertexGroupBlock> CurrentBlockSegmentGroups;   // +0x24
    CRetailVector<long>       CurrentBlockSegmentIndices;         // +0x30
    unsigned long             CurrentBlockSegmentCount;           // +0x3c
    void InsertBlock(unsigned long index, unsigned long* added);
    void PaintCurrentBlockSegment(unsigned long index);
};
void CVertexGroupBlockOptimiser::PaintCurrentBlockSegment(unsigned long index)
{
    CurrentBlockSegmentIndices[index] = CurrentBlockSegmentCount;
    CVertexGroupBlock& segment = CurrentBlockSegmentGroups.back();
    segment.AddGroupsFrom(InputList[index]);
    for (unsigned long i = 0; i < CurrentBlockSegmentIndices.size(); ++i) {
        if (RemainingBlocks[i] && CurrentBlockSegmentIndices[i] == -1 && segment.HasIntersection(InputList[i]))
            PaintCurrentBlockSegment(i);
    }
}