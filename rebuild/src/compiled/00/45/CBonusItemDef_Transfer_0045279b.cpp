// CBonusItemDef::Transfer(CPersistContext& context) -- persist the CBonusItemDef's fields (20 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CBonusItemDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045279B, 233 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CBonusItemDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012322E4, whose RTTI type descriptor is .?AVCBonusItemDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CBonusItemDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CBonusItemDef@@UAEXAAVCPersistContext@@@Z" agrees; family H053 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(bool&) 0x0043314A, Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CBonusItemDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CBonusItemDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(long& value);                                  // 0x00431102
};
class CBonusItemDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012322E4 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float HealthModifier;                              // +0x28
    bool AddsFatness;                                  // +0x2C
    long StaminaModifier;                              // +0x30
    float SecondsToApplyHealthIncreaseOver;            // +0x34
    long WillExperience;                               // +0x38
    long SkillExperience;                              // +0x3C
    long StrengthExperience;                           // +0x40
    long MoralityChange;                               // +0x44
    float AgreeablenessChange;                         // +0x48
    float AttractivenessChange;                        // +0x4C
    float ScarinessChange;                             // +0x50
    float MaxHealthIncrease;                           // +0x54
    long MaxStaminaIncrease;                           // +0x58
    bool ChangeTimeOfDayToDay;                         // +0x5C
    bool ChangeTimeOfDayToNight;                       // +0x5D
    float SpeedMultiplier;                             // +0x60
    float SecondsForSpeedMultiplierToLast;             // +0x64
    long ParticleEffect;                               // +0x68
    bool AddsDrunkenness;                              // +0x6C
    float AlcoholicStrength;                           // +0x70
};

void CBonusItemDef::Transfer(CPersistContext& context)
{
    context.Transfer(HealthModifier);
    context.Transfer(AddsFatness);
    context.Transfer(StaminaModifier);
    context.Transfer(SecondsToApplyHealthIncreaseOver);
    context.Transfer(WillExperience);
    context.Transfer(SkillExperience);
    context.Transfer(StrengthExperience);
    context.Transfer(MoralityChange);
    context.Transfer(AgreeablenessChange);
    context.Transfer(AttractivenessChange);
    context.Transfer(ScarinessChange);
    context.Transfer(MaxHealthIncrease);
    context.Transfer(MaxStaminaIncrease);
    context.Transfer(ChangeTimeOfDayToDay);
    context.Transfer(ChangeTimeOfDayToNight);
    context.Transfer(SpeedMultiplier);
    context.Transfer(SecondsForSpeedMultiplierToLast);
    context.Transfer(ParticleEffect);
    context.Transfer(AddsDrunkenness);
    context.Transfer(AlcoholicStrength);
}