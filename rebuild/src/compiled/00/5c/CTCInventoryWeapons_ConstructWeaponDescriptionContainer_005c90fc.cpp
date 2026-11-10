// CTCInventoryWeapons::ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long itemDefIndex, float damage) const -- fills a weapon item's description container with three text lines: damage (against the given wielded damage), augmentations and value; the weight class (shown in red for a heavy weapon whose stat thresholds the hero has not reached); the kill count
// Leaf: ?ConstructWeaponDescriptionContainer@CTCInventoryWeapons@@QBEXPAVCComponent@NUISystem@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C90FC, 2150 bytes, thiscall `ret 0Ch`, no EH frame; optimize("s",on) (the 0x005C6B26 TU header).
// Retail-asm: 0x005C920A `fistp dword ptr [ebp-14h]` and 0x005C921C `fistp dword ptr [ebp-20h]` -- the two GFFloatToLongNear roundings (engine/GFFloatToLongNear.h, the engine's inline-asm float-to-long helper; VC7.1 emits no bare dword fistp from C++).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructWeaponDescriptionContainer@CTCInventoryWeapons@@QBEXPAVCComponent@NUISystem@@JM@Z (Ego_r donor
//   order: after PutWeaponsInSheathes 0x005C8101 / ConstructScreen 0x005C8459, before RefreshSheathedWeapons 0x005C9962); sole caller
//   RefreshWeaponDescriptions 0x005CACB9. The body is the 0x005C6B26 description (damage / augmentations / value parts byte-identical in
//   shape) written into a local CWideString and split over three UI text components.
// Callees (beyond 0x005C6B26's): CWideString() 0x0099AED0, CWideString::operator=(const CWideString&) 0x0099B7D0 with
//   CWideString::NullString 0x013BCA24, NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D,
//   CCountedPointer<CComponent>::Reset 0x00429C6F / ReleaseData 0x004291DE, CComponent::AddChild (slot 59, +0xEC), CText::SetValue
//   (slot 144, +0x240), CComponent::GetStates (slot 82, +0x148), map<unsigned long,CUIState>::operator[] 0x0042D5B1,
//   CTCHeroExperience::GetTrainableStatLevel 0x006D6520, _Rb_global<bool>::_M_increment 0x004292C0, __ftol2 0x00BFEA70.
// Data: "PC_TEXT_INSIDE_CONTAINER_TEMPLATE" 0x012537D8 / "UI_TEXT_INSIDE_CONTAINER_TEMPLATE" 0x01247D70 (CPlayerGui::IsLiveGUIMode,
//   +0x2BE), 0.0f 0x0122DEDC.
// Shape: the 0x005C6B26 header and parts; each text component is created in an if/else over IsLiveGUIMode() with one CreateComponent
//   call per arm (VC7.1 hoists the shared `push -1` / temp address and cross-jumps the arms: `je ; push PC ; jmp ; push UI ; call`,
//   0x005C9563; a ternary inside one call loads the literal into eax first instead); the text pointer is named before AddChild (it is
//   read before the call, `mov esi,[ebp-34h]`, 0x005C959E); the hero's combat def is copied by GetPHeroCombatDef() into a declared
//   pointer (`inc ; inc ; dec`, 0x005C9757) and tested before the walk; FABLE_RETAIL_STLP_MAP makes map::operator[]'s body visible
//   (still out of line), so its key temporary shares the int->float temporary's slot and the second key store is scheduled before
//   the colour byte store (0x005C97C2).
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
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_LIVEGUI
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF
#define FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP
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
#include "engine/CPlayerGui.h"
#include "engine/CTCHeroExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIState.h"

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



void CTCInventoryWeapons::ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long itemDefIndex, float damage) const
{
    RefreshActiveWeaponData();
    CCountedPointer<NUISystem::CComponent> textComponent;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    const NInventory::CPerItemData* itemData = GetPerItemDataForItem(itemDefIndex);
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
    // the item's value: the default price, or the buy price of the shop the hero is in when it stocks the item
    {
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
    }
    // first text line: damage, augmentations and value
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* line = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    line->SetValue(text, true, true);
    text = CWideString::NullString;
    // second text line: a melee weapon's weight class
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + miscDef->SpaceSeparator;
        if (weaponDef->Class == 2)
            text += bank.GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
        else if (weaponDef->Class == 1)
            text += bank.GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
    }
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* weightLine = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    weightLine->SetValue(text, true, true);
    text = CWideString::NullString;
    // a heavy weapon the hero is not yet trained for (a heavy-weapon stat threshold above his level) shows its weight in red
    if (weaponDef->Class == 2) {
        const CTCHeroExperience* experience;
        if (ParentThing->PeekTC(&experience) && ParentThing->HasTC(TCI_INVENTORY_EXPERIENCE)) {
            CDefPointer<const CHeroCombatDef> combatDef;
            combatDef = experience->GetPHeroCombatDef();
            if (!combatDef.IsNull()) {
                for (std::map<EHeroTrainableStatType, long>::const_iterator it = combatDef->HeavyWeapons.StatThresholds.begin();
                     it != combatDef->HeavyWeapons.StatThresholds.end(); ++it) {
                    if (experience->GetTrainableStatLevel(it->first) < it->second) {
                        weightLine->GetStates()[0].Colour.B *= 0.0f;
                        weightLine->GetStates()[0].Colour.G *= 0.0f;
                    }
                }
            }
        }
    }
    // third text line: the kill count
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_KILLS") + miscDef->SpaceSeparator;
    text += GFIntToWideString(itemData->NumberOfKills);
    text += NEWLINE;
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* killsLine = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    killsLine->SetValue(text, true, true);
    text = CWideString::NullString;
}