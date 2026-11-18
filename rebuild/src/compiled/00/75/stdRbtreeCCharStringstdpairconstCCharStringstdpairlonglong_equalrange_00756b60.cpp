// std::_Rb_tree<CCharString, std::pair<const CCharString, std::pair<long, long>>, ...>::equal_range(const key_type& k) -- STLport: the [lower_bound(k), upper_bound(k)) iterator pair
// Leaf: ?equal_range@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@std@@U12@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756b60, 42 bytes, thiscall, pair result through the hidden pointer, `ret 8`.
// Identity (probable): its _M_lower_bound 0x00756100 is bound to the CCharString -> pair<long, long> tree (as the stand-in ?_M_lower_bound@?$map@VCCharString@@U?$pair@JJ@std@@...@QBEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z) and sits beside that tree's hinted insert_unique 0x00756790 (family L056; CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine); no caller (unreferenced).
// Callees: _M_upper_bound(const key_type&) const 0x007564F0 and _M_lower_bound(const key_type&) const 0x00756100 (thiscall).
// Shape (family L057): `return pair<iterator, iterator>(lower_bound(k), upper_bound(k));` -- the upper bound is computed
//   first (right-to-left arguments) and kept in ebx.
// NF85 (q11063): moved off the row-local CCharString / STLport tree model onto stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE:
//   equal_range inline over the tree's private out-of-line _M_lower_bound / _M_upper_bound, explicit instantiation; the 0x004BB270 recipe)
//   and fable_string.h: _M_lower_bound 0x00756100 (NF82) is now bound under its private ABE spelling (was the public QBE stand-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
template std::pair<std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, std::_Select1st<std::pair<CCharString const, std::pair<long, long> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, std::pair<long, long> > > >::iterator, std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, std::_Select1st<std::pair<CCharString const, std::pair<long, long> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, std::pair<long, long> > > >::iterator> std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, std::_Select1st<std::pair<CCharString const, std::pair<long, long> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, std::pair<long, long> > > >::equal_range(const CCharString&);
