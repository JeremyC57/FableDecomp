#ifndef FABLE_ENGINE_CTCWEAPON_H
#define FABLE_ENGINE_CTCWEAPON_H
#ifdef FABLE_RETAIL_CTCWEAPON
// Retail branch (chunk 732, CThingPlayerCreature melee rows): the polymorphic CTCWeapon over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCWeapon@@
// : CTCBase@0). Members are not modelled; only the out-of-line Ego_r.pdb / FableWin.pdb queries the rows call:
// ?IsMeleeWeapon@CTCWeapon@@QBE_NXZ 0x005DAA00, ?IsProjectileWeapon@CTCWeapon@@QBE_NXZ 0x005DAA40, ?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z
// 0x005DAB10 (landed rows). StatGetInterfaceType: TCI_WEAPON (72; retail tests bit 8 of +0x28).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCWEAPON_ONCREATE
#include "engine/CDefPointer.h"
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_DAMAGE
#include "engine/EWeaponClass.h"
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_GETWEAPONTYPE
enum EWeaponType;                                                      // Ego_r.pdb enum (enumerators not modelled)
#endif
class CThing;
class CTCWeapon : public CTCBase {
#ifdef FABLE_RETAIL_CTCWEAPON_GETWEAPONTYPE
    // Retail opt-in (bwf1 / agentC2, row 0x00651B04): Ego_r.pdb / FableWin.pdb ?GetWeaponType@CTCWeapon@@QBE?AW4EWeaponType@@XZ, out of line at 0x005DA7D0.
public:
    EWeaponType GetWeaponType() const;                                   // 0x005DA7D0
private:
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:
#ifdef FABLE_RETAIL_COMBAT_SETSTRUCK_PASS29
EWeaponClass GetWeaponClass() const;
 bool IsAbleToBeUsedToBlock(const CThing&,const CThing*,const CThing*) const;
 bool IsBlockBrokenOnBlock(const CThing&,const CThing*,const CThing*) const;
#endif
long GetExplosionType()const;void IncrementNumThingsKilled(CThing*);
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_WEAPON; }
#ifdef FABLE_RETAIL_RANGED_COMBAT_FIXTURE_PASS27
    CTCWeapon(CThing&);
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_SETLOADED
    // Both PDBs original nonconst void(bool,CThing*), native005dac50.
    void SetProjectileWeaponLoaded(bool,CThing*);
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_GETCREATEDSHOTDEFGLOBALINDEX
    // e06 egod opt-in (row 0x008BF280): Ego_r.pdb ?GetCreatedShotDefGlobalIndex@CTCWeapon@@QBEJXZ (const), out of line at 0x005DA750.
    // Declaration only.
    long GetCreatedShotDefGlobalIndex() const;               // 0x005DA750
#endif
    bool IsMeleeWeapon() const;                              // 0x005DAA00
    bool IsProjectileWeapon() const;                         // 0x005DAA40
    bool IsUsableBy(const CThing&) const;                    // 0x005DAB10
#ifdef FABLE_RETAIL_CTCWEAPON_GETMISSILELOADCARRYSLOT
    // NF101 opt-in (round 9, q11260, row 0x0078B070): Ego_r.pdb (0x0063A760) / FableWin.pdb (0x0239B390)
    // ?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ (const), out of line at 0x005DA920 (reads the weapon def's MissileLoadCarrySlot +0x94 through
    // a CDefPointer copy; landed binder 0x008D5790 already spells it). Declaration only.
    long GetMissileLoadCarrySlot() const;                    // 0x005DA920
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_READYPROJECTILE
    // Retail opt-in (chunk Q17, row 0x008BF500): ?ReadyProjectile@CTCWeapon@@QAEXXZ, out of line at 0x005DA4E0. The name is the landed
    // binding's (in neither PDB name set). Declaration only.
    void ReadyProjectile();                                  // 0x005DA4E0
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_RESETAIMING
    // Opt-in (agentNF25b, 2026-11-12): Ego_r.pdb + FableWin.pdb ?ResetAiming@CTCWeapon@@QAEXXZ, out of line at 0x005DA4E0 (AimingTime
    // +0x14 = -1, AimingTimeOverride +0x18 = false; Ego_r donor order IsAimedForLongEnough / SetAimingTimeOverride / SetAiming /
    // ResetAiming / FrameUpdate / IsMeleeWeaponClass = retail 0x005DA490 / 4C0 / 4D0 / 4E0 / 4F0 / 510). Supersedes the
    // FABLE_RETAIL_CTCWEAPON_READYPROJECTILE spelling (in neither PDB). Declaration only.
    void ResetAiming();                                      // 0x005DA4E0
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_AIMING
    // Opt-in (agentNF26, 2026-11-12): Ego_r.pdb + FableWin.pdb ?IsAimedForLongEnough@CTCWeapon@@QBE_NM@Z and
    // ?SetAimingTimeOverride@CTCWeapon@@QAEXXZ, out of line at 0x005DA490 / 0x005DA4C0 (Ego_r donor order IsAimedForLongEnough /
    // SetAimingTimeOverride / SetAiming / ResetAiming = retail 0x005DA490 / 4C0 / 4D0 / 4E0). IsAimedForLongEnough reads AimingTimeOverride
    // +0x18, then AimingTime +0x14 over CMainGameComponent::ConstantFPS against the argument (`ret 4`); SetAimingTimeOverride stores
    // AimingTimeOverride = true. Declarations only.
    bool IsAimedForLongEnough(float time) const;             // 0x005DA490
    void SetAimingTimeOverride();                            // 0x005DA4C0
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_TARGETINGRANGE
    // Retail opt-in (chunk 1025, row 0x008028D0 GetTargetingRangeForTargetType): Ego_r.pdb ?GetTargetingRange@CTCWeapon@@QBEJXZ (Ego_r
    // 0x0063A6D0), out of line in retail at 0x005DA890 (Ego_r donor order GetTargetingRange / TargetingFOV / IsAnalogueLoading at a
    // 0x30 stride = retail 0x005DA890 / 0x005DA8C0 / 0x005DA8F0; the row converts the long result with `fild`). Declaration only.
    long GetTargetingRange() const;                          // 0x005DA890
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_ISPROJECTILE
    // Retail opt-in (chunk 1004, row 0x005CA68E): Ego_r.pdb ?IsProjectile@CTCWeapon@@QBE_NXZ (Ego_r 0x0063A8A0), out of line in
    // retail at 0x005DAA70 (after IsProjectileWeapon 0x005DAA40, the Ego_r order). Declaration only.
    bool IsProjectile() const;                               // 0x005DAA70
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_BLOCK
    // Nested opt-in (chunk 775, row 0x005DACF0): Ego_r.pdb (0x0063AAB0) / FableWin.pdb
    // ?IsAbleToBeUsedToBlock@CTCWeapon@@QBE_NABVCThing@@PBV2@1@Z; one caller (CTCCombat, 0x00735039).
    bool IsAbleToBeUsedToBlock(const CThing& blocker, const CThing* attacker, const CThing* attackerWeapon) const;   // 0x005DACF0
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_DAMAGE
    // Nested opt-in (chunk 900, rows 0x005C8459 / 0x005C9A95 CTCInventoryWeapons::ConstructScreen(PC)): Ego_r.pdb
    // ?GetDamage@CTCWeapon@@QBEMXZ (0x005DAAA0, the active melee weapon's damage) and ?IsMeleeWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z
    // (0x005DA510, static `__fastcall`, the weapon def's Class +0x2c in ecx), both out of line in retail. Declarations only.
    float GetDamage() const;                                 // 0x005DAAA0
    static bool __fastcall IsMeleeWeaponClass(EWeaponClass weaponClass);   // 0x005DA510
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_LOADING
    // Retail opt-in (chunk 796, row 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate): Ego_r.pdb ?IsAnalogueLoading@CTCWeapon@@QBE_NXZ and
    // ?IsProjectileWeaponLoaded@CTCWeapon@@QBE_NXZ (both const), out of line in retail at 0x005DA8F0 / 0x005DA980 (the strafe pull-back
    // test calls them on the primary-slot weapon). Declarations only: no layout change.
    bool IsAnalogueLoading() const;                          // 0x005DA8F0
    bool IsProjectileWeaponLoaded() const;                   // 0x005DA980
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_AUTOLOADING
    // Retail opt-in (hand3c3, row 0x0078A690 CTCHeroRangedAttacks::FrameUpdate): ?IsProjectileWeaponAutoLoading@CTCWeapon@@QBE_NXZ
    // (FableWin.pdb 0x0239B3E0, Ego_r.pdb 0x0063A790 tc_weapon.obj #25, Ego_d.pdb), out of line in retail at 0x005DA950 (landed).
    // Declaration only: no layout change.
    bool IsProjectileWeaponAutoLoading() const;              // 0x005DA950
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_MEMBERS
    // Retail opt-in (chunk 1002, row 0x005CB2B0 CTCInventoryWeapons::SetThingAsActiveWeapon): the Ego_r.pdb members after the CTCBase part
    // (struct_layouts_egor.tsv, sizeof 0x28, the same offsets as the generated default branch below): the row writes Wielded +0x24
    // (`mov byte ptr [esi+24h],0` on the previous weapon, `mov byte ptr [eax+24h],1` on the new one).
#ifdef FABLE_RETAIL_CTCWEAPON_ONCREATE
    // Nested opt-in (agentNF8, row 0x005DA6E0 CTCWeapon::OnCreate): WeaponDef typed as its Ego_r.pdb CDefPointer<const CWeaponDef>
    // (same 4 bytes; needs FABLE_RETAIL_DEF_CLASS_BASE and engine/CDefPointer.h) and Ego_r.pdb ?OnCreate@CTCWeapon@@EAEXXZ (the private
    // override of CTCBase slot 4, CTCWeapon vtable slot 4 = 0x005DA6E0). Same layout.
private:
    virtual void OnCreate();                                 // slot 4 0x005DA6E0
public:
#ifdef FABLE_RETAIL_WEAPON_ARROW_TRAIL_PASS26
public:
    CDefPointer<const CWeaponDef> GetWeaponDef() const { return WeaponDef; }
private:
#endif
    CDefPointer<const class CWeaponDef> WeaponDef;           // +0x0c
#else
    unsigned char    WeaponDef[0x4];                         // +0x0c CDefPointer<const CWeaponDef>
#endif
    bool             ProjectileWeaponLoaded;                 // +0x10
    long             AimingTime;                             // +0x14
    bool             AimingTimeOverride;                     // +0x18
    long             NumThingsKilled;                        // +0x1c
    long             TimeWielded;                            // +0x20
    bool             Wielded;                                // +0x24
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x005DAE10
public:
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual(s) this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
public:
    virtual void FrameUpdate();  // slot 9, 0x005DA4F0
    virtual void OnSerialise(CPersistContext&, bool);  // slot 1, 0x005DA9B0
public:
#endif // FABLE_RETAIL_CTCWEAPON_VSLOTS3
};
#ifdef FABLE_RETAIL_CTCWEAPON_MEMBERS
FABLE_STATIC_ASSERT_AT(125, sizeof(CTCWeapon) == 0x28);
#endif
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCWeapon = 0x28.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;
#ifdef FABLE_RETAIL_CTCWEAPON_ISUSABLEBY
class CThing;
#endif

#pragma pack(push, 1)
class CTCWeapon {
public:
#ifdef FABLE_RETAIL_CTCWEAPON_ISPROJECTILEWEAPONLOADED
    // NF113 opt-in (PRIORITY q6397, row 0x005DA980): Ego_r.pdb 0x0063A7C0 / FableWin.pdb 0x0239B430 ?IsProjectileWeaponLoaded@CTCWeapon@@QBE_NXZ
    // (public const; the retail branch declares it under FABLE_RETAIL_CTCWEAPON_LOADING) on this generated layout, for the self row that
    // reads WeaponDef_Object +0x0C / ProjectileWeaponLoaded +0x10. Declaration only: no layout change.
    bool IsProjectileWeaponLoaded() const;                   // 0x005DA980
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_AUTOLOADING
    // Same opt-in as in the retail branch above, on this generated layout. Declaration only: no layout change.
    bool IsProjectileWeaponAutoLoading() const;              // 0x005DA950
#endif
#ifdef FABLE_RETAIL_CTCWEAPON_ISUSABLEBY
    // NF177 opt-in (PRIORITY q11929, rows 0x005DAD10 / 0x005DAD60): Ego_r.pdb 0x0063A950 / FableWin.pdb 0x0239AF10
    // ?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z (public const; the retail branch declares it unconditionally) on this generated layout.
    // Declaration only: no layout change.
    bool IsUsableBy(const CThing&) const;                    // 0x005DAB10
#endif
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    CDefPointeeBase* WeaponDef_Object;                       // +0x0c CDefPointer<CWeaponDef_const_>::Object
    bool             ProjectileWeaponLoaded;                 // +0x10
    unsigned char    _pad_0x11[0x3];                         // +0x11
    long             AimingTime;                             // +0x14
    bool             AimingTimeOverride;                     // +0x18
    unsigned char    _pad_0x19[0x3];                         // +0x19
    long             NumThingsKilled;                        // +0x1c
    long             TimeWielded;                            // +0x20
    bool             Wielded;                                // +0x24
    unsigned char    _pad_0x25[0x3];                         // +0x25
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(157, sizeof(CTCWeapon) == 0x28);
FABLE_STATIC_ASSERT_AT(158, offsetof(CTCWeapon, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(159, offsetof(CTCWeapon, WeaponDef_Object) == 0xc);
FABLE_STATIC_ASSERT_AT(160, offsetof(CTCWeapon, ProjectileWeaponLoaded) == 0x10);
FABLE_STATIC_ASSERT_AT(161, offsetof(CTCWeapon, AimingTime) == 0x14);
FABLE_STATIC_ASSERT_AT(162, offsetof(CTCWeapon, AimingTimeOverride) == 0x18);
FABLE_STATIC_ASSERT_AT(163, offsetof(CTCWeapon, NumThingsKilled) == 0x1c);
FABLE_STATIC_ASSERT_AT(164, offsetof(CTCWeapon, TimeWielded) == 0x20);
FABLE_STATIC_ASSERT_AT(165, offsetof(CTCWeapon, Wielded) == 0x24);

#endif // FABLE_RETAIL_CTCWEAPON
#endif // FABLE_ENGINE_CTCWEAPON_H
