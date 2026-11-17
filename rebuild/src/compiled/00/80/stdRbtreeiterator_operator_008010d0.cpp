// std::_Rb_tree_iterator<pair<EIdleStateGroup const, long>, _Nonconst_traits<pair<EIdleStateGroup const, long> > >::operator++() -- STLport 4.6 _tree.h: step the node with
//   _Rb_global<bool>::_M_increment (inlined) and store it back once; returns *this
// Leaf: ??E?$_Rb_tree_iterator@U?$pair@$$CBW4EIdleStateGroup@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EIdleStateGroup@@J@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008010D0, 62 bytes, /O2 TU, thiscall (no arguments).
// NF61 (round 7, q10632): the explicit instantiation of stlport/fable_map.h _Rb_tree_iterator::operator++, kept out of line by
//   FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE (retail calls it as this instance's COMDAT) for the instance whose insert_unique 0x00801EC0 (landed by NF58, its
//   template arguments proven there from the Ego_r.pdb member / landed callers and the node size) calls it with exactly this
//   spelling; the value-type declarations are that row's. Was the stand-in `CRetailTreeIterator::Increment` (a local node model).
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#include "stlport/fable_map.h"
enum EIdleStateGroup;
namespace std {
template _Rb_tree_iterator<pair<EIdleStateGroup const, long>, _Nonconst_traits<pair<EIdleStateGroup const, long> > >& _Rb_tree_iterator<pair<EIdleStateGroup const, long>, _Nonconst_traits<pair<EIdleStateGroup const, long> > >::operator++();
}
