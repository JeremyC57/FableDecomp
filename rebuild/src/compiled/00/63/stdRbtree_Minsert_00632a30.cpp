// std::_Rb_tree<NUISystem::ETableSprites, pair<NUISystem::ETableSprites const, long>, _Select1st<pair<NUISystem::ETableSprites const, long> >, less<NUISystem::ETableSprites >, allocator<pair<NUISystem::ETableSprites const, long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4ETableSprites@NUISystem@@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@U?$less@W4ETableSprites@NUISystem@@@4@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4ETableSprites@NUISystem@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632A30, 191 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<NUISystem::ETableSprites, long> tree; Ego_r.pdb members of this type: CUIDef::Sprites. Landed
//   callers of this address: 0x00632990 std::_Rb_tree<ETableSprites,pair<const ETableSprites,long>,_Select1st<..>,less<E.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `_unattributed::Insert_CRetailTree_00632a30`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace NUISystem { enum ETableSprites; }
namespace std {
template _Rb_tree<NUISystem::ETableSprites, pair<NUISystem::ETableSprites const, long>, _Select1st<pair<NUISystem::ETableSprites const, long> >, less<NUISystem::ETableSprites >, allocator<pair<NUISystem::ETableSprites const, long> > >::iterator _Rb_tree<NUISystem::ETableSprites, pair<NUISystem::ETableSprites const, long>, _Select1st<pair<NUISystem::ETableSprites const, long> >, less<NUISystem::ETableSprites >, allocator<pair<NUISystem::ETableSprites const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<NUISystem::ETableSprites const, long>&, _Rb_tree_node_base*);
}
