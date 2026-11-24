// CTCInventoryTrade::AddBuyItemToList(long itemDefIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool dynamically) -- console shop: builds the buy-menu entry for one shop item (a branch entry when the hero can afford it, has room for it or it is a special case, the shop has some and the hero does not already own the special; else a plain entry made unavailable), records it in ItemsToBuy, wires its seven actions (add / remove the item model, load / unload the buy description and the buy-actions menu, load the entry's name) and adds it to the list; returns whether it is a branch entry
// Grade: RELOCATION_MATCH (q.py) -- the unavailable arm is written twice: the outer if tests affordable && (space || special) && quantity > 0, the inner if tests !already_have_special (0x0059FF9A..0x0059FFA0 `cmp ; push -1 ; jne 0x5A0028`: the inner arms' common first push is hoisted, the inner else is tail-merged into the outer else one instruction in); PeekTC(GetInventoryType(item_def), &ptc_inventory) with no inventory_type local (Ego_d has none)
// Leaf: ?AddBuyItemToList@CTCInventoryTrade@@QAE_NJJPAVCList@NUISystem@@PAVCViewport@3@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059FEE3, 1556 bytes, thiscall `ret 14h`, bool in al. Frame-pointer size TU: optimize("y",off) + ("s",on).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU   // NF364: Reset<CComponent> 0x00429C6F pick-any copy in this y-off / s-on TU
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"
using namespace NUISystem;

bool CTCInventoryTrade::AddBuyItemToList(long item_def, long quantity, CList* pbuyable_list, CViewport* pviewport, bool dynamic)
{
    bool buyable = true;
    CManager* pmanager = CManager::GetInstance();
    CCountedPointer<CComponent> pmenu_entry;
    const CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const NInventory::CTCInventoryBase* ptc_inventory = 0;
    pplayer->PeekTC(CTCInventoryItem::GetInventoryType(item_def), &ptc_inventory);
    bool already_have_special = AlreadyHaveSpecial(item_def);
    if (IsItemAffordable(item_def, 1) && (ptc_inventory->IsSpaceInInventoryForItem(item_def) || IsSpecialCase(item_def)) && quantity > 0) {
        if (!already_have_special) {
            pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        } else {
            buyable = false;
            pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            pmenu_entry->MakeUnavailable();
        }
    } else {
        buyable = false;
        pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                             0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        pmenu_entry->MakeUnavailable();
    }
    std::pair<CComponent* const, std::pair<long, long> > value_pair(pmenu_entry.Get(), std::pair<long, long>(item_def, quantity));
    ItemsToBuy.insert(value_pair);
    CMenuEntry* pmenu_entry_real = static_cast<CMenuEntry*>(pmenu_entry.Get());
    CCountedPointer<CAction> paction_load_mesh(new CAction);
    paction_load_mesh->ActionType = UI_ACTION_ADD_MESH_CHILD;
    paction_load_mesh->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_mesh->AddParam(pmenu_entry.Get(), paction_load_mesh);
    CWideString component_type(L"UI_ITEM_MODEL_TEMPLATE_NEW");
    paction_load_mesh->UIActionParams.push_back(new CActionParamString(component_type));
    pmenu_entry->AddActionOnSelected(paction_load_mesh);
    CCountedPointer<CAction> paction_unload_mesh(new CAction);
    paction_unload_mesh->ActionType = UI_ACTION_REMOVE_MESH_CHILD;
    paction_unload_mesh->AddParam(pmenu_entry.Get(), paction_unload_mesh);
    pmenu_entry_real->AddActionOnUnselected(paction_unload_mesh);
    CCountedPointer<CAction> paction_load_item_description(new CAction);
    paction_load_item_description->ActionType = UI_ACTION_LOAD_BUY_ITEM_DESC;
    paction_load_item_description->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_item_description->AddParam(pviewport, paction_load_item_description);
    paction_load_item_description->AddParam(pmenu_entry.Get(), paction_load_item_description);
    pmenu_entry->AddActionOnSelected(paction_load_item_description);
    CCountedPointer<CAction> paction_unload_item_description(new CAction);
    paction_unload_item_description->ActionType = UI_ACTION_UNLOAD_BUY_ITEM_DESC;
    paction_unload_item_description->AddParam(pmenu_entry.Get(), paction_unload_item_description);
    pmenu_entry_real->AddActionOnUnselected(paction_unload_item_description);
    CCountedPointer<CAction> paction_load_action_menu(new CAction);
    paction_load_action_menu->ActionType = UI_ACTION_LOAD_BUY_ACTION_MENU;
    paction_load_action_menu->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_action_menu->AddParam(pmenu_entry.Get(), paction_load_action_menu);
    pmenu_entry->AddPreAction(paction_load_action_menu);
    CCountedPointer<CAction> paction_unload_action_menu(new CAction);
    paction_unload_action_menu->ActionType = UI_ACTION_UNLOAD_BUY_ACTION_MENU;
    paction_unload_action_menu->AddParam(pmenu_entry.Get(), paction_unload_action_menu);
    pmenu_entry_real->AddActionOnUnselected(paction_unload_action_menu);
    CCountedPointer<CAction> paction_load_item_name(new CAction);
    paction_load_item_name->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    paction_load_item_name->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_item_name->AddParam(pmenu_entry.Get(), paction_load_item_name);
    paction_load_item_name->UIActionParams.push_back(new CActionParamSLONG(quantity));
    pmenu_entry_real->AddActionOnVisible(paction_load_item_name);
    pmenu_entry_real->AddUIActionParam(pmenu_entry.Get());
    if (dynamic)
        pbuyable_list->AddScrollingChildDynamicallyAndSetOffsets(pmenu_entry);
    else
        pbuyable_list->AddScrollingChildAndSetOffsets(pmenu_entry);
    return buyable;
}