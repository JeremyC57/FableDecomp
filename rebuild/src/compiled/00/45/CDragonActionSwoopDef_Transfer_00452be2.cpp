// CDragonActionSwoopDef::Transfer(CPersistContext& context) -- persist the CDragonActionSwoopDef's fields (15 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CDragonActionSwoopDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00452BE2, 178 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CDragonActionSwoopDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01232744, whose RTTI type descriptor is .?AVCDragonActionSwoopDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CDragonActionSwoopDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CDragonActionSwoopDef@@UAEXAAVCPersistContext@@@Z" agrees; family H054 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CDragonActionSwoopDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CDragonActionSwoopDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
};
class CDragonActionSwoopDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01232744 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float IntoDelay;                                   // +0x28
    float OutOfDelay;                                  // +0x2C
    float Distance;                                    // +0x30
    float MidPointsControlOffset;                      // +0x34
    float StartHeight;                                 // +0x38
    float MidHeight;                                   // +0x3C
    float EndHeight;                                   // +0x40
    float FlightSpeed;                                 // +0x44
    float MaxSplineProgressForSplineAdjustment;        // +0x48
    float MaxSplineProgressForBurninationEffect;       // +0x4C
    float MinSplineProgressForWingFlap;                // +0x50
    float MaxExplosionRange;                           // +0x54
    float ExplosionLineProjectionAngle;                // +0x58
    long FramesBetweenExplosions;                      // +0x5C
    float WindSpeed;                                   // +0x60
};

void CDragonActionSwoopDef::Transfer(CPersistContext& context)
{
    context.Transfer(IntoDelay);
    context.Transfer(OutOfDelay);
    context.Transfer(Distance);
    context.Transfer(MidPointsControlOffset);
    context.Transfer(StartHeight);
    context.Transfer(MidHeight);
    context.Transfer(EndHeight);
    context.Transfer(FlightSpeed);
    context.Transfer(MaxSplineProgressForSplineAdjustment);
    context.Transfer(MaxSplineProgressForBurninationEffect);
    context.Transfer(MinSplineProgressForWingFlap);
    context.Transfer(MaxExplosionRange);
    context.Transfer(ExplosionLineProjectionAngle);
    context.Transfer(FramesBetweenExplosions);
    context.Transfer(WindSpeed);
}