// CCombatAbilityUseProjectileWeaponDef::operator=(const CCombatAbilityUseProjectileWeaponDef&) -- def copy: CDefClassBase part, then the projectile-attack ability record
// Leaf: ??4CCombatAbilityUseProjectileWeaponDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453F93, 49 bytes, size-optimised TU (odd address in the /Os def region).
// Callees: CDefClassBase::operator= @ 0x00431F10 (propagated label "CThingBuildingDef::operator="),
//          CCombatAbilityData::operator= @ 0x00453F18.
// Identification: PDB CCombatAbilityUseProjectileWeaponDef (0x48 bytes) = CDefClassBase + CombatAbility
//   (CCombatAbilityProjectileAttackData, 0x20 bytes) at +0x28: CCombatAbilityData base (out-of-line operator=),
//   AbleToStrafeAndShoot (bool) at +0x18, ShotAccuracyPercentage at +0x1c.
// Shape: as 0x00453F63 -- the record's operator= is inlined (`__forceinline`; the DDK out-lines the implicit
//   one) and its byte + dword are copied relative to the record pointers (here edi/esi, `this` in ebx).
#pragma optimize("s", on)

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