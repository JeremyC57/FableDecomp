#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CCombatAbilityUseProjectileWeaponDef::operator=(const CCombatAbilityUseProjectileWeaponDef&) -- def copy: CDefClassBase part, then the projectile-attack ability record
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453F93, 49 bytes, size-optimised TU (odd address in the /Os def region).
// Callees: CDefClassBase::operator= @ 0x00431F10 (propagated label "CThingBuildingDef::operator="),
//          CCombatAbilityData::operator= @ 0x00453F18.
// Identification: PDB CCombatAbilityUseProjectileWeaponDef (0x48 bytes) = CDefClassBase + CombatAbility
//   (CCombatAbilityProjectileAttackData, 0x20 bytes) at +0x28: CCombatAbilityData base (out-of-line operator=),
//   AbleToStrafeAndShoot (bool) at +0x18, ShotAccuracyPercentage at +0x1c.
// Shape: as 0x00453F63 -- the record's operator= is inlined (`__forceinline`; the DDK out-lines the implicit
//   one) and its byte + dword are copied relative to the record pointers (here edi/esi, `this` in ebx).

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CCombatAbilityData {
    virtual void v00();
    long PercentageChance;              // +0x04
    float SecondsDuration;              // +0x08
    float SecondsToWaitBeforeRepeat;    // +0x0c
    long Anim;                          // +0x10 CDefString
    long InterfaceType;                 // +0x14 ECombatAbilityInterfaceType
    CCombatAbilityData& operator=(const CCombatAbilityData& other);   // 0x00453F18
};

struct CCombatAbilityProjectileAttackData : CCombatAbilityData {
    bool AbleToStrafeAndShoot;          // +0x18
    long ShotAccuracyPercentage;        // +0x1c
    __forceinline CCombatAbilityProjectileAttackData& operator=(const CCombatAbilityProjectileAttackData& other)
    {
        CCombatAbilityData::operator=(other);
        AbleToStrafeAndShoot = other.AbleToStrafeAndShoot;
        ShotAccuracyPercentage = other.ShotAccuracyPercentage;
        return *this;
    }
};

struct CCombatAbilityUseProjectileWeaponDef : CDefClassBase {
    CCombatAbilityProjectileAttackData CombatAbility;   // +0x28
    CCombatAbilityUseProjectileWeaponDef& operator=(const CCombatAbilityUseProjectileWeaponDef& other);
};

CCombatAbilityUseProjectileWeaponDef& CCombatAbilityUseProjectileWeaponDef::operator=(const CCombatAbilityUseProjectileWeaponDef& other)
{
    CDefClassBase::operator=(other);
    CombatAbility = other.CombatAbility;
    return *this;
}


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
void CCombatAbilityData::v00() {}
CCombatAbilityData& CCombatAbilityData::operator=(const CCombatAbilityData& other) { PercentageChance = other.PercentageChance; SecondsDuration = other.SecondsDuration; SecondsToWaitBeforeRepeat = other.SecondsToWaitBeforeRepeat; Anim = other.Anim; InterfaceType = other.InterfaceType; return *this; }

int main()
{
    static CCombatAbilityUseProjectileWeaponDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.CombatAbility.PercentageChance = 7;
    b.CombatAbility.Anim = 11;
    b.CombatAbility.AbleToStrafeAndShoot = true;
    b.CombatAbility.ShotAccuracyPercentage = 85;
    CCombatAbilityUseProjectileWeaponDef* r = &(a = b);
    if (r != &a) { std::printf("00453f93_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "00453f93"); return 1; }
    if (!(a.CombatAbility.PercentageChance == b.CombatAbility.PercentageChance)) { std::printf("00453f93_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.Anim == b.CombatAbility.Anim)) { std::printf("00453f93_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.AbleToStrafeAndShoot == b.CombatAbility.AbleToStrafeAndShoot)) { std::printf("00453f93_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.ShotAccuracyPercentage == b.CombatAbility.ShotAccuracyPercentage)) { std::printf("00453f93_TEST FAIL\n"); return 1; }
    std::printf("00453f93_TEST PASS\n");
    return 0;
}
