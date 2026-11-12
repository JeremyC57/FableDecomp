// CTCInventoryTrade::ConstructTradeDescriptionClothing(long itemDefIndex, unsigned long count, bool buying, bool special) -- builds the
//   description container of a clothing / non-weapon trade item: the price line (shop price against the default price, tinted when they
//   differ), then either the item's own description (special shop items) or the profit / loss line when selling, the armour rating, the
//   number already owned, the stock and delivery lines, the unmet heavy-weapon stat thresholds and the no-armour-rating description.
// Leaf: ?ConstructTradeDescriptionClothing@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005AAF28, 4550 bytes, thiscall `ret 10h`, frame pointer + `leave`, no EH frame;
//   optimize("y",off) + optimize("s",on) like every landed CTCInventoryTrade row (0x005A9A87, 0x005A8C66, 0x005AF64E).
// Identity: Ego_r.pdb + FableWin.pdb ?ConstructTradeDescriptionClothing@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z; sole
//   caller CTCInventoryTrade::ConstructTradeDescription 0x005AD571 (forwards its four arguments; `count` is never read).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>::Reset 0x00429C6F /
//   ReleaseData 0x004291DE, CCharString(const char*,long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString() 0x0099AED0 /
//   ~CWideString 0x0099B510 / operator=(const CWideString&) 0x0099B7D0 / operator=(const wchar_t*) 0x0099B800 / operator+= 0x0099B8D0 /
//   operator!= 0x0099B0F0 / ToCharString 0x0099B2C0, operator+(CWideString,CWideString) 0x0099BE70, operator+(const wchar_t*,CWideString)
//   0x0099BFF0, GFIntToWideString 0x00405F00, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 / GetPDefFromGlobalIndex 0x009AD9E0 / GetDef<CUIMiscThingsDef> 0x0042B312 /
//   GetDef<CThingBaseDef> 0x0042AF3C, CDefClassBase::GetSubDef<CStockItemDef> 0x0059350C / GetSubDef<CWeaponDef> 0x0042B1A6,
//   CTCBase::GetDefinitionManager 0x006869D0, NInventory::CTCInventoryBase::GetBuildingThing 0x005BC4C4 / GetBoughtPrice 0x005BEAD5 /
//   GetNumberOfItemsOfTypeInInventory 0x005BDF08 / GetItemDescription (slot 78), bitset<274>::test 0x00410DE0, CVectorMap::LowerBound
//   0x0040F020 / const 0x004365B0, CTCShop::GetCurrentSellPrice 0x007743B0 / GetCurrentBuyPrice 0x007742C0 / IsBarberShop 0x00772490 /
//   IsTattooShop 0x00772510 / IsTitleShop 0x00772590 / GetQuantity 0x007726B0 / GetMaxStock 0x00772720 / GetDaysTillRestock 0x00772790,
//   CPlayerManager::GetMainPlayer 0x00449960, CPlayer::PeekPControlledCreature 0x00487DD0, CTCInventoryItem::GetInventoryType 0x005D99D0 /
//   IsWeapon 0x005D97F0, CTCInventoryClothing::GetItemDescriptionArmourRating 0x005B6767 / GetItemDescriptionNoArmourRating 0x005B62FE,
//   CTCInventoryExperience::GetExperienceStatName 0x0056F26F, CTCHeroExperience::GetTrainableStatLevel 0x006D6520,
//   _Rb_global<bool>::_M_increment 0x004292C0, map<unsigned long,CUIState>::operator[] 0x0042D5B1, __ftol2 0x00BFEA70;
//   virtuals CComponent::AddChild (+0xEC), CText::SetValue (+0x240), CComponent::GetStates (+0x148), CTCBase::GetInterfaceType (+0x54).
// Shape: every text component is created in an if/else over InventoryTradeOpen with one CreateComponent per arm (cross-jumped
//   `je ; push PC ; jmp ; push UI ; call`); the CText pointer of each line is ONE local `line`, re-assigned after every Reset (VC7.1 gives
//   each live range its own home: ebx, [ebp-24h], edi -- separate named pointers per line colour the frame slots differently); the def
//   lookups are same-TU template bodies kept out of line (as 0x005C90FC) so the CDefPointer locals stay unaliased; the hero's combat
//   def is assigned from GetPHeroCombatDef() into a declared pointer (`inc ; inc ; dec`, 0x005ABDA7).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADEDESCRIPTION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_STOCKINFO
#define FABLE_RETAIL_CTCSHOP_INTERFACETYPE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF
#define FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_ARMOUR
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_NOARMOUR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWeaponDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CTCHeroExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/CCountedPointer.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

// Same-TU bodies of the def lookups (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312, GetDef<CThingBaseDef> 0x0042AF3C,
// GetSubDef<CStockItemDef> 0x0059350C, GetSubDef<CWeaponDef> 0x0042B1A6), as 0x005C90FC.
#pragma auto_inline(off)
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)
NUISystem::CComponent* CTCInventoryTrade::ConstructTradeDescriptionClothing(long itemDefIndex, unsigned long count, bool buying, bool special)
{
    NUISystem::CManager* mgr = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* container;
    CCountedPointer<NUISystem::CComponent> textComponent;
    if (InventoryTradeOpen) {
        container = mgr->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false);
        textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    } else {
        container = mgr->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE", false);
        textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    }
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    NUISystem::CText* line = static_cast<NUISystem::CText*>(textComponent.Get());
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(&shop);
    long price;
    if (buying && !special)
        price = shop->GetCurrentSellPrice(itemDefIndex, 1);
    else
        price = shop->GetCurrentBuyPrice(itemDefIndex, 1);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    CDefPointer<const CStockItemDef> stockDef;
    long defaultPrice;
    if (!def.IsNull() && def->GetSubDef(&stockDef))
        defaultPrice = stockDef->DefaultPrice;
    else
        defaultPrice = 1;
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    if (special) {
        text = bank.GetTextBySymbol("TEXT_GUI_PRICE") + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
        if (price < defaultPrice) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.G *= 0.5f;
        } else if (price > defaultPrice) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.R *= 0.5f;
        }
        if (InventoryTradeOpen)
            textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        const CThingPlayerCreature* player = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const NInventory::CTCInventoryBase* inventory = 0;
        player->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
        text = L"";
        inventory->GetItemDescription(text, itemDefIndex, true);
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
    } else {
        text = bank.GetTextBySymbol(miscDef->TradeCostString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
        if ((price > defaultPrice && buying) || (price < defaultPrice && !buying)) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.G *= 0.5f;
        } else if ((price < defaultPrice && buying) || (price > defaultPrice && !buying)) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.R *= 0.5f;
        }
        const CThingPlayerCreature* player = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const NInventory::CTCInventoryBase* inventory = 0;
        ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(itemDefIndex);
        player->PeekTC(inventoryType, &inventory);
        bool specialShop = shop->IsBarberShop() || shop->IsTattooShop() || shop->IsTitleShop();
        if (!buying) {
            if (InventoryTradeOpen)
                textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<NUISystem::CText*>(textComponent.Get());
            long profit = price - inventory->GetBoughtPrice(itemDefIndex);
            if (profit >= 0) {
                text = bank.GetTextBySymbol(miscDef->TradeProfitString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(profit);
                if (profit != 0) {
                    textComponent->GetStates()[0].Colour.B *= 0.5f;
                    textComponent->GetStates()[0].Colour.R *= 0.5f;
                }
            } else {
                text = bank.GetTextBySymbol(miscDef->TradeLossString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(-profit);
                textComponent->GetStates()[0].Colour.B *= 0.5f;
                textComponent->GetStates()[0].Colour.G *= 0.5f;
            }
            container->AddChild(textComponent);
            line->SetValue(text, true, true);
        }
        if (inventory->GetInterfaceType() == TCI_INVENTORY_CLOTHING) {
            if (InventoryTradeOpen)
                textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<NUISystem::CText*>(textComponent.Get());
            text = CWideString::NullString;
            static_cast<const CTCInventoryClothing*>(inventory)->GetItemDescriptionArmourRating(text, itemDefIndex);
            if (text != CWideString::NullString) {
                container->AddChild(textComponent);
                line->SetValue(text, true, true);
            }
        }
        if (buying && !specialShop) {
            if (InventoryTradeOpen)
                textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<NUISystem::CText*>(textComponent.Get());
            long owned = inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
            text = bank.GetTextBySymbol(miscDef->TradeAlreadyOwnsString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(owned);
            container->AddChild(textComponent);
            line->SetValue(text, true, true);
        }
        if (!specialShop) {
            if (InventoryTradeOpen)
                textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<NUISystem::CText*>(textComponent.Get());
            long quantity = shop->GetQuantity(itemDefIndex);
            long maxStock = shop->GetMaxStock(itemDefIndex);
            text = bank.GetTextBySymbol(miscDef->TradeNumberInStockString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(quantity);
            text += L"/" + GFIntToWideString(maxStock);
            container->AddChild(textComponent);
            line->SetValue(text, true, true);
            if (InventoryTradeOpen)
                textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<NUISystem::CText*>(textComponent.Get());
            long days = shop->GetDaysTillRestock(itemDefIndex);
            text = bank.GetTextBySymbol(miscDef->TradeDeliveryString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator;
            if (days != 0)
                text += GFIntToWideString(days) + miscDef->SpaceSeparator + bank.GetTextBySymbol(miscDef->TradeDaysString.ToCharString());
            else
                text += bank.GetTextBySymbol(miscDef->TradeNoDeliveryString.ToCharString());
            container->AddChild(textComponent);
            line->SetValue(text, true, true);
        }
        if (buying && CTCInventoryItem::IsWeapon(itemDefIndex)) {
            CDefPointer<const CThingBaseDef> thingDef;
            GetDefinitionManager().GetDef(itemDefIndex, &thingDef);
            CDefPointer<const CWeaponDef> weaponDef;
            thingDef->GetSubDef(&weaponDef);
            if (weaponDef->Class == 2) {
                CTCHeroExperience* experience;
                CTCInventoryExperience* experienceInventory;
                if (ParentThing->GetTC(&experience) && ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &experienceInventory)) {
                    CDefPointer<const CHeroCombatDef> combatDef;
                    combatDef = experience->GetPHeroCombatDef();
                    if (!combatDef.IsNull()) {
                        for (std::map<EHeroTrainableStatType, long>::const_iterator it = combatDef->HeavyWeapons.StatThresholds.begin();
                             it != combatDef->HeavyWeapons.StatThresholds.end(); ++it) {
                            if (experience->GetTrainableStatLevel(it->first) < it->second) {
                                if (InventoryTradeOpen)
                                    textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                                else
                                    textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                                line = static_cast<NUISystem::CText*>(textComponent.Get());
                                text = experienceInventory->GetExperienceStatName(it->first) + miscDef->SpaceSeparator
                                     + bank.GetTextBySymbol("TEXT_GUI_MENU_EXP_LEVEL") + miscDef->SpaceSeparator
                                     + GFIntToWideString(it->second) + bank.GetTextBySymbol("TEXT_GUI_MENU_REQUIRED");
                                textComponent->GetStates()[0].Colour.B *= 0.5f;
                                textComponent->GetStates()[0].Colour.G *= 0.5f;
                                container->AddChild(textComponent);
                                line->SetValue(text, true, true);
                            }
                        }
                    }
                }
            }
        }
        if (inventory->GetInterfaceType() == TCI_INVENTORY_CLOTHING) {
            if (InventoryTradeOpen)
                textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<NUISystem::CText*>(textComponent.Get());
            text = CWideString::NullString;
            static_cast<const CTCInventoryClothing*>(inventory)->GetItemDescriptionNoArmourRating(text, itemDefIndex);
            container->AddChild(textComponent);
            line->SetValue(text, true, true);
        }
    }
    return container;
}
