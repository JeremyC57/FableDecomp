// CTCInventoryAbilities::ContructItemLists(std::vector<std::pair<long,long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& actions) -- console expression item rows: for every item a branch entry named after the item (registered in the inventory's menu-entry map), its quick-access direction icon when it is assigned to one, the item's details as description (also shown in the owner's scrolling viewport), its name and icon, the item def index as the entry's thing parameter, and the shared actions list as its child
// Leaf: ?ContructItemLists@CTCInventoryAbilities@@AAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F077F, 1248 bytes, thiscall `ret 10h`; biased frame (`lea ebp,[esp-68h]`, optimize "s").
//   FableWin.pdb ?ContructItemLists@CTCInventoryAbilities@@AAEX.. (private; the PDB's own spelling).
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   CTCInventoryItem::GetDescription 0x005D95C0 / GetDetails 0x005D9630 / GetEngineGraphic 0x005D96A0 (static __fastcall),
//   GetTC<CTCInventory> through the bitset test 0x00410DE0 + LowerBound 0x0040F020, map<long,CComponent*>::operator[] 0x005949C3,
//   CCountedPointer<CComponent> 0x00429C15 / ReleaseData 0x004291DE, CCountedPointer<CText> ShareData 0x00429460 / ReleaseData
//   0x0042925C, CCountedPointer<CMenuEntry> ShareData 0x0048524C / ReleaseData 0x004850F9; CText slot 144 SetValue, CViewport 142
//   AddFakeChild, CComponent 59 AddChild / 65 GetType, CSprite 145 SetGraphicIndex, CMenuEntry 150 AddThingActionParam, CList 152
//   AddScrollingChildAndSetOffsets.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ITEMLISTS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"

void CTCInventoryAbilities::ContructItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                                              CCountedPointer<NUISystem::CComponent>& actions)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < items.size(); ++i) {
        CWideString name = CTCInventoryItem::GetDescription(items[i].first);
        CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(name, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f,
                                                                                  CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        CTCInventory* inventory = 0;
        ParentThing->GetTC(&inventory);
        inventory->ItemsMenuEntries[items[i].first] = entry.Get();
        // the quick-access direction the item is assigned to: left (2), right (3), up (0), down (1)
        if (inventory->QuickAccessItems[2] == items[i].first) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[2] = icon.Get();
        } else if (inventory->QuickAccessItems[3] == items[i].first) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[3] = icon.Get();
        } else if (inventory->QuickAccessItems[0] == items[i].first) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[0] = icon.Get();
        } else if (inventory->QuickAccessItems[1] == items[i].first) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[1] = icon.Get();
        }
        CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
        CCountedPointer<NUISystem::CText> descriptionText;
        CWideString details = CTCInventoryItem::GetDetails(items[i].first);
        descriptionText.SetStaticCast(description);
        descriptionText->SetValue(details, true, true);
        entry->AddChild(description);
        NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
        if (viewport)
            viewport->AddFakeChild(description.Get());
        CCountedPointer<NUISystem::CComponent> nameComponent(ui->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
        CCountedPointer<NUISystem::CText> nameText;
        nameText.SetStaticCast(nameComponent);
        nameText->SetValue(name, true, true);
        entry->AddChild(nameComponent);
        if (CTCInventoryItem::GetEngineGraphic(items[i].first).BankIndex != 0) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
            if (icon->GetType() == static_cast<NUISystem::EType>(0))
                static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(items[i].first).BankIndex);
            entry->AddChild(icon);
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        menuEntry->AddThingActionParam(items[i].first);
        list->AddScrollingChildAndSetOffsets(entry);
        entry->AddChild(actions);
    }
}