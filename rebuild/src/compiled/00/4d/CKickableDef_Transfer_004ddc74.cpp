// CKickableDef::Transfer(CPersistContext& context) -- persist the CKickableDef's fields (11 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CKickableDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDC74, 134 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CKickableDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123A7AC, whose RTTI type descriptor is .?AVCKickableDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CKickableDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CKickableDef@@UAEXAAVCPersistContext@@@Z" agrees; family H052 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(EOpinionDeedType&) 0x00456B7D, Transfer(bool&) 0x0043314A.
// Layout: Ego_r.pdb CKickableDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CKickableDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
enum EOpinionDeedType { };                                  // enumerators not needed
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(EOpinionDeedType& value);                      // 0x00456B7D
    void Transfer(bool& value);                                  // 0x0043314A
};
class CKickableDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123A7AC slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float BaseDistanceToTravel;                        // +0x28
    float StrengthScaleRange;                          // +0x2C
    float SpeedScaleRange;                             // +0x30
    float BaseSpeedScaleRange;                         // +0x34
    float RandomBase;                                  // +0x38
    float RandomRange;                                 // +0x3C
    long ShotToAttach;                                 // +0x40
    EOpinionDeedType OpinionDeedType;                  // +0x44
    long OpinionDeedFaction;                           // +0x48
    EOpinionDeedType OpinionDeedTypeToEnemies;         // +0x4C
    bool IgnoreNavigationTest;                         // +0x50
};

void CKickableDef::Transfer(CPersistContext& context)
{
    context.Transfer(BaseDistanceToTravel);
    context.Transfer(StrengthScaleRange);
    context.Transfer(SpeedScaleRange);
    context.Transfer(BaseSpeedScaleRange);
    context.Transfer(RandomBase);
    context.Transfer(RandomRange);
    context.Transfer(ShotToAttach);
    context.Transfer(OpinionDeedType);
    context.Transfer(OpinionDeedFaction);
    context.Transfer(OpinionDeedTypeToEnemies);
    context.Transfer(IgnoreNavigationTest);
}