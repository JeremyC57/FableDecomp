// CTCInventoryMap::ConstructQuestReviewListPC(NUISystem::CComponent* container, NUISystem::CComponent* parent) -- PC quest screen, "current quests" page: find the parent's PC_UI_FRAME_QUESTS frame, walk the quest manager's active quest cards and collect their things; if there are any, build a PC_SUB_LIST, add a PC_TITLE_QUEST_CURRENT header to it and position-attach the header to the frame, then let the hero's CTCInventoryQuests fill the list (screen type 1); returns the list, or 0 when no quest is active
// Leaf: ?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FC8A0, 616 bytes, thiscall `ret 8`, esp frame (sub esp,28h).
// Identity: Ego_r.pdb 0x0064CE90 / FableWin.pdb 0x020AEC70 ?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z
//   (two CComponent* arguments, `ret 8`): the only other (CComponent*, CComponent*) private quest-list builder besides
//   ConstructPastQuestsListPC 0x005FDDD0, called from ConstructQuestsScreenPC (0x006027E9); the console twin 0x005FC2D0
//   ConstructQuestReviewList has the same body with the UI_ strings, this one uses "PC_UI_FRAME_QUESTS", "PC_SUB_LIST",
//   "PC_TITLE_QUEST_CURRENT" and calls CTCInventoryQuests::ConstructQuestListPC 0x0061E6D0. The landed placeholder
//   "ConstructQuestsScreen_CurrentQuestsPC" was descriptive.
// Callees: as 0x005FC2D0, plus CComponent slot 3 FindChild(CCharString) (+0x0C, the string built in its argument slot) and slot 124
//   AddPositionChild(CComponent*) (+0x1F0); CTCInventoryQuests::ConstructQuestListPC 0x0061E6D0 in place of ConstructQuestList.
// Levers: as 0x005FC2D0 -- the TC pointer zero-initialised (retail stores 0 to its slot before the UsingComponents test) and the
//   same-TU GetDef body.
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

CComponent* CTCInventoryMap::ConstructQuestReviewListPC(CComponent* container, CComponent* parent)
{
    CComponent* frame = parent->FindChild("PC_UI_FRAME_QUESTS");
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
    CComponent* list = manager->CreateComponent("PC_SUB_LIST", false);
    CCountedPointer<CComponent> title(manager->CreateComponent("PC_TITLE_QUEST_CURRENT", false));
    static_cast<CList*>(list)->AddNonScrollingChild(title);
    frame->AddPositionChild(title.Get());
    inventory_quests->ConstructQuestListPC(container, quests, static_cast<CList*>(list), (QuestScreenType)1, static_cast<CList*>(parent));
    return list;
}
