// 005850fd CTCInventory::GetStyleCardsListUIComponent
// Leaf: ?GetStyleCardsListUIComponent@CTCInventory@@QAEXJJJPAVCComponent@NUISystem@@@Z
// Flags: none
// Retail: Fable.exe 0x005850fd, 1678 bytes, thiscall.

#define FABLE_RETAIL_UI_TEXT_GETVALUE_MUTABLE_PASS29
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOMPONENT
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORY_STYLEENTRY
#define FABLE_RETAIL_CTCINVENTORY_STYLECARDS_LIST
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryStats.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/GFDebugPDynamicCast.h"
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#include "engine/EUIInventoryCategory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CHairCardDef.h"
#include "engine/CTattooDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CGameDefinitionManager.h"

#pragma optimize("y", off)
#pragma optimize("s", on)
using namespace NUISystem;
#define FABLE_RETAIL_GFINTTOWIDESTRING_DECL_ONLY_PASS29
#include "engine/GFIntToWideString.h"
// The console UI calls the same-TU specialization at 0x0057FF92.
template <>
bool CDefClassBase::GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CAppearanceModifierDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CAppearanceModifierDef> def(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = def;
        return true;
    }
    return false;
}

void CTCInventory::GetStyleCardsListUIComponent(long tot_num_haircut_cards, long tot_num_facial_hair_cards, long tot_num_tattoo_cards, CComponent* pparent_list)
{
    CManager* pmanager = CManager::GetInstance();
    std::vector<std::pair<long, long> > all_items;
    std::vector<std::pair<long, long> > haircut;
    std::vector<std::pair<long, long> > facial_hair;
    std::vector<std::pair<long, long> > tattoo;
    GetAllItems(all_items);
    for (std::vector<std::pair<long, long> >::iterator it = all_items.begin(); it != all_items.end(); ) {
        if (CTCInventoryItem::IsBook(it->first))
            it = all_items.erase(it);
        else
            ++it;
    }
    for (unsigned long count = 0; count < all_items.size(); ++count) {
        if (CTCInventoryItem::IsExpression(all_items[count].first))
            continue;
        switch (CTCInventoryItem::GetUIInventoryCategory(all_items[count].first)) {
        case UI_INVENTORY_CATEGORY_TROPHIES:
        case UI_INVENTORY_CATEGORY_POTIONS:
        case UI_INVENTORY_CATEGORY_GIFTS:
        case UI_INVENTORY_CATEGORY_PRODUCE:
            break;
        case UI_INVENTORY_CATEGORY_OTHER: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull() || !ptattoo_def.IsNull()) {
                if (!phair_card_def.IsNull()) {
                    CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                    CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                    if (!db.IsNull())
                        db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        if ((pappearance_modifier_def->CoversBodyAreaFlags & 0x80) || (pappearance_modifier_def->CoversBodyAreaFlags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (pappearance_modifier_def->CoversBodyAreaFlags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                } else if (!ptattoo_def.IsNull())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        default: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull() || !ptattoo_def.IsNull()) {
                if (!phair_card_def.IsNull()) {
                    CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                    CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                    if (!db.IsNull())
                        db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        if ((pappearance_modifier_def->CoversBodyAreaFlags & 0x80) || (pappearance_modifier_def->CoversBodyAreaFlags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (pappearance_modifier_def->CoversBodyAreaFlags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                } else if (!ptattoo_def.IsNull())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        }
    }
    CComponent* pmenu_entry = 0;
    CComponent* pmenu_entry_title = 0;
    CText* pmenu_entry_title_real = 0;
    CWideString title_value;
    CComponent* pmenu_entry_subtitle = 0;
    CText* pmenu_entry_subtitle_real = 0;
    CComponent* pviewport = pparent_list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    CViewport* pviewport_real = GFDebugPDynamicCast(pviewport,(CViewport*)0);
    pmenu_entry = pparent_list->FindChild("UI_MENU_ENTRY_HAIRCUTS");
    ConstructStyleEntry(pmenu_entry, haircut, "UI_HAIRCUTS_TITLE", pparent_list);
    pviewport_real->AddFakeChild(pmenu_entry->FindChild("UI_HAIRCUTS_DESC"));
    pmenu_entry_title = pmenu_entry->FindChild("UI_HAIRCUTS_BUTTON");
    pmenu_entry_title = pmenu_entry_title->FindChild("UI_TEXT_HAIRCUTS");
    pmenu_entry_title_real = GFDebugPDynamicCast(pmenu_entry_title,(CText*)0);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_haircut_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("UI_HAIRCUTS_SUB_TITLE");
    pmenu_entry_subtitle_real = GFDebugPDynamicCast(pmenu_entry_subtitle,(CText*)0);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("UI_MENU_ENTRY_FACIAL_HAIR");
    ConstructStyleEntry(pmenu_entry, facial_hair, "UI_FACIAL_HAIR_TITLE", pparent_list);
    pviewport_real->AddFakeChild(pmenu_entry->FindChild("UI_FACIAL_HAIR_DESC"));
    pmenu_entry_title = pmenu_entry->FindChild("UI_FACIAL_HAIR_BUTTON");
    pmenu_entry_title = pmenu_entry_title->FindChild("UI_TEXT_FACIAL_HAIR");
    pmenu_entry_title_real = GFDebugPDynamicCast(pmenu_entry_title,(CText*)0);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_facial_hair_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("UI_FACIAL_HAIR_SUB_TITLE");
    pmenu_entry_subtitle_real = GFDebugPDynamicCast(pmenu_entry_subtitle,(CText*)0);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("UI_MENU_ENTRY_TATTOOS");
    ConstructStyleEntry(pmenu_entry, tattoo, "UI_TATTOOS_TITLE", pparent_list);
    pviewport_real->AddFakeChild(pmenu_entry->FindChild("UI_TATTOOS_DESC"));
    pmenu_entry_title = pmenu_entry->FindChild("UI_TATTOOS_BUTTON");
    pmenu_entry_title = pmenu_entry_title->FindChild("UI_TEXT_TATTOOS");
    pmenu_entry_title_real = GFDebugPDynamicCast(pmenu_entry_title,(CText*)0);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_tattoo_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("UI_TATTOOS_SUB_TITLE");
    pmenu_entry_subtitle_real = GFDebugPDynamicCast(pmenu_entry_subtitle,(CText*)0);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

}