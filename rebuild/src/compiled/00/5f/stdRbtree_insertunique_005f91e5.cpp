// std::_Rb_tree<CTCInventoryAbilities::EUISpellCategory, std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*>, std::_Select1st<std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*> >, std::less<CTCInventoryAbilities::EUISpellCategory>, std::allocator<std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*> > >::insert_unique(const value_type&) -- STLport 4.6 insert_unique(const value_type&): descend comparing keys, --j on a left-most miss, insert through _M_insert unless the key exists; pair<iterator,bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EUISpellCategory@CTCInventoryAbilities@@U?$pair@$$CBW4EUISpellCategory@CTCInventoryAbilities@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4EUISpellCategory@CTCInventoryAbilities@@PAVCComponent@NUISystem@@@std@@@4@U?$less@W4EUISpellCategory@CTCInventoryAbilities@@@4@V?$allocator@U?$pair@$$CBW4EUISpellCategory@CTCInventoryAbilities@@PAVCComponent@NUISystem@@@std@@@4@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EUISpellCategory@CTCInventoryAbilities@@PAVCComponent@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EUISpellCategory@CTCInventoryAbilities@@PAVCComponent@NUISystem@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EUISpellCategory@CTCInventoryAbilities@@PAVCComponent@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F91E5, 134 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<CTCInventoryAbilities::EUISpellCategory, NUISystem::CComponent*> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail: the landed
//   hinted insert_unique(iterator, const value_type&) 0x005F9714 (proven: its only caller is the map<CTCInventoryAbilities::EUISpellCategory,
//   NUISystem::CComponent*>::insert(iterator, const value_type&) forwarder 0x005F9C88, bound with these types) calls this tree's _M_insert
//   0x005F915D and insert_unique(const value_type&) 0x005F91E5; _M_insert builds the node with _M_create_node 0x005F8C1F; (2) Ego_r.pdb
//   CTCInventoryAbilities::CategoriesLists is map<CTCInventoryAbilities::EUISpellCategory, NUISystem::CComponent*>, and FableWin.pdb has the
//   _Tmap_traits<CTCInventoryAbilities::EUISpellCategory, NUISystem::CComponent*> tree.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_005f8c1f,CUnprovenValue_005f8c1f,CUnprovenKeyOfValue_005f8c1f,std::less<CUnprovenKey_005f8c1f>,std::allocator<CUnprovenValue_005f8c1f> >::insert_unique (leaf ?insert_unique@?$_Rb_tree@UCUnprovenKey_005f8c1f@@UCUnprovenValue_005f8c1f@@UCUnprovenKeyOfValue_005f8c1f@@U?$less@UCUnprovenKey_005f8c1f@@@std@@V?$allocator@UCUnprovenValue_005f8c1f@@@5@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@UCUnprovenValue_005f8c1f@@U?$_Nonconst_traits@UCUnprovenValue_005f8c1f@@@std@@@std@@_N@2@ABUCUnprovenValue_005f8c1f@@@Z); name-only change.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP, the _tree.c insert bodies under
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY) and engine/CTCInventoryAbilities.h (nested EUISpellCategory, FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC)
//   replace the copied STLport text; /Os TU as retail (the landed 0x005F9714 neighbour's pragmas); the explicit instantiation emits the member.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryAbilities.h"

template std::pair<std::_Rb_tree<CTCInventoryAbilities::EUISpellCategory, std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*>, std::_Select1st<std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*> >, std::less<CTCInventoryAbilities::EUISpellCategory>, std::allocator<std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*> > >::iterator, bool> std::_Rb_tree<CTCInventoryAbilities::EUISpellCategory, std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*>, std::_Select1st<std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*> >, std::less<CTCInventoryAbilities::EUISpellCategory>, std::allocator<std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*> > >::insert_unique(const std::pair<CTCInventoryAbilities::EUISpellCategory const, NUISystem::CComponent*>&);
