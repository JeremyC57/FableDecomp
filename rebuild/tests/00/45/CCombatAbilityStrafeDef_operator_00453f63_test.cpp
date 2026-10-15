#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CCombatAbilityStrafeDef::operator=(const CCombatAbilityStrafeDef&) -- def copy: CDefClassBase part, then the strafing ability record
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
void CCombatAbilityData::v00() {}
CCombatAbilityData& CCombatAbilityData::operator=(const CCombatAbilityData& other) { PercentageChance = other.PercentageChance; SecondsDuration = other.SecondsDuration; SecondsToWaitBeforeRepeat = other.SecondsToWaitBeforeRepeat; Anim = other.Anim; InterfaceType = other.InterfaceType; return *this; }

int main()
{
    static CCombatAbilityStrafeDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.CombatAbility.PercentageChance = 7;
    b.CombatAbility.SecondsDuration = 2.5f;
    b.CombatAbility.SecondsToWaitBeforeRepeat = 3.5f;
    b.CombatAbility.Anim = 11;
    b.CombatAbility.InterfaceType = 2;
    b.CombatAbility.RotationSpeed = 90.0f;
    CCombatAbilityStrafeDef* r = &(a = b);
    if (r != &a) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "00453f63"); return 1; }
    if (!(a.CombatAbility.PercentageChance == b.CombatAbility.PercentageChance)) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.SecondsDuration == b.CombatAbility.SecondsDuration)) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.SecondsToWaitBeforeRepeat == b.CombatAbility.SecondsToWaitBeforeRepeat)) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.Anim == b.CombatAbility.Anim)) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.InterfaceType == b.CombatAbility.InterfaceType)) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    if (!(a.CombatAbility.RotationSpeed == b.CombatAbility.RotationSpeed)) { std::printf("00453f63_TEST FAIL\n"); return 1; }
    std::printf("00453f63_TEST PASS\n");
    return 0;
}