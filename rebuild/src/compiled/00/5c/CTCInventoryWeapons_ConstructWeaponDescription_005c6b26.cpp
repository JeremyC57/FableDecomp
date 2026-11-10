// CTCInventoryWeapons::ConstructWeaponDescription(CWideString& text, long itemDefIndex, float damage, bool fromDef) const -- builds a weapon item's description: the damage line (against the given wielded damage), the augmentations line, for an owned item its value (the shop's buy price when the hero stands in a shop that stocks it, else the default price), for a melee weapon its weight class, and its kill count
// Leaf: ?ConstructWeaponDescription@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C6B26, 1668 bytes, thiscall `ret 10h`, no EH frame; optimize("s",on).
// Retail-asm: 0x005C6CBA `fistp dword ptr [ebp-14h]` and 0x005C6CCC `fistp dword ptr [ebp-18h]` -- the two GFFloatToLongNear roundings (engine/GFFloatToLongNear.h, the engine's inline-asm float-to-long helper; VC7.1 emits no bare dword fistp from C++).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructWeaponDescription@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z (Ego_r donor 0x006261B8,
//   before ConstructWeaponDescriptionNoDamageNorWeight 0x005C71AA, the retail order); callers GetItemDescription 0x005C7913 and
//   ConstructWeaponDescriptionContainer 0x005C90FC.
// Callees: as ConstructWeaponDescriptionDamage 0x005C7452 (RefreshActiveWeaponData 0x005C30D3, CGameDefinitionManager::Get 0x0044C6B0,
//   GetGlobalIndexFromClassIndex 0x009AD390, GetDef<CUIMiscThingsDef> 0x0042B312, CTCBase::GetDefinitionManager 0x006869D0,
//   GetPDefFromGlobalIndex 0x009AD9E0, GetSubDef<CWeaponDef> 0x0042B1A6 / <CObjectAugmentationsDef> 0x0042AF9E, vector<long>::resize
//   0x004428F0, CTCWeapon::IsMeleeWeaponClass 0x005DA510, GetDamageMultiplierFromAugmentationSet 0x007661C0, GetTextBySymbol 0x009C95E0,
//   GFIntToWideString 0x00405F00, operator+ 0x0099BE70, CWideString::operator+= 0x0099B8D0) plus CTCInventoryBase::GetPerItemDataForItem
//   0x005BE8D3, CTCObjectAugmentations::GetAugmentationName 0x00765F90, GetSubDef<CStockItemDef> 0x0059350C, the expanded
//   CThing::PeekTC<CTCVolumeContainmentTracker> (bitset 0x00410DE0, const LowerBound 0x004365B0) and GetTC<CTCShop> (LowerBound
//   0x0040F020), GetPItem 0x00A01B50, IS_THING_ALIVE<CThing*> 0x0040DCE0, CTCShop::IsValidStockItem 0x007723D0 / GetCurrentBuyPrice
//   0x007742C0.
// Data: "TEXT_GUI_MENU_DAMAGE" 0x01254584, "_AUGMENTATIONS" 0x01254568, "_NONE" 0x01254554, "_WEAPON_VALUE" 0x01254538, "_WEAPON_WEIGHT"
//   0x0125451C, "_HEAVY" 0x01254508, "_LIGHT" 0x012544F4, "_KILLS" 0x012544E0; NEWLINE 0x013B8BA8; 10.0f 0x01237F0C.
// Shape: the TU header and damage part of the landed 0x005C7452; the per-item data is a `const CPerItemData*` (the kill count is read
//   through it at the end, `mov edx,[esi+0Ch]`); the "none" text is written in both elses (no augmentations at all / none filled):
//   VC7.1 cross-jumps the two copies, and the surviving one keeps the in-branch schedule retail has (`push -1 ; lea ecx,[ebp+8] ; push
//   str`, 0x005C6E8A; a single `if (none)` join block schedules its argument setups in source order); IS_THING_ALIVE<CThing*> is the
//   same-TU body of the landed 0x005CB2B0 (the shop thing passed through a copy in the dead `text` slot).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_CTCSHOP_INTERFACETYPE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWeaponDef.h"
#include "engine/CObjectAugmentationsDef.h"
#include "engine/CTCWeapon.h"
#include "engine/CThingShotDef.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CStockItemDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCVolumeContainmentTracker.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

namespace { extern const CWideString NEWLINE; }                  // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L"\n")

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312,
// GetSubDef<CWeaponDef> 0x0042B1A6, GetSubDef<CObjectAugmentationsDef> 0x0042AF9E): VC7.1 sees that they only write through the
// out pointer, so the CDefPointer locals whose addresses they take stay unaliased.
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
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);
// Same-TU body of IS_THING_ALIVE<CThing*> (out of line in retail: 0x0040DCE0, the landed `thing != 0 && !IsDead()` body), as the
// landed 0x005CB2B0: the shop thing is passed through a copy in a dead argument slot (`mov [ebp+8],esi ; lea ecx,[ebp+8]`).
#pragma auto_inline(off)
template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
#pragma auto_inline(on)


void CTCInventoryWeapons::ConstructWeaponDescription(CWideString& text, long itemDefIndex, float damage, bool fromDef) const
{
    RefreshActiveWeaponData();
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    // the item's per-item data: from its def's initial augmentations, or the owned item's
    NInventory::CPerItemData data;
    const NInventory::CPerItemData* itemData;
    if (fromDef) {
        CDefPointer<const CObjectAugmentationsDef> augmentationsDef;
        if (def->GetSubDef(&augmentationsDef)) {
            const CObjectAugmentationsDef* a = augmentationsDef.Get();
            data.Augmentations.resize(a->NumberOfSlots, 0);
            for (unsigned long i = 0; i < a->InitialAugmentations.size(); ++i) {
                if (i >= data.Augmentations.size())
                    break;
                data.Augmentations[i] = a->InitialAugmentations[i];
            }
        }
        itemData = &data;
    } else {
        itemData = GetPerItemDataForItem(itemDefIndex);
    }
    // damage: the weapon def's own (melee) or its shot's hit damage (ranged), times the augmentation multiplier, in tenths
    float weaponDamage;
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class))
        weaponDamage = weaponDef->Damage;
    else {
        CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot));
        weaponDamage = shot->HitThingDamage;
    }
    long itemDamage = GFFloatToLongNear(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(itemData->Augmentations) * weaponDamage * 10.0f);
    long difference = itemDamage - GFFloatToLongNear(damage * 10.0f);
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_DAMAGE") + miscDef->SpaceSeparator;
    text += GFIntToWideString(itemDamage) + miscDef->SpaceSeparator;
    text += miscDef->OpenBracket;
    if (difference > 0)
        text += miscDef->Positive;
    text += GFIntToWideString(difference) + miscDef->CloseBracket;
    text += NEWLINE;
    // the names of the filled augmentation slots, comma separated, or "none"
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscDef->SpaceSeparator;
    const std::vector<long>& augmentations = itemData->Augmentations;
    if (!augmentations.empty()) {
        long last = -1;
        long count = augmentations.size();
        long used = 0;
        for (long i = 0; i < count; ++i) {
            if (augmentations[i] > 0) {
                ++used;
                last = i;
            }
        }
        if (used > 0) {
            for (long i = 0; i < count; ++i) {
                if (augmentations[i] > 0) {
                    text += CTCObjectAugmentations::GetAugmentationName(augmentations[i]);
                    if (i < last)
                        text += miscDef->CommaSeparator;
                }
            }
        } else {
            text += bank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
        }
    } else {
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
    }
    text += NEWLINE;
    // an owned item's value: the default price, or the buy price of the shop the hero is in when it stocks the item
    if (!fromDef) {
        CDefPointer<const CStockItemDef> stockDef;
        def->GetSubDef(&stockDef);
        long value = stockDef->DefaultPrice;
        const CTCVolumeContainmentTracker* tracker;
        if (ParentThing->PeekTC(&tracker)) {
            CThing* shopThing = tracker->GetPThingInside();
            CTCShop* shop;
            if (IS_THING_ALIVE(shopThing) && shopThing->GetTC(&shop) && shop->IsValidStockItem(itemDefIndex))
                value = shop->GetCurrentBuyPrice(itemDefIndex, 1);
        }
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_VALUE") + miscDef->SpaceSeparator + GFIntToWideString(value);
        text += NEWLINE;
    }
    // a melee weapon's weight class
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + miscDef->SpaceSeparator;
        if (weaponDef->Class == 2)
            text += bank.GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
        else if (weaponDef->Class == 1)
            text += bank.GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
        text += NEWLINE;
    }
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_KILLS") + miscDef->SpaceSeparator;
    text += GFIntToWideString(itemData->NumberOfKills);
    text += NEWLINE;
}