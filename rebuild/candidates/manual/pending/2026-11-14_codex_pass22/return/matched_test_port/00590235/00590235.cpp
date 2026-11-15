// 00590235 CTCInventory::GetItemsListUIComponent
// Inferred: OTHER/default continue exits reproduce native cleanup; later debug donor uses break.
// Leaf: ?GetItemsListUIComponent@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: 0x00590235,1393 bytes,thiscall ret4.
// Flags: none
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_IMPLICIT_GENERATED_SPECIALS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_CONSOLEMENUS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEFINITION_ORIGINAL_USER_CONSTRUCTORS_PASS22
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CHairCardDef.h"
#include "engine/CTattooDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;
typedef std::vector<std::pair<long,long> > CItemVector;
namespace {extern CCharString TITLE_NAME;}

CComponent* CTCInventory::GetItemsListUIComponent(CComponent* pparent_list)
{
    CManager* pmanager = CManager::GetInstance();
    ItemsMenuEntries.clear();
    AssignedIcons.clear();
    CItemVector all_items;
    CItemVector trophies;
    CItemVector potions;
    CItemVector gifts;
    CItemVector produce;
    CItemVector other;
    GetAllItems(all_items);
    for (std::pair<long, long>* it = all_items.begin(); it != all_items.end(); ) {
        if (CTCInventoryItem::IsBook(it->first))
            it = all_items.erase(it);
        else
            ++it;
    }
    for (unsigned long count = 0; count < all_items.size(); ++count) {
        if (!CTCInventoryItem::IsExpression(all_items[count].first)) {
        CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
        CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
        if (!phair_card_def.IsNull() || !ptattoo_def.IsNull())
            continue;
        switch (CTCInventoryItem::GetUIInventoryCategory(all_items[count].first)) {
        case UI_INVENTORY_CATEGORY_TROPHIES:  trophies.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_POTIONS:  potions.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_GIFTS:  gifts.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_PRODUCE:  produce.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_OTHER:  other.push_back(all_items[count]); continue;
        default: other.push_back(all_items[count]); continue;
        }
        }
    }
    CComponent* pcomponent = pmanager->CreateComponent("UI_SUB_LIST", false);
    CList* plist = GFDebugPDynamicCast(pcomponent,(CList*)0);
    CCountedPointer<CComponent> pbackbutton(pmanager->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> pselectbutton(pmanager->CreateComponent("UI_SELECT_FOR_LIST", false));
    plist->AddNonScrollingChild(pbackbutton);
    plist->AddNonScrollingChild(pselectbutton);
    CCountedPointer<CComponent> ptitle2(pmanager->CreateComponent("UI_ITEM_TITLE", false));
    ptitle2->SetName(TITLE_NAME);
    plist->AddNonScrollingChild(ptitle2);
    plist->InitialiseOffsets(5);
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CWideString menuparam;
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
    ConstructItemCategoryList(menuparam, plist, trophies, pparent_list, "UI_RING_PIC_TROPHIES", "TEXT_GUI_MENUGUIDE_ITEMS_TROPHIES", false);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
    ConstructItemCategoryList(menuparam, plist, potions, pparent_list, "UI_RING_PIC_POTIONS", "TEXT_GUI_MENUGUIDE_ITEMS_MAGICPOTIONS", false);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
    ConstructItemCategoryList(menuparam, plist, gifts, pparent_list, "UI_RING_PIC_GIFTS", "TEXT_GUI_MENUGUIDE_ITEMS_GIFTS", true);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
    ConstructItemCategoryList(menuparam, plist, produce, pparent_list, "UI_RING_PIC_PRODUCE", "TEXT_GUI_MENUGUIDE_ITEMS_PRODUCE", false);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_OTHER");
    ConstructItemCategoryList(menuparam, plist, other, pparent_list, "UI_RING_PIC_OTHERS", "TEXT_GUI_MENUGUIDE_ITEMS_OTHERS", false);
    return pcomponent;
}
