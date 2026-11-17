// std::_Rb_tree<CTCMapwho*, CTCMapwho*, _Identity<CTCMapwho* >, less<CTCMapwho* >, allocator<CTCMapwho* > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@PAVCTCMapwho@@PAV1@U?$_Identity@PAVCTCMapwho@@@std@@U?$less@PAVCTCMapwho@@@3@V?$allocator@PAVCTCMapwho@@@3@@std@@AAE?AU?$_Rb_tree_iterator@PAVCTCMapwho@@U?$_Nonconst_traits@PAVCTCMapwho@@@std@@@2@PAU_Rb_tree_node_base@2@0ABQAVCTCMapwho@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085DED0, 179 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the set<CTCMapwho*> tree; no Ego_r.pdb member of this exact type; its two sources: FableWin.pdb
//   ?GetAllThings@CThingMap@NThingMap@@QAEXAAV?$set@PAVCTCMapwho@@U?$less@PAVCTCMapwho@@@std@@V?$allocator@PAVCTCMapwho@@@3@@std@@@Z
//   (the landed caller of its insert_unique 0x0085E140 is NThingMap::CThingMap::GetAllThings 0x0085D010), and the 0x14-byte node with
//   the unsigned (`jb`) pointer compare. Landed callers of this address: 0x0085E140 _unattributed::InsertUnique_CRetailTree_0085e140.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CMemoryAllocatorVariableSize::InsertNode`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
class CTCMapwho;
namespace std {
template _Rb_tree<CTCMapwho*, CTCMapwho*, _Identity<CTCMapwho* >, less<CTCMapwho* >, allocator<CTCMapwho* > >::iterator _Rb_tree<CTCMapwho*, CTCMapwho*, _Identity<CTCMapwho* >, less<CTCMapwho* >, allocator<CTCMapwho* > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, CTCMapwho* const&, _Rb_tree_node_base*);
}
