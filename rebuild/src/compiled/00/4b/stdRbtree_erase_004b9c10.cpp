// std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::erase(iterator first, iterator last) -- STLport 4.6 set<CCharString> range erase: [begin, end) clears the whole tree, any other range is erased node by node
// Leaf: ?erase@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@QAEXU?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9C10, length 198 bytes, `ret 8` (thiscall: first, last).
// Identity: set<CCharString> (Ego_r.pdb CQuestManager::QuestsWithResourcesLoadedThatAreNotActivated, CSymbolMap::NamesUsed and
//   CFaction::AlliedFactions / EnemyFactions are set<CCharString>); its landed callers CQuestManager::ActivateQuests 0x004B3CE0,
//   set<CCharString>::erase(const K&) 0x004BB270 and CSymbolMap::RemoveSymbolsOfGroupType 0x009E5360 all bind this exact spelling, and
//   its callee _M_erase 0x004ABA70 is the same tree's (landed as ?_M_erase@?$_Rb_tree@VCCharString@@V1@..). The catalog's
//   map<CCharString, long> was a stand-in (one address, one tree: retail has no identical-code folding).
// Callees: _M_erase 0x004ABA70 (set<CCharString>), _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (landed, __cdecl),
//   ~CCharString 0x0099EAE0, free 0x00BFEA14 (the node allocator's deallocate).
// Shape: STLport 4.6 _tree.c `erase(iterator, iterator)`: clear() and erase(iterator) inline, `erase(first++)` with the successor
//   taken inline; the header node is reached through the alloc-proxy pointer at +0 and the count is at +4.
// NF85 (q11056): moved off the row-local CCharString / iterator / two-argument tree model onto stlport/fable_map.h (the 0x00638EE0
//   recipe: FABLE_RETAIL_STLP_TREE_ERASE + _ERASE_BODY + _POSTINC + _ERASE_KEY, explicit instantiation) and fable_string.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"
#include "fable_string.h"
template void std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> >::erase(std::_Rb_tree_iterator<CCharString, std::_Nonconst_traits<CCharString> >, std::_Rb_tree_iterator<CCharString, std::_Nonconst_traits<CCharString> >);
