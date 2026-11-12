// std::_Rb_tree<long, std::pair<long const, std::vector<NUISystem::CMenuEntry *> >, ...>::find(long const &) -- STLport 4.6 find(const key_type&): _M_find inlined -- lower-bound walk from the root, end() unless the found key is not greater; iterator returned through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056D130, 71 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, vector<NUISystem::CMenuEntry*> >. Sources: (1) retail: the landed real-typed CTCBoastUI code 0x0056AD40 calls this find
//   and its relocation binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb has the struct
//   map<long, vector<NUISystem::CMenuEntry*> > and Ego_r.pdb / FableWin.pdb carry its _Tmap_traits<long, vector<NUISystem::CMenuEntry*> >.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_0056d130,CUnprovenValue_0056d130,CUnprovenKeyOfValue_0056d130,std::less<CUnprovenKey_0056d130>,std::allocator<CUnprovenValue_0056d130> >::find (leaf ?find@?$_Rb_tree@UCUnprovenKey_0056d130@@UCUnprovenValue_0056d130@@UCUnprovenKeyOfValue_0056d130@@U?$less@UCUnprovenKey_0056d130@@@std@@V?$allocator@UCUnprovenValue_0056d130@@@5@@std@@QAE?AU?$_Rb_tree_iterator@UCUnprovenValue_0056d130@@U?$_Nonconst_traits@UCUnprovenValue_0056d130@@@std@@@2@ABUCUnprovenKey_0056d130@@@Z); name-only change: the
//   retail body, now on the shared tree model (the local model text has no vector for the mapped type).
// Model: the shared stlport/fable_map.h tree (STLport 4.6 find(const key_type&), the member-template _M_find inlined) over the shared
//   fable_containers.h vector; explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF). Replaces the invented stand-in (was CRetailTree_longkey::find).
// Leaf: ?find@?$_Rb_tree@JU?$pair@$$CBJV?$vector@PAVCMenuEntry@NUISystem@@V?$allocator@PAVCMenuEntry@NUISystem@@@std@@@std@@@std@@U?$_Select1st@U?$pair@$$CBJV?$vector@PAVCMenuEntry@NUISystem@@V?$allocator@PAVCMenuEntry@NUISystem@@@std@@@std@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJV?$vector@PAVCMenuEntry@NUISystem@@V?$allocator@PAVCMenuEntry@NUISystem@@@std@@@std@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJV?$vector@PAVCMenuEntry@NUISystem@@V?$allocator@PAVCMenuEntry@NUISystem@@@std@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJV?$vector@PAVCMenuEntry@NUISystem@@V?$allocator@PAVCMenuEntry@NUISystem@@@std@@@std@@@std@@@2@@2@ABJ@Z
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
namespace NUISystem { class CMenuEntry; }

typedef std::pair<const long, std::vector<NUISystem::CMenuEntry*> > CMenuEntryListMapValue;
typedef std::_Rb_tree<long, CMenuEntryListMapValue, std::_Select1st<CMenuEntryListMapValue>, std::less<long>, std::allocator<CMenuEntryListMapValue> > CMenuEntryListTree;
template CMenuEntryListTree::iterator CMenuEntryListTree::find(const long&);
