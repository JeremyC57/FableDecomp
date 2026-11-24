// CTCInventoryMap::ConstructQuestReviewList(NUISystem::CComponent* parent) -- quest screen, "current quests" page: walk the quest manager's active quest cards and collect their things; if there are any, build a UI_SUB_LIST with a UI_TITLE_QUEST_CURRENT header and let the hero's CTCInventoryQuests fill it (screen type 1); returns the list, or 0 when no quest is active
// Leaf: ?ConstructQuestReviewList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FC2D0, 562 bytes, thiscall `ret 4`, esp frame (sub esp,24h).
// Identity: Ego_r.pdb 0x0064C6C0 / FableWin.pdb 0x02088CA0 ?ConstructQuestReviewList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z:
//   the only (CComponent*) private quest-list builder of CTCInventoryMap besides ConstructPastQuestsList 0x005FCBE0, called next to
//   it from ConstructQuestsScreen (0x005FE231 / 0x005FE115); it reads CQuestManager::ActiveQuestCards (+0x5C) and the string
//   "UI_TITLE_QUEST_CURRENT". The landed placeholder "ConstructQuestsScreen_CurrentQuests" was descriptive.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetTC<CTCInventoryQuests>, TCI 25),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, vector<CThing*>::_M_insert_overflow 0x00436AC0, malloc / free,
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex
//   0x009AD390, GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312, NUISystem::CManager::GetInstance 0x0041E5F2, CreateComponent
//   0x0041DB1D, operator new / delete, CCountedPointer<CComponent>::DeleteData 0x00428B10, CList slot 145 AddNonScrollingChild
//   (+0x244), CTCInventoryQuests::ConstructQuestList 0x0061B610.
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
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW
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

CComponent* CTCInventoryMap::ConstructQuestReviewList(CComponent* parent)
{
    CTCInventoryQuests* inventory_quests = 0;
    ParentThing->GetTC(&inventory_quests);

    const std::list<CIntelligentPointer<CThing> >& quest_cards = CQuestManager::Get().GetActiveQuestCards();
    std::vector<CThing*> quests;
    for (std::list<CIntelligentPointer<CThing> >::const_iterator it = quest_cards.begin(); it != quest_cards.end(); ++it)
        quests.push_back(it->Get());

    if (quest_cards.size() <= 0)
        return 0;

    CDefPointer<const CUIMiscThingsDef> misc_things_def;
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index, &misc_things_def);

    CManager* manager = CManager::GetInstance();
    CComponent* list = manager->CreateComponent("UI_SUB_LIST", false);
    CCountedPointer<CComponent> title(manager->CreateComponent("UI_TITLE_QUEST_CURRENT", false));
    static_cast<CList*>(list)->AddNonScrollingChild(title);
    inventory_quests->ConstructQuestList(quests, static_cast<CList*>(list), (QuestScreenType)1, static_cast<CList*>(parent));
    return list;
}
