// CVertexGroupBlock::~CVertexGroupBlock() -- GroupToIndexMap (+0x0C) cleared and its head node freed, then Groups (+0) freed
// Leaf: ??1CVertexGroupBlock@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96DA0.
// Callees: the map's out-of-line erase @ 0x004507A1, free @ 0x00BFEA14 twice.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CEntryListBuffer_00a96da0) onto the shared engine/CVertexGroupBlock.h
// (opt-in FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS; the header names ~CVertexGroupBlock 0x00A96DA0): Ego_r.pdb 0x00BD5B30 + FableWin.pdb
// ??1CVertexGroupBlock@@QAE@XZ; members Groups (CArray<unsigned long>) +0 and GroupToIndexMap (std::map<unsigned long,long>) +0x0C.
// Include chain from the landed 0x00AD9770 CVertexGroupBlockList::RemoveAllSubsets.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

CVertexGroupBlock::~CVertexGroupBlock()
{
}
