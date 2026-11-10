// CTCInventoryTrade::AttachToFrame(NUISystem::CComponent* component) -- PC trade screen: hangs the gold readout, the coin sprite and the category ring pictures of the open trade menu on its inactive frame (so they move with it), then makes the menu's scroll arrows and back / close buttons observe mouse movement through the UI manager
// Leaf: ?AttachToFrame@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059BB1B, 1451 bytes, thiscall `ret 4`, no frame pointer (`sub esp,10h`): optimize("s",on)
//   only, as the landed ConstructActualWantedInventoryPC 0x005A5B31 of the same TU (`push 19h ; pop ebp`, `or ebx,-1`).
// Identity: Ego_r.pdb / FableWin.pdb ?AttachToFrame@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@@Z, between
//   GetMenuBranchFromDefIndex 0x0059B79A and GetMenuBranchFromDefIndexPC 0x0059C0C6 as in the Ego_r.pdb donor order (0x00607E68 /
//   0x006081AF / 0x0060884E); its callers are the three PC inventory builders ConstructActualSellInventoryPC 0x005A4CDD,
//   ConstructActualWantedInventoryPC 0x005A5B31 (landed, calls it AttachToFrame) and ConstructActualBuyInventoryPC 0x005AC10E. The
//   component argument is not read: every child is found from the open trade menu PCurrentOpenTradeMenu (+0x224).
// Callees: CComponent::FindChild (slot 3, CCharString by value: CCharString(const char*, long) 0x0099EBF0), AddPositionChild
//   (slot 124, +0x1F0), CObserver::ObserveEvent (slot 3 of the CObserver base at +4), NUISystem::CManager::GetInstance 0x0041E5F2
//   and CObservable::AddObserver (slot 2, +8).
// Shape: each position child is found into one reused local before the frame's call (retail loads the frame's vtable after the
//   lookup), and the second-level entries of the three category lists go through ONE reused local `entry`: VC7.1 then gives
//   all of them the dead clothing-entry frame word ([esp+10h]), as retail does (0x0059BC7A / 0x0059BDE8 / 0x0059BE80); a
//   nested `list->FindChild(..)->FindChild(..)` temporary takes the just-dead weapons / items entry word instead.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_enums.h"
using namespace NUISystem;

void CTCInventoryTrade::AttachToFrame(CComponent* component)
{
    CComponent* menu = PCurrentOpenTradeMenu;
    CComponent* frame = menu->FindChild("PC_UI_INACTIVE_FRAME");
    if (frame) {
        CComponent* child;
        CComponent* entry;
        child = menu->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN");
        frame->AddPositionChild(child);
        child = menu->FindChild("PC_GOLD_COIN_SPRITE");
        frame->AddPositionChild(child);
        CComponent* clothingEntry = menu->FindChild("PC_TRADING_MENU_CLOTHING_ENTRY");
        child = clothingEntry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);
        CComponent* weaponsEntry = menu->FindChild("PC_TRADING_MENU_WEAPONS_ENTRY");
        child = weaponsEntry->FindChild("PC_RING_PIC_WEAPONS");
        frame->AddPositionChild(child);
        CComponent* itemsEntry = menu->FindChild("PC_TRADING_MENU_ITEMS_ENTRY");
        child = itemsEntry->FindChild("PC_RING_PIC_ITEMS");
        frame->AddPositionChild(child);

        CComponent* clothingList = clothingEntry->FindChild("PC_TRADING_CLOTHING_LIST");
        entry = clothingList->FindChild("PC_TRADING_MENU_HEAD_ENTRY");
        child = entry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);
        entry = clothingList->FindChild("PC_TRADING_MENU_HANDS_ENTRY");
        child = entry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);
        entry = clothingList->FindChild("PC_TRADING_MENU_TORSO_ENTRY");
        child = entry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);
        entry = clothingList->FindChild("PC_TRADING_MENU_SUITS_ENTRY");
        child = entry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);
        entry = clothingList->FindChild("PC_TRADING_MENU_LEGS_ENTRY");
        child = entry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);
        entry = clothingList->FindChild("PC_TRADING_MENU_FEET_ENTRY");
        child = entry->FindChild("PC_RING_PIC_CLOTHING");
        frame->AddPositionChild(child);

        CComponent* weaponsList = weaponsEntry->FindChild("PC_TRADING_WEAPONS_LIST");
        entry = weaponsList->FindChild("PC_TRADING_MENU_MELEE_ENTRY");
        child = entry->FindChild("PC_RING_PIC_MELEE");
        frame->AddPositionChild(child);
        entry = weaponsList->FindChild("PC_TRADING_MENU_RANGED_ENTRY");
        child = entry->FindChild("PC_RING_PIC_RANGED");
        frame->AddPositionChild(child);

        CComponent* itemsList = itemsEntry->FindChild("PC_TRADING_ITEMS_LIST");
        entry = itemsList->FindChild("PC_TRADING_MENU_TROPHIES_ENTRY");
        child = entry->FindChild("PC_RING_PIC_TROPHIES");
        frame->AddPositionChild(child);
        entry = itemsList->FindChild("PC_TRADING_MENU_POTIONS_ENTRY");
        child = entry->FindChild("PC_RING_PIC_POTIONS");
        frame->AddPositionChild(child);
        entry = itemsList->FindChild("PC_TRADING_MENU_GIFTS_ENTRY");
        child = entry->FindChild("PC_RING_PIC_GIFTS");
        frame->AddPositionChild(child);
        entry = itemsList->FindChild("PC_TRADING_MENU_PRODUCE_ENTRY");
        child = entry->FindChild("PC_RING_PIC_PRODUCE");
        frame->AddPositionChild(child);
        entry = itemsList->FindChild("PC_TRADING_MENU_OTHERS_ENTRY");
        child = entry->FindChild("PC_RING_PIC_OTHERS");
        frame->AddPositionChild(child);
    }

    // the scroll arrows and the back / close buttons observe mouse movement
    CComponent* button = menu->FindChild("SCROLL_LIST_UP");
    if (button) {
        CManager::GetInstance()->AddObserver(button);
        button->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    button = menu->FindChild("SCROLL_DESCRIPTION_UP");
    if (button) {
        CManager::GetInstance()->AddObserver(button);
        button->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    button = menu->FindChild("SCROLL_LIST_DOWN");
    if (button) {
        CManager::GetInstance()->AddObserver(button);
        button->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    button = menu->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (button) {
        CManager::GetInstance()->AddObserver(button);
        button->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    button = menu->FindChild("PC_BACK_CLICKABLE");
    if (button) {
        CManager::GetInstance()->AddObserver(button);
        button->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    button = menu->FindChild("PC_CLOSE_CLICKABLE");
    if (button) {
        CManager::GetInstance()->AddObserver(button);
        button->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
}