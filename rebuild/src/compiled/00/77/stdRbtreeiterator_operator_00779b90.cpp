// std::_Rb_tree_iterator<std::pair<const long,float>, std::_Const_traits<std::pair<const long,float> > >::operator--() -- STLport 4.6
// iterator operator--(): _Rb_global<bool>::_M_decrement inlined (in-order predecessor; header -> rightmost)
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBJM@std@@U?$_Const_traits@U?$pair@$$CBJM@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00779B90, 79 bytes.
// Identity (NF42, round 5): the map<long,float> const iterator of CExperienceDef::LifeTimeForDistanceThresholds (engine/CExperienceDef.h
// +0x44, Ego_r / FableWin type); the landed caller 0x00779EF0 already binds this symbol. Descriptive-by-library: retail is STLport
// 4.6.2 (stl/_tree.h), both PDBs are Dinkumware builds, so no PDB holds this mangling. Was a placeholder
// instance over address-named value and traits classes on a local copy of the STLport tree; now an explicit
// instantiation over the shared stlport/fable_map.h.
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree_iterator<pair<const long, float>, _Const_traits<pair<const long, float> > >&
    _Rb_tree_iterator<pair<const long, float>, _Const_traits<pair<const long, float> > >::operator--();
}
