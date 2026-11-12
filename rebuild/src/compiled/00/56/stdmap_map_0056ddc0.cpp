// std::map<NUISystem::CMenuEntry*, CQuestManager::CBoast >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@PAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@U?$less@PAVCMenuEntry@NUISystem@@@std@@V?$allocator@U?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@@6@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056DDC0, 54 bytes; node 0x34.
// Identity (proven, agentFAM07): the landed ~CTCBoastUI 0x0056AC80 and CTCBoastUI::Activate 0x0056AD40 call this tree's _M_erase 0x0056D440 and bind it as
//   _Rb_tree<NUISystem::CMenuEntry*, pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>, ...> (the boast menu's entry -> boast
//   map); ??0 / ??1 of map<NUISystem::CMenuEntry*, CQuestManager::CBoast> are in the PDB name sets; the ctor is the COMDAT right before
//   the map's destructor 0x0056DE00 (calls 0x0056D440); node 0x34 = 0x10 + 4 + sizeof(CBoast) 0x20 (Ego_r CBoast is 32 bytes).
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
//   Was the placeholder CUnprovenContainer_0056d440::CUnprovenContainer_0056d440 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port): the shared stlport/fable_map.h (STLport 4.6 map / _Rb_tree; retail branch FABLE_RETAIL_STLP_MAP declares
//   the no-argument map()) and engine/CQuestManager.h (nested CBoast, Ego_r 32 bytes); the explicit instantiation emits map() as
//   its own COMDAT, as retail kept every used inline member (no /OPT:REF). Was FAM07's copied STLport text + local CQuestManager.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CQuestManager.h"
namespace NUISystem { class CMenuEntry; }

template std::map<NUISystem::CMenuEntry*, CQuestManager::CBoast>::map();
