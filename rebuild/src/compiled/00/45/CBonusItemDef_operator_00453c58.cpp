// CBonusItemDef::operator=(const CBonusItemDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CBonusItemDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453C58, 141 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CBonusItemDef::Copy 0x00453C53 (catalogued CBonusItemDef::Copy); the member
//   copies (dword / byte at each offset) are Ego_r.pdb CBonusItemDef's members in order (20 members from +0x28).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M033, representative 0x00453C58): compiler-generated `operator=`, emitted here through Copy's
//   `*this = *(const CBonusItemDef*)other;`; `optimize("s", on)` keeps every copy in eax/al (speed rotates eax/ecx/edx).
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x012322E4: slot 0 0x0044F9D3 (implicit inline dtor); real overrides from the def vtable scan
class CBonusItemDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                 HealthModifier;                 // +0x28
    bool                  AddsFatness;                    // +0x2c
    long                  StaminaModifier;                // +0x30
    float                 SecondsToApplyHealthIncreaseOver; // +0x34
    long                  WillExperience;                 // +0x38
    long                  SkillExperience;                // +0x3c
    long                  StrengthExperience;             // +0x40
    long                  MoralityChange;                 // +0x44
    float                 AgreeablenessChange;            // +0x48
    float                 AttractivenessChange;           // +0x4c
    float                 ScarinessChange;                // +0x50
    float                 MaxHealthIncrease;              // +0x54
    long                  MaxStaminaIncrease;             // +0x58
    bool                  ChangeTimeOfDayToDay;           // +0x5c
    bool                  ChangeTimeOfDayToNight;         // +0x5d
    float                 SpeedMultiplier;                // +0x60
    float                 SecondsForSpeedMultiplierToLast; // +0x64
    long                  ParticleEffect;                 // +0x68
    bool                  AddsDrunkenness;                // +0x6c
    float                 AlcoholicStrength;              // +0x70
    virtual void Transfer(CPersistContext& context);         // slot 18 0x0045279B
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C390
};
void CBonusItemDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBonusItemDef*)other;
}