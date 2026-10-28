// CWeaponDef::operator=(const CWeaponDef& other) -- memberwise def assignment: CDefClassBase part, weapon type/class/property, strings, the flourish particle set and trail set, the two augmentation maps, projectile and targeting fields and the four sheathe animation names; returns *this
// Leaf: ??4CWeaponDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E493E, length 429 bytes; size-optimised def TU (odd address).
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment; the body is reached
// through the `jmp` thunk 0x004E4939 in slot 19 of vtable 0x0124291C = CWeaponDef (rtti-vtables.tsv).
// Callees: CDefClassBase::operator= 0x00431F10, CCharString::operator= 0x0099EFB0 (x7),
// CObjectAugmentationParticleSet::operator= 0x004E4D3B, map<EObjectAugmentationType,CWeaponTrailGraphicSet>::operator=
// 0x004E4AEB, map<EObjectAugmentationType,long>::operator= 0x004E4C16.
// Layout: Ego_r.pdb CWeaponDef (0xF4) with 16-byte maps and 52-byte particle set; retail STLport maps are 12 bytes and the
// particle set's two CArray<CDefString> are 12 each (0x2C), so: AnimationGroupName +0x34, FlourishParticles +0x50,
// FlourishSpecialWeaponTrail +0x7C, WeaponTrails +0x84, UseAnalogueLoading +0x90, ProjectileWeaponHelperNameTip +0x98,
// ArrowTrails +0xB0, ReloadSpeedRange +0xBC, NextWeaponForBoss +0xC8, NeedsLoading +0xD0, the four anim names +0xD4..+0xE0.
// Shape: the compiler-generated memberwise assignment written out in declaration order (CHeroExperienceDef 0x004F914A
// model); the trail set and float range are small PODs with implicit assignment (dword moves); the last member's address
// is formed with `add edi,0E0h` because nothing reads `other` after it.
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x28];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
class CObjectAugmentationParticleSet { public:                     // retail 0x2C bytes
    unsigned char _body[0x2c];
    CObjectAugmentationParticleSet& operator=(const CObjectAugmentationParticleSet& other);   // 0x004E4D3B
};
class CWeaponTrailGraphicSet { public:
    long Attack;
    long Knockdown;
};
struct CWeaponTrailMap {                                    // map<EObjectAugmentationType,CWeaponTrailGraphicSet> (12 bytes)
    void* head; unsigned long size; unsigned long _compare;
    CWeaponTrailMap& operator=(const CWeaponTrailMap& other);   // 0x004E4AEB
};
struct CArrowTrailMap {                                     // map<EObjectAugmentationType,long> (12 bytes)
    void* head; unsigned long size; unsigned long _compare;
    CArrowTrailMap& operator=(const CArrowTrailMap& other);     // 0x004E4C16
};
class CFloatRange { public:
    float HighValue;
    float LowValue;
};

class CWeaponDef : public CDefClassBase { public:                         // vtable 0x0124291C
    long Type;                                              // +0x28 EWeaponType
    long Class;                                             // +0x2c EWeaponClass
    unsigned long Property;                                 // +0x30
    CCharString AnimationGroupName;                         // +0x34
    long SheatheObject;                                     // +0x38
    bool SheatheObjectAlwaysVisible;                        // +0x3c
    float AnimationSpeedVal;                                // +0x40
    float Damage;                                           // +0x44
    long DamageType;                                        // +0x48 EDamageAttribute
    long Explosion;                                         // +0x4c
    CObjectAugmentationParticleSet FlourishParticles;       // +0x50
    CWeaponTrailGraphicSet FlourishSpecialWeaponTrail;      // +0x7c
    CWeaponTrailMap WeaponTrails;                           // +0x84
    bool UseAnalogueLoading;                                // +0x90
    long MissileLoadCarrySlot;                              // +0x94
    CCharString ProjectileWeaponHelperNameTip;              // +0x98
    CCharString ProjectileWeaponHelperNameBase;             // +0x9c
    long Shot;                                              // +0xa0
    long Ammo;                                              // +0xa4
    long TargetingRange;                                    // +0xa8
    float TargetingFOV;                                     // +0xac
    CArrowTrailMap ArrowTrails;                             // +0xb0
    CFloatRange ReloadSpeedRange;                           // +0xbc
    bool ProjectileWeaponAutoReload;                        // +0xc4
    bool ProjectileWeaponStartLoaded;                       // +0xc5
    bool UseExtendedProjectileTrails;                       // +0xc6
    long NextWeaponForBoss;                                 // +0xc8
    float MeleeTargetingArc;                                // +0xcc
    bool NeedsLoading;                                      // +0xd0
    CCharString UnsheatheToCombatAnim;                      // +0xd4
    CCharString UnsheatheToNormalAnim;                      // +0xd8
    CCharString SheatheFromCombatAnim;                      // +0xdc
    CCharString SheatheFromNormalAnim;                      // +0xe0
    CWeaponDef& operator=(const CWeaponDef& other);
};

CWeaponDef& CWeaponDef::operator=(const CWeaponDef& other)
{
    CDefClassBase::operator=(other);
    Type = other.Type;
    Class = other.Class;
    Property = other.Property;
    AnimationGroupName = other.AnimationGroupName;
    SheatheObject = other.SheatheObject;
    SheatheObjectAlwaysVisible = other.SheatheObjectAlwaysVisible;
    AnimationSpeedVal = other.AnimationSpeedVal;
    Damage = other.Damage;
    DamageType = other.DamageType;
    Explosion = other.Explosion;
    FlourishParticles = other.FlourishParticles;
    FlourishSpecialWeaponTrail = other.FlourishSpecialWeaponTrail;
    WeaponTrails = other.WeaponTrails;
    UseAnalogueLoading = other.UseAnalogueLoading;
    MissileLoadCarrySlot = other.MissileLoadCarrySlot;
    ProjectileWeaponHelperNameTip = other.ProjectileWeaponHelperNameTip;
    ProjectileWeaponHelperNameBase = other.ProjectileWeaponHelperNameBase;
    Shot = other.Shot;
    Ammo = other.Ammo;
    TargetingRange = other.TargetingRange;
    TargetingFOV = other.TargetingFOV;
    ArrowTrails = other.ArrowTrails;
    ReloadSpeedRange = other.ReloadSpeedRange;
    ProjectileWeaponAutoReload = other.ProjectileWeaponAutoReload;
    ProjectileWeaponStartLoaded = other.ProjectileWeaponStartLoaded;
    UseExtendedProjectileTrails = other.UseExtendedProjectileTrails;
    NextWeaponForBoss = other.NextWeaponForBoss;
    MeleeTargetingArc = other.MeleeTargetingArc;
    NeedsLoading = other.NeedsLoading;
    UnsheatheToCombatAnim = other.UnsheatheToCombatAnim;
    UnsheatheToNormalAnim = other.UnsheatheToNormalAnim;
    SheatheFromCombatAnim = other.SheatheFromCombatAnim;
    SheatheFromNormalAnim = other.SheatheFromNormalAnim;
    return *this;
}