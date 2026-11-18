// CTCInventoryTrade::ApplyHairCard(const CHairCardDef&) -- if the owning thing has the attachable-appearance-modifiers component (interface 0x5E), applies the card's hair object as an appearance modifier with replacement
// Leaf: ?ApplyHairCard@CTCInventoryTrade@@QAEXABVCHairCardDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D9C2, 78 bytes.
// Callees: std::bitset<274>::test @ 0x00410DE0 (the thing's interface presence bits at +0x20), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   @ 0x0040F020 (the sorted interface map at +0x44), CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(long) @ 0x00706950.
// Callers: CTCInventoryTrade::ProcessTradeAction 0x005AEBFE, BuyItem 0x005AF64E, BuyItemPC 0x005B0AAC.
// Shape: the thing's inline GetTC(type, &out) (CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED) -- bitset test, key in the `push ecx` frame slot,
//   the map pointer a named local (`add esi,44h`), a miss taking the end entry; frame pointer in a size TU (`optimize("y", off)` + "s").
// Name fix (agentNF91, q11198): Ego_r.pdb + FableWin.pdb ?ApplyHairCard@CTCInventoryTrade@@QAEXABVCHairCardDef@@@Z (a const reference);
//   engine/CTCInventoryTrade.h declares it at this address. The old row defined ?ApplyHairCard@CTCInventoryTrade_Retail@@QAEXPBVCHairCardDef@@@Z
//   on a local CTCInventoryTrade_Retail / CHairCardDef / CTCHeroAttachableAppearanceModifiers model; now on the shared headers (the retail
//   CTCInventoryTrade, CHairCardDef over CSubDefClassBase with HairObject +0x28, and the NF91 overlay opt-in
//   FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_ADDWITHREPLACE, Ego_r.pdb + FableWin.pdb).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_ADDWITHREPLACE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CHairCardDef.h"
#include "engine/CVectorMap.h"
#include "engine/CThing.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
void CTCInventoryTrade::ApplyHairCard(const CHairCardDef& hairCard)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &modifiers))
        modifiers->AddAppearanceModifierWithReplace(hairCard.HairObject);
}
