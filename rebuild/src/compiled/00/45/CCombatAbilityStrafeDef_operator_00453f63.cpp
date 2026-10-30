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

class CCombatAbilityStrafingData : public CCombatAbilityData { public:
    float RotationSpeed;                // +0x18
    __forceinline CCombatAbilityStrafingData& operator=(const CCombatAbilityStrafingData& other)
    {
        CCombatAbilityData::operator=(other);
        RotationSpeed = other.RotationSpeed;
        return *this;
    }
};

// def vtable 0x01232674: slot 0 0x0044FDD3 (implicit inline dtor); real overrides from the def vtable scan
class CCombatAbilityStrafeDef : public CDefClassBase { public:
    CCombatAbilityStrafingData CombatAbility;   // +0x28
    CCombatAbilityStrafeDef& operator=(const CCombatAbilityStrafeDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00452B93
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00453F5E
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C599
};

CCombatAbilityStrafeDef& CCombatAbilityStrafeDef::operator=(const CCombatAbilityStrafeDef& other)
{
    CDefClassBase::operator=(other);
    CombatAbility = other.CombatAbility;
    return *this;
}