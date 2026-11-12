// CVertexGroupBlockList::RemoveAllSubsets() -- drop every block that is a proper subset of another block: the others are copied in
// order into a new array, which is then assigned back.
// Leaf: ?RemoveAllSubsets@CVertexGroupBlockList@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9770, 280 bytes.
// Naming: FableWin.pdb / Ego_r.pdb ?RemoveAllSubsets@CVertexGroupBlockList@@QAEXXZ; GetBlockCount is the PDB's inline accessor.
// Callees: CVertexGroupBlock::IsProperSubsetOf 0x00AD8CA0 (landed), vector<unsigned long>::vector(const vector&) 0x00535470 and
//   map<unsigned long, long>::map(const map&) 0x00A96E40 (the block's member-wise copy, inside the inline push_back),
//   vector<CVertexGroupBlock>::_M_insert_overflow 0x00ADCE60, vector<CVertexGroupBlock>::operator= 0x00ADCA30,
//   std::__destroy_aux 0x00ADC190 (/Gr COMDAT) and free 0x00BFEA14 (the local array's destructor).
// Shape: what the compiler may assume about each callee decides the registers. IsProperSubsetOf's body is visible (same source file,
//   defined first), so the block array's start is not re-read inside the inner loop (ebp walks it); operator= and _M_insert_overflow
//   are STLport header bodies the TU sees (called out of line), so the local array never escapes: its finish pointer lives in ebp
//   across the element copies and its start / finish survive the assignment into the destructor (0x00AD9855 / 0x00AD986A).
//   The inner loop's bound is a local copy of the count; the subset flag folds into the two exits.
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"
// The landed 0x00AD8CA0 body (same retail source file, defined before this function): visible to the compiler but out of line, so
// it knows the call does not write the block array.
#pragma auto_inline(off)
bool CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) const
{
    unsigned long count = GetGroupCount();
    if (count < other.GetGroupCount()) {
        for (unsigned long i = 0; i < count; ++i) {
            if (!other.HasGroup(GetGroup(i)))
                return false;
        }
        return true;
    }
    return false;
}
#pragma auto_inline(on)
// No shared header: FableWin.pdb / Ego_r.pdb CVertexGroupBlockList (AddBlock, Clear, GetBlock, GetBlockCount, HasBlock, RemoveAllSubsets),
// one CArray<CVertexGroupBlock> (the landed HasBlock 0x00AD8D10 / push_back 0x00AD9720 rows).
class CVertexGroupBlockList {
public:
    unsigned long GetBlockCount() const { return Blocks.size(); }
    void RemoveAllSubsets();
private:
    CArray<CVertexGroupBlock> Blocks;              // +0x00
};
void CVertexGroupBlockList::RemoveAllSubsets()
{
    CArray<CVertexGroupBlock> new_blocks;
    for (unsigned long i = 0; i < GetBlockCount(); ++i) {
        const CVertexGroupBlock& block = Blocks[i];
        bool is_subset = false;
        unsigned long count = GetBlockCount();
        for (unsigned long j = 0; j < count; ++j) {
            if (block.IsProperSubsetOf(Blocks[j])) {
                is_subset = true;
                break;
            }
        }
        if (!is_subset)
            new_blocks.push_back(block);
    }
    Blocks = new_blocks;
}