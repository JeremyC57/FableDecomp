// std::_Rb_tree<long, std::pair<long const, unsigned long>, std::_Select1st<std::pair<long const, unsigned long> >, std::less<long>, std::allocator<std::pair<long const, unsigned long> > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, unsigned long> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@JU?$pair@$$CBJK@std@@U?$_Select1st@U?$pair@$$CBJK@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJK@std@@@2@@std@@QAE@ABU?$less@J@1@ABV?$allocator@U?$pair@$$CBJK@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E8FF0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, unsigned long> (agentFAM07 proved this tree for 0x005E9320 map<long, unsigned long>::map, RENAMES 2026-11-11): CTCQuestCompletionUI::StartQuestExperience / StartQuestGold / StartQuestRenown (Ego_r.pdb map<long, unsigned long>): the landed ~CTCQuestCompletionUI 0x005E41E0 calls this tree's _M_erase 0x005E8C00 three times and binds it as _Rb_tree<long, pair<const long, unsigned long>, ...>.
//   This _Rb_tree constructor 0x005E8FF0 is the COMDAT right before the tree's ~_Rb_tree 0x005E9030, which calls that _M_erase 0x005E8C00.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_005e8c00,CUnprovenValue_005e8c00,CUnprovenKeyOfValue_005e8c00,std::less<CUnprovenKey_005e8c00>,std::allocator<CUnprovenValue_005e8c00> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_005e8c00@@UCUnprovenValue_005e8c00@@UCUnprovenKeyOfValue_005e8c00@@U?$less@UCUnprovenKey_005e8c00@@@std@@V?$allocator@UCUnprovenValue_005e8c00@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_005e8c00@@@1@ABV?$allocator@UCUnprovenValue_005e8c00@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

template std::_Rb_tree<long, std::pair<long const, unsigned long>, std::_Select1st<std::pair<long const, unsigned long> >, std::less<long>, std::allocator<std::pair<long const, unsigned long> > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, unsigned long> >&);
