// std::_Rb_tree<long, std::pair<long const, CShopItem>, std::_Select1st<std::pair<long const, CShopItem> >, std::less<long>, std::allocator<std::pair<long const, CShopItem> > >::erase(std::_Rb_tree_iterator<std::pair<long const, CShopItem>, std::_Nonconst_traits<std::pair<long const, CShopItem> > >, std::_Rb_tree_iterator<std::pair<long const, CShopItem>, std::_Nonconst_traits<std::pair<long const, CShopItem> > >) -- STLport 4.6 erase (stlport/fable_map.h)
// Leaf: ?erase@?$_Rb_tree@JU?$pair@$$CBJVCShopItem@@@std@@U?$_Select1st@U?$pair@$$CBJVCShopItem@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBJVCShopItem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCShopItem@@@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007775B0, oracle length 189 bytes.
// Identity: (1) the landed caller 0x00777B30 (std::_Rb_tree<long, std::pair<long const, CShopItem>, std::_Select1st<std::pair<long const, CShopItem>>, std::less<long>, std::allocator<std::pair<long const, CShopItem>>>::erase) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<JVCShopItem@@> instantiation (_Tmap_traits@JVCShopItem@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"
#include "engine/CShopItem.h"
template void std::_Rb_tree<long, std::pair<long const, CShopItem>, std::_Select1st<std::pair<long const, CShopItem> >, std::less<long>, std::allocator<std::pair<long const, CShopItem> > >::erase(std::_Rb_tree_iterator<std::pair<long const, CShopItem>, std::_Nonconst_traits<std::pair<long const, CShopItem> > >, std::_Rb_tree_iterator<std::pair<long const, CShopItem>, std::_Nonconst_traits<std::pair<long const, CShopItem> > >);
