// CVertexGroupBlockOptimiser::ReorderInput(const CVertexGroupBlockList& solution) -- rebuild the input list in solution order: for each solution block in turn, every input block not yet taken that is a subset of it, in input order
// Inline-budget: header opt-ins that set expanded vs called, each removed singly from this source: FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE (the const map find, visible but out of line, as retail's IsSubsetOf 0x00AD8C30 calls it at 0x006699C0: removed, DIFFER(410v416); plain _TREE_CONSTFIND keeps this row RELOCATION_MATCH but expands find into the same-TU IsSubsetOf COMDAT, 160 B DIFFER against retail's 108 B, while with NOINLINE the COMDAT is 108 B and byte-equal, relocations only), FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE (map copy called at 0x00AD9A4F, 0x00A96E40: removed, DIFFER(416v416)), FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL (the range destroy called at 0x00AD9B04, 0x00ADC190: removed, the loop is expanded, DIFFER(422v416)), FABLE_RETAIL_STLP_VECTOR_C (vector::operator= 0x00ADCA30 called at 0x00AD9AE1: removed, the assignment becomes a word copy, DIFFER(416v416)). No flag, no pragma.
// Leaf: ?ReorderInput@CVertexGroupBlockOptimiser@@QAEXABVCVertexGroupBlockList@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9980, 416 bytes, `ret 4`.
// Naming: FableWin.pdb (lib_vertex_group_block.obj #25) / Ego_r.pdb / Ego_d.pdb ?ReorderInput@CVertexGroupBlockOptimiser@@QAEXABVCVertexGroupBlockList@@@Z
//   (the catalogue's CVertexGroupBlockList::FilterBySubset was an autoname: NAME_FIXES.tsv). InputList is the optimiser's first member, so
//   `this` addresses the block list directly. Locals as Ego_d: input_size, remaining_input, new_input_list, c_order, c_input.
//   FableWin.pdb puts it in the same object as IsSubsetOf (#9) and RemoveAllSubsets (#18, landed 0x00AD9770), which this TU follows.
// Callees: CVertexGroupBlock::IsSubsetOf 0x00AD8C30 (landed; body below), CArray<bool>(size_type, const bool&, const allocator&) 0x00451583,
//   vector<unsigned long>::vector(const vector&) 0x00535470 and map<unsigned long, long>::map(const map&) 0x00A96E40 (the block copy inside
//   the inline AddBlock / push_back), vector<CVertexGroupBlock>::_M_insert_overflow 0x00ADCE60, vector<CVertexGroupBlock>::operator=
//   0x00ADCA30, std::__destroy_aux 0x00ADC190 (__fastcall) and free 0x00BFEA14 (the two local arrays' destructors).
// Shape: IsSubsetOf's body is visible, so VC7.1 knows the call writes nothing: InputList[c_input] is computed once into esi and reused
//   by the push_back (0x00AD9A23 / 0x00AD9A40), and `this` is spilled once (0x00AD99B2) instead of being re-read through a register.
//   IsSubsetOf declaration-only (no same-TU body): DIFFER(410v416); IsSubsetOf is called in every variant.
// Inferred: new_input_list is declared before remaining_input (Ego_d, a later revision, has them the other way round). Two retail sites
//   prove the order: new_input_list's three words are zeroed at 0x00AD99B6 / 0x00AD99BA / 0x00AD99BE, before the CArray<bool> constructor
//   call at 0x00AD99C7; and remaining_input is freed first (`push eax ; call free` at 0x00AD9AEE / 0x00AD9AEF), before new_input_list's
//   __destroy_aux (0x00AD9B04) and free (0x00AD9B0E). So new_input_list is the older object. Ego_d's order: DIFFER(416v416).
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_ISSUBSETOF
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"
// Fidelity: same-TU strong definition of 0x00AD8C30 (retail TU-mate)
bool CVertexGroupBlock::IsSubsetOf(const CVertexGroupBlock& other) const
{
    unsigned long count = GetGroupCount();
    if (count <= other.GetGroupCount()) {
        for (unsigned long i = 0; i < count; ++i) {
            if (!other.HasGroup(GetGroup(i)))
                return false;
        }
        return true;
    }
    return false;
}
// No shared header: FableWin.pdb / Ego_d.pdb CVertexGroupBlockList (GetBlockCount, both operator[], AddBlock), one CArray<CVertexGroupBlock>
// (the landed RemoveAllSubsets 0x00AD9770 / HasBlock 0x00AD8D10 model).
class CVertexGroupBlockList {
public:
    unsigned long GetBlockCount() const { return Blocks.size(); }
    CVertexGroupBlock& operator[](unsigned long index) { return Blocks[index]; }
    const CVertexGroupBlock& operator[](unsigned long index) const { return Blocks[index]; }
    void AddBlock(const CVertexGroupBlock& block) { Blocks.push_back(block); }
private:
    CArray<CVertexGroupBlock> Blocks;              // +0x00
};
// The optimiser's retail layout as the landed AddBlocks 0x00AD9F10 models it (Ego_r.pdb member names; STLport containers).
class CVertexGroupBlockOptimiser {
public:
    void ReorderInput(const CVertexGroupBlockList& solution);
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
};
void CVertexGroupBlockOptimiser::ReorderInput(const CVertexGroupBlockList& solution)
{
    unsigned long input_size = InputList.GetBlockCount();
    CVertexGroupBlockList new_input_list;
    CArray<bool> remaining_input(input_size, true);
    for (unsigned long c_order = 0; c_order < solution.GetBlockCount(); ++c_order) {
        for (unsigned long c_input = 0; c_input < input_size; ++c_input) {
            if (remaining_input[c_input] && InputList[c_input].IsSubsetOf(solution[c_order])) {
                new_input_list.AddBlock(InputList[c_input]);
                remaining_input[c_input] = false;
            }
        }
    }
    InputList = new_input_list;
}