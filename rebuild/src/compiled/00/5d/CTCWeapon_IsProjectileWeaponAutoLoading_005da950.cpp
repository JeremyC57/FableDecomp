// CTCWeapon::IsProjectileWeaponAutoLoading() const -- whether the weapon def reloads the projectile weapon automatically, read through a counted def-pointer copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA950, length 35 bytes (+ int3 padding).
// Callees: CWeaponDef virtual slot 1 (+0x04, destroy when the count drops to zero).
// Label correction: the manifest's "IsAnalogueLoading" is one getter too late. The CTCWeapon def getters at
// 0x005DA870.. follow the Ego_r declaration order (GetProperties +0x30, GetTargetingRange +0xa8, TargetingFOV +0xac,
// IsAnalogueLoading +0x90 @ 0x005DA8F0, GetMissileLoadCarrySlot +0x94, then this one): retail CWeaponDef sits 0x0c below
// Ego_r.pdb up to the ArrowTrails map and 0x10 below after it, so +0xc4 = PDB +0xd4 ProjectileWeaponAutoReload.
// Layout: engine/CTCWeapon.h (WeaponDef +0x0c).
// Shape: the def pointer copied into a local with its add-ref under a null test, the byte parked in bl across the
// release (`mov eax,[ecx+4] ; ... ; dec eax ; mov [ecx+4],eax`, no null test), `mov al,bl` as the bool result.
// Leaf: ?IsProjectileWeaponAutoLoading@CTCWeapon@@QBE_NXZ
// Naming (NF173, 2026-11-20): q11685: defined on the shared engine/CTCWeapon.h class through FABLE_RETAIL_CTCWEAPON_AUTOLOADING (FableWin.pdb
//   0x0239B3E0 + Ego_r.pdb 0x0063A790 ?IsProjectileWeaponAutoLoading@CTCWeapon@@QBE_NXZ), no longer on the CTCWeapon_Getters stand-in.
#define FABLE_RETAIL_CTCWEAPON_AUTOLOADING
#include "engine/CTCWeapon.h"

class CDefPointeeBase { public:                                    // completes the header's forward declaration
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04): called when the count drops to zero
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};

enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };

class CWeaponDef : public CDefPointeeBase { public:                       // retail layout (0x0c below Ego_r.pdb from +0x50, 0x10 below past ArrowTrails)
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

class CThing;

bool CTCWeapon::IsProjectileWeaponAutoLoading() const
{
    CWeaponDefPointer def(WeaponDef_Object);
    return def->ProjectileWeaponAutoReload;
}