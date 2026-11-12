// std::_Rb_tree<long, std::pair<long const, CShopItem>, std::_Select1st<std::pair<long const, CShopItem> >, std::less<long>, std::allocator<std::pair<long const, CShopItem> > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, CShopItem> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@JU?$pair@$$CBJVCShopItem@@@std@@U?$_Select1st@U?$pair@$$CBJVCShopItem@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@2@@std@@QAE@ABU?$less@J@1@ABV?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00777520, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, CShopItem> (agentFAM07 proved this tree for 0x00777A00 map<long, CShopItem>::map, RENAMES 2026-11-11): CTCShop::Stock (Ego_r.pdb map<long, CShopItem>): the landed ~CTCShop 0x007783F0 calls this tree's _M_erase 0x00776AA0 and binds it as _Rb_tree<long, pair<const long, CShopItem>, ...>; ??0 / ??1 of map<long, CShopItem> are in the PDB name sets.
//   This _Rb_tree constructor 0x00777520 is the COMDAT right before the tree's ~_Rb_tree 0x00777560, which calls that _M_erase 0x00776AA0.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00776aa0,CUnprovenValue_00776aa0,CUnprovenKeyOfValue_00776aa0,std::less<CUnprovenKey_00776aa0>,std::allocator<CUnprovenValue_00776aa0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00776aa0@@UCUnprovenValue_00776aa0@@UCUnprovenKeyOfValue_00776aa0@@U?$less@UCUnprovenKey_00776aa0@@@std@@V?$allocator@UCUnprovenValue_00776aa0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00776aa0@@@1@ABV?$allocator@UCUnprovenValue_00776aa0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP) and engine/CShopItem.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CShopItem.h"

template std::_Rb_tree<long, std::pair<long const, CShopItem>, std::_Select1st<std::pair<long const, CShopItem> >, std::less<long>, std::allocator<std::pair<long const, CShopItem> > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, CShopItem> >&);
