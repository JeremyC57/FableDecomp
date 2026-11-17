// std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, ...>::_M_upper_bound(const CCharString& k) const -- STLport 4.6 upper-bound walk: the first node whose key is greater than k, or the header
// Leaf: ?_M_upper_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@ABEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007564F0, 67 bytes, thiscall, `ret 4`; returns the node pointer.
// Identity (proven): its callers are the equal_range 0x00756B60, erase-by-key 0x00756C60 and insert 0x00756790 of the CTCHeroRememberCrimes TU's map<CCharString, pair<long, long> > (the landed _M_find 0x00756350 / TransferMap 0x00756D00 of that tree: Ego_r.pdb CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine).
// Callees: CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<: equal data pointers -> false,
//   null k -> true, null node key -> false).
// Shape (family M084, representative 0x004B9D50): STLport 4.6 _tree.c `_M_upper_bound`: `__y = header; __x = root; while (__x)
//   if (_M_key_compare(__k, _S_key(__x))) __y = __x, __x = _S_left(__x); else __x = _S_right(__x); return __y;` -- k's data
//   pointer loaded once (edi) -- VC7.1 hoists it only when it can see that CBasicString<char>::operator< writes no memory,
//   so that callee's body is compiled in this TU (noinline, as the landed _M_find 0x00756350 of the same family); speed TU.
// NF85 (q11063, claimed: the equal_range 0x00756B60 port binds this private spelling): moved off the row-local CBasicString /
//   CCharString / tree models onto stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE + _BODY_PASS22, explicit instantiation;
//   the NF82 recipe of this tree's _M_lower_bound 0x00756100 and of 0x004B9D50) and fable_string.h; the own symbol is the private STLport
//   ABE spelling (was the public QBE stand-in), as 0x00756100's.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree_node_base* std::_Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString>, allocator<pair<CCharString const, pair<long, long> > > >::_M_upper_bound(const CCharString&) const;
}
