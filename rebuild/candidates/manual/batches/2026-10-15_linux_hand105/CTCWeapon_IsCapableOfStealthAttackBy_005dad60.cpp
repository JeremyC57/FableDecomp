// CTCWeapon::IsCapableOfStealthAttackBy(const CThing&) -- only a weapon of def type 0 can stealth-attack, and then only when the thing can use it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DAD60, length 58 bytes.
// Callees: CTCWeapon::IsUsableBy(const CThing&) @ 0x005DAB10, CWeaponDef virtual slot 1 (+0x04, destroy at count zero).
// Layout: engine/CTCWeapon.h (WeaponDef +0x0c); CWeaponDef Type +0x28 (Ego_r.pdb, same offset in retail).
// Shape: the type read through a counted def-pointer copy into edi; `if (type == 0) return IsUsableBy(thing); return
// false;` (the early `return false` polarity puts the false tail first) -- a call, not a tail jump (esi is restored).
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
    bool IsCapableOfStealthAttackBy(const CThing& thing) const;
};

bool CTCWeapon_Getters::IsCapableOfStealthAttackBy(const CThing& thing) const
{
    long weaponType;
    {
        CWeaponDefPointer def(WeaponDef_Object);
        weaponType = def->Type;
    }
    if (weaponType == 0)
        return IsUsableBy(thing);
    return false;
}
