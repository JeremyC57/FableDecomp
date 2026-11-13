// CTCQuickAccessMenu::ContructItemLists(std::vector<std::pair<long,long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& actions_list) -- quick-access item entries
// Leaf: ?ContructItemLists@CTCQuickAccessMenu@@AAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_ACTIONLISTS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/CTCQuickAccessMenu.h"

void CTCQuickAccessMenu::ContructItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                                           CCountedPointer<NUISystem::CComponent>& actions_list)
{
    using namespace NUISystem;
    CManager* manager = CManager::GetInstance();
    CTCInventory* inventory = 0;
    ParentThing->GetTC(&inventory);
    for (unsigned long i = 0; i < items.size() && true; ++i) {
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS",
                                                                            0.0f, "UI_QUICK_ACCESS_EXPRESSION_MENU_ENTRY_TEXT", 0.0f, "PC_BUTTON_BASTARD"));
        inventory->ItemsMenuEntries[items[i].first] = entry.Get();
        if (inventory->QuickAccessItems[2] == items[i].first) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[2] = icon.Get();
        } else if (inventory->QuickAccessItems[3] == items[i].first) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[3] = icon.Get();
        } else if (inventory->QuickAccessItems[0] == items[i].first) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[0] = icon.Get();
        } else if (inventory->QuickAccessItems[1] == items[i].first) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[1] = icon.Get();
        }
        CMenuEntry* menu_entry = static_cast<CMenuEntry*>(entry.Get());
        CCountedPointer<CAction> load_name(new CAction);
        load_name->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
        load_name->UIActionParams.push_back(new CActionParamSLONG(items[i].first));
        load_name->AddParam(entry.Get(), load_name);
        load_name->UIActionParams.push_back(new CActionParamSLONG(1));
        menu_entry->AddActionOnVisible(load_name);
        CCountedPointer<CAction> load_icon(new CAction);
        load_icon->ActionType = UI_ACTION_LOAD_QUICK_MENU_EXPRESSIONS_ICON;
        load_icon->UIActionParams.push_back(new CActionParamSLONG(items[i].first));
        load_icon->AddParam(entry.Get(), load_icon);
        menu_entry->AddActionOnVisible(load_icon);
        menu_entry->AddThingActionParam(items[i].first);
        list->AddScrollingChildAndSetOffsets(entry);
        entry->AddChild(actions_list);
    }
}
