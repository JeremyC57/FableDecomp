// std::_Rb_tree<long, std::pair<const long, long>, std::_Select1st<...>, std::less<long>, std::allocator<...> >::_M_erase(_Rb_tree_node*) -- STLport recursive subtree erase of map<long, long>: erase the right subtree, step left, free each node
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJJ@std@@@2@@Z
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA2AC, recursive, thiscall; node freed with free 0x00BFEA14 (the pair<const long, long> value has a trivial destructor).
// Identity: STLport 4.6 _tree.c _M_erase (no PDB copy: both PDBs are Dinkumware; descriptive-by-library). The map<long, long> type and
//   the private member from the callers: the landed 0x00626E40 CTCQuickAccessMenu::ConstructItemCategoryList, 0x00774610
//   CTCShop::GetStockList (map<long, long> GetStockList, Ego_r.pdb) and 0x006CF1D0 CTCWife::~CTCWife (GiftsReceived) all bind this spelling.
// NF108 (2026-11-19, q4185): re-authored on the shared stlport/fable_map.h as the explicit instantiation (the 0x007AE0E0 recipe); it was the
//   AutoTinyTreeWalker::BuildTreeArray stand-in (catalog "LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray").
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_map.h"

typedef std::map<long, long> TLongLongMap;
template void TLongLongMap::_Rep_type::_M_erase(std::_Rb_tree_node<TLongLongMap::value_type>* x);
