// CTCInventoryAbilities::ConstructPCItemLists(std::vector<std::pair<long,long> >& items, NUISystem::CList* list, NUISystem::CComponent* parent, CCountedPointer<NUISystem::CComponent>& actions) -- PC expression item rows: for every item a branch entry named after the item (registered in the inventory's menu-entry map) holding its details, a draggable assignment icon and its faded twin showing the item's graphic (dropping it sets the expression item to assign), its name and icon, each registered with the PC frame; the entry carries the item def index as its thing parameter and the shared actions list, and the list starts / stops the icon observing mouse movement on selection / inactivity
// Leaf: ?ConstructPCItemLists@CTCInventoryAbilities@@QAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F1526, 1514 bytes, thiscall `ret 10h`; biased frame (`lea ebp,[esp-68h]`, optimize "s").
//   FableWin.pdb ?ConstructPCItemLists@CTCInventoryAbilities@@QAEX..; the PC twin of ContructItemLists 0x005F077F, called by
//   ConstructPCExpressionCategoryList 0x005F45B5.
// Callees: as 0x005F077F, plus operator new 0x00BFEA1A, CAction 0x0042BE50 / AddParam(param, self) 0x0053FDE0, CCountedPointer<CAction>
//   0x0042AA29 / ReleaseData 0x00429160, list<CActionParam*>::push_back 0x0042B646; CComponent slots 67 AddActionOnSelected,
//   70 AddActionOnInactive, 74 AddActionOnDropped, 124 AddPositionChild.
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
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
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
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"

void CTCInventoryAbilities::ConstructPCItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* parent,
                                                 CCountedPointer<NUISystem::CComponent>& actions)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* frame = parent->FindChild("PC_UI_FRAME");
    for (unsigned long i = 0; i < items.size(); ++i) {
        CWideString name = CTCInventoryItem::GetDescription(items[i].first);
        CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(name, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                                                  CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        CTCInventory* inventory = 0;
        ParentThing->GetTC(&inventory);
        inventory->ItemsMenuEntries[items[i].first] = entry.Get();
        CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("PC_DESCRIPTION_INDEPENDANT", false));
        CCountedPointer<NUISystem::CText> descriptionText;
        CWideString details = CTCInventoryItem::GetDetails(items[i].first);
        descriptionText.SetStaticCast(description);
        descriptionText->SetValue(details, true, true);
        entry->AddChild(description);
        // the draggable assignment icon (dropping it picks the expression item to assign) and its faded twin
        CCountedPointer<NUISystem::CComponent> assignmentIcon(ui->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON", false));
        NUISystem::CComponent* iconSprite = assignmentIcon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
        CCountedPointer<NUISystem::CComponent> fadedIcon(ui->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_FADED", false));
        NUISystem::CSprite* fadedSprite = static_cast<NUISystem::CSprite*>(fadedIcon.Get());
        CCountedPointer<NUISystem::CAction> assign(new NUISystem::CAction);
        assign->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC;
        assign->UIActionParams.push_back(new NUISystem::CActionParamSLONG(items[i].first));
        assignmentIcon->AddActionOnDropped(assign);
        if (items[i].first != 0) {
            static_cast<NUISystem::CSprite*>(iconSprite)->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(items[i].first).BankIndex);
            fadedSprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(items[i].first).BankIndex);
        }
        entry->AddChild(assignmentIcon);
        entry->AddChild(fadedIcon);
        if (frame)
            frame->AddPositionChild(description.Get());
        NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
        if (viewport)
            viewport->AddFakeChild(description.Get());
        CCountedPointer<NUISystem::CComponent> nameComponent(ui->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
        CCountedPointer<NUISystem::CText> nameText;
        nameText.SetStaticCast(nameComponent);
        nameText->SetValue(name, true, true);
        entry->AddChild(nameComponent);
        if (frame)
            frame->AddPositionChild(nameComponent.Get());
        if (CTCInventoryItem::GetEngineGraphic(items[i].first).BankIndex != 0) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("PC_ICON_INDEPENDANT", false));
            if (icon->GetType() == static_cast<NUISystem::EType>(0))
                static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(items[i].first).BankIndex);
            entry->AddChild(icon);
            if (frame)
                frame->AddPositionChild(icon.Get());
        }
        NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Get());
        if (menuEntry)
            menuEntry->AddThingActionParam(items[i].first);
        entry->AddChild(actions);
        list->AddScrollingChildAndSetOffsets(entry);
        // while the entry is selected the assignment icon follows the mouse; it stops when the list goes inactive
        CCountedPointer<NUISystem::CAction> observe(new NUISystem::CAction);
        observe->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observe->AddParam(assignmentIcon.Get(), observe);
        observe->UIActionParams.push_back(new NUISystem::CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observe);
        CCountedPointer<NUISystem::CAction> ignore(new NUISystem::CAction);
        ignore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignore->AddParam(assignmentIcon.Get(), ignore);
        ignore->UIActionParams.push_back(new NUISystem::CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnInactive(ignore);
    }
}