// CTCInventoryTrade::AlreadyHaveSpecial checks hairstyle, tattoo and title shop items against the active hero.
// Grade: DIFFER(725v716). The tattoo card pointer, size home and loop coloring still differ.
// Leaf: ?AlreadyHaveSpecial@CTCInventoryTrade@@AAE_NJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059FC17, 716 bytes, thiscall `ret 4`, bool in al (ebx = 0). Frame-pointer size TU: optimize("y",off) + ("s",on).

#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_TITLEOBJECT
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_WEARING
#define FABLE_RETAIL_CTCHEROMORPH
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_MEMBERS
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCHero.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTCInventoryItem.h"

// The CTCInventoryTrade TU's copies of the two def templates (landed BuyItem 0x005AF64E spelling): the member template GetDef and
// GetSubDef<CHeroTitleDef>, out of line (`call 0042AF3Ch` / `call 005B1E5Ah`).
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
template <> bool CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CHeroTitleDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

bool CTCInventoryTrade::AlreadyHaveSpecial(long def_index)
{
    CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(def_index);
    if (!phair_card_def.IsNull()) {
        CTCHeroAttachableAppearanceModifiers* ptc_modifiers;
        if (ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &ptc_modifiers) &&
            ptc_modifiers->IsWearingAppearanceModifier(phair_card_def->HairObject, APPEARANCE_HAIR))
            return true;
    }
    CDefPointer<const CTattooDef> ptattoo_card_def = CTCInventoryItem::GetTattooCardDef(def_index);
    if (!ptattoo_card_def.IsNull()) {
        CTCHeroMorph* ptc_morph;
        ParentThing->GetTC(TCI_APPEARANCE_MORPH, &ptc_morph);
        if (ptc_morph) {
            std::vector<CDefPointer<const CTattooDef> > tattoos;
            ptc_morph->GetTattoos(tattoos);
            if (tattoos.empty()) {
                if (ptattoo_card_def->RemovesAllTattoos)
                    return true;
                return false;
            }
            const CTattooDef* card = ptattoo_card_def.Get();
            if (card->RemovesAllTattoos) {
                if (tattoos.size() == 1 && ptc_morph->HasAPermanentTattooCoveringArea())
                    return true;
                return false;
            }
            unsigned long tattoo_count = tattoos.size();
            for (unsigned long count = 0; count < tattoo_count; ++count) {
                const CTattooDef* tattoo = tattoos[count].Get();
                if (tattoo->BankIndex == card->BankIndex) {
                    if (tattoo->BankIndex != -1 || card->BankIndex != -1)
                        return true;
                    if (tattoo->CustomFileName == card->CustomFileName)
                        return true;
                } else if (tattoo->Permanent && tattoo->IsConflicting(*card))
                    return true;
            }
        }
    }
    CDefPointer<const CThingBaseDef> pbase_def;
    CDefPointer<const CHeroTitleDef> ptitle_def;
    if (GetDefinitionManager().GetDef(def_index, &pbase_def) && pbase_def->GetSubDef(&ptitle_def)) {
        CTCHero* ptc_hero;
        ParentThing->GetTC(TCI_HERO, &ptc_hero);
        if (ptc_hero && ptc_hero->HeroTitleObjectDef == def_index)
            return true;
    }
    return false;
}
