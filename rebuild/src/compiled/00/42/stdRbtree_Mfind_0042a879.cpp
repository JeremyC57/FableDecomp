// std::_Rb_tree<long, pair<const long, EEngine2DLayer >, ...>::_M_find<long>(const long& k) const -- STLport 4.6.1 exact-key lookup of NUISystem::CManager's map<long, EEngine2DLayer > (Layers +0x70): walk from the root keeping the last node whose key is not less than k, then return it unless it is the header or k is less than its key (then the header, i.e. end())
// Leaf: ??$_M_find@J@?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@ABEPAU?$_Rb_tree_node@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@1@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042A879, 56 bytes (unaligned entry, size-optimised TU), thiscall, `ret 4`;
//   returns the node pointer (header node pointer = end()); `jl` = the signed key compare.
// Name (NF228, 2026-11-22, q12590): the STLport spelling the header produces (STLport 4.6.1 _tree.h `template <class _KT> _Rb_tree_node<_Value>*
//   _M_find(const _KT& __k) const`, a private member template: `ABE`; STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder NUISystem::CManager::GetLayer 0x0041E3B2 (Layers.find, FABLE_RETAIL_STLP_TREE_MFIND_DECL)
//   calls it under this spelling. Was a row-local non-template STLport model whose leaf was the non-template
//   ?_M_find@?$_Rb_tree@...@std@@ABEPAU?$_Rb_tree_node@...@2@ABJ@Z.
//   Family G008 (10 byte-identical bodies, each its own COMDAT: retail has no ICF, so each address is one tree type).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (the default-path _M_find body); the explicit
//   instantiation of the member template emits it as its own COMDAT (the landed 0x0042B467 _M_find<CCharString> pattern; access is not
//   checked in an explicit instantiation, C++03 14.7.2/8).
#pragma optimize("s", on)
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree_node<std::pair<const long, EEngine2DLayer > >* std::_Rb_tree<long, std::pair<const long, EEngine2DLayer >, std::_Select1st<std::pair<const long, EEngine2DLayer > >, std::less<long>, std::allocator<std::pair<const long, EEngine2DLayer > > >::_M_find<long>(const long&) const;
