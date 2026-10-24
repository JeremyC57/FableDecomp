// CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock() -- lower bound on the block count when the current block is closed: the existing blocks plus the blocks the remaining groups need after filling the current block's free slots
// Leaf: ?CalcCurrentBestTheoreticalResultNewBlock@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD8FE0, 0x43 bytes (+ int3 padding), thiscall, plain `ret`, unsigned long.
// Identity: Ego_r.pdb ?CalcCurrentBestTheoreticalResultNewBlock@CVertexGroupBlockOptimiser@@AAEKXZ (no-argument private
// unsigned long). Its only caller is AddBlocks 0x00AD9F10, which calls this one first and the segment-based sibling
// 0x00AD9D10 later, so the NewBlock / NewBlock2 assignment follows that call order (the two bodies are not otherwise
// told apart by the PDB). Catalog "VertexGroup_CountComponents" is descriptive.
// Callee: CountRemainingGroups() const @ 0x00AD8ED0 (thiscall, returns the number of groups still unplaced).
// Layout (retail, as the landed CalcCurrentBestTheoreticalResult 0x00AD9D70): Blocks list +0x10 (STLport list, 4-byte
// node pointer), CurrentBlock +0x14 (Groups vector at +0), MaxN +0x44.
// Shape: the count of remaining groups first (held in eax across the arithmetic), `free = MaxN - CurrentBlock size`
// (unsigned, `sar 2`), the STLport list::size() distance loop over Blocks, then `blocks + (MaxN - free + remaining - 1)
// / MaxN` with MaxN re-read for the unsigned `div`.
class CVertexGroupBlock {
public:
    unsigned long* m_GroupsFirst;                              // +0x00 Groups (CArray<unsigned long>)
    unsigned long* m_GroupsLast;                               // +0x04
    unsigned long* m_GroupsEnd;                                // +0x08
    unsigned long GroupCount() const { return (unsigned long)(m_GroupsLast - m_GroupsFirst); }
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
    unsigned char      Remaining_to_Indices[0x24];             // +0x18 RemainingBlocks, CurrentBlockSegmentGroups, CurrentBlockSegmentIndices
    unsigned long      CurrentBlockSegmentCount;               // +0x3C
    unsigned long      MaxGroupIndex;                          // +0x40
    unsigned long      MaxN;                                   // +0x44

    unsigned long CountRemainingGroups() const;                // 0x00AD8ED0
    unsigned long CalcCurrentBestTheoreticalResultNewBlock();
};

unsigned long CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResultNewBlock()
{
    unsigned long remaining = CountRemainingGroups();
    unsigned long free = MaxN - CurrentBlock->GroupCount();
    unsigned long blocks = Blocks.size();
    return blocks + (MaxN - free + remaining - 1) / MaxN;
}