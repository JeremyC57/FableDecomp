// std::list<CVertexGroupBlock>::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel,
// counting the nodes
// Leaf: ?size@?$list@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADB570, 17 bytes / 9 instructions, thiscall, plain `ret`; no callees.
// NF39 (2026-11-16): moved off its `SortTreeRecursively(Tree*)` stand-in (catalog label "CGuiControlTreePane::SortTreeRecursively", propagated) onto
//   the PDB spelling: FableWin.pdb (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv @0x01796f03 and @0x034fd3b0, 17 bytes / 9 insns) names
//   ?size@?$list@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QBEIXZ; the landed caller already binds that symbol; the element type
//   from the shared engine/CVertexGroupBlock.h (Ego_r.pdb CVertexGroupBlockOptimiser::Blocks is list<CVertexGroupBlock>).
// Model: the shared stlport/fable_containers.h list (STLport 4.6 stl/_list.h); (opt-ins FABLE_RETAIL_STLP_LIST + _SIZE_DISTANCE: STLport's own `distance(begin(), end())`); explicit instantiation emits the member as its own COMDAT, as
//   retail kept every used inline member.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

template unsigned int std::list<CVertexGroupBlock, std::allocator<CVertexGroupBlock> >::size() const;
