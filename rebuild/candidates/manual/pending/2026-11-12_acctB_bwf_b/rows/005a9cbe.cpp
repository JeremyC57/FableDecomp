// CTCInventoryTrade::ConstructTradeDescriptionWeapon(long itemDefIndex, unsigned long, bool selling, bool brief) -- builds the trade-screen description container of a weapon-shop item: the price line (TradeCost/"TEXT_GUI_PRICE": price and default price in brackets, faded red/green against the default), then either (brief) the item's own inventory description, or the profit/loss line on a sale, the weapon damage / weight lines, the "already owns" count, the stock and delivery lines (not for barber / tattoo / title shops), a red "required" line per heavy-weapon stat threshold the hero has not reached, and the weapon's no-damage-nor-weight description
// Leaf: ?ConstructTradeDescriptionWeapon@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A9CBE, 4714 bytes, thiscall `ret 10h`, no EH frame; optimize("y",off) + ("s",on) (the CTCInventoryTrade TU).
// Identity: Ego_r.pdb (0x0060D3CD) + FableWin.pdb (0x023283BC) ?ConstructTradeDescriptionWeapon@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z;
//   sole caller CTCInventoryTrade::ConstructTradeDescription 0x005AD571 (after CTCInventoryItem::IsWeapon). Parameter names descriptive (not in the PDB).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCharString(const char*,long) 0x0099EBF0 / ~ 0x0099EAE0,
//   CCountedPointer<CComponent>::Reset 0x00429C6F / ReleaseData 0x004291DE, CWideString() 0x0099AED0 / ~ 0x0099B510 / = 0x0099B7D0 / =(L"") 0x0099B800
//   / += 0x0099B8D0 / != 0x0099B0F0 / ToCharString 0x0099B2C0, operator+ 0x0099BE70 / (L"/", w) 0x0099BFF0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 / GetPDefFromGlobalIndex 0x009AD9E0, GetDef<CUIMiscThingsDef> 0x0042B312,
//   GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CStockItemDef> 0x0059350C, GetSubDef<CWeaponDef> 0x0042B1A6, CTCInventoryBase::GetBuildingThing 0x005BC4C4,
//   bitset<274>::test 0x00410DE0, CVectorMap::LowerBound 0x0040F020 / const 0x004365B0, CTCShop::GetCurrentSellPrice 0x007743B0 / GetCurrentBuyPrice
//   0x007742C0 / IsBarberShop 0x00772490 / IsTattooShop 0x00772510 / IsTitleShop 0x00772590 / GetQuantity 0x007726B0 / GetMaxStock 0x00772720 /
//   GetDaysTillRestock 0x00772790, CTCBase::GetDefinitionManager 0x006869D0, GFIntToWideString 0x00405F00, NGameText::CDataBank::GetTextBySymbol
//   0x009C95E0, map<unsigned long,CUIState>::operator[] 0x0042D5B1, __ftol2 0x00BFEA70, CPlayerManager::GetMainPlayer 0x00449960,
//   CPlayer::PeekPControlledCreature 0x00487DD0, CTCInventoryItem::GetInventoryType 0x005D99D0 / IsWeapon 0x005D97F0,
//   NInventory::CTCInventoryBase::GetBoughtPrice 0x005BEAD5 / GetNumberOfItemsOfTypeInInventory 0x005BDF08, CTCInventoryWeapons::GetItemDescriptionDamage
//   0x005C7F07 / Weight 0x005C7D0D / NoDamageNorWeight 0x005C7B10, CTCHeroExperience::GetTrainableStatLevel 0x006D6520,
//   CTCInventoryExperience::GetExperienceStatName 0x0056F26F, _Rb_global<bool>::_M_increment 0x004292C0; virtual CComponent::AddChild (+0xEC),
//   GetStates (+0x148), CText::SetValue (+0x240), CTCInventoryBase::GetItemDescription (+0x138), CTCBase::GetInterfaceType (+0x54).
// Shape: the 0x005C90FC ConstructWeaponDescriptionContainer idioms (same-TU noinline GetDef / GetSubDef bodies so the CDefPointer out-pointers stay
//   unaliased, FABLE_RETAIL_STLP_MAP, the PC/UI if-else creation the compiler cross-jumps). ONE `CText* line` local serves every text line (retail
//   keeps it in ebx / [ebp-20h] throughout; one named local per line gave each its own slot and shifted the whole frame). The brief form is the
//   if-arm of an if/else (an early return duplicated the destructor tail).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADEDESCRIPTION
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ITEMDESCRIPTION
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_STOCKQUERIES
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF
#define FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroCombatDef.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIState.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

class CThingPlayerCreature : public CThing { };

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

NUISystem::CComponent* CTCInventoryTrade::ConstructTradeDescriptionWeapon(long itemDefIndex, unsigned long, bool selling, bool brief)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> textComponent;
    NUISystem::CComponent* description;
    if (InventoryTradeOpen) {
        description = manager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false);
        textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    } else {
        description = manager->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE", false);
        textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    }
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    NUISystem::CText* line = static_cast<NUISystem::CText*>(textComponent.Get());
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    long price;
    if (selling && !brief)
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
    if (brief) {
        text = bank.GetTextBySymbol("TEXT_GUI_PRICE") + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
        if (price < defaultPrice) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.G *= 0.5f;
        } else if (price > defaultPrice) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.R *= 0.5f;
        }
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const NInventory::CTCInventoryBase* inventory = 0;
        hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
        text = L"";
        inventory->GetItemDescription(text, itemDefIndex, true);
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
    } else {
    text = bank.GetTextBySymbol(miscDef->TradeCostString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
    text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;
    description->AddChild(textComponent);
    line->SetValue(text, true, true);
    if ((price > defaultPrice && selling) || (price < defaultPrice && !selling)) {
        textComponent->GetStates()[0].Colour.B *= 0.5f;
        textComponent->GetStates()[0].Colour.G *= 0.5f;
    } else if ((price < defaultPrice && selling) || (price > defaultPrice && !selling)) {
        textComponent->GetStates()[0].Colour.B *= 0.5f;
        textComponent->GetStates()[0].Colour.R *= 0.5f;
    }
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const NInventory::CTCInventoryBase* inventory = 0;
    hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
    bool special = shop->IsBarberShop() || shop->IsTattooShop() || shop->IsTitleShop();
    if (!selling) {
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
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
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
    }
    if (inventory->GetInterfaceType() == TCI_INVENTORY_WEAPONS) {
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        text = CWideString::NullString;
        static_cast<const CTCInventoryWeapons*>(inventory)->GetItemDescriptionDamage(text, itemDefIndex);
        if (text != CWideString::NullString) {
            description->AddChild(textComponent);
            line->SetValue(text, true, true);
        }
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        text = CWideString::NullString;
        static_cast<const CTCInventoryWeapons*>(inventory)->GetItemDescriptionWeight(text, itemDefIndex);
        if (text != CWideString::NullString) {
            description->AddChild(textComponent);
            line->SetValue(text, true, true);
        }
    }
    if (selling && !special) {
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        text = bank.GetTextBySymbol(miscDef->TradeAlreadyOwnsString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator
             + GFIntToWideString(inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex));
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
    }
    if (!special) {
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        long quantity = shop->GetQuantity(itemDefIndex);
        long maxStock = shop->GetMaxStock(itemDefIndex);
        text = bank.GetTextBySymbol(miscDef->TradeNumberInStockString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(quantity);
        text += L"/" + GFIntToWideString(maxStock);
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        long days = shop->GetDaysTillRestock(itemDefIndex);
        text = bank.GetTextBySymbol(miscDef->TradeDeliveryString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator;
        if (days != 0)
            text += GFIntToWideString(days) + miscDef->SpaceSeparator + bank.GetTextBySymbol(miscDef->TradeDaysString.ToCharString());
        else
            text += bank.GetTextBySymbol(miscDef->TradeNoDeliveryString.ToCharString());
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
    }
    if (selling && CTCInventoryItem::IsWeapon(itemDefIndex)) {
        CDefPointer<const CThingBaseDef> thingDef;
        GetDefinitionManager().GetDef(itemDefIndex, &thingDef);
        CDefPointer<const CWeaponDef> weaponDef;
        thingDef->GetSubDef(&weaponDef);
        if (weaponDef->Class == 2) {
            CTCHeroExperience* experience;
            CTCInventoryExperience* inventoryExperience;
            if (ParentThing->GetTC(TCI_HERO_EXPERIENCE, &experience) && ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &inventoryExperience)) {
                CDefPointer<const CHeroCombatDef> combatDef;
                combatDef = experience->GetPHeroCombatDef();
                if (!combatDef.IsNull()) {
                    for (std::map<EHeroTrainableStatType, long>::const_iterator it = combatDef->HeavyWeapons.StatThresholds.begin();
                         it != combatDef->HeavyWeapons.StatThresholds.end(); ++it) {
                        if (experience->GetTrainableStatLevel(it->first) < it->second) {
                            if (InventoryTradeOpen)
                                textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                            else
                                textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                            line = static_cast<NUISystem::CText*>(textComponent.Get());
                            text = inventoryExperience->GetExperienceStatName(it->first) + miscDef->SpaceSeparator
                                 + bank.GetTextBySymbol("TEXT_GUI_MENU_EXP_LEVEL") + miscDef->SpaceSeparator
                                 + GFIntToWideString(it->second) + miscDef->SpaceSeparator
                                 + bank.GetTextBySymbol("TEXT_GUI_MENU_REQUIRED");
                            textComponent->GetStates()[0].Colour.B *= 0.5f;
                            textComponent->GetStates()[0].Colour.G *= 0.5f;
                            description->AddChild(textComponent);
                            line->SetValue(text, true, true);
                        }
                    }
                }
            }
        }
    }
    if (inventory->GetInterfaceType() == TCI_INVENTORY_WEAPONS) {
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<NUISystem::CText*>(textComponent.Get());
        text = CWideString::NullString;
        static_cast<const CTCInventoryWeapons*>(inventory)->GetItemDescriptionNoDamageNorWeight(text, itemDefIndex, selling);
        description->AddChild(textComponent);
        line->SetValue(text, true, true);
    }
    }
    return description;
}
