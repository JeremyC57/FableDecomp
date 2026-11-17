// std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, ...>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport 4.6 node insert of map<CCharString, pair<long, long> > (CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine): link a new node for v under y (left or right by key order) and rebalance; returns the new node's iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756150, 250 bytes, thiscall `ret 14h` (hidden iterator result, x, y, v, w).
// Label correction: catalog "RBTree_Insert" -> STLport 4.x _Rb_tree<CCharString, pair<const CCharString,
//   pair<long,long> >, ...>::_M_insert of the CTCHeroRememberCrimes TU's map (the landed insert_unique 0x00756250 and
//   erase 0x00756420 are the same tree; node 0x1C = 0x10 header + CCharString + two longs).
// Callees: malloc 0x00BFEA0E (_M_create_node's allocate(1), inlined), CCharString copy constructor 0x0099EC30,
//   CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator<), _Rb_global<bool>::_Rebalance
//   0x0042971D (static __cdecl).
// Shape: STLport verbatim -- `if (y == header || (w == 0 && (x != 0 || key_compare(KeyOfValue()(v), key(y)))))` link
//   left (root + rightmost when y is the header, else leftmost) else right (rightmost); _M_create_node inlined in both
//   arms; parent/children set, _Rebalance(z, root), ++count, iterator(z) through the hidden result.
// NF82 (q10989): moved off the row-local CBasicString / CCharString / pair / tree models onto stlport/fable_map.h
//   (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, explicit instantiation, the landed 0x004ABC70 recipe) and fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY). The row's own symbol is now the private STLport _M_insert with the two-argument
//   iterator (_Rb_tree_iterator<V, _Nonconst_traits<V> >), the spelling NF80's 0x00756250 binds; the old Leaf line was partial.
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString >, allocator<pair<CCharString const, pair<long, long> > > >::iterator _Rb_tree<CCharString, pair<CCharString const, pair<long, long> >, _Select1st<pair<CCharString const, pair<long, long> > >, less<CCharString >, allocator<pair<CCharString const, pair<long, long> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CCharString const, pair<long, long> >&, _Rb_tree_node_base*);
}
