// CWeaponDef::Transfer(CPersistContext&) -- serialise every weapon-definition field in retail order
// Leaf: ?Transfer@CWeaponDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E8CE9, 511 bytes, `ret 4`; odd address + `push ebp ; mov ebp,esp` frame
//   = size-optimised TU (#pragma optimize("s", on)).
// Callees (CPersistContext typed transfers; manifest labels are propagated "UpdateShadowScene" junk):
//   EWeaponType 0x004E8EE8, EWeaponClass 0x004E8F8C, unsigned long 0x00431020, CCharString 0x00431143,
//   long 0x00431102, bool 0x0043314A, float 0x00431061, EDamageAttribute 0x00459042,
//   CObjectAugmentationParticleSet 0x004E9550, CWeaponTrailGraphicSet 0x004E8FCD,
//   map<EObjectAugmentationType,CWeaponTrailGraphicSet> 0x004E90DA, map<EObjectAugmentationType,long> 0x004E9321,
//   CFloatRange 0x00458F2B; CCharString(const char*, long) 0x0099EBF0 and ~CCharString 0x0099EAE0.
// Shape: three CCharString members carry a default value ("SWORD", "weapon_pos_a", "weapon_pos_b"); the default is
//   built as a CCharString temporary (in the dead `context` argument slot [ebp+8], since the context already sits
//   in edi) and destroyed after the transfer without being passed -- an inline transfer-with-default helper whose
//   default argument is unused in the release build.
// Layout: Ego_r.pdb CWeaponDef names/types; retail offsets equal the PDB up to +0x50, then CObjectAugmentationParticleSet
//   is 0x2C bytes in retail (PDB 0x34) and the STLport maps are 12 bytes (PDB 16): FlourishSpecialWeaponTrail +0x7C,
//   WeaponTrails +0x84, UseAnalogueLoading +0x90 ... SheatheFromNormalAnim +0xE0 (read off the lea offsets).
//   Local model: no engine/ header exists for CWeaponDef.
#pragma optimize("s", on)

enum EWeaponType { WEAPON_TYPE_NONE };
enum EWeaponClass { WEAPON_CLASS_NONE };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE };

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CObjectAugmentationParticleSet { public: unsigned char _body[0x2c]; };
class CWeaponTrailGraphicSet { public: long Graphic; long Trail; };
struct CWeaponTrailMap { void* _M_header[3]; };           // map<EObjectAugmentationType,CWeaponTrailGraphicSet>
struct CArrowTrailMap { void* _M_header[3]; };            // map<EObjectAugmentationType,long>
class CFloatRange { public: float Min; float Max; };

class CPersistContext { public:
    void Transfer(EWeaponType& value);                  // 0x004E8EE8
    void Transfer(EWeaponClass& value);                 // 0x004E8F8C
    template <class T> void Transfer(T& value);                // 0x00431020
    void Transfer(EDamageAttribute& value);             // 0x00459042
    void Transfer(CObjectAugmentationParticleSet& value); // 0x004E9550
    void Transfer(CWeaponTrailGraphicSet& value);       // 0x004E8FCD
    void Transfer(CWeaponTrailMap& value);              // 0x004E90DA
    void Transfer(CArrowTrailMap& value);               // 0x004E9321
    // the default value is only checked in debug builds
    void Transfer(CCharString& value, const CCharString& defaultValue) { Transfer(value); }
};

class CWeaponDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                          // +0x04..+0x27 CDefClassBase
    EWeaponType Type;                                   // +0x28
    EWeaponClass Class;                                 // +0x2c
    unsigned long Property;                             // +0x30
    CCharString AnimationGroupName;                     // +0x34
    long SheatheObject;                                 // +0x38
    bool SheatheObjectAlwaysVisible;                    // +0x3c
    float AnimationSpeedVal;                            // +0x40
    float Damage;                                       // +0x44
    EDamageAttribute DamageType;                        // +0x48
    long Explosion;                                     // +0x4c
    CObjectAugmentationParticleSet FlourishParticles;   // +0x50
    CWeaponTrailGraphicSet FlourishSpecialWeaponTrail;  // +0x7c
    CWeaponTrailMap WeaponTrails;                       // +0x84
    bool UseAnalogueLoading;                            // +0x90
    long MissileLoadCarrySlot;                          // +0x94
    CCharString ProjectileWeaponHelperNameTip;          // +0x98
    CCharString ProjectileWeaponHelperNameBase;         // +0x9c
    long Shot;                                          // +0xa0
    long Ammo;                                          // +0xa4
    long TargetingRange;                                // +0xa8
    float TargetingFOV;                                 // +0xac
    CArrowTrailMap ArrowTrails;                         // +0xb0
    CFloatRange ReloadSpeedRange;                       // +0xbc
    bool ProjectileWeaponAutoReload;                    // +0xc4
    bool ProjectileWeaponStartLoaded;                   // +0xc5
    bool UseExtendedProjectileTrails;                   // +0xc6
    long NextWeaponForBoss;                             // +0xc8
    float MeleeTargetingArc;                            // +0xcc
    bool NeedsLoading;                                  // +0xd0
    CCharString UnsheatheToCombatAnim;                  // +0xd4
    CCharString UnsheatheToNormalAnim;                  // +0xd8
    CCharString SheatheFromCombatAnim;                  // +0xdc
    CCharString SheatheFromNormalAnim;                  // +0xe0
};

void CWeaponDef::Transfer(CPersistContext& context)
{
    context.Transfer(Type);
    context.Transfer(Class);
    context.Transfer(Property);
    context.Transfer(AnimationGroupName, "SWORD");
    context.Transfer(SheatheObject);
    context.Transfer(SheatheObjectAlwaysVisible);
    context.Transfer(AnimationSpeedVal);
    context.Transfer(Damage);
    context.Transfer(DamageType);
    context.Transfer(Explosion);
    context.Transfer(FlourishParticles);
    context.Transfer(FlourishSpecialWeaponTrail);
    context.Transfer(WeaponTrails);
    context.Transfer(UseAnalogueLoading);
    context.Transfer(MissileLoadCarrySlot);
    context.Transfer(ProjectileWeaponHelperNameTip, "weapon_pos_a");
    context.Transfer(ProjectileWeaponHelperNameBase, "weapon_pos_b");
    context.Transfer(Shot);
    context.Transfer(Ammo);
    context.Transfer(TargetingRange);
    context.Transfer(TargetingFOV);
    context.Transfer(ArrowTrails);
    context.Transfer(ReloadSpeedRange);
    context.Transfer(ProjectileWeaponAutoReload);
    context.Transfer(ProjectileWeaponStartLoaded);
    context.Transfer(UseExtendedProjectileTrails);
    context.Transfer(NextWeaponForBoss);
    context.Transfer(MeleeTargetingArc);
    context.Transfer(NeedsLoading);
    context.Transfer(UnsheatheToCombatAnim);
    context.Transfer(UnsheatheToNormalAnim);
    context.Transfer(SheatheFromCombatAnim);
    context.Transfer(SheatheFromNormalAnim);
}