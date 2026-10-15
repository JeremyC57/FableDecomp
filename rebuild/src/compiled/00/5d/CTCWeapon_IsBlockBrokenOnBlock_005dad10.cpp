// CTCWeapon::IsBlockBrokenOnBlock(const CThing&, const CThing*, const CThing*) -- a block is broken when the attacker can use this weapon, the weapon is of class 3 and a second (third-party) thing is given
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DAD10, length 69 bytes (+ int3 padding).
// Callees: CTCWeapon::IsUsableBy(const CThing&) @ 0x005DAB10, CWeaponDef virtual slot 1 (+0x04, destroy at count zero).
// Layout: engine/CTCWeapon.h (WeaponDef +0x0c); CWeaponDef Class +0x2c.
// Shape: IsUsableBy gets the first argument re-pushed; the class read through a counted def-pointer copy; the third
// argument's null test last; shared `mov al,1` / `xor al,al` tails with `ret 0Ch`.
#include "engine/CTCWeapon.h"

struct CDefPointeeBase {                                    // completes the header's forward declaration
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04): called when the count drops to zero
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};

enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };

struct CWeaponDef : CDefPointeeBase {                       // retail layout (0x0c below Ego_r.pdb from +0x50, 0x10 below past ArrowTrails)
    unsigned char _pad_0x08[0x20];
    long          Type;                                     // +0x28 EWeaponType
    EWeaponClass  Class;                                    // +0x2c
    unsigned char _pad_0x30[0x10];
    float         AnimationSpeedVal;                        // +0x40
    unsigned char _pad_0x44[0x80];
    bool          ProjectileWeaponAutoReload;               // +0xc4 (retail; PDB +0xd4)
    unsigned char _pad_0xc5[0xb];
    bool          NeedsLoading;                             // +0xd0 (retail; PDB +0xe0)
};

struct CWeaponDefPointer {                                  // CDefPointer<const CWeaponDef>
    CWeaponDef* Object;
    CWeaponDefPointer(CDefPointeeBase* object) : Object(static_cast<CWeaponDef*>(object))
    {
        if (Object)
            Object->AddRef();
    }
    ~CWeaponDefPointer() { Object->Release(); }
    CWeaponDef* operator->() const { return Object; }
};

struct CThing;

struct CTCWeapon_Getters : CTCWeapon {
    bool IsUsableBy(const CThing& thing) const;                   // 0x005DAB10
    bool IsBlockBrokenOnBlock(const CThing& attacker, const CThing* blocker, const CThing* other) const;
};

bool CTCWeapon_Getters::IsBlockBrokenOnBlock(const CThing& attacker, const CThing* blocker, const CThing* other) const
{
    if (IsUsableBy(attacker)) {
        EWeaponClass weaponClass;
        {
            CWeaponDefPointer def(WeaponDef_Object);
            weaponClass = def->Class;
        }
        if (weaponClass == WEAPON_CLASS_3 && other)
            return true;
    }
    return false;
}