// CTCInventory::GetStyleCardsListUIComponent classifies style cards and populates haircut, facial hair and tattoo lists.
// Grade: DIFFER(1564v1582). Donor card scopes improve the early CPU operand allocation and control flow. Card-release scheduling and the title setter CPU operand remain different.
// Leaf: ?GetStyleCardsListUIComponent@CTCInventory@@QAEXJJJPAVCComponent@NUISystem@@@Z
// Flags: none
// Retail: Fable.exe 0x00589402, 1582 bytes, thiscall.

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
#include "engine/CTCInventoryItem.h"
#include "engine/CHairCardDef.h"
#include "engine/CTattooDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CGameDefinitionManager.h"

#pragma optimize("y", off)
#pragma optimize("s", on)
using namespace NUISystem;
CWideString FABLE_FASTCALL GFIntToWideString(long);
// Inline-budget: original same-TU specialization at 0x0057FF92 is a called COMDAT.
template <>
#ifdef _MSC_VER
__declspec(noinline)
#endif
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
    CComponent* pviewport = pparent_list->FindChild("PC_UI_FRAME");
    CComponent* pmenu_entry;
    CComponent* pmenu_entry_title;
    CComponent* pmenu_entry_subtitle;
    NUISystem::CText* pmenu_entry_title_real;
    NUISystem::CText* pmenu_entry_subtitle_real;
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
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull()) {
                CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                if (!db.IsNull()) {
                    CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                    db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        long flags = pappearance_modifier_def->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (flags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                }
            } else {
                if (ptattoo_def.IsNull())
                    continue;
                if (!ptattoo_def->IsInvalidCustomTattoo())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        default: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull()) {
                CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                if (!db.IsNull()) {
                    db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        long flags = pappearance_modifier_def->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (flags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                }
            } else {
                if (ptattoo_def.IsNull())
                    continue;
                if (!ptattoo_def->IsInvalidCustomTattoo())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        }
    }
    CWideString title_value;
    pmenu_entry = pparent_list->FindChild("PC_MENU_ENTRY_HAIRCUTS");
    ConstructStyleEntryPC(pmenu_entry, haircut, pviewport, "PC_HAIRCUTS_TITLE", pparent_list);
    pmenu_entry_title = pmenu_entry->FindChild("PC_HAIRCUTS_BUTTON")->FindChild("PC_TEXT_HAIRCUTS");
    pmenu_entry_title_real = static_cast<NUISystem::CText*>(pmenu_entry_title);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_haircut_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("PC_HAIRCUTS_SUBTITLE");
    pmenu_entry_subtitle_real = static_cast<NUISystem::CText*>(pmenu_entry_subtitle);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("PC_MENU_ENTRY_FACIAL_HAIR");
    ConstructStyleEntryPC(pmenu_entry, facial_hair, pviewport, "PC_FACIAL_HAIR_TITLE", pparent_list);
    pmenu_entry_title = pmenu_entry->FindChild("PC_FACIAL_HAIR_BUTTON")->FindChild("PC_TEXT_FACIAL_HAIR");
    pmenu_entry_title_real = static_cast<NUISystem::CText*>(pmenu_entry_title);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_facial_hair_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("PC_FACIAL_HAIR_SUBTITLE");
    pmenu_entry_subtitle_real = static_cast<NUISystem::CText*>(pmenu_entry_subtitle);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("PC_MENU_ENTRY_TATTOOS");
    ConstructStyleEntryPC(pmenu_entry, tattoo, pviewport, "PC_TATTOOS_TITLE", pparent_list);
    pmenu_entry_title = pmenu_entry->FindChild("PC_TATTOOS_BUTTON")->FindChild("PC_TEXT_TATTOOS");
    pmenu_entry_title_real = static_cast<NUISystem::CText*>(pmenu_entry_title);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_tattoo_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("PC_TATTOOS_SUBTITLE");
    pmenu_entry_subtitle_real = static_cast<NUISystem::CText*>(pmenu_entry_subtitle);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

}
