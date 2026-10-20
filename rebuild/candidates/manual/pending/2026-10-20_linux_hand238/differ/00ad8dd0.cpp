// Grade: DIFFER(240v243) -- register allocation of the outer loop: retail keeps the input count in ebp and spills i/block (edi/esi) around the inner find loop; ours keeps the count in a frame word
// CVertexGroupBlockOptimiser::CanInsertMoreBlocks() const -- true when some remaining input block would add no more new groups to the current block than it still has room for (MaxN less its group count)
// Leaf: CanInsertMoreBlocks
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
    unsigned long             MaxGroupIndex;                      // +0x40
    unsigned long             MaxN;                               // +0x44
    bool CanInsertMoreBlocks() const;
};
bool CVertexGroupBlockOptimiser::CanInsertMoreBlocks() const
{
    unsigned long space = MaxN - CurrentBlock->Groups.size();
    for (unsigned long i = 0; i < InputList.size(); ++i) {
        if (RemainingBlocks[i]) {
            const CVertexGroupBlock& block = InputList[i];
            unsigned long newGroups = 0;
            unsigned long n = block.Groups.size();
            for (const unsigned long* group = block.Groups.begin(); n > 0; --n, ++group) {
                if (CurrentBlock->GroupToIndexMap.find(*group) == CurrentBlock->GroupToIndexMap.Header)
                    ++newGroups;
            }
            if (newGroups <= space)
                return true;
        }
    }
    return false;
}
