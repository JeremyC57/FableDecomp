// CThingMultiArrowShotDef::Transfer(CPersistContext& context) -- persist the CThingMultiArrowShotDef's fields (11 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CThingMultiArrowShotDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00451CA2, 134 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CThingMultiArrowShotDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01232BBC, whose RTTI type descriptor is .?AVCThingMultiArrowShotDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CThingMultiArrowShotDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CThingMultiArrowShotDef@@UAEXAAVCPersistContext@@@Z" agrees; family H052 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A.
// Layout: Ego_r.pdb CThingMultiArrowShotDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CThingMultiArrowShotDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(bool& value);                                  // 0x0043314A
};
class CThingMultiArrowShotDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01232BBC slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float AccelerationScalar;                          // +0x28
    float MaxDistance;                                 // +0x2C
    float MaxAngle;                                    // +0x30
    float TimeToReachTarget;                           // +0x34
    float MaxAngleToTurnDegs;                          // +0x38
    long MultiArrowWeaponTrail;                        // +0x3C
    long MultiArrowHitEffect;                          // +0x40
    long MultiArrowTrailEffect;                        // +0x44
    float MultiArrowSpeed;                             // +0x48
    bool MultiArrowDeflectionByMelee;                  // +0x4C
    float MultiArrowHitEffectPercentage;               // +0x50
};

void CThingMultiArrowShotDef::Transfer(CPersistContext& context)
{
    context.Transfer(AccelerationScalar);
    context.Transfer(MaxAngle);
    context.Transfer(MaxDistance);
    context.Transfer(TimeToReachTarget);
    context.Transfer(MaxAngleToTurnDegs);
    context.Transfer(MultiArrowWeaponTrail);
    context.Transfer(MultiArrowHitEffect);
    context.Transfer(MultiArrowTrailEffect);
    context.Transfer(MultiArrowSpeed);
    context.Transfer(MultiArrowDeflectionByMelee);
    context.Transfer(MultiArrowHitEffectPercentage);
}