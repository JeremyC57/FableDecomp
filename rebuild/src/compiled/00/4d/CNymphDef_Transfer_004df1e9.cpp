// CNymphDef::Transfer(CPersistContext& context) -- persist the CNymphDef's fields (10 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CNymphDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF1E9, 123 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CNymphDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123DE3C, whose RTTI type descriptor is .?AVCNymphDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CNymphDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CNymphDef@@UAEXAAVCPersistContext@@@Z" agrees; family H058 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CNymphDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CNymphDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
};
class CNymphDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123DE3C slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float MinAttackRange;                              // +0x28
    float MaxHeight;                                   // +0x2C
    float MinHeight;                                   // +0x30
    float CircleRange;                                 // +0x34
    long FlyAroundTime;                                // +0x38
    long ShotTrapType;                                 // +0x3C
    long ShotsBeforeIdle;                              // +0x40
    float IdleTimeBetweenShots;                        // +0x44
    float FadeInTime;                                  // +0x48
    float FadeOutTime;                                 // +0x4C
};

void CNymphDef::Transfer(CPersistContext& context)
{
    context.Transfer(MinAttackRange);
    context.Transfer(MinHeight);
    context.Transfer(MaxHeight);
    context.Transfer(CircleRange);
    context.Transfer(FlyAroundTime);
    context.Transfer(ShotTrapType);
    context.Transfer(ShotsBeforeIdle);
    context.Transfer(IdleTimeBetweenShots);
    context.Transfer(FadeInTime);
    context.Transfer(FadeOutTime);
}