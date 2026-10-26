// CJackDragonDef::Transfer(CPersistContext& context) -- persist the CJackDragonDef's fields (20 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CJackDragonDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00452EA5, 233 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CJackDragonDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123294C, whose RTTI type descriptor is .?AVCJackDragonDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CJackDragonDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CJackDragonDef@@UAEXAAVCPersistContext@@@Z" agrees; family H053 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CJackDragonDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CJackDragonDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(float& value);                                 // 0x00431061
};
class CJackDragonDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123294C slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float MinTimeBetweenSlams;                         // +0x28
    long MaxHitsBeforeFlyAway;                         // +0x2C
    float MaxFirebeamDistance;                         // +0x30
    float MinSnapDistance;                             // +0x34
    float MaxSnapDistance;                             // +0x38
    float MaxTimeAtLongRangeBeforeFlyAway;             // +0x3C
    float MaxSlamDistance;                             // +0x40
    long MaxHoverLoops;                                // +0x44
    float PercentChanceOfFakeLandingSwoop;             // +0x48
    float MaxTimeOnGround;                             // +0x4C
    float DragonGroundHeightOffsetBodge;               // +0x50
    float HoverBackOffOffset;                          // +0x54
    long FramesBetweenBurninateUpdate;                 // +0x58
    long FramesBetweenFireBeamUpdate;                  // +0x5C
    float HoverObstructionRadius;                      // +0x60
    float StrafeTriggerDistance;                       // +0x64
    float GetHitSnapBuildupTime;                       // +0x68
    float NotHitSnapBuildupTime;                       // +0x6C
    float MaxTimeBeforeSnap;                           // +0x70
    float MinRoarDistance;                             // +0x74
};

void CJackDragonDef::Transfer(CPersistContext& context)
{
    context.Transfer(MaxHitsBeforeFlyAway);
    context.Transfer(MinTimeBetweenSlams);
    context.Transfer(MaxFirebeamDistance);
    context.Transfer(MinSnapDistance);
    context.Transfer(MaxSnapDistance);
    context.Transfer(MaxTimeAtLongRangeBeforeFlyAway);
    context.Transfer(MaxSlamDistance);
    context.Transfer(MaxHoverLoops);
    context.Transfer(PercentChanceOfFakeLandingSwoop);
    context.Transfer(MaxTimeOnGround);
    context.Transfer(DragonGroundHeightOffsetBodge);
    context.Transfer(HoverBackOffOffset);
    context.Transfer(FramesBetweenFireBeamUpdate);
    context.Transfer(FramesBetweenBurninateUpdate);
    context.Transfer(HoverObstructionRadius);
    context.Transfer(StrafeTriggerDistance);
    context.Transfer(GetHitSnapBuildupTime);
    context.Transfer(NotHitSnapBuildupTime);
    context.Transfer(MaxTimeBeforeSnap);
    context.Transfer(MinRoarDistance);
}