// std::_Rb_tree_iterator::operator--() -- STLport 4.6 tree iterator pre-decrement (_Rb_global::_M_decrement inlined), value type char
// Leaf: ??F?$_Rb_tree_iterator@DU?$_Nonconst_traits@D@std@@@std@@QAEAAU01@XZ
// Identity: the respelled caller 0x00514B50 (set<char>) (agentFAM12) calls this address as this operator-- of the proven value type
//   (agentFAM12 CALLEE_FIXES.tsv; the caller row's Leaf/types come from its own landed binding + the Ego_r/FableWin name sets);
//   the body is STLport 4.6 _Rb_tree_iterator::operator-- (the head-node test, rightmost of the left subtree, climb while a left child).
//   Was the invented stand-in CRetailTreeIterator::Decrement.
// Model: stlport/fable_map.h + the explicit instantiation of the member (its own COMDAT, as retail).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
template std::_Rb_tree_iterator<char, std::_Nonconst_traits<char > >& std::_Rb_tree_iterator<char, std::_Nonconst_traits<char > >::operator--();
