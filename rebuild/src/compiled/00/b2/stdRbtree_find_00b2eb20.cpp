// std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> >::find<long>(long const &) const -- STLport 4.6 const find(const _KT&): _M_find inlined
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2EB20, 71 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<long>, the const member-template find<long> (STLport 4.6.1 _tree.h under _STLP_MEMBER_TEMPLATES). Sources: (1) retail: the
//   landed real-typed CEngineSurfaceCompositionManager::UpdateDiskCache 0x00B2D9F0 calls this const instance out of line (push &key ; push &result ;
//   call, hidden const_iterator result) and its relocation binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv; fable_map.h
//   FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE documents the call); (2) Ego_r.pdb / FableWin.pdb carry set<long> (_Tset_traits<long>).
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00b2eb20,CUnprovenValue_00b2eb20,CUnprovenKeyOfValue_00b2eb20,std::less<CUnprovenKey_00b2eb20>,std::allocator<CUnprovenValue_00b2eb20> >::find (leaf ?find@?$_Rb_tree@UCUnprovenKey_00b2eb20@@UCUnprovenValue_00b2eb20@@UCUnprovenKeyOfValue_00b2eb20@@U?$less@UCUnprovenKey_00b2eb20@@@std@@V?$allocator@UCUnprovenValue_00b2eb20@@@5@@std@@QAE?AU?$_Rb_tree_iterator@UCUnprovenValue_00b2eb20@@U?$_Nonconst_traits@UCUnprovenValue_00b2eb20@@@std@@@2@ABUCUnprovenKey_00b2eb20@@@Z); name-only change: the
//   retail body, now spelled as the const member template the caller binds (the placeholder was the non-const find).
// Model: the shared stlport/fable_map.h tree with FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY (the STLport 4.6.1 const member-template
//   find, _M_find inlined); explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF). Replaces the invented stand-in (was CRetailTree_longkey::find).
// Leaf: ??$find@J@?$_Rb_tree@JJU?$_Identity@J@std@@U?$less@J@2@V?$allocator@J@2@@std@@QBE?AU?$_Rb_tree_iterator@JU?$_Const_traits@J@std@@@1@ABJ@Z
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#include "stlport/fable_map.h"

typedef std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> > CLongSetTree;
template CLongSetTree::const_iterator CLongSetTree::find<long>(const long&) const;
