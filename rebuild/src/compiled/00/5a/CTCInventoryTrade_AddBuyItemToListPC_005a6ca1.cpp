// 005a6ca1 CTCInventoryTrade::AddBuyItemToListPC
// Leaf: ?AddBuyItemToListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJPAVCList@3@PAVCViewport@3@_N@Z
// Flags:
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
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
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
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
#define FABLE_RETAIL_CACTION_ORIGINAL_DTOR_DECL_PASS26
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"
using namespace NUISystem;

#include "engine/CThingPlayerCreature.h"

// Inline-budget: the two STLport list operations are called at 0042AADC and 0042B646.
// The list node allocation at 00429FBE also uses size optimization. Only push_back retains a frame.

#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
template<> inline std::_List_node<CActionParam*>* std::list<NUISystem::CActionParam*>::_M_create_node(CActionParam* const& x) {
    std::_List_node<CActionParam*>* p=this->_M_node.allocate(1);
    new((void*)&p->_M_data) CActionParam*(x);
    return p;
}
template<> inline std::list<NUISystem::CActionParam*>::iterator std::list<NUISystem::CActionParam*>::insert(iterator position,CActionParam* const& x) {
    std::_List_node<CActionParam*>* tmp=_M_create_node(x);
    std::_List_node_base* n=position._M_node;
    std::_List_node_base* p=n->_M_prev;
    tmp->_M_next=n;
    tmp->_M_prev=p;
    p->_M_next=tmp;
    n->_M_prev=tmp;
    return iterator(tmp);
}
#ifdef _MSC_VER
#pragma optimize("y", off)
#endif
template<> inline void std::list<NUISystem::CActionParam*>::push_back(CActionParam* const& x){ insert(end(), x); }
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
bool CTCInventoryTrade::AddBuyItemToListPC(CComponent* pframe, long item_def, long quantity, CList* pbuyable_list, CViewport* pviewport, bool dynamic)
{
    bool buyable = true;
    CManager* pmanager = CManager::GetInstance();
    CCountedPointer<CComponent> pmenu_entry;
    const CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const NInventory::CTCInventoryBase* ptc_inventory = 0;
    pplayer->PeekTC(CTCInventoryItem::GetInventoryType(item_def), &ptc_inventory);
    bool already_have_special = AlreadyHaveSpecial(item_def);
    if (IsItemAffordable(item_def, 1) && (ptc_inventory->IsSpaceInInventoryForItem(item_def) || IsSpecialCase(item_def)) &&
        quantity > 0) {
        if (!already_have_special) {
            pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        } else {
            buyable = false;
            pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            pmenu_entry->MakeUnavailable();
        }
    } else {
        buyable = false;
        pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        pmenu_entry->MakeUnavailable();
    }
    if (IsSpecialCase(item_def)) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = UI_ACTION_TYPE_CLOSE_ACTIVE_TRADE_MENU;
        pmenu_entry->AddActionOnBack(paction);
    }
    std::map<CComponent*, std::pair<long, long> >::value_type value_pair(pmenu_entry.Get(), std::pair<long, long>(item_def, quantity));
    ItemsToBuy.insert(value_pair);
    CMenuEntry* pmenu_entry_real = GFDebugPDynamicCast(pmenu_entry.Get(), (CMenuEntry*)0);
    CCountedPointer<CAction> paction_load_mesh(new CAction);
    paction_load_mesh->ActionType = UI_ACTION_ADD_MESH_CHILD;
    paction_load_mesh->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_mesh->AddParam(pmenu_entry.Get(), paction_load_mesh);
    CWideString component_type(L"PC_MODEL");
    paction_load_mesh->UIActionParams.push_back(new CActionParamString(component_type));
    paction_load_mesh->AddParam(pframe, paction_load_mesh);
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
    paction_load_item_description->AddParam(pframe, paction_load_item_description);
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

#ifdef _MSC_VER
#pragma optimize("", on)
#endif