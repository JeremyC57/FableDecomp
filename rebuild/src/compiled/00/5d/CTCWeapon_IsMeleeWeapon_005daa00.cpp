// CTCWeapon::IsMeleeWeapon() const -- the weapon def's class is one of the two melee classes (1 or 2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DAA00, length 49 bytes (+ int3 padding).
// Callees: CWeaponDef virtual slot 1 (+0x04, destroy when the count drops to zero).
// Layout: engine/CTCWeapon.h (WeaponDef +0x0c); CWeaponDef Class +0x2c (Ego_r.pdb, same offset in retail).
// Shape: the class read through a counted def-pointer copy (released before the compares), then two compares with
// separate `xor eax,eax` / `mov eax,1` tails.
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
    bool IsMeleeWeapon() const;
};

bool CTCWeapon_Getters::IsMeleeWeapon() const
{
    EWeaponClass weaponClass;
    {
        CWeaponDefPointer def(WeaponDef_Object);
        weaponClass = def->Class;
    }
    return weaponClass == WEAPON_CLASS_1 || weaponClass == WEAPON_CLASS_2;
}