// CTCInventoryWeapons::GetActiveProjectileWeaponDamage() -- the damage of the projectile fired by the hero's active ranged weapon (its weapon def's projectile def), or 0 when no living ranged weapon is active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C3180, length 186 bytes (float in st0). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical>> @ 0x004CC340 (`__fastcall`, by reference), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>*) const @ 0x0042B1A6, CTCBase::GetDefinitionManager()
// const @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex(long) const @ 0x009AD9E0 (a CDefPointer<const CDefClassBase> by value through the
// hidden pointer), the defs' virtual slot 1 on release.
// Layout: retail CTCInventoryWeapons PActiveRangedWeapon +0x141 (PDB +0x159; an 8-byte intelligent pointer at an odd offset, hence pack(1));
// CThing::PDef +0x70; the weapon def's projectile def index at +0xA0 and the projectile def's damage at +0x54 (retail; the PDB CWeaponDef has
// MissileLoadCarrySlot at +0xA0 -- the retail def is the projectile-weapon variant).
// Shape: the result lives in [ebp-8] (zeroed with `fldz ; fstp`, read back for the return); the weapon-def out slot ([ebp-4]) is the landed
// OnCreate dance (owner addref / GetSubDef / owner release by hand, all null-tested) with the same-TU GetSubDef stand-in (the out word does
// not escape, so the manager's by-value temporary shares it once the weapon def is in edi); the projectile def is a CDefPointerCopy local
// copy-initialised from that temporary (add-ref through the returned pointer, the temporary released with the long form and zeroed) and
// released by its destructor through the `DecRefToZero(long&)` helper (`mov eax,[esi+54h] ; dec [esi+4] ; mov [ebp-8],eax ; jne`: the
// damage load is scheduled before the decrement only with the by-reference helper), then the weapon def by hand.
#include "fable_compiler.h"
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
template <class T> class CDefPointer { public:              // the out slot the definition lookups fill (released by hand, as the landed OnCreate twins)
    CDefBase* Object;                                // the pointee as its non-const base (engine/CDefPointer.h)
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase;
template <> class CDefPointer<const CDefClassBase> { public:   // the manager's by-value def reference (released by its destructor)
    CDefBase* m_p;
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
FABLE_FORCEINLINE bool DecRefToZero(long& c) { return --c == 0; }
template <class T> struct CDefPointerCopy {          // a def copied (add-ref) out of the manager's by-value temporary, released by its destructor
    T* Object;
    CDefPointerCopy(const CDefPointer<const CDefClassBase>& temp) : Object((T*)temp.m_p) { if (Object) ++Object->m_count; }
    ~CDefPointerCopy() { if (DecRefToZero(Object->m_count)) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CProjectileDef : CDefBase {
    unsigned char _pad08[0x54 - 0x08];
    float Damage;                                    // +0x54
};
class CWeaponDef : public CDefBase { public:
    unsigned char _pad08[0xa0 - 0x08];
    long ProjectileDefIndex;                         // +0xa0
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0042B1A6 GetSubDef<CWeaponDef> (Ego_r / FableWin template)
};
class CThingBaseDef : public CDefClassBase { public: };
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CDefBase*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (Ego_r.pdb QBE; hidden result pointer)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;       // +0x70
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 for CIntelligentPointer

class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
#pragma pack(push, 1)
class CTCInventoryWeapons : public CTCBase { public:
    unsigned char       _pad004[0x141 - 0x4];
    CIntelligentPointer<CThing> PActiveRangedWeapon;         // +0x141
    float GetActiveProjectileWeaponDamage() const;   // Ego_r.pdb ?GetActiveProjectileWeaponDamage@CTCInventoryWeapons@@QBEMXZ
};
#pragma pack(pop)
float CTCInventoryWeapons::GetActiveProjectileWeaponDamage() const
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CDefPointer<const CWeaponDef> def;
        def.Object = 0;
        CThingBaseDef* thingDef = static_cast<CThingBaseDef*>(PActiveRangedWeapon.GetPItem()->PBaseDef.Object);
        if (thingDef)
            ++thingDef->m_count;
        thingDef->GetSubDef(&def);
        if (thingDef && --thingDef->m_count == 0)
            thingDef->Destroy();
        CWeaponDef* weaponDef = static_cast<CWeaponDef*>(def.Object);
        {
            CDefPointerCopy<CProjectileDef> projectileDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->ProjectileDefIndex);
            damage = projectileDef->Damage;
        }
        if (--weaponDef->m_count == 0)
            weaponDef->Destroy();
    }
    return damage;
}
