// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- STLport 4.6 _M_erase: erase without rebalancing -- recurse right, destroy the value, free the node, continue left (std::map<NUISystem::CMenuEntry*, CQuestManager::CBoast >)
// Leaf: ?_M_erase@?$_Rb_tree@PAVCMenuEntry@NUISystem@@U?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@U?$_Select1st@U?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@@4@U?$less@PAVCMenuEntry@NUISystem@@@4@V?$allocator@U?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@@4@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056D440, 73 bytes, `__fastcall`, `ret 4`, self-recursive.
// Name (agentNF23, name-fix queue land1011da, agentFAM07 proof): agentFAM07: the _M_erase of the tree whose constructor 0x0056DDC0 was renamed this pass (its destructor, the next COMDAT, calls 0x0056D440; proven type in RENAMES.tsv); STLport 4.6 stl/_tree.h declares _Rb_tree::_M_erase private (the landed 0x00529CE0 template text), so VC7.1 spells it AAE
//   Was the stand-in (no self binding) (catalog CRetailTree_erase0056d440).
// Identity (proven, agentFAM07): the landed ~CTCBoastUI 0x0056AC80 and CTCBoastUI::Activate 0x0056AD40 call this tree's _M_erase 0x0056D440 and bind it as
//   _Rb_tree<NUISystem::CMenuEntry*, pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>, ...> (the boast menu's entry -> boast
//   map); ??0 / ??1 of map<NUISystem::CMenuEntry*, CQuestManager::CBoast> are in the PDB name sets; the ctor is the COMDAT right before
//   the map's destructor 0x0056DE00 (calls 0x0056D440); node 0x34 = 0x10 + 4 + sizeof(CBoast) 0x20 (Ego_r CBoast is 32 bytes).
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
// Model (agentNF24 port of agentNF23's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CQuestManager.h (nested CBoast) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_MAP
namespace NUISystem { class CMenuEntry; }
#include "stlport/fable_map.h"
#include "engine/CQuestManager.h"

template void std::_Rb_tree<NUISystem::CMenuEntry*, std::pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>, std::_Select1st<std::pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast> >, std::less<NUISystem::CMenuEntry*>, std::allocator<std::pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast> > >::_M_erase(std::_Rb_tree_node<std::pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast> >*);
