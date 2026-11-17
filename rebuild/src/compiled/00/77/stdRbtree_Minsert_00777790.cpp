// std::_Rb_tree<long, pair<const long,CShopItem>, _Select1st<pair<const long,CShopItem> >, less<long> >::_M_insert(x, y, v, w) -- STLport tree insert below a found parent: allocate a node, copy the (key, shop item) value in, link it left or right of y keeping the header's root/leftmost/rightmost, rebalance and count; the new node's iterator through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00777790, 201 bytes (+7 int3), `ret 14h` (hidden iterator result + 4
//   arguments).
// Identity: called by the landed insert_unique 0x00777860 and by insert_unique(hint) 0x00777BA0, the tree of
//   map<long, CShopItem> (CTCShop::Stock, retail +0x34; the landed CTCShop::CTCShop 0x007744A0 fills it through
//   operator[] 0x00778280). Label correction: catalog "RBTree_Insert" -> _Rb_tree<long, pair<const long, CShopItem>>::
//   _M_insert.
// Callees: malloc 0x00BFEA0E (the 0x30-byte node: 0x10 links + the 32-byte pair), _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (__cdecl).
// Shape: the landed _M_insert of 0x006AE900 with the STLport pair copy constructor: the key is a dword move and the
//   28-byte CShopItem a `rep movsd` of 7 dwords, inside the placement-new null test.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCShopItem@@@std@@U?$_Select1st@U?$pair@$$CBJVCShopItem@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCShopItem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCShopItem@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCShopItem@@@2@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/CShopItem.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<long, pair<const long, CShopItem>, _Select1st<pair<const long, CShopItem> >, less<long >, allocator<pair<const long, CShopItem> > >::iterator _Rb_tree<long, pair<const long, CShopItem>, _Select1st<pair<const long, CShopItem> >, less<long >, allocator<pair<const long, CShopItem> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const long, CShopItem>&, _Rb_tree_node_base*);
}
