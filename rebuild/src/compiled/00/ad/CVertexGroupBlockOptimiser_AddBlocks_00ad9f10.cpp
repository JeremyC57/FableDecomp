// CVertexGroupBlockOptimiser::AddBlocks(unsigned long start) -- one step of the vertex-group block packing search: stop when over the iteration budget, greedy, or unable to beat the best list; record the current list as optimal when no block remains; otherwise seed an empty block with the best starting block, or try every remaining block that fits (sorted by cost when greedy) and recurse, or open a new block and recurse
// Grade: RELOCATION_MATCH -- InsertBlock 0x00AD92E0 real body visible in the TU (#pragma auto_inline(off), called out of line as in retail); the unsigned `space < 0` exit is a folded-away device for retail's list::size() inline budget: retail inlines only the top OptimalBlockList.size() (0x00AD9F42-0x00AD9F57) and calls list<CVertexGroupBlock>::size 0x00ADB570 out of line three times in the empty-candidate path (0x00ADA15A Blocks.size(), 0x00ADA163 and 0x00ADA172 OptimalBlockList.size()); without the exit VC7.1 inlines the first two of those (828v806). Original construct unknown
// Leaf: ?AddBlocks@CVertexGroupBlockOptimiser@@AAEXK@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9F10, 0x326 bytes, __fastcall, `ret 4`. Ego_r.pdb / FableWin.pdb ?AddBlocks@CVertexGroupBlockOptimiser@@AAEXK@Z (private).
// Callees: CalcCurrentBestTheoreticalResultNewBlock 0x00AD8FE0, CalcCurrentBestTheoreticalResultNewBlock2 0x00AD9D10, FindBestStartingBlock 0x00AD94F0,
//   CanInsertMoreBlocks 0x00AD8DD0, InsertBlock 0x00AD92E0 (real body in this TU, out of line; it calls CVertexGroupBlock::AddGroup 0x00AD91F0), CVertexGroupBlock::RemoveLast 0x00AD9470 / ctor 0x00AD9030 / dtor 0x00A96DA0,
//   map<unsigned long,long>::find 0x006699C0, list<CVertexGroupBlock>::size 0x00ADB570 / operator= 0x00ADD020 / push_back 0x00ADCE10 / pop_back 0x00ADC790,
//   vector<CBlockInfo>::reserve 0x00ADB960 / push_back 0x00ADBFB0, std::sort<CBlockInfo*> 0x00ADC890 (__fastcall under /Gr), free 0x00BFEA14.
// Shape: RemoveBlock (PDB ?RemoveBlock@CVertexGroupBlockOptimiser@@AAEXKK@Z, never emitted in TLC) is InsertBlock's inverse, inlined after each recursion;
//   the new-group counter is its own local (VC7.1 homes it in the dead `start` slot [esp+54h]) and InsertBlock's out-count a per-branch local.
//   InsertBlock's real body (landed 0x00AD92E0) sits in the TU out of line: with it visible VC7.1 knows `added` does not escape,
//   so RemoveBlock's count load is scheduled above the RemainingBlocks byte store with retail's registers.
//   find / reserve / push_back bodies are visible but out of line (VC7.1 then hoists &CurrentBlock->GroupToIndexMap and walks the group pointer).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#define FABLE_RETAIL_STLP_SORT2
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_REMOVELAST
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

class CVertexGroupBlockList {
public:
    unsigned long GetBlockCount() const { return Blocks.size(); }
    CVertexGroupBlock GetBlock(unsigned long index) const;
private:
    friend class CVertexGroupBlockOptimiser;
    CArray<CVertexGroupBlock> Blocks;              // +0x00
};

class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {
    public:
        unsigned long BlockIndex;
        long Cost;
        long Benefit;
        bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
    };
private:
    CVertexGroupBlockList        InputList;                   // +0x00
    std::list<CVertexGroupBlock> OptimalBlockList;            // +0x0C
    std::list<CVertexGroupBlock> Blocks;                      // +0x10
    CVertexGroupBlock*           CurrentBlock;                // +0x14
    CArray<bool>                 RemainingBlocks;             // +0x18
    CArray<CVertexGroupBlock>    CurrentBlockSegmentGroups;   // +0x24
    CArray<long>                 CurrentBlockSegmentIndices;  // +0x30
    unsigned long                CurrentBlockSegmentCount;    // +0x3C
    unsigned long                MaxGroupIndex;               // +0x40
    unsigned long                MaxN;                        // +0x44
    unsigned long                OptimalSize;                 // +0x48
    unsigned long                MaxIterations;               // +0x4C
    bool                         Greedy;                      // +0x50
    unsigned long                Iterations;                  // +0x54
    bool CanInsertMoreBlocks() const;                         // 0x00AD8DD0
    unsigned long CalcCurrentBestTheoreticalResultNewBlock(); // 0x00AD8FE0
    void InsertBlock(unsigned long index, unsigned long& added);   // 0x00AD92E0, PDB ?InsertBlock@CVertexGroupBlockOptimiser@@AAEXKAAK@Z (landed row spells `unsigned long*`: NAME_FIXES.tsv)
    unsigned long FindBestStartingBlock();                    // 0x00AD94F0
    unsigned long CalcCurrentBestTheoreticalResultNewBlock2();// 0x00AD9D10
    void RemoveBlock(unsigned long index, unsigned long count);
    // ?HasMoreBlocks@CVertexGroupBlockOptimiser@@ABE_NXZ (Ego_r.pdb 0x00C5BE00 keeps it out of line; FableWin.pdb has it too). Its body
    // is carried here because retail TLC has NO out-of-line copy (no manifest row, no call to one): AddBlocks expands it inline at
    // 0x00AD9F66-0x00AD9F99 -- InputList.GetBlockCount() as the (end-begin)/12 imul at 0x00AD9F66-0x00AD9F7D, then the remaining-block
    // byte loop `cmp byte [RemainingBlocks+i],0 ; jne` at 0x00AD9F90-0x00AD9F99 (true exits to 0x00AD9FB0, false falls into the
    // OptimalBlockList = Blocks copy at 0x00AD9F9B). So the class's in-class body (an implicit inline member) is what the TU saw.
    bool HasMoreBlocks() const
    {
        unsigned long count = InputList.GetBlockCount();
        for (unsigned long i = 0; i < count; ++i)
            if (RemainingBlocks[i])
                return true;
        return false;
    }
    void AddBlocks(unsigned long start);
};


#pragma auto_inline(off)
void CVertexGroupBlockOptimiser::InsertBlock(unsigned long index, unsigned long& added)
{
    RemainingBlocks[index] = false;
    added = 0;
    const CVertexGroupBlock& block = InputList.Blocks[index];
    for (unsigned long i = 0; i < block.GetGroupCount(); ++i) {
        unsigned long group = block.GetGroup(i);
        if (!CurrentBlock->HasGroup(group)) {
            CurrentBlock->AddGroup(group);
            ++added;
        }
    }
}
#pragma auto_inline(on)

void CVertexGroupBlockOptimiser::RemoveBlock(unsigned long index, unsigned long count)
{
    RemainingBlocks[index] = true;
    CurrentBlock->RemoveLast(count);
}

void CVertexGroupBlockOptimiser::AddBlocks(unsigned long start)
{
    ++Iterations;
    if (!OptimalBlockList.empty()) {
        if (Iterations > MaxIterations || Greedy)
            return;
        if (CalcCurrentBestTheoreticalResultNewBlock() >= OptimalBlockList.size())
            return;
    }
    if (!HasMoreBlocks()) {
        OptimalBlockList = Blocks;
        return;
    }
    unsigned long space = MaxN - CurrentBlock->GetGroupCount();
    if (space < 0)      // always false (unsigned), folded away; it keeps the three empty-path size() calls
        return;         // out of line (call 0x00ADB570 at 0x00ADA15A / 0x00ADA163 / 0x00ADA172) as retail, top size() inline
    if (CurrentBlock->GetGroupCount() == 0) {
        unsigned long best = FindBestStartingBlock();
        unsigned long added;
        InsertBlock(best, added);
        AddBlocks(0);
        RemoveBlock(best, added);
        return;
    }
    CArray<CBlockInfo> candidates;
    candidates.reserve(InputList.GetBlockCount());
    for (unsigned long j = start; j < InputList.GetBlockCount(); ++j) {
        if (RemainingBlocks[j]) {
            const CVertexGroupBlock& block = InputList.Blocks[j];
            unsigned long count = block.GetGroupCount();
            unsigned long newGroups = 0;
            for (unsigned long k = 0; k < count; ++k)
                if (!CurrentBlock->HasGroup(block.GetGroup(k)))
                    ++newGroups;
            if (newGroups <= space) {
                CBlockInfo info;
                info.BlockIndex = j;
                info.Cost = newGroups;
                info.Benefit = count;
                candidates.push_back(info);
            }
        }
    }
    if (Greedy)
        std::sort(candidates.begin(), candidates.end());
    if (candidates.empty()) {
        if (!CanInsertMoreBlocks()) {
            if (!OptimalBlockList.empty()) {
                if (Blocks.size() >= OptimalBlockList.size())
                    return;
                if (CalcCurrentBestTheoreticalResultNewBlock2() >= OptimalBlockList.size())
                    return;
            }
            CVertexGroupBlock* saved = CurrentBlock;
            Blocks.push_back(CVertexGroupBlock());
            CurrentBlock = &Blocks.back();
            AddBlocks(0);
            Blocks.pop_back();
            CurrentBlock = saved;
        }
    } else {
        unsigned long n = candidates.size();
        for (unsigned long c = 0; c < n; ++c) {
            unsigned long added;
            InsertBlock(candidates[c].BlockIndex, added);
            unsigned long next = 0;
            if (!Greedy)
                next = candidates[c].BlockIndex + 1;
            AddBlocks(next);
            RemoveBlock(candidates[c].BlockIndex, added);
        }
    }
}