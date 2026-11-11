// CTCInventoryTrade::AlreadyHaveSpecial(long itemDefIndex) -- true when the hero already has what a special-shop item gives: the hair style, a tattoo it would duplicate or conflict with (or a remove-all card when the only tattoo is a permanent one), or the hero title it grants
// Grade: DIFFER(ours v retail) -- DIFFER(725v716): with the ported 1009b overlay opt-ins (CTCHeroMorph/CTCHero TITLEOBJECT/CTattooDef/WEARING): tattoo loop morph/card colouring (c2prio: card priority 16 > morph 9, so ours keeps the card in edi and the morph in esi, retail the reverse) and the count slot [ebp-0Ch] v retail [ebp-10h]; w/0059fc17_v1 (no card local) 701v716 is further off.
// Earlier grade (prior draft): DIFFER(725v716) -- shared headers; tattoo loop only: VC7.1 hoists &card->BankIndex (lea edi,[ecx+34h]) and spills the card to [ebp-10h]; retail keeps the card in edi and the index in esi. Without the named card local (probe 0059fc17_c, 701v716) no hoist, but card/morph swap esi/edi (c2prio: card temp 16 > morph 9; retail needs morph first)
// Leaf: ?AlreadyHaveSpecial@CTCInventoryTrade@@AAE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059FC17, 716 bytes, thiscall `ret 4`, bool in al (ebx = 0). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees (Ego_r.pdb / FableWin.pdb spellings): CTCInventoryItem::GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0 (static
//   __fastcall, CDefPointer by hidden pointer), CThing::GetTC expanded (std::bitset test 0x00410DE0, CVectorMap LowerBound 0x0040F020)
//   for TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS (94), TCI_APPEARANCE_MORPH (3) and TCI_HERO (41),
//   CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier 0x00706000, CTCHeroMorph::GetTattoos 0x0071DD50 /
//   HasAPermanentTattooCoveringArea 0x0071C2D0, vector<CDefPointer<const CTattooDef> >::~vector 0x00581353,
//   CBasicString<char>::operator== 0x004A93C0 (from the inline CCharString::operator==), CTattooDef::IsConflicting 0x0059B679,
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C and
//   CDefClassBase::GetSubDef<CHeroTitleDef> 0x005B1E5A (both emitted in this TU, below, as the landed BuyItem 0x005AF64E does).
// Shape: the hair and tattoo cards are function-scope locals (hair released last, tattoo nulled after its release); the morph and
//   hero lookups leave the pointer unset when the thing lacks the component (retail homes it in the dead parameter slot,
//   `mov esi,[ebp+8]`), the modifiers lookup is the and-chain; the tattoo scan is an index loop over the stored size
//   (`sar [ebp-0Ch],2`), comparing bank indices first, then custom file names when both are -1, else a permanent conflict.
// Headers: overlay opt-ins FABLE_RETAIL_CTATTOODEF_MEMBERS, FABLE_RETAIL_CTCHERO_TITLEOBJECT,
//   FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_WEARING and FABLE_RETAIL_CTCHEROMORPH (engine/CTCHeroMorph.h; hdr/README.md).
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

bool CTCInventoryTrade::AlreadyHaveSpecial(long itemDefIndex)
{
    CDefPointer<const CHairCardDef> hairCardDef = CTCInventoryItem::GetHairCardDef(itemDefIndex);
    if (!hairCardDef.IsNull()) {
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &modifiers) &&
            modifiers->IsWearingAppearanceModifier(hairCardDef->HairObject, APPEARANCE_HAIR))
            return true;
    }
    CDefPointer<const CTattooDef> tattooCardDef = CTCInventoryItem::GetTattooCardDef(itemDefIndex);
    if (!tattooCardDef.IsNull()) {
        CTCHeroMorph* morph;
        ParentThing->GetTC(TCI_APPEARANCE_MORPH, &morph);
        if (morph) {
            std::vector<CDefPointer<const CTattooDef> > tattoos;
            morph->GetTattoos(tattoos);
            if (tattoos.empty()) {
                if (tattooCardDef->RemovesAllTattoos)
                    return true;
                return false;
            }
            const CTattooDef* card = tattooCardDef.Get();
            if (card->RemovesAllTattoos) {
                if (tattoos.size() == 1 && morph->HasAPermanentTattooCoveringArea())
                    return true;
                return false;
            }
            unsigned int count = tattoos.size();
            for (unsigned int i = 0; i < count; ++i) {
                const CTattooDef* tattoo = tattoos[i].Get();
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
    CDefPointer<const CThingBaseDef> itemDef;
    CDefPointer<const CHeroTitleDef> titleDef;
    if (GetDefinitionManager().GetDef(itemDefIndex, &itemDef) && itemDef->GetSubDef(&titleDef)) {
        CTCHero* hero;
        ParentThing->GetTC(TCI_HERO, &hero);
        if (hero && hero->HeroTitleObjectDef == itemDefIndex)
            return true;
    }
    return false;
}
