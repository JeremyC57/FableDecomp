// CWeaponDef::~CWeaponDef() -- destroy the weapon definition's members in reverse order (four sheathe/unsheathe animation names, the arrow-trail map, the two projectile helper names, the weapon-trail map, the flourish particle set, the animation group name), then the inlined CDefPointeeBase destructor stores its vtable and tail-calls the root destructor
// Leaf: ??1CWeaponDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3D4A, length 121 bytes.
// Label correction: catalogued as CHeroExperienceDef::~CHeroExperienceDef, but its only caller is the deleting
// destructor 0x004E3D2E, slot 0 of CWeaponDef's vtable 0x0124291C (rtti-vtables.tsv), and the member offsets
// fit Ego_r.pdb CWeaponDef with retail 12-byte maps/vectors.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x7), the ArrowTrails map destructor @ 0x004DD3AD, the
// WeaponTrails map destructor @ 0x004DD36C, CObjectAugmentationParticleSet::~ @ 0x004E25F1, the root
// (manifest "~CMemoryBlock") destructor @ 0x009FC550 tail-called. Vtable 0x01230BA0 = CDefPointeeBase.
// Shape: as the landed 0x0044FA6D -- the derived class is `__declspec(novtable)` so only the inlined
// CDefPointeeBase destructor's vtable store appears; the members die in reverse declaration order.
// Layout (retail): AnimationGroupName +0x34, FlourishParticles +0x50 (0x2c bytes), FlourishSpecialWeaponTrail
// +0x7c, WeaponTrails +0x84, UseAnalogueLoading +0x90, MissileLoadCarrySlot +0x94, ProjectileWeaponHelperNameTip
// +0x98, ProjectileWeaponHelperNameBase +0x9c, Shot/Ammo/TargetingRange/TargetingFOV +0xa0..+0xac, ArrowTrails +0xb0,
// ReloadSpeedRange +0xbc, flags +0xc4, NextWeaponForBoss +0xc8, MeleeTargetingArc +0xcc, NeedsLoading +0xd0,
// UnsheatheToCombatAnim +0xd4, UnsheatheToNormalAnim +0xd8, SheatheFromCombatAnim +0xdc, SheatheFromNormalAnim +0xe0.
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550
};
class CDefPointeeBase : public CBase_009fc550 { public:          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};
class CObjectAugmentationParticleSet { public:            // 0x2c bytes in retail
    unsigned char _body[0x2c];
    ~CObjectAugmentationParticleSet();             // 0x004E25F1
};
class CWeaponTrailGraphicSet { public: long m_a, m_b; };
struct CWeaponTrailMap {                           // map<EObjectAugmentationType, CWeaponTrailGraphicSet> (retail 12 bytes)
    unsigned char _body[0x0c];
    ~CWeaponTrailMap();                            // 0x004DD36C
};
struct CArrowTrailMap {                            // map<EObjectAugmentationType, long> (retail 12 bytes)
    unsigned char _body[0x0c];
    ~CArrowTrailMap();                             // 0x004DD3AD
};

struct __declspec(novtable) CWeaponDef : CDefPointeeBase {
    unsigned char                  _pad04[0x24];   // +0x04 (CDefClassBase part)
    long                           Type;           // +0x28
    long                           Class;          // +0x2c
    unsigned long                  Property;       // +0x30
    CCharString                    AnimationGroupName;   // +0x34
    long                           SheatheObject;  // +0x38
    bool                           SheatheObjectAlwaysVisible;   // +0x3c
    float                          AnimationSpeedVal;   // +0x40
    float                          Damage;         // +0x44
    long                           DamageType;     // +0x48
    long                           Explosion;      // +0x4c
    CObjectAugmentationParticleSet FlourishParticles;   // +0x50
    CWeaponTrailGraphicSet         FlourishSpecialWeaponTrail;   // +0x7c
    CWeaponTrailMap                WeaponTrails;   // +0x84
    bool                           UseAnalogueLoading;   // +0x90
    long                           MissileLoadCarrySlot;   // +0x94
    CCharString                    ProjectileWeaponHelperNameTip;    // +0x98
    CCharString                    ProjectileWeaponHelperNameBase;   // +0x9c
    long                           Shot;           // +0xa0
    long                           Ammo;           // +0xa4
    long                           TargetingRange; // +0xa8
    float                          TargetingFOV;   // +0xac
    CArrowTrailMap                 ArrowTrails;    // +0xb0
    float                          ReloadSpeedRange[2];   // +0xbc
    bool                           ProjectileWeaponAutoReload;   // +0xc4
    bool                           ProjectileWeaponStartLoaded;  // +0xc5
    bool                           UseExtendedProjectileTrails;  // +0xc6
    long                           NextWeaponForBoss;   // +0xc8
    float                          MeleeTargetingArc;   // +0xcc
    bool                           NeedsLoading;   // +0xd0
    CCharString                    UnsheatheToCombatAnim;   // +0xd4
    CCharString                    UnsheatheToNormalAnim;   // +0xd8
    CCharString                    SheatheFromCombatAnim;   // +0xdc
    CCharString                    SheatheFromNormalAnim;   // +0xe0
    virtual void v00();
    ~CWeaponDef();
};

CWeaponDef::~CWeaponDef()
{
}