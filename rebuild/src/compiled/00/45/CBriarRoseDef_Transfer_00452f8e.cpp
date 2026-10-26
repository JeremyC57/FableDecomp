// CBriarRoseDef::Transfer(CPersistContext& context) -- persist the CBriarRoseDef's fields (11 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CBriarRoseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00452F8E, 134 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CBriarRoseDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012329B4, whose RTTI type descriptor is .?AVCBriarRoseDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CBriarRoseDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CBriarRoseDef@@UAEXAAVCPersistContext@@@Z" agrees; family H052 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CBriarRoseDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CBriarRoseDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
};
class CBriarRoseDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012329B4 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float SecondsToTeleport;                           // +0x28
    float SecondsToBeIntangible;                       // +0x2C
    float SecondsToChargeDrainLife;                    // +0x30
    long GuranteedImpostersHitBeforeAllowedToHitBriar; // +0x34
    long NumberOfHitsToKillImposters;                  // +0x38
    long EnvironmentThemeForIntangibleMode;            // +0x3C
    float SecondsForIntangibleEnvironmentThemeTransitionIn; // +0x40
    float SecondsForIntangibleEnvironmentThemeTransitionOut; // +0x44
    float SecondsBetweenWillUse;                       // +0x48
    float SecondsBetweenTeleportAway;                  // +0x4C
    float DistanceForcePushThreshold;                  // +0x50
};

void CBriarRoseDef::Transfer(CPersistContext& context)
{
    context.Transfer(SecondsToTeleport);
    context.Transfer(SecondsToBeIntangible);
    context.Transfer(SecondsToChargeDrainLife);
    context.Transfer(GuranteedImpostersHitBeforeAllowedToHitBriar);
    context.Transfer(NumberOfHitsToKillImposters);
    context.Transfer(EnvironmentThemeForIntangibleMode);
    context.Transfer(SecondsForIntangibleEnvironmentThemeTransitionIn);
    context.Transfer(SecondsForIntangibleEnvironmentThemeTransitionOut);
    context.Transfer(SecondsBetweenWillUse);
    context.Transfer(SecondsBetweenTeleportAway);
    context.Transfer(DistanceForcePushThreshold);
}