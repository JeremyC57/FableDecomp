// CTCInventoryTrade::ConstructTradeDescription(long itemDefIndex, unsigned long count, bool buying, bool special) -- builds the
//   trade-screen description container for an item: weapons and clothing go to their own builders; otherwise a price line
//   ("cost: price (value)", red/green against the default price), then either the item description (special) or the owned-count
//   line (buying) / profit-or-loss line (selling), the stock and delivery lines (not for barber/tattoo/title shops), the heavy-weapon
//   stat-requirement lines (buying a heavy weapon) and the item description; returns the container.
// Leaf: ?ConstructTradeDescription@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005AD571, 4414 bytes, thiscall `ret 10h`, frame pointer (`optimize("y",off)`), /Os TU
//   (`optimize("s",on)`, as the same-TU 0x005A3E47 / 0x005AE6AF), no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructTradeDescription@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z; callers
//   RefreshDescriptions 0x005AE6AF, RefreshBuyDescriptionsPC 0x005AE9DE, RefreshSellDescriptionsPC 0x005AEAEE, and
//   NUISystem::CManager::Action 0x0041E6D3 (three calls: 0x00425095, 0x004256FF, 0x00425CFF). Parameter names are
//   descriptive (no PDB has them).
// Callees: CTCInventoryItem::IsWeapon 0x005D97F0 / IsClothing 0x005D9C60 / GetInventoryType 0x005D99D0,
//   ConstructTradeDescriptionWeapon 0x005A9CBE / ConstructTradeDescriptionClothing 0x005AAF28, NUISystem::CManager::GetInstance
//   0x0041E5F2 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>::Reset 0x00429C6F / ReleaseData 0x004291DE,
//   CCharString(const char*,long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString() 0x0099AED0 / ~CWideString 0x0099B510 /
//   operator=(const CWideString&) 0x0099B7D0 / operator=(wchar_t) 0x0099B860 / operator+= 0x0099B8D0 / ToCharString 0x0099B2C0,
//   operator+(const CWideString&,const CWideString&) 0x0099BE70, operator+(const wchar_t*,const CWideString&) 0x0099BFF0,
//   GFIntToWideString 0x00405F00, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, NGameText::ConstructString 0x009C7B70 (long) /
//   0x009C7C20 (vector), CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 /
//   GetPDefFromGlobalIndex 0x009AD9E0, GetDef<CUIMiscThingsDef> 0x0042B312, GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CStockItemDef>
//   0x0059350C, GetSubDef<CWeaponDef> 0x0042B1A6, CTCBase::GetDefinitionManager 0x006869D0, CTCInventoryBase::GetBuildingThing
//   0x005BC4C4 / GetNumberOfItemsOfTypeInInventory 0x005BDF08 / GetBoughtPrice 0x005BEAD5 / GetItemDescription (slot 78),
//   bitset<274>::test 0x00410DE0, CVectorMap LowerBound 0x0040F020 / 0x004365B0 (the expanded GetTC / PeekTC), CTCShop
//   GetCurrentSellPrice 0x007743B0 / GetCurrentBuyPrice 0x007742C0 / IsBarberShop 0x00772490 / IsTattooShop 0x00772510 / IsTitleShop
//   0x00772590 / GetQuantity 0x007726B0 / GetMaxStock 0x00772720 / GetDaysTillRestock 0x00772790, CPlayerManager::GetMainPlayer
//   0x00449960, CPlayer::PeekPControlledCreature 0x00487DD0, CTCHeroExperience::GetTrainableStatLevel 0x006D6520,
//   CTCInventoryExperience::GetExperienceStatName 0x0056F26F, vector<CWideString>::push_back 0x004128A0 / ~vector 0x00412130,
//   map<unsigned long,CUIState>::operator[] 0x0042D5B1, _Rb_global<bool>::_M_increment 0x004292C0, __ftol2 0x00BFEA70; virtuals
//   CComponent::AddChild (+0xEC), CText::SetValue (+0x240), CComponent::GetStates (+0x148).
// Shape: every text line is `if (InventoryTradeOpen) Reset(PC) else Reset(UI)` (arms cross-jumped) + the text pointer read into ONE
//   reused `CText* line` local (one variable for all lines: separate per-line locals each took their own frame slot and shifted the
//   whole temporary area by 4 bytes) + `text = ...` + AddChild + SetValue. The combat def is declared then assigned from
//   GetPHeroCombatDef() (`inc ; inc ; dec`, 0x005AE35A). `text = NGameText::ConstructString(fmt, args)` assigns the bool result
//   through operator=(wchar_t) (retail `movzx ax,al` at 0x005AE4EA, call at 0x005AE4F2) -- as the original source did.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CWIDESTRING_ASSIGNCHAR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_CONTAINABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADEDESCRIPTION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCSHOP_INTERFACETYPE
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_STOCKINFO
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_ISCLOTHING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF
#define FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_INTERFACETYPE
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_VECTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStockItemDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CTCInventoryExperience.h"

using namespace NUISystem;

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00
template<> std::vector<CWideString>::~vector();                                  // 0x00412130

// Same-TU bodies of the def lookups (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312, GetDef<CThingBaseDef> 0x0042AF3C,
// GetSubDef<CStockItemDef> 0x0059350C, GetSubDef<CWeaponDef> 0x0042B1A6), as the landed 0x005C90FC.
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
CComponent* CTCInventoryTrade::ConstructTradeDescription(long itemDefIndex, unsigned long count, bool buying, bool special)
{
    if (CTCInventoryItem::IsWeapon(itemDefIndex))
        return ConstructTradeDescriptionWeapon(itemDefIndex, count, buying, special);
    if (CTCInventoryItem::IsClothing(itemDefIndex))
        return ConstructTradeDescriptionClothing(itemDefIndex, count, buying, special);
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> textComponent;
    CComponent* container;
    if (InventoryTradeOpen) {
        container = manager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false);
        textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    } else {
        container = manager->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE", false);
        textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    }
    CWideString text;
    CDefPointer<const CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    CText* line = static_cast<CText*>(textComponent.Get());
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(&shop);
    long price;
    if (buying && !special)
        price = shop->GetCurrentSellPrice(itemDefIndex, count);
    else
        price = shop->GetCurrentBuyPrice(itemDefIndex, count);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    CDefPointer<const CStockItemDef> stockDef;
    long value;
    if (!def.IsNull() && def->GetSubDef(&stockDef))
        value = stockDef->DefaultPrice;
    else
        value = 1;
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    if (special) {
        text = bank.GetTextBySymbol("TEXT_GUI_PRICE") + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(value) + miscDef->CloseBracket;
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
        if (price < value) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.G *= 0.5f;
        } else if (price > value) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.R *= 0.5f;
        }
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<CText*>(textComponent.Get());
        text = CWideString::NullString;
        const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const NInventory::CTCInventoryBase* inventory = 0;
        hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
        inventory->GetItemDescription(text, itemDefIndex, true);
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
    } else {
        text = bank.GetTextBySymbol(miscDef->TradeCostString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(value) + miscDef->CloseBracket;
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
        if ((price > value && buying) || (price < value && !buying)) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.G *= 0.5f;
        } else if ((price < value && buying) || (price > value && !buying)) {
            textComponent->GetStates()[0].Colour.B *= 0.5f;
            textComponent->GetStates()[0].Colour.R *= 0.5f;
        }
        const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const NInventory::CTCInventoryBase* inventory = 0;
        hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);
        bool isSpecialShop = shop->IsBarberShop() || shop->IsTattooShop() || shop->IsTitleShop();
        if (buying) {
            if (!isSpecialShop) {
                if (InventoryTradeOpen)
                    textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                else
                    textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                line = static_cast<CText*>(textComponent.Get());
                long owned = inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
                text = bank.GetTextBySymbol(miscDef->TradeAlreadyOwnsString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(owned);
                container->AddChild(textComponent);
                line->SetValue(text, true, true);
            }
        } else {
            if (InventoryTradeOpen)
                textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<CText*>(textComponent.Get());
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
        if (!isSpecialShop) {
            if (InventoryTradeOpen)
                textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<CText*>(textComponent.Get());
            long stock = shop->GetQuantity(itemDefIndex);
            long maxStock = shop->GetMaxStock(itemDefIndex);
            text = bank.GetTextBySymbol(miscDef->TradeNumberInStockString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(stock);
            text += L"/" + GFIntToWideString(maxStock);
            container->AddChild(textComponent);
            line->SetValue(text, true, true);
            if (InventoryTradeOpen)
                textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            else
                textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
            line = static_cast<CText*>(textComponent.Get());
            long days = shop->GetDaysTillRestock(itemDefIndex);
            text = bank.GetTextBySymbol(miscDef->TradeDeliveryString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator;
            if (days != 0) {
                CWideString daysText = bank.GetTextBySymbol("TEXT_GUI_MENU_DELIVERY_DAYS");
                NGameText::ConstructString(daysText, days);
                text += daysText;
            } else {
                text += bank.GetTextBySymbol(miscDef->TradeNoDeliveryString.ToCharString());
            }
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
                CTCInventoryExperience* invExperience;
                if (ParentThing->GetTC(&experience) && ParentThing->GetTC(&invExperience)) {
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
                                line = static_cast<CText*>(textComponent.Get());
                                std::vector<CWideString> args;
                                args.push_back(invExperience->GetExperienceStatName(it->first));
                                args.push_back(bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL"));
                                args.push_back(GFIntToWideString(it->second));
                                args.push_back(bank.GetTextBySymbol("TEXT_GUI_MENU_REQUIRED"));
                                CWideString fmt = bank.GetTextBySymbol("TEXT_GUI_MENU_STAT_LEVEL");
                                text = NGameText::ConstructString(fmt, args);
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
        if (InventoryTradeOpen)
            textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        else
            textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        line = static_cast<CText*>(textComponent.Get());
        text = CWideString::NullString;
        inventory->GetItemDescription(text, itemDefIndex, true);
        container->AddChild(textComponent);
        line->SetValue(text, true, true);
    }
    return container;
}
