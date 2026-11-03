// Leaf: ?ConstructWeaponDescriptionContainer@CTCInventoryWeapons@@QBEXPAVCComponent@NUISystem@@JM@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_NGAMETEXT_CDATABANK
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_NINVENTORY_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATS
#define FABLE_RETAIL_CHEROCOMBATDEF
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCWeapon.h"
#include "engine/CPlayerGui.h"
#include "fable_game_text_bank.h"
#include "fable_game.h"
#include "stlport/fable_map.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CStockItemDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroExperience.h"
#include "engine/NUISystem_CUIState.h"
// ---- ConstructWeaponDescription models (no engine header exists for these classes) ----
class CObjectAugmentationsDef : public CSubDefClassBase {   // RTTI .?AVCObjectAugmentationsDef@@ : CSubDefClassBase; Ego_r.pdb members
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CObjectAugmentationsDef", -1); }   // inline (GetSubDef<T> 0x0042AF9E)
    long              NumberOfSlots;                    // +0x28
    long              MaximumNumberOfSlots;             // +0x2c
    unsigned char     AugmentationParticles[0xc];       // +0x30 map<EObjectAugmentationType,CObjectAugmentationParticleSet> (STLport 12)
    unsigned long     EmptySlotName;                    // +0x3c
    std::vector<long> InitialAugmentations;             // +0x40 (Ego_r +0x44)
};
namespace NInventory {
class CPerItemData {                                    // Ego_r.pdb CPerItemData, retail 0x24 (12-byte STLport vector); ctor 0x005C14A6
public:
    CPerItemData()
        : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1)
    {
    }
    std::vector<long> Augmentations;                    // +0x00
    long              NumberOfKills;                    // +0x0c
    long              TimeWielded;                      // +0x10
    long              BestTrophyWitnessesAheadToDate;   // +0x14
    bool              QuestPersistent;                  // +0x18
    long              QuestPersistentRefCount;          // +0x1c
    float             BestFishWeight;                   // +0x20
};
}
class CTCVolumeContainmentTracker : public CTCBase {    // Ego_r.pdb CTCVolumeContainmentTracker (TCI_VOLUME_CONTAINMENT_TRACKER 117)
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VOLUME_CONTAINMENT_TRACKER; }
    CIntelligentPointer<CThing> PThingInside;           // +0x0c
};
class CTCObjectAugmentations {
public:
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);   // 0x007661C0
    static CWideString __fastcall GetAugmentationName(long defIndex);                                        // 0x00765F90
};
namespace { extern const CWideString NEWLINE; }        // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L"\n"; ESTABLISHED_SPELLINGS)
// Retail-asm: 0x005C6CB4 `fstp dword ptr [ebp-18h]` / 0x005C6CB7 `fld dword ptr [ebp-18h]` / 0x005C6CBA `fistp dword ptr [ebp-14h]` -- the engine's inline-assembly float-to-long helper (the scaled damage stored to its by-value copy and reloaded for a bare dword fistp; again at 0x005C6CC6..0x005C6CCC). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`: no C/C++ form gives the bare dword fistp.
__forceinline long __fastcall GFFastFloatToLong(float value)   // FableWin.pdb ?GFFastFloatToLong@@YIJM@Z (out of line only in the debug build)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
extern const CWideString g_EmptyWideString;             // 0x013BCA24 (established data spelling)
// ---- TU-local models (no engine header exists for these classes) ----
extern CCharString g_CCharString_013b8bac;              // 0x013B8BAC "Weapon_Descriptions" (established data spelling)
extern CCharString g_CCharString_013b8bc0;              // 0x013B8BC0 "Augmentations_Menu_Entry_Name" (established data spelling)
CWideString __fastcall GFIntToWideString(long value);   // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z
namespace NUISystem {
class CText : public CChangingStateComponent {          // RTTI .?AVCText@NUISystem@@ : CChangingStateComponent, vtable 0x01249CCC
public:
    virtual CCountedPointer<CFontBank>& GetFont();                                 // slot 142 0x0054FF10
    virtual void SetValue(const CCharString& text, bool lookUpTag, bool keepSize);  // slot 145 0x0054F370
    virtual void SetValue(const CWideString& text, bool lookUpTag, bool keepSize);  // slot 144 0x0054F410
    virtual void SetValue(unsigned long index, const CWideString& text, bool keepSize);   // slot 143 0x0054FBC0
};
class CViewport : public CChangingStateComponent {      // RTTI .?AVCViewport@NUISystem@@ : CChangingStateComponent
public:
    virtual void AddFakeChild(CComponent* child);                                    // slot 142 (Ego_r ?AddFakeChild@CViewport@NUISystem@@UAEXPAVCComponent@2@@Z)
};
}
class CWeaponDef : public CSubDefClassBase {            // Ego_r.pdb CWeaponDef (members at the retail offsets); RTTI : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CWeaponDef", -1); }
    long          Type;                                 // +0x28 EWeaponType
    EWeaponClass  Class;                                // +0x2c
    unsigned long Property;                             // +0x30
    CCharString   AnimationGroupName;                   // +0x34
    long          SheatheObject;                        // +0x38
    bool          SheatheObjectAlwaysVisible;           // +0x3c
    float         AnimationSpeedVal;                    // +0x40
    float         Damage;                               // +0x44
    long          DamageType;                           // +0x48 EDamageAttribute
    long          Explosion;                            // +0x4c
    unsigned char FlourishParticles_to_MissileLoadCarrySlot[0xa0 - 0x50];   // +0x50 FlourishParticles .. ProjectileWeaponHelperNameBase
    long          Shot;                                 // +0xa0 (Ego_r +0xAC; the CThingShotDef of the projectile)
};
class CThingShotDef : public CParentDefClassBase {      // Ego_r.pdb CThingShotDef (retail CParentDefClassBase 0x39 -> members from +0x3c)
public:
    float         Speed;                                // +0x3c
    long          PrimaryEffect;                        // +0x40
    long          PrimaryEffectSound;                   // +0x44
    long          AttachedContinuousEffect;             // +0x48
    long          AttachedContinuousSound;              // +0x4c
    bool          UseEndTimeLineOnContinousEffect;      // +0x50
    float         HitThingDamage;                       // +0x54
};
// The TU's own out-of-line template bodies (COMDATs retail calls: IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340,
// GetSubDef<CWeaponDef> 0x0042B1A6).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing && !thing->IsDead();
}
template <> __declspec(noinline) bool __fastcall IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing)
{
    return thing.Get() && !thing->IsDead();
}
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (!def.IsNull()) {
        *out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CIntelligentPointer<CThing>              PActiveMeleeWeapon;                 // +0x134
    long                                     ActiveMeleeWeaponDefIndex;          // +0x13c CDefIndex
    bool                                     MeleeWeaponCarriedOnLoad;           // +0x140
    CIntelligentPointer<CThing>              PActiveRangedWeapon;                // +0x141
    long                                     ActiveRangedWeaponDefIndex;         // +0x14c CDefIndex
    bool                                     RangedWeaponCarriedOnLoad;          // +0x150
    unsigned char                            ActiveWeaponPrimitiveHandles[0xc];  // +0x154 vector<CEnginePrimitiveHandle>
    long                                     ConfiscatedActiveMeleeWeaponDefIndex;   // +0x160
    long                                     ConfiscatedActiveRangedWeaponDefIndex;  // +0x164
    long                                     PreviouslyWieldedMeleeWeaponDefIndex;   // +0x168
    long                                     PreviouslyWieldedRangedWeaponDefIndex;  // +0x16c
    std::map<NUISystem::CComponent*, long>   WeaponMenuItemMap;                  // +0x170
    NUISystem::CComponent*                   WeaponsList;                        // +0x17c
    void RefreshActiveWeaponData() const;                                            // 0x005C30D3
    void ConstructWeaponDescription(CWideString& description, long defIndex, float damage, bool useDefaultItemData) const;   // 0x005C6B26
    void ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long defIndex, float damage) const;   // 0x005C90FC
    void SetWeaponMappingToMenuEntry(NUISystem::CComponent* entry, long defIndex);   // 0x0042D405
    float GetActiveMeleeWeaponDamage() const;
    float GetActiveProjectileWeaponDamage() const;
    void RefreshWeaponDescriptions();
};
void CTCInventoryWeapons::ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long defIndex, float damage) const
{
    RefreshActiveWeaponData();
    CCountedPointer<NUISystem::CComponent> textComponent;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef;
    long miscThingsIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(miscThingsIndex, &miscThingsDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    const NInventory::CPerItemData* itemData = GetPerItemDataForItem(defIndex);
    float weaponDamage;
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        weaponDamage = weaponDef->Damage;
    } else {
        CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
        weaponDamage = shotDef->HitThingDamage;
    }
    long baseDamage = GFFastFloatToLong(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(itemData->Augmentations) * weaponDamage * 10.0f);
    long currentDamage = GFFastFloatToLong(damage * 10.0f);
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    long damageDifference = baseDamage - currentDamage;
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_DAMAGE") + miscThingsDef->SpaceSeparator;
    text += GFIntToWideString(baseDamage) + miscThingsDef->SpaceSeparator;
    text += miscThingsDef->OpenBracket;
    if (damageDifference > 0)
        text += miscThingsDef->Positive;
    text += GFIntToWideString(damageDifference) + miscThingsDef->CloseBracket;
    text += NEWLINE;
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscThingsDef->SpaceSeparator;
    const std::vector<long>& augmentations = itemData->Augmentations;
    if (!augmentations.empty()) {
        long lastAugmentation = -1;
        long count = augmentations.size();
        long numAugmentations = 0;
        for (long i = 0; i < count; ++i) {
            if (augmentations[i] > 0) {
                ++numAugmentations;
                lastAugmentation = i;
            }
        }
        if (numAugmentations > 0) {
            for (long i = 0; i < count; ++i) {
                if (augmentations[i] > 0) {
                    text += CTCObjectAugmentations::GetAugmentationName(augmentations[i]);
                    if (i < lastAugmentation)
                        text += miscThingsDef->CommaSeparator;
                }
            }
        } else {
            text += textBank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
        }
    } else {
        text += textBank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
    }
    text += NEWLINE;
    {
        CDefPointer<const CStockItemDef> stockItemDef;
        def->GetSubDef(&stockItemDef);
        long value = stockItemDef->DefaultPrice;
        const CTCVolumeContainmentTracker* tracker;
        if (ParentThing->PeekTC(&tracker)) {
            CThing* thingInside = tracker->PThingInside.Get();
            CTCShop* shop;
            if (IS_THING_ALIVE(thingInside) && thingInside->GetTC(&shop) && shop->IsValidStockItem(defIndex))
                value = shop->GetCurrentBuyPrice(defIndex, 1);
        }
        text += textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_VALUE") + miscThingsDef->SpaceSeparator + GFIntToWideString(value);
    }
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* textItem = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    textItem->SetValue(text, true, true);
    text = g_EmptyWideString;
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        text += textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + miscThingsDef->SpaceSeparator;
        if (weaponDef->Class == WC_HEAVY)
            text += textBank.GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
        else if (weaponDef->Class == WC_LIGHT)
            text += textBank.GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
    }
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    textItem = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    textItem->SetValue(text, true, true);
    text = g_EmptyWideString;
    if (weaponDef->Class == WC_HEAVY) {
        const CTCHeroExperience* experience;
        if (ParentThing->PeekTC(&experience) && ParentThing->HasTC(TCI_INVENTORY_EXPERIENCE)) {
            CDefPointer<const CHeroCombatDef> combatDef;
            combatDef = experience->GetPHeroCombatDef();
            if (!combatDef.IsNull()) {
                const std::map<EHeroTrainableStatType, long>& thresholds = combatDef->HeavyWeapons.StatThresholds;
                for (std::map<EHeroTrainableStatType, long>::const_iterator it = thresholds.begin(); it != thresholds.end(); ++it) {
                    if (experience->GetTrainableStatLevel(it->first) < it->second) {
                        NUISystem::CUIState& blueState = textItem->GetStates()[0];
                        blueState.Colour.B *= 0.0f;
                        NUISystem::CUIState& greenState = textItem->GetStates()[0];
                        greenState.Colour.G *= 0.0f;
                    }
                }
            }
        }
    }
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_KILLS") + miscThingsDef->SpaceSeparator;
    text += GFIntToWideString(itemData->NumberOfKills);
    text += NEWLINE;
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    textItem = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    textItem->SetValue(text, true, true);
    text = g_EmptyWideString;
}
