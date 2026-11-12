// std::_Rb_tree_iterator::operator--() -- STLport 4.6 tree iterator pre-decrement (_Rb_global::_M_decrement inlined), value type CBankFileAsyncData*
// Leaf: ??F?$_Rb_tree_iterator@PAVCBankFileAsyncData@@U?$_Nonconst_traits@PAVCBankFileAsyncData@@@std@@@std@@QAEAAU01@XZ
// Identity: the respelled caller 0x009D7CF0 (set<CBankFileAsyncData*>) (agentFAM12) calls this address as this operator-- of the proven value type
//   (agentFAM12 CALLEE_FIXES.tsv; the caller row's Leaf/types come from its own landed binding + the Ego_r/FableWin name sets);
//   the body is STLport 4.6 _Rb_tree_iterator::operator-- (the head-node test, rightmost of the left subtree, climb while a left child).
//   Was the invented stand-in CRetailTreeIterator::Decrement.
// Model: stlport/fable_map.h + the explicit instantiation of the member (its own COMDAT, as retail).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CBankFileAsyncData;
template std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData* > >& std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData* > >::operator--();
