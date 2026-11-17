// std::_Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long >, allocator<pair<long const, CDefString> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCDefString@@@std@@U?$_Select1st@U?$pair@$$CBJVCDefString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCDefString@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCDefString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCDefString@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCDefString@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DAFA0, 191 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<long, CDefString> tree; Ego_r.pdb members of this type:
//   CHeroExperienceDef::ComboMultiplierIncrease, CHeroExperienceDef::ComboMultiplierLock, CHeroExperienceDef::ComboMultiplierLost,
//   CHeroExperienceDef::ComboMultiplierRunningHitsIncrease (+1 more). Landed callers of this address: 0x006DB450
//   std::_Rb_tree<long,std::pair<long const, CDefString >,std::_Select1st<std::pair<.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `_unattributed::Insert_CRetailTree_006dafa0`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
namespace std {
template _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long >, allocator<pair<long const, CDefString> > >::iterator _Rb_tree<long, pair<long const, CDefString>, _Select1st<pair<long const, CDefString> >, less<long >, allocator<pair<long const, CDefString> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, CDefString>&, _Rb_tree_node_base*);
}
