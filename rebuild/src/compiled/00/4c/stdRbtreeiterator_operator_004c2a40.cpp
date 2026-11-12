// std::_Rb_tree_iterator::operator--() -- STLport 4.6 tree iterator pre-decrement (_Rb_global::_M_decrement inlined), value type pair<void* const, CRGBColour>
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@QAXVCRGBColour@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@QAEAAU01@XZ
// Identity: the respelled caller 0x004C4960 insert_unique (map<void*, CRGBColour>) (agentFAM12) calls this address as this operator-- of the proven value type
//   (agentFAM12 CALLEE_FIXES.tsv; the caller row's Leaf/types come from its own landed binding + the Ego_r/FableWin name sets);
//   the body is STLport 4.6 _Rb_tree_iterator::operator-- (the head-node test, rightmost of the left subtree, climb while a left child).
//   Was the invented stand-in CRetailTreeIterator::Decrement.
// Model: stlport/fable_map.h + the explicit instantiation of the member (its own COMDAT, as retail).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CRGBColour.h"
template std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >& std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >::operator--();
