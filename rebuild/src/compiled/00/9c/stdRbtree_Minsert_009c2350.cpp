// std::_Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity<CDisplayModeDesc >, less<CDisplayModeDesc >, allocator<CDisplayModeDesc > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCDisplayModeDesc@@V1@U?$_Identity@VCDisplayModeDesc@@@std@@U?$less@VCDisplayModeDesc@@@3@V?$allocator@VCDisplayModeDesc@@@3@@std@@AAE?AU?$_Rb_tree_iterator@VCDisplayModeDesc@@U?$_Nonconst_traits@VCDisplayModeDesc@@@std@@@2@PAU_Rb_tree_node_base@2@0ABVCDisplayModeDesc@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2350, 225 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the set<CDisplayModeDesc> tree; no Ego_r.pdb member of this exact type; its two sources: the landed
//   insert_unique 0x009C2800 (CDisplayManager::EnumerateDisplayModes) binds this set<CDisplayModeDesc> spelling, and the out-of-line
//   comparator 0x009C1860 is landed as std::less<CDisplayModeDesc>::operator() (the CDisplayModeDesc ordering); the 0x20-byte node =
//   0x10 links + the 16-byte CDisplayModeDesc. Landed callers of this address: 0x009C2800 std::_Rb_tree<CDisplayModeDesc,
//   CDisplayModeDesc, std::_Identity<CDisplayModeDes.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CMemoryAllocatorVariableSize::InsertNode_24byte`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CDisplayModeDesc.h"
namespace std {
// std::less<CDisplayModeDesc>::operator() is the out-of-line COMDAT 0x009C1860 (landed under that STLport name; it expands the
// CDisplayModeDesc ordering): declared only here, as the retail _M_insert calls it (the 0x004759AD explicit-specialisation device).
template<> struct less<CDisplayModeDesc> {
    bool operator()(const CDisplayModeDesc& a, const CDisplayModeDesc& b) const;   // 0x009C1860
};
}
namespace std {
template _Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity<CDisplayModeDesc >, less<CDisplayModeDesc >, allocator<CDisplayModeDesc > >::iterator _Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity<CDisplayModeDesc >, less<CDisplayModeDesc >, allocator<CDisplayModeDesc > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, CDisplayModeDesc const&, _Rb_tree_node_base*);
}
