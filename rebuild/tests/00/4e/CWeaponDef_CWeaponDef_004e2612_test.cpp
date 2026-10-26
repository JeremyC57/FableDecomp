// Behaviour test for 0x004E2612 CWeaponDef::CWeaponDef(): base and member constructors run, particle set,
// trail set and reload range defaulted, plain scalars left alone.
#include <cstdio>
#include <cstring>
#include <new>
static int g_strings = 0, g_maps = 0, g_base = 0;
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
class CSubDefClassBase { public: virtual void v00() {} unsigned char _base[0x24]; CSubDefClassBase() { ++g_base; } };
template <class T> class CArray { public: T* Data; long Size; long Capacity; CArray() : Data(0), Size(0), Capacity(0) {} };
class CDefString { public: long Index; };
class CObjectAugmentationParticleSet { public:
    float PeriodSecs; long ParticleEffect, ParticleEffectToBlendTo, AttackParticleEffect, AttackParticleEffectToBlendTo;
    CArray<CDefString> StartPoint, EndPoint;
    CObjectAugmentationParticleSet() : PeriodSecs(0.0f) {}
};
class CWeaponTrailGraphicSet { public: long Attack, Knockdown; CWeaponTrailGraphicSet() : Attack(-1), Knockdown(-1) {} };
struct CWeaponTrailMap { void* h[3]; CWeaponTrailMap() { ++g_maps; } };
struct CArrowTrailMap { void* h[3]; CArrowTrailMap() { ++g_maps; } };
class CFloatRange { public: float HighValue, LowValue; CFloatRange() : HighValue(0.0f), LowValue(0.0f) {} };
class CWeaponDef : public CSubDefClassBase { public:
    virtual void v00() {}
    long Type, Class; unsigned long Property; CCharString AnimationGroupName; long SheatheObject;
    bool SheatheObjectAlwaysVisible; float AnimationSpeedVal, Damage; long DamageType, Explosion;
    CObjectAugmentationParticleSet FlourishParticles; CWeaponTrailGraphicSet FlourishSpecialWeaponTrail;
    CWeaponTrailMap WeaponTrails; bool UseAnalogueLoading; long MissileLoadCarrySlot;
    CCharString ProjectileWeaponHelperNameTip, ProjectileWeaponHelperNameBase;
    long Shot, Ammo, TargetingRange; float TargetingFOV; CArrowTrailMap ArrowTrails; CFloatRange ReloadSpeedRange;
    bool ProjectileWeaponAutoReload, ProjectileWeaponStartLoaded, UseExtendedProjectileTrails;
    long NextWeaponForBoss; float MeleeTargetingArc; bool NeedsLoading;
    CCharString UnsheatheToCombatAnim, UnsheatheToNormalAnim, SheatheFromCombatAnim, SheatheFromNormalAnim;
    CWeaponDef();
};
CWeaponDef::CWeaponDef() {}
int main()
{
    static unsigned char buf[sizeof(CWeaponDef)];
    std::memset(buf, 0x5a, sizeof(buf));
    CWeaponDef* d = new (buf) CWeaponDef;
    long pat; std::memset(&pat, 0x5a, sizeof(pat));
    bool ok = g_base == 1 && g_strings == 7 && g_maps == 2
        && d->FlourishParticles.PeriodSecs == 0.0f && d->FlourishParticles.StartPoint.Data == 0
        && d->FlourishParticles.EndPoint.Capacity == 0 && d->FlourishParticles.StartPoint.Size == 0
        && d->FlourishSpecialWeaponTrail.Attack == -1 && d->FlourishSpecialWeaponTrail.Knockdown == -1
        && d->ReloadSpeedRange.HighValue == 0.0f && d->ReloadSpeedRange.LowValue == 0.0f
        && d->Shot == pat && d->FlourishParticles.ParticleEffect == pat;
    if (!ok) { std::printf("004e2612_TEST FAIL\n"); return 1; }
    std::printf("004e2612_TEST PASS\n");
    return 0;
}