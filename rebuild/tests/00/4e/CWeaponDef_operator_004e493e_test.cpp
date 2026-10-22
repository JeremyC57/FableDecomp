// Behaviour test for CWeaponDef::operator= (0x004E493E): the reconstructed body with recording stub callees.
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

struct CDefClassBase {
    unsigned char _body[0x28];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CCharString {
    void* m_pStringData;
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
struct CObjectAugmentationParticleSet {                     // retail 0x2C bytes
    unsigned char _body[0x2c];
    CObjectAugmentationParticleSet& operator=(const CObjectAugmentationParticleSet& other);   // 0x004E4D3B
};
struct CWeaponTrailGraphicSet {
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
struct CFloatRange {
    float HighValue;
    float LowValue;
};

struct CWeaponDef : CDefClassBase {                         // vtable 0x0124291C
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

static int g_base = 0, g_str = 0, g_particles = 0, g_trails = 0, g_arrows = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& o) { for (int i = 0; i < 0x28; ++i) _body[i] = o._body[i]; ++g_base; return *this; }
CCharString& CCharString::operator=(const CCharString& o) { m_pStringData = o.m_pStringData; ++g_str; return *this; }
CObjectAugmentationParticleSet& CObjectAugmentationParticleSet::operator=(const CObjectAugmentationParticleSet& o) { _body[0] = o._body[0]; ++g_particles; return *this; }
CWeaponTrailMap& CWeaponTrailMap::operator=(const CWeaponTrailMap& o) { size = o.size; ++g_trails; return *this; }
CArrowTrailMap& CArrowTrailMap::operator=(const CArrowTrailMap& o) { size = o.size; ++g_arrows; return *this; }
#include <cstdio>
#include <cstring>
static CWeaponDef a, b;
int main()
{
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a._body[5] = 7; a.Type = 3; a.Class = 4; a.Property = 0x55; a.AnimationGroupName.m_pStringData = &a;
    a.SheatheObject = 9; a.SheatheObjectAlwaysVisible = true; a.AnimationSpeedVal = 1.5f; a.Damage = 2.5f;
    a.DamageType = 6; a.Explosion = 11; a.FlourishParticles._body[0] = 0x21;
    a.FlourishSpecialWeaponTrail.Attack = 12; a.FlourishSpecialWeaponTrail.Knockdown = 13; a.WeaponTrails.size = 14;
    a.UseAnalogueLoading = true; a.MissileLoadCarrySlot = 15; a.ProjectileWeaponHelperNameTip.m_pStringData = &b;
    a.Shot = 16; a.Ammo = 17; a.TargetingRange = 18; a.TargetingFOV = 0.25f; a.ArrowTrails.size = 19;
    a.ReloadSpeedRange.HighValue = 3.0f; a.ReloadSpeedRange.LowValue = 1.0f; a.ProjectileWeaponStartLoaded = true;
    a.NextWeaponForBoss = 20; a.MeleeTargetingArc = 0.75f; a.NeedsLoading = true; a.SheatheFromNormalAnim.m_pStringData = &a;
    CWeaponDef* r = &(b = a);
    bool ok = r == &b && g_base == 1 && g_str == 7 && g_particles == 1 && g_trails == 1 && g_arrows == 1
        && b._body[5] == 7 && b.Type == 3 && b.Class == 4 && b.Property == 0x55 && b.AnimationGroupName.m_pStringData == &a
        && b.SheatheObject == 9 && b.SheatheObjectAlwaysVisible && b.AnimationSpeedVal == 1.5f && b.Damage == 2.5f
        && b.DamageType == 6 && b.Explosion == 11 && b.FlourishParticles._body[0] == 0x21
        && b.FlourishSpecialWeaponTrail.Attack == 12 && b.FlourishSpecialWeaponTrail.Knockdown == 13 && b.WeaponTrails.size == 14
        && b.UseAnalogueLoading && b.MissileLoadCarrySlot == 15 && b.ProjectileWeaponHelperNameTip.m_pStringData == &b
        && b.Shot == 16 && b.Ammo == 17 && b.TargetingRange == 18 && b.TargetingFOV == 0.25f && b.ArrowTrails.size == 19
        && b.ReloadSpeedRange.HighValue == 3.0f && b.ReloadSpeedRange.LowValue == 1.0f && !b.ProjectileWeaponAutoReload
        && b.ProjectileWeaponStartLoaded && b.NextWeaponForBoss == 20 && b.MeleeTargetingArc == 0.75f && b.NeedsLoading
        && b.SheatheFromNormalAnim.m_pStringData == &a;
    if (!ok) { std::printf("004e493e_TEST FAIL\n"); return 1; }
    std::printf("004e493e_TEST PASS\n");
    return 0;
}