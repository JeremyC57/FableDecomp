// CTCWeapon::IsProjectileWeaponLoaded() const -- a weapon that needs loading is loaded only when flagged so; any other weapon always counts as loaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA980, length 48 bytes.
// Callees: CWeaponDef virtual slot 1 (+0x04, destroy when the count drops to zero).
// Layout: engine/CTCWeapon.h (WeaponDef +0x0c, ProjectileWeaponLoaded +0x10); CWeaponDef NeedsLoading is the retail byte
// +0xd0 (Ego_r.pdb +0xe0; the retail def sits 0x10 lower in this region, as the sheathe-animation strings at +0xd4..).
// Shape: the def flag read into a named local inside its own scope (the counted copy released before the test),
// then `if (needs) return ProjectileWeaponLoaded; return true;` -- the `mov al,1` tail last.
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
    bool IsProjectileWeaponLoaded() const;
};

bool CTCWeapon_Getters::IsProjectileWeaponLoaded() const
{
    bool needsLoading;
    {
        CWeaponDefPointer def(WeaponDef_Object);
        needsLoading = def->NeedsLoading;
    }
    if (needsLoading)
        return ProjectileWeaponLoaded;
    return true;
}