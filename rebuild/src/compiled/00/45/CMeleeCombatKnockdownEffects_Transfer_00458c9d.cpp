// CMeleeCombatKnockdownEffects::Transfer(CPersistContext& context) -- persist the CMeleeCombatKnockdownEffects's fields (11 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CMeleeCombatKnockdownEffects@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458C9D, 134 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CMeleeCombatKnockdownEffects::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01234DC4, whose RTTI type descriptor is .?AVCMeleeCombatKnockdownEffects@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CMeleeCombatKnockdownEffects@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CMeleeCombatKnockdownEffects@@UAEXAAVCPersistContext@@@Z" agrees; family H052 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A, Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CMeleeCombatKnockdownEffects members rebased to retail (def base 4 bytes shorter); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(float& value);                                 // 0x00431061
};
class CMeleeCombatKnockdownEffects {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01234DC4 slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    long BlendInFrames;                                // +0x3C (Ego_r +0x40)
    long BlendOutFrames;                               // +0x40 (Ego_r +0x44)
    long AttackerLightingChannel;                      // +0x44 (Ego_r +0x48)
    long TargetLightingChannel;                        // +0x48 (Ego_r +0x4C)
    long EnvironmentTheme;                             // +0x4C (Ego_r +0x50)
    bool CameraFOVChange;                              // +0x50 (Ego_r +0x54)
    bool CameraPan;                                    // +0x51 (Ego_r +0x55)
    float AttackerAndTargetSpeedMultiplier;            // +0x54 (Ego_r +0x58)
    bool PauseEntities;                                // +0x58 (Ego_r +0x5C)
    bool NonAttackingEntities;                         // +0x59 (Ego_r +0x5D)
    bool AttackerInvulnerable;                         // +0x5A (Ego_r +0x5E)
};

void CMeleeCombatKnockdownEffects::Transfer(CPersistContext& context)
{
    context.Transfer(BlendInFrames);
    context.Transfer(BlendOutFrames);
    context.Transfer(AttackerLightingChannel);
    context.Transfer(TargetLightingChannel);
    context.Transfer(EnvironmentTheme);
    context.Transfer(CameraFOVChange);
    context.Transfer(CameraPan);
    context.Transfer(AttackerAndTargetSpeedMultiplier);
    context.Transfer(PauseEntities);
    context.Transfer(NonAttackingEntities);
    context.Transfer(AttackerInvulnerable);
}