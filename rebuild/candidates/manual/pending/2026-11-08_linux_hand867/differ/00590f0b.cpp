// CTCInventory::GetItemsListUIComponentPC(NUISystem::CComponent* parent) -- the PC inventory screen's items page: drop the books from the hero's item list, sort the rest (abilities, expressions, hair cards and tattoo cards excluded) into the five UI categories (trophies, potions, gifts, produce, others), build the sub-list with the quick-access item slots (one containment per slot with its observe / assign / unassign / swap actions and the assigned item's icon), the page title registered with the PC frame, and one titled category list per category from the text bank; returns the sub-list
// Residue (v1, DIFFER 2831v2907, md 719, 9 grades; structurally identical to the ConstructItemCategoryListPC tail modulo colouring): (a) ours specialises the ability-skip `continue` cleanup (the fact "[itemDef slot] != 0" survives the visible same-TU GetSubDef call, so the null test is dropped and the release is laid inline, sharing only the final call), retail jumps to the shared release chain 0x5910C4; (b) retail duplicates the card-skip releases at 0x591219 with the tattoo handle in ecx, ours threads into the chain with eax (scratch rotation, downstream of (a)); (c) colouring: retail homes the text-bank reference in memory ([ebp+5Ch], reloaded 4x) and keeps `this` in ebx; ours (c2/00590f0b.v1.trace) ranks textBank prio 64 -> ebx above `this` prio -32 (split). Quantity: the bank local's register-candidate status (same as 0x005F59C7; d5 diagnostic there reproduced retail's colouring once the bank's address escaped).
// Next idea: find the spelling that kills the itemDef non-null fact across GetSubDef (declare abilityDef before the GetDef call, test the GetDef result instead of IsNull, or call GetSubDef through a declared-only non-template overload); then the bank spelling from 0x005F59C7 (a natural non-candidate form) fixes (c) for both twins at once.
// Leaf: ?GetItemsListUIComponentPC@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00590F0B, 2907 bytes, `ret 4`. Biased frame (lea ebp,[esp-74h]): optimize("s",on) alone.
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   _Rb_tree::clear 0x00593235, CTCInventoryBase::GetAllItems 0x005BF482, CTCInventoryItem::IsBook 0x005D9970 / IsExpression
//   0x005D9830 / GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0 / GetUIInventoryCategory 0x005D96F0, vector<pair<long,
//   long> >::erase 0x00593139 / push_back 0x005947B6, CMainGameComponent::PeekDefinitionManager 0x00415FBC,
//   CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040, CDefClassBase::GetSubDef<CAbilityDef> 0x005936C2,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / ~ 0x004291DE, CCountedPointer<CAction>(T*) 0x0042AA29 / ~ 0x00429160, operator
//   new 0x00BFEA1A, CAction::CAction 0x0042BE50 / AddParam 0x0053FDE0 / AddParam(param) 0x0053FDB0, list<CActionParam*>::push_back
//   0x0042B646, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   CTCInventory::GetQuickAccessItemGraphicIndex 0x00581FA4, CWideString 0x0099AED0 / = 0x0099B7D0 / ~ 0x0099B510,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0 (CMainGameComponent::Get() +0x14), CTCInventory::ConstructItemCategoryListPC
//   0x005907AB, free 0x00BFEA14 (the six vectors). Virtuals: CComponent FindChild 3, AddChild 59, AddAction 66, AddActionOnSelected
//   67, AddActionOnDropped 74, AddActionOnDroppedNowhere 75, SetName 83, AddPositionChild 124; CList AddNonScrollingChild 145,
//   InitialiseOffsets 156; CSprite SetGraphicIndex 145. The global 0x013B8B4C is the file's anonymous-namespace CCharString
//   TITLE_NAME ("ITEMS TITLE"; the landed ConstructItemCategoryListPC 0x005907AB of this TU names it the same way).
// Shape: the book filter is an erase loop over the item vector; the object-def handle is zeroed before the manager lookup and the
//   ability sub-def handle after it; an item with an ability sub-def, an expression, a hair card or a tattoo card is skipped with
//   `continue` (each skip releases the live handles in place: the card handles with the in-place count test, the def handles
//   with the fused one); the category switch's case 4 and default both feed the others list; the slot loop is the expressions
//   page's (0x005F59C7) with `this` as the inventory.
// Layout: engine/CTCInventory.h (retail branch, FABLE_RETAIL_CTCINVENTORY_QUICKACCESS / _PCMENUS) is `this`: QuickAccessItems
//   +0x148, ItemsMenuEntries +0x1fc, AssignedIcons +0x208; CMainGameComponent::PTextBank +0x14.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_PCMENUS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventory.h"
#include "engine/CThingObjectDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/fable_script_text.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CList;
using NUISystem::CSprite;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

class CAbilityDef;                                            // the ability (spell) sub-def class; only its name is needed here

namespace { extern CCharString TITLE_NAME; }                  // 0x013B8B4C ("ITEMS TITLE")

// Same-TU bodies of the two def lookups the row calls (retail instantiates them in this TU and calls them out of line:
// CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040, CDefClassBase::GetSubDef<CAbilityDef> 0x005936C2). With the bodies
// visible VC7.1 knows the handles' addresses do not escape, so their releases are the fused `dec [ecx+4] ; jne` with the null
// store dropped, as retail's are; the card handles filled by the out-of-line static lookups keep the in-place form and the store.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.IsNull())
        return false;
    out->SetDynamicCast(def);
    return true;
}
template <> bool CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer<const CAbilityDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CAbilityDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

CComponent* CTCInventory::GetItemsListUIComponentPC(CComponent* parent)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CComponent* frame = parent->FindChild("PC_UI_FRAME");
    ItemsMenuEntries.clear();
    AssignedIcons.clear();

    // the carried items, books dropped (they have their own page), sorted into the five item categories
    std::vector<std::pair<long, long> > allItems;
    std::vector<std::pair<long, long> > trophies;
    std::vector<std::pair<long, long> > potions;
    std::vector<std::pair<long, long> > gifts;
    std::vector<std::pair<long, long> > produce;
    std::vector<std::pair<long, long> > others;
    GetAllItems(allItems);
    for (std::vector<std::pair<long, long> >::iterator it = allItems.begin(); it != allItems.end();) {
        if (CTCInventoryItem::IsBook(it->first))
            it = allItems.erase(it);
        else
            ++it;
    }
    for (unsigned long i = 0; i < allItems.size(); ++i) {
        // abilities (spells), expressions, hair cards and tattoo cards are shown on other pages
        CDefPointer<const CThingObjectDef> itemDef;
        CMainGameComponent::Get()->PeekDefinitionManager().GetDef(allItems[i].first, &itemDef);
        CDefPointer<const CAbilityDef> abilityDef;
        if (!itemDef.IsNull() && itemDef->GetSubDef(&abilityDef))
            continue;
        if (CTCInventoryItem::IsExpression(allItems[i].first))
            continue;
        CDefPointer<const CHairCardDef> hairCardDef = CTCInventoryItem::GetHairCardDef(allItems[i].first);
        CDefPointer<const CTattooDef> tattooCardDef = CTCInventoryItem::GetTattooCardDef(allItems[i].first);
        if (!hairCardDef.IsNull() || !tattooCardDef.IsNull())
            continue;
        switch (CTCInventoryItem::GetUIInventoryCategory(allItems[i].first)) {
        case 0:  trophies.push_back(allItems[i]); break;
        case 1:  potions.push_back(allItems[i]);  break;
        case 2:  gifts.push_back(allItems[i]);    break;
        case 3:  produce.push_back(allItems[i]);  break;
        case 4:  others.push_back(allItems[i]);   break;
        default: others.push_back(allItems[i]);   break;
        }
    }

    // the page: a sub-list holding the quick-access items container and the bottom-left position dummy
    CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CComponent> itemsContainer(manager->CreateComponent("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER", false));
    subList->AddNonScrollingChild(itemsContainer);
    CCountedPointer<CComponent> positionDummy(manager->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT", false));
    subList->AddNonScrollingChild(positionDummy);
    positionDummy->AddPositionChild(itemsContainer.Data);

    // one containment slot per quick-access item, named from its 1-based number
    for (unsigned long slot = 0; slot < QuickAccessItems.size(); ++slot) {
        unsigned long slotNumber = slot + 1;
        CCharString containmentName = "PC_EXPRESSION_ITEM_CONTAINMENT_" + GFIntToCharString(slotNumber);
        CComponent* containment = subList->FindChild(containmentName);
        if (containment) {
            // selecting the slot in the list raises the two observe events on it
            CCountedPointer<CAction> observeAction(new CAction);
            observeAction->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeAction->AddParam(containment, observeAction);
            observeAction->UIActionParams.push_back(new CActionParamULONG(25));
            subList->AddActionOnSelected(observeAction);
            CCountedPointer<CAction> observeAction2(new CAction);
            observeAction2->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeAction2->AddParam(containment, observeAction2);
            observeAction2->UIActionParams.push_back(new CActionParamULONG(30));
            subList->AddActionOnSelected(observeAction2);
            // clicking the slot assigns the item being assigned to it
            CCountedPointer<CAction> assignAction(new CAction);
            assignAction->ActionType = NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC;
            assignAction->AddParam(containment, assignAction);
            assignAction->UIActionParams.push_back(new CActionParamULONG(slot));
            containment->AddAction(assignAction);
            if (QuickAccessItems[slot] == 0) {
                // an empty slot shows the plain inside-container
                CCountedPointer<CComponent> emptyContainer(manager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER", false));
                containment->AddChild(emptyContainer);
            } else {
                // an assigned slot shows the item's icon, draggable off (unassign) and onto another slot (swap)
                CCountedPointer<CComponent> iconContainer(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
                CComponent* iconSprite = iconContainer->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
                if (iconSprite && QuickAccessItems[slot] != 0)
                    static_cast<CSprite*>(iconSprite)->SetGraphicIndex(GetQuickAccessItemGraphicIndex(slot));
                CCountedPointer<CAction> unassignAction(new CAction);
                unassignAction->ActionType = NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
                unassignAction->AddParam(containment, unassignAction);
                unassignAction->UIActionParams.push_back(new CActionParamULONG(slot));
                iconContainer->AddActionOnDroppedNowhere(unassignAction);
                CCountedPointer<CAction> observeIconAction(new CAction);
                observeIconAction->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                observeIconAction->AddParam(iconContainer.Data, observeIconAction);
                observeIconAction->UIActionParams.push_back(new CActionParamULONG(25));
                subList->AddActionOnSelected(observeIconAction);
                CCountedPointer<CAction> swapAction(new CAction);
                swapAction->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
                swapAction->UIActionParams.push_back(new CActionParamSLONG(QuickAccessItems[slot]));
                swapAction->UIActionParams.push_back(new CActionParamSLONG(slot));
                swapAction->AddParam(containment);
                iconContainer->AddActionOnDropped(swapAction);
                containment->AddChild(iconContainer);
            }
        }
    }

    // the page title, registered with the PC frame so it is positioned with it
    CCountedPointer<CComponent> title(manager->CreateComponent("PC_ITEM_TITLE", false));
    title->SetName(TITLE_NAME);
    subList->AddNonScrollingChild(title);
    if (frame)
        frame->AddPositionChild(title.Data);
    subList->InitialiseOffsets(5);

    // the five category lists, titled from the text bank (gifts check for a usable target)
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CWideString categoryTitle;
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
    ConstructItemCategoryListPC(categoryTitle, subList, trophies, parent, "PC_RING_PIC_TROPHIES", "TEXT_GUI_MENUGUIDE_ITEMS_TROPHIES", false);
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
    ConstructItemCategoryListPC(categoryTitle, subList, potions, parent, "PC_RING_PIC_POTIONS", "TEXT_GUI_MENUGUIDE_ITEMS_MAGICPOTIONS", false);
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
    ConstructItemCategoryListPC(categoryTitle, subList, gifts, parent, "PC_RING_PIC_GIFTS", "TEXT_GUI_MENUGUIDE_ITEMS_GIFTS", true);
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
    ConstructItemCategoryListPC(categoryTitle, subList, produce, parent, "PC_RING_PIC_PRODUCE", "TEXT_GUI_MENUGUIDE_ITEMS_PRODUCE", false);
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_OTHER");
    ConstructItemCategoryListPC(categoryTitle, subList, others, parent, "PC_RING_PIC_OTHERS", "TEXT_GUI_MENUGUIDE_ITEMS_OTHERS", false);
    return subList;
}
