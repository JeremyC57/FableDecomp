// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- STLport 4.6 _M_erase: erase without rebalancing -- recurse right, destroy the value, free the node, continue left (std::map<long, CShopItem >)
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCShopItem@@@std@@U?$_Select1st@U?$pair@$$CBJVCShopItem@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCShopItem@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00776AA0, oracle length 51 bytes.
// Name (agentNF23, name-fix queue land1011da, agentFAM07 proof): agentFAM07: the _M_erase of the tree whose constructor 0x00777A00 was renamed this pass (its destructor, the next COMDAT, calls 0x00776AA0; proven type in RENAMES.tsv); STLport 4.6 stl/_tree.h declares _Rb_tree::_M_erase private (the landed 0x00529CE0 template text), so VC7.1 spells it AAE
//   Was the stand-in ?FreeTree_CTreeOwner_00776aa0@CTreeOwner_00776aa0@@QAEXPAUSNode@@@Z (catalog _unattributed).
// Identity (proven, agentFAM07): CTCShop::Stock (Ego_r.pdb map<long, CShopItem>): the landed ~CTCShop 0x007783F0 calls this tree's _M_erase 0x00776AA0 and binds it
//   as _Rb_tree<long, pair<const long, CShopItem>, ...>; ??0 / ??1 of map<long, CShopItem> are in the PDB name sets; the ctor is the
//   COMDAT right before the map's destructor 0x00777A40; node 0x30 = 0x10 + 4 + CShopItem 0x1C.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
// Model (agentNF24 port of agentNF23's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CShopItem.h replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CShopItem.h"

template void std::_Rb_tree<long, std::pair<long const, CShopItem>, std::_Select1st<std::pair<long const, CShopItem> >, std::less<long>, std::allocator<std::pair<long const, CShopItem> > >::_M_erase(std::_Rb_tree_node<std::pair<long const, CShopItem> >*);
