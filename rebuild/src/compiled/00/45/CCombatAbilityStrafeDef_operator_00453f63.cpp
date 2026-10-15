// CCombatAbilityStrafeDef::operator=(const CCombatAbilityStrafeDef&) -- def copy: CDefClassBase part, then the strafing ability record
// Leaf: ??4CCombatAbilityStrafeDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453F63, 43 bytes, size-optimised TU (odd address in the /Os def region).
// Callees: CDefClassBase::operator= @ 0x00431F10 (manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), CCombatAbilityData::operator= @ 0x00453F18.
// Identification: PDB CCombatAbilityStrafeDef (0x44 bytes) = CDefClassBase (0x25, packed) + CombatAbility
//   (CCombatAbilityStrafingData, 0x1c bytes) at +0x28. CCombatAbilityStrafingData derives from the polymorphic
//   CCombatAbilityData (0x18 bytes, out-of-line operator= that skips the vptr) and adds RotationSpeed at +0x18.
// Shape: the record's own assignment is inlined with its `this`/`other` kept in ebx/edi (`add edi,28h ;
//   lea ebx,[esi+28h]`), then RotationSpeed is copied relative to them. The DDK out-lines an IMPLICIT
//   assignment of a record whose base has its own operator=, so the record's operator= is written out and
//   `__forceinline`; the def's own operator= is then the plain memberwise form.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
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

struct CCombatAbilityStrafingData : CCombatAbilityData {
    float RotationSpeed;                // +0x18
    __forceinline CCombatAbilityStrafingData& operator=(const CCombatAbilityStrafingData& other)
    {
        CCombatAbilityData::operator=(other);
        RotationSpeed = other.RotationSpeed;
        return *this;
    }
};

struct CCombatAbilityStrafeDef : CDefClassBase {
    CCombatAbilityStrafingData CombatAbility;   // +0x28
    CCombatAbilityStrafeDef& operator=(const CCombatAbilityStrafeDef& other);
};

CCombatAbilityStrafeDef& CCombatAbilityStrafeDef::operator=(const CCombatAbilityStrafeDef& other)
{
    CDefClassBase::operator=(other);
    CombatAbility = other.CombatAbility;
    return *this;
}