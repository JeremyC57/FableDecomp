// CTCInventoryMap::ConstructPastQuestsList(NUISystem::CComponent* parent) -- quest screen, "past quests" page: copy the quest manager's completed quest cards, collect their things; if there are any, build a UI_SUB_LIST with a UI_TITLE_QUEST_PAST header and let the hero's CTCInventoryQuests fill it (screen type 2); returns the list, or 0 when no quest is completed
// Leaf: ?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FCBE0, 637 bytes, thiscall `ret 4`, esp frame (sub esp,2Ch).
// Identity: Ego_r.pdb 0x0064F5D0 / FableWin.pdb 0x02088600 ?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z;
//   the string 0x01257298 "UI_TITLE_QUEST_PAST" and CQuestManager::GetCompletedQuestCards (+0x60) agree (the catalog's
//   "ConstructQuestsScreen_PastQuests" is descriptive).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetTC<CTCInventoryQuests>, TCI 25), malloc / free,
//   list<CIntelligentPointer<CThing> >::_M_insert_dispatch<const_iterator> 0x00618100 (the list copy), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, vector<CThing*>::_M_insert_overflow 0x00436AC0, _List_base<CIntelligentPointer<CThing> >::clear 0x004B8050,
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex
//   0x009AD390, GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312, NUISystem::CManager::GetInstance 0x0041E5F2, CreateComponent
//   0x0041DB1D, operator new / delete, CCountedPointer<CComponent>::DeleteData 0x00428B10, CList slot 145 AddNonScrollingChild
//   (+0x244), CTCInventoryQuests::ConstructQuestList 0x0061B610.
// Levers: the same-TU _List_base::clear body kept out of line (FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE): with a declaration only, c2
//   reloads the copied list's head after every call and null-tests it before the free (667 v 637 bytes); the same-TU GetDef body
//   (the def release is `dec [ecx+4] ; jne`). The list copy's range insert stays out of line on VC7.1's own budget (0x00618100).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS
#include "stlport/fable_containers.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/CThing.h"
#include "engine/CQuestManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
using namespace NUISystem;

// Same-TU body of the def lookup retail's TU holds (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312), as the landed
// CTCInventoryTrade::ConstructSpecialShopList 0x005A29F4: VC7.1 sees it only writes through the out pointer.
#pragma auto_inline(off)
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

CComponent* CTCInventoryMap::ConstructPastQuestsList(CComponent* parent)
{
    CTCInventoryQuests* inventory_quests;
    ParentThing->GetTC(&inventory_quests);

    std::list<CIntelligentPointer<CThing> > quest_cards = CQuestManager::Get().GetCompletedQuestCards();
    std::vector<CThing*> quests;
    for (std::list<CIntelligentPointer<CThing> >::iterator it = quest_cards.begin(); it != quest_cards.end(); ++it)
        quests.push_back(it->Get());

    if (quest_cards.size() <= 0)
        return 0;

    CDefPointer<const CUIMiscThingsDef> misc_things_def;
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index, &misc_things_def);

    CManager* manager = CManager::GetInstance();
    CComponent* list = manager->CreateComponent("UI_SUB_LIST", false);
    CCountedPointer<CComponent> title(manager->CreateComponent("UI_TITLE_QUEST_PAST", false));
    static_cast<CList*>(list)->AddNonScrollingChild(title);
    inventory_quests->ConstructQuestList(quests, static_cast<CList*>(list), (QuestScreenType)2, static_cast<CList*>(parent));
    return list;
}