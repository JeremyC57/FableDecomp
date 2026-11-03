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
