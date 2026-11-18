// CTCInventoryTrade::ConstructSellActionsListPC(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically) -- PC variant of 0x005A45F6 (PC list and entry templates, 0/1 entry scales): builds the sell-actions sub-list for one item: a "sell" entry plus "sell 10", "sell 50" and "sell all (n)" entries as the count allows; when the count is non-zero and the shop stocks the item, remembers the list in ItemSellLists[parent] and attaches it to the parent
// Leaf: ?ConstructSellActionsListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A731F, 1735 bytes, `ret 10h`, bool in al (bl). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / dtor 0x004291DE, CPlayerManager::GetMainPlayer 0x00449960 (CMainGameComponent::Get()
//   0x013B86A0 +0x1C) + CPlayer::GetControlledCreature 0x00487DD0, CTCInventoryBase::GetTradingThing 0x005BC4C4, std::bitset::test
//   0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (inline GetInterface, type 0x53 = CTCShop), CTCInventoryItem::GetInventoryCategory 0x005D99D0,
//   CDataBank::GetTextBySymbol 0x009C95E0 (text bank at CMainGameComponent::Get() +0x14), IntToWideString 0x00405F00 (`__fastcall`),
//   CWideString operator+ 0x0099BE70 (`__fastcall`), CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   operator new 0x00BFEA1A, CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646
//   (same-TU noinline stand-in), CTCShop::IsValidStockItem 0x007723D0, ItemSellLists operator[] 0x005B3167.
// Shape: the AddSellItemToList 0x0059ED42 TU conventions; the hero's tab presence test is a bare statement (its result is
//   dropped, so only the bitset call survives); the shop pointer is zero-initialised in memory and the lookup key reuses its
//   slot; the three literal addresses of the repeated entry construction are CSE'd into ebx/edi/esi by VC; the item count is
//   an if/else chain of four SetItemCount calls that VC cross-jumps into one call site (a ?: chain becomes seta/inc);
//   the list object is read into a local before the map operator[] call; the two returns share the destructor tail through bl.
// Layout: retail CTCInventoryTrade ItemSellLists +0x20C (map<CComponent*, CComponent*>, PDB +0x258); CActionParamULONG
//   (vtable 0x0122F844, type 5) 12 bytes.
// NF85 (q11059): moved off the row-local CCharString / CWideString / CDataBank / CThing / CTCShop / CPlayer / CPlayerManager /
//   CMainGameComponent / NUISystem CActionParam / CActionParamULONG / CAction / CComponent / CManager / map stand-in / CTCInventoryTrade
//   models onto the shared headers, with the include and opt-in set of its landed console twin 0x005A45F6 (same TU). Bindings that change:
//   CreateMenuEntryComponent 0x00428494 now binds the Ego_r.pdb spelling ..MV5@MV5@@Z (the local model's non-const 6th parameter gave the
//   stand-in ..MV5@M2@Z); the own leaf is the Ego_r.pdb / FableWin.pdb ?ConstructSellActionsListPC@..@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z
//   (count is `long`, as CTCInventoryTrade.h and the landed caller CManager::Action 0x0041E6D3 have it; the old row said unsigned long).
//   The hero pointer type is engine/CThingPlayerCreature.h's retail branch (FABLE_RETAIL_CTHINGPLAYERCREATURE over the retail creature
//   chain CTHINGPHYSICAL / CTHINGGAMEOBJECT / CTHINGCREATUREBASE), not the twin's empty local `class CThingPlayerCreature : public CThing`.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_CONTAINABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"
using namespace NUISystem;
using NGameText::CDataBank;


CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00


bool CTCInventoryTrade::ConstructSellActionsListPC(CComponent* parent, long itemDefIndex, long count, bool dynamically)
{
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> actionsList(CManager::GetInstance()->CreateComponent("PC_LIST_SELL_ACTIONS", false));
    CList* list = static_cast<CList*>(actionsList.Get());
    const NInventory::CTCInventoryBase* inventory = 0;
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    CTCShop* shop = 0;
    const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
    CWideString sellText = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL");
    CCountedPointer<CComponent> sellEntry(manager->CreateMenuEntryComponent(sellText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<CAction> sellAction(new CAction);
    sellAction->ActionType = UI_ACTION_TYPE_SELL;
    sellEntry->AddAction(sellAction);
    unsigned long quantity = count;
    if (quantity > 50)
        list->InitialiseOffsets(4);
    else if (quantity > 10)
        list->InitialiseOffsets(3);
    else if (quantity > 1)
        list->InitialiseOffsets(2);
    else
        list->InitialiseOffsets(1);
    list->SetDefaultChild(0L);
    list->AddScrollingChildAndSetOffsets(sellEntry);
    if (quantity > 10) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + CWideString(L" ") + GFIntToWideString(10);
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SELL_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(10));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity > 50) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + CWideString(L" ") + GFIntToWideString(50);
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SELL_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(50));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity > 1) {
        CWideString label = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL_ALL") + CWideString(L" (") + GFIntToWideString(quantity) + CWideString(L")");
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(label, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SELL_BULK;
        action->UIActionParams.push_back(new CActionParamULONG(quantity));
        entry->AddAction(action);
        list->AddScrollingChildAndSetOffsets(entry);
    }
    if (quantity != 0 && shop->IsValidStockItem(itemDefIndex)) {
        ItemSellLists[parent] = actionsList.Get();
        if (dynamically)
            parent->AddChildDynamically(actionsList);
        else
            parent->AddChild(actionsList);
        return true;
    }
    return false;
}
