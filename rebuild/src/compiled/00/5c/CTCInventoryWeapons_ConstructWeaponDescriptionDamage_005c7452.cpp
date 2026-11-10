// CTCInventoryWeapons::ConstructWeaponDescriptionDamage(CWideString& text, long itemDefIndex, float damage) const -- appends the damage line of a weapon item to its description: the weapon's damage (its def's own, or its shot's hit damage for a ranged weapon) times the multiplier of its def's initial augmentations, in tenths, and in brackets the signed difference to the given (wielded weapon's) damage
// Leaf: ?ConstructWeaponDescriptionDamage@CTCInventoryWeapons@@QBEXAAVCWideString@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C7452, 698 bytes, thiscall `ret 0Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Retail-asm: 0x005C75CA `fistp dword ptr [ebp-0Ch]` and 0x005C75DC `fistp dword ptr [ebp-8]` -- the two GFFloatToLongNear roundings (engine/GFFloatToLongNear.h, the engine's inline-asm float-to-long helper; VC7.1 emits no bare dword fistp from C++).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructWeaponDescriptionDamage@CTCInventoryWeapons@@QBEXAAVCWideString@@JM@Z (Ego_r donor
//   0x00626ACE, between ConstructWeaponDescriptionNoDamageNorWeight 0x005C71AA and ConstructWeaponDescriptionWeight 0x005C770C, the
//   retail order); caller GetItemDescriptionDamage 0x005C7F07.
// Callees: RefreshActiveWeaponData 0x005C30D3, CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390,
//   GetDef<CUIMiscThingsDef> 0x0042B312, CTCBase::GetDefinitionManager 0x006869D0, GetPDefFromGlobalIndex 0x009AD9E0,
//   GetSubDef<CWeaponDef> 0x0042B1A6 / <CObjectAugmentationsDef> 0x0042AF9E, vector<long>::resize 0x004428F0, CTCWeapon::IsMeleeWeaponClass
//   0x005DA510, CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet 0x007661C0, CDataBank::GetTextBySymbol 0x009C95E0,
//   GFIntToWideString 0x00405F00, operator+(CWideString) 0x0099BE70, CWideString::operator+= 0x0099B8D0, free 0x00BFEA14.
// Shape: the def lookups are the same-TU template bodies (as the landed ConstructScreen 0x005C8459); the per-item data is the inline
//   NInventory::CPerItemData (FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR) with STLport's visible noinline resize
//   (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE); the shot def is released through FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (the
//   HitThingDamage load before the `dec`); the two roundings are the engine's GFFloatToLongNear (engine/GFFloatToLongNear.h: the bare
//   `fld ; fistp dword` pairs at 0x005C75C7 / 0x005C75D9, the shared-header retail-asm helper with its FABLE_PORTABLE twin).
#pragma optimize("y", off)
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

void CTCInventoryWeapons::ConstructWeaponDescriptionDamage(CWideString& text, long itemDefIndex, float damage) const
{
    RefreshActiveWeaponData();
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    NInventory::CPerItemData data;
    {
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
    }
    // the item's damage: the weapon def's own (melee) or its shot's hit damage (ranged), times the augmentation multiplier, in tenths
    float weaponDamage;
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class))
        weaponDamage = weaponDef->Damage;
    else {
        CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot));
        weaponDamage = shot->HitThingDamage;
    }
    long itemDamage = GFFloatToLongNear(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(data.Augmentations) * weaponDamage * 10.0f);
    long difference = itemDamage - GFFloatToLongNear(damage * 10.0f);
    // "Damage: <n> (<+/-difference to the wielded weapon>)"
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_DAMAGE") + miscDef->SpaceSeparator;
    text += GFIntToWideString(itemDamage) + miscDef->SpaceSeparator;
    text += miscDef->OpenBracket;
    if (difference > 0)
        text += miscDef->Positive;
    text += GFIntToWideString(difference) + miscDef->CloseBracket;
}