// CTCInventory::GetItemDescription(CWideString& text, long itemDefIndex, bool hidePrice) const -- append an inventory item's description to `text`: unless hidden, its price (the stock def's default price, or the buy price of the shop the hero stands in when it stocks the item); for a hair or tattoo card the attractiveness, scariness and alignment it gives (a hair card also against the hero's worn hair of the same body area, as "[+]<value> ([+]<change>)"); then the item's details text
// Leaf: ?GetItemDescription@CTCInventory@@UBEXAAVCWideString@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005841F9, 2033 bytes, thiscall `leave ; ret 0Ch`, no EH frame; frame-pointer size TU
//   (optimize("y",off) + ("s",on), as the landed CTCInventory rows of this TU, e.g. OnItemSelected 0x00583E26).
// Identity: CTCInventory vtable 0x01250DE4 slot 78 (the only reference to 0x005841F9), the slot of
//   CTCInventoryBase::GetItemDescription 0x005BC6B3 (Experience table 0x0124F11C) and of the landed Clothing 0x005B5CF8 / Weapons
//   0x005C7913 overrides; Ego_r.pdb + FableWin.pdb ?GetItemDescription@CTCInventory@@UBEXAAVCWideString@@J_N@Z (`ret 0Ch`: three
//   arguments); the landed ConstructItemDescription 0x005819DD calls it through +0x138 with false. The catalog's
//   CTCShop::BuildItemDescription label is propagated and wrong. Starts right after 0x005841F6 `ret 0Ch` (no padding; packed TU)
//   and ends at 0x005849E7 `ret 0Ch`, the next function 0x005849EA follows at once: extent 2033 bytes confirmed.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0,
//   CDefClassBase::GetSubDef<CStockItemDef> 0x0059350C / <CAppearanceModifierDef> 0x0057FF92 (shared same-TU body), the expanded
//   CThing::PeekTC<CTCVolumeContainmentTracker> / <CTCHeroAttachableAppearanceModifiers> (bitset test 0x00410DE0, const LowerBound
//   0x004365B0) and GetTC<CTCShop> (LowerBound 0x0040F020), CIntelligentPointer GetPItem 0x00A01B50, IS_THING_ALIVE<CThing*>
//   0x0040DCE0, CTCShop::IsValidStockItem 0x007723D0 / GetCurrentBuyPrice 0x007742C0, NGameText::CDataBank::GetTextBySymbol
//   0x009C95E0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CWideString(const unsigned short*) 0x0099B6B0 / ~
//   0x0099B510 / += (const CWideString&) 0x0099B8D0 / += (const unsigned short*) 0x0099B940, GFIntToWideString 0x00405F00,
//   CTCInventoryItem::GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0 / GetDetails 0x005D9630, CAppearanceModifierDef::
//   GetTypeBalancedAttractiveness / Scariness / Goodstrength 0x0057FB86 / 0x0057FBBB / 0x0057FBF0, CTattooDef:: the same
//   0x0057FC46 / 0x0057FC71 / 0x0057FC9C, list<long> _List_base(const allocator&) 0x0042AB58 / ~_List_base 0x0042AB73,
//   CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList 0x00706800, __ftol2 0x00BFEA70.
// Data: "TEXT_GUI_PRICE" 0x012501E4, L": " 0x01236A50, L"\n" 0x01231728, "TEXT_GUI_MENU_ATTRACTIVENESS" 0x012501C4,
//   "TEXT_GUI_MENU_SCARINESS" 0x012501A0, "TEXT_GUI_MENU_ALIGNMENT_MODIFIER" 0x0124E824, L" " 0x0122D820, L"+" 0x01231760,
//   L" (" 0x012501BC, L")" 0x012501B8, 100.0f 0x0124FEC4.
// Shape (measured with c2prio / the frame-packer trace, see NOTES.md): the price block is the landed
//   CTCInventoryWeapons::ConstructWeaponDescription 0x005C6B26 / CalculateArmorValues 0x005B4EBA one (stock def declared before
//   the price, the price lines as `text += L": "` through the `const unsigned short*` operator); the stat lines are the landed
//   Clothing GetItemDescription 0x005B5CF8 shape with CWideString(L"..") temporaries. Three c2 decisions fix the spelling:
//   (1) the tattoo def is read once into edi (0x0058456C `mov edi,[ebp-0Ch] ; test edi,edi`): a `const CTattooDef&` taken
//   after the IsNull test (the CDefPointer itself is address-taken by GetTattooCardDef, so `tattooCard->` reloads it after each
//   call); (2) `text` keeps ebx only while its colouring priority stays above the first stat value's (29 v 28): the hero-stats
//   def is read straight from the manager (`GetDefinitionManager().PHeroStatsDef`) -- the PeekHeroStatsDef() reference accessor
//   adds an inline return temporary to the outer block and ties them at 28 (value wins the tie on its key); (3) each stat has
//   its own named value (attractiveness / scariness / alignment): one re-assigned `value` is a single weight-9 packer node that
//   takes the dead `text` home [ebp+8] first, where retail keeps [ebp+8] for the label / literal temporaries and the values at
//   [ebp-34h]. The show-change flag lives in the dead `hidePrice` word ([ebp+13h]).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_ITEMDESCRIPTION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CHEROSTATSDEF_APPEARANCESCALING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_TYPEBALANCED
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_COVERS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_UPLOADCHANGES
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_CTCSHOP_INTERFACETYPE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTattooDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCShop.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);        // 0x0040DCE0 (CThing*)
// Same-TU body of IS_THING_ALIVE<CThing*> (out of line in retail: 0x0040DCE0), as the landed OnItemSelected 0x00583E26 of this TU.
#pragma auto_inline(off)
template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
#pragma auto_inline(on)

void CTCInventory::GetItemDescription(CWideString& text, long itemDefIndex, bool hidePrice) const
{
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    // the price: the default one, or the buy price of the shop the hero is in when it stocks the item
    if (!hidePrice) {
        CDefPointer<const CStockItemDef> stockDef;
        long price = 0;
        if (def->GetSubDef(&stockDef))
            price = stockDef->DefaultPrice;
        const CTCVolumeContainmentTracker* tracker;
        if (ParentThing->PeekTC(&tracker)) {
            CThing* shopThing = tracker->GetPThingInside();
            CTCShop* shop;
            if (IS_THING_ALIVE(shopThing) && shopThing->GetTC(&shop) && shop->IsValidStockItem(itemDefIndex))
                price = shop->GetCurrentBuyPrice(itemDefIndex, 1);
        }
        text += bank.GetTextBySymbol("TEXT_GUI_PRICE");
        text += L": ";
        text += GFIntToWideString(price);
        text += L"\n";
    }
    // a hair or tattoo card: the appearance it gives
    CDefPointer<const CHairCardDef> hairCard = CTCInventoryItem::GetHairCardDef(itemDefIndex);
    CDefPointer<const CTattooDef> tattooCard = CTCInventoryItem::GetTattooCardDef(itemDefIndex);
    if (!hairCard.IsNull() || !tattooCard.IsNull()) {
        float itemAttractiveness = 0.0f;
        bool showChange = false;
        float curAttractiveness = 0.0f;
        float itemScariness = 0.0f;
        float curScariness = 0.0f;
        float itemGoodstrength = 0.0f;
        float curGoodstrength = 0.0f;
        const CHeroStatsDef* stats = GetDefinitionManager().PHeroStatsDef;
        if (!hairCard.IsNull()) {
            CDefPointer<const CDefClassBase> hairDef = GetDefinitionManager().GetPDefFromGlobalIndex(hairCard->HairObject);
            CDefPointer<const CAppearanceModifierDef> modifier;
            if (!hairDef.IsNull()) {
                hairDef->GetSubDef(&modifier);
                if (!modifier.IsNull()) {
                    itemAttractiveness = modifier->GetTypeBalancedAttractiveness(stats->AppearanceModifierScaling);
                    itemScariness = modifier->GetTypeBalancedScariness(stats->AppearanceModifierScaling);
                    itemGoodstrength = modifier->GetTypeBalancedGoodstrength(stats->AppearanceModifierScaling);
                    // against what the hero wears on the same body area
                    const CTCHeroAttachableAppearanceModifiers* appearance;
                    if (ParentThing->PeekTC(&appearance)) {
                        std::list<long> worn;
                        appearance->GetAppearanceModifierList(worn, (EHeroAttachableAppearanceModifierType)modifier->AppearanceType);
                        for (std::list<long>::const_iterator it = worn.begin(), end = worn.end(); it != end; ++it) {
                            CDefPointer<const CDefClassBase> wornDef = GetDefinitionManager().GetPDefFromGlobalIndex(*it);
                            CDefPointer<const CAppearanceModifierDef> wornModifier;
                            if (!wornDef.IsNull() && wornDef->GetSubDef(&wornModifier) &&
                                wornModifier->CoversBodyAreaFlags == modifier->CoversBodyAreaFlags) {
                                curAttractiveness += wornModifier->GetTypeBalancedAttractiveness(stats->AppearanceModifierScaling);
                                curScariness += wornModifier->GetTypeBalancedScariness(stats->AppearanceModifierScaling);
                                curGoodstrength += wornModifier->GetTypeBalancedGoodstrength(stats->AppearanceModifierScaling);
                            }
                        }
                    }
                    showChange = true;
                }
            }
        } else if (!tattooCard.IsNull()) {
            const CTattooDef& tattoo = *tattooCard;
            itemAttractiveness = tattoo.GetTypeBalancedAttractiveness(stats->AppearanceModifierScaling);
            itemScariness = tattoo.GetTypeBalancedScariness(stats->AppearanceModifierScaling);
            itemGoodstrength = tattoo.GetTypeBalancedGoodstrength(stats->AppearanceModifierScaling);
            showChange = false;
        }
        long attractiveness = (long)(itemAttractiveness * 100.0f);
        long attractivenessChange = attractiveness - (long)(curAttractiveness * 100.0f);
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_ATTRACTIVENESS");
        text += CWideString(L" ");
        if (attractiveness > 0)
            text += CWideString(L"+");
        text += GFIntToWideString(attractiveness);
        if (showChange) {
            text += CWideString(L" (");
            if (attractivenessChange > 0)
                text += CWideString(L"+");
            text += GFIntToWideString(attractivenessChange);
            text += CWideString(L")");
        }
        text += CWideString(L"\n");
        long scariness = (long)(itemScariness * 100.0f);
        long scarinessChange = scariness - (long)(curScariness * 100.0f);
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_SCARINESS");
        text += CWideString(L" ");
        if (scariness > 0)
            text += CWideString(L"+");
        text += GFIntToWideString(scariness);
        if (showChange) {
            text += CWideString(L" (");
            if (scarinessChange > 0)
                text += CWideString(L"+");
            text += GFIntToWideString(scarinessChange);
            text += CWideString(L")");
        }
        text += CWideString(L"\n");
        long alignment = (long)(itemGoodstrength * 100.0f);
        long alignmentChange = alignment - (long)(curGoodstrength * 100.0f);
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT_MODIFIER");
        text += CWideString(L" ");
        if (alignment > 0)
            text += CWideString(L"+");
        text += GFIntToWideString(alignment);
        if (showChange) {
            text += CWideString(L" (");
            if (alignmentChange > 0)
                text += CWideString(L"+");
            text += GFIntToWideString(alignmentChange);
            text += CWideString(L")");
        }
        text += CWideString(L"\n");
    }
    text += CTCInventoryItem::GetDetails(itemDefIndex);
}
