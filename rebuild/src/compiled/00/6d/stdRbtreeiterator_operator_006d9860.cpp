// std::_Rb_tree_iterator<pair<EHeroExperienceStatCategory const, float>, _Nonconst_traits<pair<EHeroExperienceStatCategory const, float> > >::operator--() -- STLport 4.6 _tree.h: step the node with
//   _Rb_global<bool>::_M_decrement (inlined) and store it back once; returns *this
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D9860, 79 bytes, /O2 TU, thiscall (no arguments).
// NF61 (round 7, q10629): the explicit instantiation of stlport/fable_map.h _Rb_tree_iterator::operator--, kept out of line by
//   FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE (retail calls it as this instance's COMDAT) for the instance whose insert_unique 0x006DB610 (landed by NF58, its
//   template arguments proven there from the Ego_r.pdb member / landed callers and the node size) calls it with exactly this
//   spelling; the value-type declarations are that row's. Was the stand-in `CRetailTreeIterator::Decrement` (a local node model).
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EHeroExperienceStatCategory.h"
namespace std {
template _Rb_tree_iterator<pair<EHeroExperienceStatCategory const, float>, _Nonconst_traits<pair<EHeroExperienceStatCategory const, float> > >& _Rb_tree_iterator<pair<EHeroExperienceStatCategory const, float>, _Nonconst_traits<pair<EHeroExperienceStatCategory const, float> > >::operator--();
}
