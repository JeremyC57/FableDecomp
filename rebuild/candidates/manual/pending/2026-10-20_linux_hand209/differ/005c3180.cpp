// Grade: DIFFER(188v186 ours v retail) -- frame-slot sharing (the QFE/RTM gap): retail homes the manager's by-value def temporary in the weapon-def out slot ([ebp-4]) once that def is in edi; the lane gives it its own word ([ebp-8], `sub esp,0Ch`); every instruction otherwise identical
// CTCInventoryWeapons::GetActiveProjectileWeaponDamage() -- the damage of the projectile fired by the hero's active ranged weapon (its weapon def's projectile def), or 0 when no living ranged weapon is active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C3180, length 186 bytes (float in st0). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical>> @ 0x004CC340 (`__fastcall`, by reference), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<T>*) @ 0x0042B1A6 (catalog GetSubDef<CAbilityDef>, a byte twin), CTCBase::
// GetDefinitions() @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex(long) @ 0x009AD9E0 (a counted def pointer by value through the
// hidden pointer), the defs' virtual slot 1 on release.
// Layout: retail CTCInventoryWeapons PActiveRangedWeapon +0x141 (PDB +0x159; an 8-byte intelligent pointer at an odd offset, hence pack(1));
// CThing::PDef +0x70; the weapon def's projectile def index at +0xA0 and the projectile def's damage at +0x54 (retail; the PDB CWeaponDef has
// MissileLoadCarrySlot at +0xA0 -- the retail def is the projectile-weapon variant).
// Shape: the result lives in [ebp-8] (zeroed with `fldz ; fstp`, read back for the return); the weapon-def out slot ([ebp-4]) is the landed
// OnCreate dance (owner addref / GetSubDef / owner release by hand, all null-tested) and, once its def is in edi, the same word homes the
// manager's by-value temporary, which is copied (addref) into the projectile def and released with the manager's long form and zeroed; the
// projectile def (esi) and the weapon def (edi) are released by hand without null tests after the damage is read.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRefCounted {                                 // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()                          // inlined into ~CDefPointerTemp
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointerTemp {                             // the manager's by-value def reference
    CDefBase* m_p;
    __forceinline ~CDefPointerTemp()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
template <class T> struct CDefPointer {              // the out slot the definition lookups fill (released by hand, as the landed OnCreate twins)
    T* Object;
    T* operator->() const { return Object; }
};
struct CProjectileDef : CDefBase {
    unsigned char _pad08[0x54 - 0x08];
    float Damage;                                    // +0x54
};
struct CWeaponDef : CDefBase {
    unsigned char _pad08[0xa0 - 0x08];
    long ProjectileDefIndex;                         // +0xa0
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CWeaponDef>* out);    // 0x0042B1A6
};
struct CDefinitionManager {
    CDefPointerTemp GetPDefFromGlobalIndex(long index);   // 0x009AD9E0 (hidden result pointer)
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
};
struct CIntelligentPointer {                         // CIntelligentPointer<CThing> (8 bytes)
    void*   __vftable;
    CThing* PItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 for CIntelligentPointer

namespace NInventory {
#pragma pack(push, 1)
struct CTCInventoryWeapons {
    unsigned char       _pad000[0x141];
    CIntelligentPointer PActiveRangedWeapon;         // +0x141
    CDefinitionManager* GetDefinitions();            // 0x006869D0 (CTCBase)
    float GetActiveProjectileWeaponDamage();
};
#pragma pack(pop)
float CTCInventoryWeapons::GetActiveProjectileWeaponDamage()
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CDefPointer<CWeaponDef> def;
        def.Object = 0;
        CDefClassBase* thingDef = PActiveRangedWeapon.GetPItem()->PDef.Object;
        if (thingDef)
            ++thingDef->m_count;
        thingDef->GetSubDef(&def);
        if (thingDef && --thingDef->m_count == 0)
            thingDef->Destroy();
        CWeaponDef* weaponDef = def.Object;
        CProjectileDef* projectileDef;
        {
            CDefPointerTemp found = GetDefinitions()->GetPDefFromGlobalIndex(weaponDef->ProjectileDefIndex);
            projectileDef = (CProjectileDef*)found.m_p;
            if (projectileDef)
                ++projectileDef->m_count;
        }
        damage = projectileDef->Damage;
        if (--projectileDef->m_count == 0)
            projectileDef->Destroy();
        if (--weaponDef->m_count == 0)
            weaponDef->Destroy();
    }
    return damage;
}
}
