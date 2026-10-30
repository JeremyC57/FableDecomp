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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CCombatAbilityData { public:
    virtual void v00();
    long PercentageChance;              // +0x04
    float SecondsDuration;              // +0x08
    float SecondsToWaitBeforeRepeat;    // +0x0c
    long Anim;                          // +0x10 CDefString
    long InterfaceType;                 // +0x14 ECombatAbilityInterfaceType
    CCombatAbilityData& operator=(const CCombatAbilityData& other);   // 0x00453F18
};

class CCombatAbilityProjectileAttackData : public CCombatAbilityData { public:
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

// def vtable 0x012326DC: slot 0 0x0044FE12 (implicit inline dtor); real overrides from the def vtable scan
class CCombatAbilityUseProjectileWeaponDef : public CDefClassBase { public:
    CCombatAbilityProjectileAttackData CombatAbility;   // +0x28
    CCombatAbilityUseProjectileWeaponDef& operator=(const CCombatAbilityUseProjectileWeaponDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00452BB4
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00453F8E
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C5B9
};

CCombatAbilityUseProjectileWeaponDef& CCombatAbilityUseProjectileWeaponDef::operator=(const CCombatAbilityUseProjectileWeaponDef& other)
{
    CDefClassBase::operator=(other);
    CombatAbility = other.CombatAbility;
    return *this;
}