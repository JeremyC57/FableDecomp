// std::__distance<std::_Rb_tree_iterator<CCharString, std::_Nonconst_traits<CCharString> > >(const iterator& first, const iterator& last, const std::input_iterator_tag&) -- STLport 4.6 input-iterator __distance over the set<CCharString> tree iterator: count increments from first to last (operator++ inlined)
// Leaf: ??$__distance@U?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@std@@@std@@YAHABU?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@0@0ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B84A0, 78 bytes, __cdecl (both iterators and the tag by reference).
// Identity: its one landed caller is set<CCharString>::_Rb_tree::erase(const key_type&) 0x004BB270 (NF82), whose `distance(p.first,
//   p.second)` binds exactly this spelling (probed); the tree is set<CCharString> (Ego_r.pdb CQuestManager / CSymbolMap::NamesUsed /
//   CFaction sets). The old map<CCharString, long> iterator was a stand-in (one address, one instance: retail has no identical-code folding).
// NF85 (q11057): moved off the row-local STLport tree / iterator model onto the shared headers: stlport/fable_containers.h
//   FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE (the same body 0x004BB270 sees, explicit instantiation) and stlport/fable_map.h for the
//   iterator (its operator++ is _Rb_global<bool>::_M_increment, inline).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
template ptrdiff_t __cdecl std::__distance(const std::_Rb_tree_iterator<CCharString, std::_Nonconst_traits<CCharString> >&, const std::_Rb_tree_iterator<CCharString, std::_Nonconst_traits<CCharString> >&, const std::input_iterator_tag&);
