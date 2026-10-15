// CTCWeaponTrail::HasWeaponTrailPoints() const -- a projectile owner has points when exactly one projectile trail exists, any other owner when it has at least one weapon trail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E4A0, length 33 bytes (+ int3 padding).
// Ego_r.pdb: ?HasWeaponTrailPoints@CTCWeaponTrail@@QBE_NXZ.
// Layout (Ego_r.pdb CTCWeaponTrail): CTCBase 0xc (ParentThing +4), NoTrails +0x0c, NoProjectileTrails +0x10. The owner
// thing's property word at CThing +0x2c carries the projectile bit 0x00800000.
// Shape: `xor eax,eax` is hoisted above the branch and each arm sets al (`sete` / `setg`) -- the two compares are
// returned directly from the if/else arms.
struct CThing {
    unsigned char _pad_00[0x2c];
    unsigned long m_properties;                             // +0x2c (0x00800000 = projectile)
};

enum { THING_PROPERTY_PROJECTILE = 0x00800000 };

struct CTCWeaponTrail {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    long          NoTrails;                                 // +0x0c
    long          NoProjectileTrails;                       // +0x10
    bool HasWeaponTrailPoints() const;
};

bool CTCWeaponTrail::HasWeaponTrailPoints() const
{
    if (ParentThing->m_properties & THING_PROPERTY_PROJECTILE)
        return NoProjectileTrails == 1;
    return NoTrails > 0;
}
