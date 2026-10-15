// CTCWeapon::GetAnimationSpeedValue() const -- the weapon def's animation speed, read through a counted def-pointer copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA850, length 32 bytes.
// Callees: CWeaponDef virtual slot 1 (+0x04, destroy when the count drops to zero).
// Layout: engine/CTCWeapon.h (WeaponDef +0x0c); CWeaponDef AnimationSpeedVal +0x40 (Ego_r.pdb, same offset in retail).
// Shape: the def pointer copied into a local (`test ecx,ecx ; je ; inc [ecx+4]`), the float moved through eax into
// the `push ecx` frame slot, the release with no null test, `fld` of the slot last.
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

struct CTCWeapon_Getters : CTCWeapon {
    float GetAnimationSpeedValue() const;
};

float CTCWeapon_Getters::GetAnimationSpeedValue() const
{
    CWeaponDefPointer def(WeaponDef_Object);
    return def->AnimationSpeedVal;
}
