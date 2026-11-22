// CVertexGroupBlock::~CVertexGroupBlock() -- GroupToIndexMap (+0x0C) cleared and its head node freed, then Groups (+0) freed
// Leaf: ??1CVertexGroupBlock@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96DA0.
// Callees: the map's out-of-line erase @ 0x004507A1, free @ 0x00BFEA14 twice.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CEntryListBuffer_00a96da0) onto the shared engine/CVertexGroupBlock.h
// (opt-in FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS; the header names ~CVertexGroupBlock 0x00A96DA0): Ego_r.pdb 0x00BD5B30 + FableWin.pdb
// ??1CVertexGroupBlock@@QAE@XZ; members Groups (CArray<unsigned long>) +0 and GroupToIndexMap (std::map<unsigned long,long>) +0x0C.
// Include chain from the landed 0x00AD9770 CVertexGroupBlockList::RemoveAllSubsets.
// NF275 (2026-11-22, nf273 ruling 2b): the destructor is the header's inline body (FABLE_RETAIL_CVERTEXGROUPBLOCK_DTOR_INLINE), emitted here
//   as a pick-any COMDAT like every includer's copy, so the self row no longer defines it strongly against the inline copies of
//   0x00AD94F0 / 0x00ADCE60 (LNK4006).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_DTOR_INLINE
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline destructor (a local block built by the out-of-line
// constructor 0x00AD9030 and destroyed at scope end), so this object emits ??1CVertexGroupBlock@@QAE@XZ as a pick-any COMDAT, as
// retail's 0x00A96DA0 is. Internal to this TU; never called.
void ReferenceVertexGroupBlockDtor()
{
    CVertexGroupBlock block;
}
}
