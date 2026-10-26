// Behaviour test for retail 0x004e8ce9: candidate source embedded verbatim, then spies and a main.
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
    void Transfer(CCharString& value, const CCharString& defaultValue);
};
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value);
template <> void CPersistContext::Transfer<CCharString>(CCharString& value);
inline void CPersistContext::Transfer(CCharString& value, const CCharString& defaultValue) { Transfer(value); }
template <> void CPersistContext::Transfer<long>(long& value);
template <> void CPersistContext::Transfer<bool>(bool& value);
template <> void CPersistContext::Transfer<float>(float& value);
template <> void CPersistContext::Transfer<CFloatRange>(CFloatRange& value);

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

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_n; static void* g_ptr[64]; static char g_kind[64];
static int g_ctor, g_dtor; static const char* g_lastDefault;
static void rec(void* p, char k) { g_ptr[g_n] = p; g_kind[g_n++] = k; }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_ctor; g_lastDefault = text; if (length != -1) g_ctor += 100; }
CCharString::~CCharString() { ++g_dtor; }
void CPersistContext::Transfer(EWeaponType& v) { rec(&v, 'T'); }
void CPersistContext::Transfer(EWeaponClass& v) { rec(&v, 'C'); }
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& v) { rec(&v, 'u'); }
template <> void CPersistContext::Transfer<CCharString>(CCharString& v) { rec(&v, 's'); }
template <> void CPersistContext::Transfer<long>(long& v) { rec(&v, 'l'); }
template <> void CPersistContext::Transfer<bool>(bool& v) { rec(&v, 'b'); v = !v; }
template <> void CPersistContext::Transfer<float>(float& v) { rec(&v, 'f'); }
void CPersistContext::Transfer(EDamageAttribute& v) { rec(&v, 'D'); }
void CPersistContext::Transfer(CObjectAugmentationParticleSet& v) { rec(&v, 'P'); }
void CPersistContext::Transfer(CWeaponTrailGraphicSet& v) { rec(&v, 'G'); }
void CPersistContext::Transfer(CWeaponTrailMap& v) { rec(&v, 'M'); }
void CPersistContext::Transfer(CArrowTrailMap& v) { rec(&v, 'A'); }
template <> void CPersistContext::Transfer<CFloatRange>(CFloatRange& v) { rec(&v, 'R'); }
static unsigned char g_store[sizeof(CWeaponDef) + 16];
int main()
{
    CWeaponDef* d = (CWeaponDef*)g_store;              /* storage only: members are reached by address */
    CPersistContext c;
    d->CWeaponDef::Transfer(c);
    void* expect[33] = { &d->Type, &d->Class, &d->Property, &d->AnimationGroupName, &d->SheatheObject,
        &d->SheatheObjectAlwaysVisible, &d->AnimationSpeedVal, &d->Damage, &d->DamageType, &d->Explosion,
        &d->FlourishParticles, &d->FlourishSpecialWeaponTrail, &d->WeaponTrails, &d->UseAnalogueLoading,
        &d->MissileLoadCarrySlot, &d->ProjectileWeaponHelperNameTip, &d->ProjectileWeaponHelperNameBase, &d->Shot,
        &d->Ammo, &d->TargetingRange, &d->TargetingFOV, &d->ArrowTrails, &d->ReloadSpeedRange,
        &d->ProjectileWeaponAutoReload, &d->ProjectileWeaponStartLoaded, &d->UseExtendedProjectileTrails,
        &d->NextWeaponForBoss, &d->MeleeTargetingArc, &d->NeedsLoading, &d->UnsheatheToCombatAnim,
        &d->UnsheatheToNormalAnim, &d->SheatheFromCombatAnim, &d->SheatheFromNormalAnim };
    const char* kinds = "TCuslbffDlPGMblsslllfARbbblfbssss";
    int ok = g_n == 33 && strlen(kinds) == 33;
    for (int i = 0; ok && i < 33; ++i) ok = g_ptr[i] == expect[i] && g_kind[i] == kinds[i];
    ok = ok && g_ctor == 3 && g_dtor == 3 && strcmp(g_lastDefault, "weapon_pos_b") == 0;
    ok = ok && d->SheatheObjectAlwaysVisible && d->NeedsLoading && d->UseAnalogueLoading;
    if (!ok) { printf("004e8ce9_TEST FAIL\n"); return 1; }
    printf("004e8ce9_TEST PASS\n");
    return 0;
}