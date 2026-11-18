// std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, std::_Select1st<std::pair<CCharString const, std::pair<long, long> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, std::pair<long, long> > > >::erase(const CCharString& key) -- STLport 4.6 erase by key on map<CCharString, pair<long, long> >'s tree: take the equal_range, count it with distance, erase that range, return the count
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAEIABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756C60, thiscall `ret 4`.
// Callees: _M_upper_bound 0x007564F0 and _M_lower_bound 0x00756100 (equal_range's pair arguments, evaluated right to left),
//   __distance(first, last, const input_iterator_tag&) 0x00755CB0 (cdecl, out of line), erase(iterator, iterator) 0x00756420.
// Shape: the set<CCharString> erase(const key_type&) 0x004BB270 (NF82) on this tree: equal_range inline over the private out-of-line
//   bounds, the distance body visible but out of line (stlport/fable_containers.h FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE).
// NF85 (q11063): moved off the row-local STLport model (its bounds were declared returning _Rb_tree_node<V>*, the stand-in
//   ?_M_lower_bound@..@ABEPAU?$_Rb_tree_node@..) onto stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE, explicit
//   instantiation) and fable_string.h: 0x00756100 / 0x007564F0 are now bound under their private ABEPAU_Rb_tree_node_base spellings.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
template size_t std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, std::_Select1st<std::pair<CCharString const, std::pair<long, long> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, std::pair<long, long> > > >::erase(const CCharString&);
