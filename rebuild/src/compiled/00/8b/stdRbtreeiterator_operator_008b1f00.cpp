// _Rb_tree<ECombatSequenceIsValidType, pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)>, _Select1st<pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> >, less<ECombatSequenceIsValidType>, allocator<pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> > >::_Rb_tree_iterator<V, _Nonconst_traits<V> >::operator--() -- STLport 4.6 _tree.h: step to the in-order predecessor (_M_decrement inlined)
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBW4ECombatSequenceIsValidType@@P6I_NABVCThingCreatureBase@@0@Z@std@@U?$_Nonconst_traits@U?$pair@$$CBW4ECombatSequenceIsValidType@@P6I_NABVCThingCreatureBase@@0@Z@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B1F00.
// Identity: the tree of CCombatSequenceBase::IsValidPredicates (0x013BAFA8) (Ego_d's map type, EGOD_FINDINGS section 1); descriptive-by-library (STLport 4.6 spelling from stlport/fable_map.h;
//   both PDBs are Dinkumware).
// NF72 (round 7, q10894): was a generic stand-in tree (descriptive address-suffixed class); re-authored as the explicit instantiation of fable_map.h FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE (the thiscall COMDAT body),
//   the form of NF69's landed tree rows of the same maps (0x00818000 / 0x008B2DD0); the callers NF69 ported bind this symbol.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
class CThingCreatureBase;
enum ECombatSequenceIsValidType;
template std::_Rb_tree_iterator<std::pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)>, std::_Nonconst_traits<std::pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> > >& std::_Rb_tree_iterator<std::pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)>, std::_Nonconst_traits<std::pair<ECombatSequenceIsValidType const, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> > >::operator--();
