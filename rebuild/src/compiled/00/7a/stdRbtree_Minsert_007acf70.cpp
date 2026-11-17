// std::_Rb_tree<CCharString, std::pair<CCharString const, bool>, ...>::_M_insert(x, y, value, hint) -- STLport 4.6 node insert of map<CCharString, bool>: link a new node and rebalance the tree.
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@_N@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ACF70, 238 bytes, thiscall `ret 14h` (hidden iterator result, x, y, v, w).
// Callees: malloc 0x00BFEA0E (_M_create_node's allocate(1), inlined), CCharString copy constructor 0x0099EC30,
//   CBasicString<char>::operator< 0x00429950 (behind the inlined CCharString::operator<), _Rb_global<bool>::_Rebalance 0x0042971D.
// Identity: map<CCharString, bool> (catalog); the landed insert_unique 0x007AD060 and hinted insert_unique 0x008ACA90 of the same
//   map bind this private STLport spelling.
// NF82 (q10990): moved off the row-local CBasicString (with its own copy of the operator< body) / CCharString / pair / tree models
//   onto stlport/fable_map.h (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, explicit instantiation, the landed 0x004ABC70 recipe) and
//   fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY); the own symbol is unchanged.
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, bool >, _Select1st<pair<CCharString const, bool > >, less<CCharString >, allocator<pair<CCharString const, bool > > >::iterator _Rb_tree<CCharString, pair<CCharString const, bool >, _Select1st<pair<CCharString const, bool > >, less<CCharString >, allocator<pair<CCharString const, bool > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CCharString const, bool >&, _Rb_tree_node_base*);
}
