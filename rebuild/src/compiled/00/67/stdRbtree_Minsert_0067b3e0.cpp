// std::_Rb_tree<EOpinionReactionType, pair<EOpinionReactionType const, EOpinionTargetingConditionType>, _Select1st<pair<EOpinionReactionType const, EOpinionTargetingConditionType> >, less<EOpinionReactionType >, allocator<pair<EOpinionReactionType const, EOpinionTargetingConditionType> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EOpinionReactionType@@U?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@std@@U?$_Select1st@U?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@std@@@3@U?$less@W4EOpinionReactionType@@@3@V?$allocator@U?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067B3E0, 191 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<EOpinionReactionType, EOpinionTargetingConditionType> tree; Ego_r.pdb members of this type:
//   COpinionReactionManagerDef::TargetingCondition. Landed callers of this address: 0x0067BAF0
//   std::_Rb_tree<EOpinionReactionType,std::pair<EOpinionReactionType const, EOpinio.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `_unattributed::Insert_CRetailTree_0067b3e0`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
enum EOpinionReactionType;
enum EOpinionTargetingConditionType;
namespace std {
template _Rb_tree<EOpinionReactionType, pair<EOpinionReactionType const, EOpinionTargetingConditionType>, _Select1st<pair<EOpinionReactionType const, EOpinionTargetingConditionType> >, less<EOpinionReactionType >, allocator<pair<EOpinionReactionType const, EOpinionTargetingConditionType> > >::iterator _Rb_tree<EOpinionReactionType, pair<EOpinionReactionType const, EOpinionTargetingConditionType>, _Select1st<pair<EOpinionReactionType const, EOpinionTargetingConditionType> >, less<EOpinionReactionType >, allocator<pair<EOpinionReactionType const, EOpinionTargetingConditionType> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EOpinionReactionType const, EOpinionTargetingConditionType>&, _Rb_tree_node_base*);
}
