// CVertexGroupBlockOptimiser::InsertBlock(unsigned long index, unsigned long* added) -- take input block `index` off the remaining list and merge its groups into the current block, counting the groups that were new
// Leaf: ?InsertBlock@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD92E0, length 153 bytes (padded to 160), `ret 8`.
// Callees: CVertexGroupBlock::AddGroup(unsigned long) @ 0x00AD91F0 (catalog "AnimBlockGroup_AddBlock").
// Layout (retail; PDB offsets differ by the debug containers): InputList +0x00 (vector of 0x18-byte CVertexGroupBlock),
// OptimalBlockList +0x0c, Blocks +0x10, CurrentBlock +0x14, RemainingBlocks +0x18 (bytes), CurrentBlockSegmentGroups +0x24,
// CurrentBlockSegmentIndices +0x30, CurrentBlockSegmentCount +0x3c.
// Shape: the block pointer is kept in the dead `index` argument slot; the map find is the inline STLport walk.
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
class CVertexGroupBlock { public:                         // retail 0x18 bytes (PDB 32: debug containers)
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
void CVertexGroupBlockOptimiser::InsertBlock(unsigned long index, unsigned long* added)
{
    RemainingBlocks[index] = false;
    *added = 0;
    const CVertexGroupBlock& block = InputList[index];
    for (unsigned long i = 0; i < block.Groups.size(); ++i) {
        unsigned long group = block.Groups[i];
        if (CurrentBlock->GroupToIndexMap.find(group) == CurrentBlock->GroupToIndexMap.Header) {
            CurrentBlock->AddGroup(group);
            ++*added;
        }
    }
}