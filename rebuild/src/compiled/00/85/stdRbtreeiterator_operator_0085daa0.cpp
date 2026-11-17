// std::_Rb_tree_iterator<CTCMapwho*, _Nonconst_traits<CTCMapwho* > >::operator--() -- STLport 4.6 _tree.h: step the node with
//   _Rb_global<bool>::_M_decrement (inlined) and store it back once; returns *this
// Leaf: ??F?$_Rb_tree_iterator@PAVCTCMapwho@@U?$_Nonconst_traits@PAVCTCMapwho@@@std@@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085DAA0, 79 bytes, /O2 TU, thiscall (no arguments).
// NF61 (round 7, q10634): the explicit instantiation of stlport/fable_map.h _Rb_tree_iterator::operator--, kept out of line by
//   FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE (retail calls it as this instance's COMDAT) for the instance whose insert_unique 0x0085E140 (landed by NF58, its
//   template arguments proven there from the Ego_r.pdb member / landed callers and the node size) calls it with exactly this
//   spelling; the value-type declarations are that row's. Was the stand-in `CRetailTreeIterator::Decrement` (a local node model).
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
class CTCMapwho;
namespace std {
template _Rb_tree_iterator<CTCMapwho*, _Nonconst_traits<CTCMapwho* > >& _Rb_tree_iterator<CTCMapwho*, _Nonconst_traits<CTCMapwho* > >::operator--();
}
