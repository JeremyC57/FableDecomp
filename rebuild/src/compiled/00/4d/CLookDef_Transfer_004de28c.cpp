// CLookDef::Transfer(CPersistContext& context) -- persist the CLookDef's fields (15 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CLookDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE28C, 178 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CLookDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123AE14, whose RTTI type descriptor is .?AVCLookDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CLookDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CLookDef@@UAEXAAVCPersistContext@@@Z" agrees; family H054 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(bool&) 0x0043314A.
// Layout: Ego_r.pdb CLookDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CLookDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(bool& value);                                  // 0x0043314A
};
class CLookDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123AE14 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float EyesTurnRangeXY;                             // +0x28
    float EyesTurnRangeYZ;                             // +0x2C
    float HeadTurnRangeXY;                             // +0x30
    float HeadTurnRangeYZUp;                           // +0x34
    float HeadTurnRangeYZDown;                         // +0x38
    float MaxTurnSpeed;                                // +0x3C
    bool AllowTurningViaAnimation;                     // +0x40
    float AnimationTurnSpeedMultiplier;                // +0x44
    float CombatMaxTurnSpeed;                          // +0x48
    bool CombatAllowTurningViaAnimation;               // +0x4C
    bool AllowOversteer;                               // +0x4D
    bool AllowUndersteer;                              // +0x4E
    bool DrawEyePrimitives;                            // +0x4F
    float CutOffNoLooking;                             // +0x50
    bool AlwaysUpdateHeadPos;                          // +0x54
};

void CLookDef::Transfer(CPersistContext& context)
{
    context.Transfer(EyesTurnRangeXY);
    context.Transfer(EyesTurnRangeYZ);
    context.Transfer(HeadTurnRangeXY);
    context.Transfer(HeadTurnRangeYZUp);
    context.Transfer(HeadTurnRangeYZDown);
    context.Transfer(MaxTurnSpeed);
    context.Transfer(AllowTurningViaAnimation);
    context.Transfer(AnimationTurnSpeedMultiplier);
    context.Transfer(CombatMaxTurnSpeed);
    context.Transfer(CombatAllowTurningViaAnimation);
    context.Transfer(AllowOversteer);
    context.Transfer(AllowUndersteer);
    context.Transfer(DrawEyePrimitives);
    context.Transfer(CutOffNoLooking);
    context.Transfer(AlwaysUpdateHeadPos);
}