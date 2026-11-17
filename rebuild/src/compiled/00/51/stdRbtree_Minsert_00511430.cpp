// std::_Rb_tree<long, long, _Identity<long >, less<long >, allocator<long > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JJU?$_Identity@J@std@@U?$less@J@2@V?$allocator@J@2@@std@@AAE?AU?$_Rb_tree_iterator@JU?$_Nonconst_traits@J@std@@@2@PAU_Rb_tree_node_base@2@0ABJ0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00511430, 179 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the set<long> tree; Ego_r.pdb members of this type: CEditWorldMap::MapsAlteredByPen,
//   CGameTimeManager::StopTimeIndexes, CMapDesc::VisibleMaps, CTCCameraPointScriptedSpline::ValidAnimList (+3 more). Landed callers of
//   this address: 0x00512DA0 std::_Rb_tree<long,long,std::_Identity<long >,std::less<long >,std::allocator<lo.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CMemoryAllocatorVariableSize::InsertNode`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<long, long, _Identity<long >, less<long >, allocator<long > >::iterator _Rb_tree<long, long, _Identity<long >, less<long >, allocator<long > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const long&, _Rb_tree_node_base*);
}
