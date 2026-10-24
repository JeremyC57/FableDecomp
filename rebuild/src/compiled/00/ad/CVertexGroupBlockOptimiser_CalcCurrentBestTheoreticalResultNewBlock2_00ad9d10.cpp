// CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock2() -- recompute the block segments, then a lower bound on the block count: the existing blocks plus, per segment, the blocks its groups need when every new block shares one group
// Leaf: ?CalcCurrentBestTheoreticalResultNewBlock2@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9D10, 0x4C bytes (+ int3 padding), thiscall, plain `ret`, unsigned long.
// Identity: Ego_r.pdb ?CalcCurrentBestTheoreticalResultNewBlock2@CVertexGroupBlockOptimiser@@AAEKXZ; the only caller is
// AddBlocks 0x00AD9F10, which calls the CountRemainingGroups-based sibling 0x00AD8FE0 first and this one later (NewBlock /
// NewBlock2 assigned by that order). It recomputes the segments like the landed CalcCurrentBestTheoreticalResult
// 0x00AD9D70. Catalog "VertexGroupBlockOptimiser_CountComponents" is descriptive.
// Callee: CalcCurrentBlockSegments() @ 0x00AD9B20.
// Layout (retail, as 0x00AD9D70): Blocks list +0x10, CurrentBlockSegmentGroups +0x24 (vector of 0x18-byte
// CVertexGroupBlock, Groups vector at +0), CurrentBlockSegmentCount +0x3C, MaxN +0x44.
// Shape: STLport list::size() distance loop for the start value; the segment loop is strength-reduced (element
// pointer + `dec esi` down-counter, MaxN and MaxN - 1 hoisted into edi/ebx); each step adds
// `(groupCount + MaxN - 3) / (MaxN - 1)` (unsigned `div`, `lea eax,[eax+edi-3]`).
class CVertexGroupBlock {                                      // 0x18 bytes in retail
public:
    unsigned long* m_GroupsFirst;                              // +0x00 Groups (CArray<unsigned long>)
    unsigned long* m_GroupsLast;                               // +0x04
    unsigned long* m_GroupsEnd;                                // +0x08
    unsigned char  GroupToIndexMap[0x0C];                      // +0x0C map<unsigned long, long>
    unsigned long GroupCount() const { return (unsigned long)(m_GroupsLast - m_GroupsFirst); }
};

struct CBlockArray {                                           // CArray<CVertexGroupBlock> (retail STLport vector)
    CVertexGroupBlock* _M_start;
    CVertexGroupBlock* _M_finish;
    CVertexGroupBlock* _M_end_of_storage;
    CVertexGroupBlock& operator[](unsigned long n) const { return *(_M_start + n); }
};

struct CBlockNode { CBlockNode* _M_next; CBlockNode* _M_prev; };
struct CBlockList {                                            // list<CVertexGroupBlock> (STLport, 4 bytes)
    CBlockNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (CBlockNode* it = _M_node->_M_next; it != _M_node; it = it->_M_next)
            ++n;
        return n;
    }
};

class CVertexGroupBlockOptimiser {
public:
    unsigned char      InputList_OptimalBlockList[0x10];        // +0x00 InputList (12), OptimalBlockList (4)
    CBlockList         Blocks;                                 // +0x10
    CVertexGroupBlock* CurrentBlock;                           // +0x14
    unsigned char      RemainingBlocks[0x0C];                  // +0x18
    CBlockArray        CurrentBlockSegmentGroups;              // +0x24
    unsigned char      CurrentBlockSegmentIndices[0x0C];       // +0x30
    unsigned long      CurrentBlockSegmentCount;               // +0x3C
    unsigned long      MaxGroupIndex;                          // +0x40
    unsigned long      MaxN;                                   // +0x44

    void CalcCurrentBlockSegments();                           // 0x00AD9B20
    unsigned long CalcCurrentBestTheoreticalResultNewBlock2();
};

unsigned long CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock2()
{
    CalcCurrentBlockSegments();
    unsigned long result = Blocks.size();
    for (unsigned long i = 0; i < CurrentBlockSegmentCount; ++i)
        result += (CurrentBlockSegmentGroups[i].GroupCount() + MaxN - 3) / (MaxN - 1);
    return result;
}