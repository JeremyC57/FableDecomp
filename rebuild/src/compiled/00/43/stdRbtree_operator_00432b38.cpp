// std::_Rb_tree<CCharString, std::pair<const CCharString, long>, std::_Select1st<...>, std::less<CCharString>, std::allocator<...> >::operator=(const _Rb_tree&) -- STLport tree assignment of map<CCharString, long>: unless self-assigned, clear(), then reset the header to empty or clone the source root with _M_copy and re-derive the leftmost / rightmost links; copy the count and the comparator
// Leaf: ??4?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00432B38, 115 bytes; size-optimised TU (`and dword ptr [..],0`).
// Identity: STLport 4.6 _tree.c operator= (no PDB copy: both PDBs are Dinkumware; descriptive-by-library). STLport's map::operator= is
//   implicit and inline (`_M_t = x._M_t`), so the out-of-line COMDAT is the tree's; the landed 0x004321F0 / 0x0046CE2D (and, after
//   NF108's ports, 0x0045C54E multimap / 0x004E4450 / 0x004E5C89 / 0x00675C00 / 0x00A2CF50) all call it for map<CCharString, long> members.
// Callees: the tree's clear() 0x0043027A (out of line in this size TU), _M_copy 0x00432BAB (landed with the PDB-style spelling).
// NF108 (2026-11-19, q6236): re-authored on the shared stlport/fable_map.h as the explicit instantiation, the 0x00432A35 recipe
//   (FABLE_RETAIL_STLP_TREE_ASSIGN_BODY under the size pragma); it was the CRetailTree_00432bab stand-in (catalog "CRetailTree_00432bab::operator=").
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"

namespace std {
typedef _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >, less<CCharString>,
                 allocator<pair<const CCharString, long> > > TCharStringLongTree;
template TCharStringLongTree& TCharStringLongTree::operator=(const TCharStringLongTree&);
}
