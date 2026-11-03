// Leaf: ?RefreshWeaponDescriptions@CTCInventoryWeapons@@QAEXXZ
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
#define FABLE_RETAIL_DEFPTR_RELEASE_ACCESSOR
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
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
class CWeaponDef : public CDefClassBase {               // Ego_r.pdb CWeaponDef (members at the retail offsets)
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
    void ConstructWeaponDescription(CWideString& description, long defIndex, float damage, bool pc) const;
    void ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long defIndex, float damage) const;   // 0x005C90FC
    void SetWeaponMappingToMenuEntry(NUISystem::CComponent* entry, long defIndex);   // 0x0042D405
    float GetActiveMeleeWeaponDamage() const;
    float GetActiveProjectileWeaponDamage() const;
    void RefreshWeaponDescriptions();
};
// Same-TU bodies of the map COMDATs retail calls (map<CComponent*, long>::operator[] 0x0042D1B7, _Rb_tree::erase(const key&)
// 0x005CC31B): STLport 4.6 _map.h / _tree.h verbatim.
#pragma auto_inline(off)
namespace std {
template <> __declspec(noinline) long& map<NUISystem::CComponent*, long>::operator[](NUISystem::CComponent* const& k)
{
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, value_type(k, long()));
    return (*i).second;
}
typedef _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >,
                 less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > > _WeaponMenuItemTree;
template <> __declspec(noinline) pair<_WeaponMenuItemTree::iterator, _WeaponMenuItemTree::iterator>
_WeaponMenuItemTree::equal_range(NUISystem::CComponent* const& k)
{
    _Link_type y = this->_M_header._M_data;
    _Link_type x = _M_root();
    while (x != 0) {
        if (_M_key_compare(k, _S_key(x))) { y = x; x = _S_left(x); }
        else x = _S_right(x);
    }
    return pair<iterator, iterator>(lower_bound(k), iterator(y));
}
template <> __declspec(noinline) size_t
_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >,
         less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > >::erase(NUISystem::CComponent* const& x)
{
    pair<iterator, iterator> p = equal_range(x);
    size_t n = 0;
    for (iterator it = p.first; it != p.second; ++it)
        ++n;
    erase(p.first, p.second);
    return n;
}
}
#pragma auto_inline(on)
void CTCInventoryWeapons::RefreshWeaponDescriptions()
{
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef;
    long miscThingsIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(miscThingsIndex, &miscThingsDef);
    std::list<NUISystem::CComponent*> descriptions;
    if (WeaponsList) {
        NUISystem::CComponent* augmentEntry = WeaponsList->FindChild(g_CCharString_013b8bc0);
        if (augmentEntry) {
            NUISystem::CText* text = (NUISystem::CText*)augmentEntry->FindChildNonRecursive("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE")->FindChildNonRecursive("MENU_ENTRY_TEXT");
            CWideString label = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_AUGMENT") + CTCInventoryStats::GetXString();
            label += GFIntToWideString(NUISystem::CManager::GetInstance()->GetNumberAugmentations());
            text->SetValue(label, true, true);
        }
        WeaponsList->FindAllChildren(g_CCharString_013b8bac, descriptions);
        float meleeDamage = 0.0f;
        float rangedDamage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCWeapon* weapon = 0;
            PActiveMeleeWeapon->PeekTC(&weapon);
            meleeDamage = weapon->GetDamage();
        }
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CDefPointer<const CWeaponDef> weaponDef;
            PActiveRangedWeapon->GetPBaseDef()->GetSubDef(&weaponDef);
            CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
            rangedDamage = shotDef->HitThingDamage;
        }
        std::list<NUISystem::CComponent*>::iterator end = descriptions.end();
        for (std::list<NUISystem::CComponent*>::iterator it = descriptions.begin(); it != end; ++it) {
            NUISystem::CComponent* entry = *it;
            long defIndex = WeaponMenuItemMap[entry];
            if (defIndex > 0) {
                CDefPointer<const CThingBaseDef> thingDef;
                GetDefinitionManager().CDefinitionManager::GetDef(defIndex, &thingDef);
                CDefPointer<const CWeaponDef> weaponDef;
                thingDef->GetSubDef(&weaponDef);
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                entry->SetDeletion(deletion);
                WeaponMenuItemMap.erase(entry);
                CCountedPointer<NUISystem::CComponent> container;
                if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
                    container.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE", false));
                else
                    container.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_ONE_OFF", false));
                container->SetName(g_CCharString_013b8bac);
                if (entry->GetParentViewport()) {
                    NUISystem::CViewport* viewport = (NUISystem::CViewport*)entry->GetParentViewport();
                    if (viewport)
                        viewport->AddFakeChild(container.Get());
                }
                ConstructWeaponDescriptionContainer(container.Get(), defIndex, CTCWeapon::IsMeleeWeaponClass(weaponDef->Class) ? meleeDamage : rangedDamage);
                if (entry->GetParent())
                    entry->GetParent()->AddChildDynamically(container);
                SetWeaponMappingToMenuEntry(container.Get(), defIndex);
            }
        }
    }
}
