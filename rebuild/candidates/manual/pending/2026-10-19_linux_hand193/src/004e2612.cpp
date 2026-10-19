// CWeaponDef::CWeaponDef() -- weapon definition default constructor: base sub-def, then every member with a constructor
// Leaf: ??0CWeaponDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2612, length 164 bytes, `ret`.
// The manifest label "CEngineVSConstantLayoutLights::CEngineVSConstantLayoutLights" is propagated: the body stores
// vtable 0x0124291C, which rtti-vtables.tsv names CWeaponDef (slot 0 0x004E3D2E is its deleting dtor, whose
// ~CWeaponDef 0x004E3D4A destroys exactly the members built here, in reverse).
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (stores vtable 0x01231D54 = CSubDefClassBase; landed rows
// call it CShoveHaPennyRegionDef, a propagated label), CCharString::CCharString() @ 0x0099E4B0,
// the two STLport map constructors @ 0x004DF8CB (WeaponTrails) and 0x004DF8E9 (ArrowTrails).
// Retail layout (0xE4 bytes; the PDB CWeaponDef is 0xF4): base 0..0x33, Type +0x28, Class +0x2C, Property +0x30,
// AnimationGroupName +0x34, SheatheObject +0x38, SheatheObjectAlwaysVisible +0x3C, AnimationSpeedVal +0x40,
// Damage +0x44, DamageType +0x48, Explosion +0x4C, FlourishParticles +0x50 (0x2C: PeriodSecs, four effect longs,
// two 12-byte CArray<CDefString> at +0x14/+0x20), FlourishSpecialWeaponTrail +0x7C (Attack, Knockdown = -1),
// WeaponTrails map +0x84 (12-byte STLport map), UseAnalogueLoading +0x90, MissileLoadCarrySlot +0x94,
// ProjectileWeaponHelperNameTip/Base +0x98/+0x9C, Shot +0xA0, Ammo +0xA4, TargetingRange +0xA8, TargetingFOV +0xAC,
// ArrowTrails map +0xB0, ReloadSpeedRange +0xBC (CFloatRange, both zero), the three bools +0xC4..,
// NextWeaponForBoss +0xC8, MeleeTargetingArc +0xCC, NeedsLoading +0xD0, the four sheathe/unsheathe anims +0xD4..+0xE0.
// Shape: size-optimised TU (`fldz ; fstp` float zeros, `or [m],-1`, `lea` address formation): every member object
// has an inline constructor; the plain scalars are left uninitialised.
#pragma optimize("s", on)

struct CCharString {
    void* PStringData;
    CCharString();                    // 0x0099E4B0
};

struct CSubDefClassBase {
    virtual void v00();
    unsigned char _base[0x24];        // +0x04 sub-def base members (0x28 bytes in all)
    CSubDefClassBase();               // 0x0044C0C0
};

template <class T> struct CArray {
    T* Data;
    long Size;
    long Capacity;
    CArray() : Data(0), Size(0), Capacity(0) {}
};

struct CDefString { long Index; };

struct CObjectAugmentationParticleSet {
    float PeriodSecs;
    long ParticleEffect;
    long ParticleEffectToBlendTo;
    long AttackParticleEffect;
    long AttackParticleEffectToBlendTo;
    CArray<CDefString> StartPoint;
    CArray<CDefString> EndPoint;
    CObjectAugmentationParticleSet() : PeriodSecs(0.0f) {}
};

struct CWeaponTrailGraphicSet {
    long Attack;
    long Knockdown;
    CWeaponTrailGraphicSet() : Attack(-1), Knockdown(-1) {}
};

struct CWeaponTrailMap {              // map<EObjectAugmentationType, CWeaponTrailGraphicSet>
    void* _M_header[3];
    CWeaponTrailMap();                // 0x004DF8CB
};

struct CArrowTrailMap {               // map<EObjectAugmentationType, long>
    void* _M_header[3];
    CArrowTrailMap();                 // 0x004DF8E9
};

struct CFloatRange {
    float HighValue;
    float LowValue;
    CFloatRange() : HighValue(0.0f), LowValue(0.0f) {}
};

struct CWeaponDef : CSubDefClassBase {
    virtual void v00();
    long Type;
    long Class;
    unsigned long Property;
    CCharString AnimationGroupName;
    long SheatheObject;
    bool SheatheObjectAlwaysVisible;
    float AnimationSpeedVal;
    float Damage;
    long DamageType;
    long Explosion;
    CObjectAugmentationParticleSet FlourishParticles;
    CWeaponTrailGraphicSet FlourishSpecialWeaponTrail;
    CWeaponTrailMap WeaponTrails;
    bool UseAnalogueLoading;
    long MissileLoadCarrySlot;
    CCharString ProjectileWeaponHelperNameTip;
    CCharString ProjectileWeaponHelperNameBase;
    long Shot;
    long Ammo;
    long TargetingRange;
    float TargetingFOV;
    CArrowTrailMap ArrowTrails;
    CFloatRange ReloadSpeedRange;
    bool ProjectileWeaponAutoReload;
    bool ProjectileWeaponStartLoaded;
    bool UseExtendedProjectileTrails;
    long NextWeaponForBoss;
    float MeleeTargetingArc;
    bool NeedsLoading;
    CCharString UnsheatheToCombatAnim;
    CCharString UnsheatheToNormalAnim;
    CCharString SheatheFromCombatAnim;
    CCharString SheatheFromNormalAnim;
    CWeaponDef();
};

CWeaponDef::CWeaponDef()
{
}
