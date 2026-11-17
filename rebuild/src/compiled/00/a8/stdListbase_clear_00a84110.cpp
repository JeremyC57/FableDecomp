// CRetailList_ListOfCountedPointer::clear() -- destroy every node's inner list (+0xc: clear it, free its head node), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84110, length 80 bytes.
// Callees: the inner list's clear @ 0x00A80A20 (the landed CRetailList_CountedPointer::clear), free @ 0x00BFEA14 (the inner head, then the node). Shape: the landed list clear @ 0x00A80A20 with the inner list's
// inlined destructor (its address kept in edi for the clear call, its head re-read through edi and null-tested before the free). The chunk label "CGuiControlTreePane::SortTreeRecursively"
// is a propagated STL label.
// NF50 (round 7): moved onto the shared headers. The node value at +8 is Ego_r.pdb CNavRegion (RegionID +0, Connections +4 =
// list<CCountedPointer<CNavRegionConnection> >, MostRecentSearchID +8; engine/CNavRegion.h) and the caller CNavQuadTree::~CNavQuadTree
// 0x00A7A7F0 passes Ego_r.pdb CNavQuadTree::Regions = list<CNavRegion>, so this is STLport 4.6 _list.c _List_base<CNavRegion>::clear
// (descriptive-by-library). FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS gives the plain (non-inline) template member, so the inlined
// CNavRegion destructor's Connections.~list calls the 0x00A80A20 instantiation out of line, binding
// ?clear@?$_List_base@V?$CCountedPointer@VCNavRegionConnection@@@@V?$allocator@V?$CCountedPointer@VCNavRegionConnection@@@@@std@@@std@@QAEXXZ
// (nf41 e175). The landed callers 0x00A7A7F0 / 0x00A84640 / 0x00A84BE0 still bind three stand-in spellings (NF50 NOTES).
// Leaf: ?clear@?$_List_base@VCNavRegion@@V?$allocator@VCNavRegion@@@std@@@std@@QAEXXZ
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS
#include "engine/CNavRegion.h"
namespace std {
template void _List_base<CNavRegion, allocator<CNavRegion> >::clear();
}
