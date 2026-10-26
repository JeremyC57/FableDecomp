// CSpecialAbilitiesThunderLightningStormDef::Transfer(CPersistContext& context) -- persist the CSpecialAbilitiesThunderLightningStormDef's fields (11 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSpecialAbilitiesThunderLightningStormDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457F91, 134 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSpecialAbilitiesThunderLightningStormDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01234814, whose RTTI type descriptor is .?AVCSpecialAbilitiesThunderLightningStormDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CSpecialAbilitiesThunderLightningStormDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSpecialAbilitiesThunderLightningStormDef@@UAEXAAVCPersistContext@@@Z" agrees; family H052 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(float&) 0x00431061, Transfer(CDefString&) 0x0045228F.
// Layout: Ego_r.pdb CSpecialAbilitiesThunderLightningStormDef members rebased to retail (def base 4 bytes shorter); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class CPersistContext {
public:
    template <class T> void Transfer(T& value);                                  // 0x00431102
};
class CSpecialAbilitiesThunderLightningStormDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01234814 slot 18
    unsigned char _base[0x3C - 4];                        // def base body
    long NumBeams;                                     // +0x3C (Ego_r +0x40)
    float StartRadius;                                 // +0x40 (Ego_r +0x44)
    float EndRadius;                                   // +0x44 (Ego_r +0x48)
    float SkyBeamStartTime;                            // +0x48 (Ego_r +0x4C)
    float MainBeamsStartTime;                          // +0x4C (Ego_r +0x50)
    float BeamRotationStartSpeed;                      // +0x50 (Ego_r +0x54)
    float BeamRotationEndSpeed;                        // +0x54 (Ego_r +0x58)
    float TotalLifetime;                               // +0x58 (Ego_r +0x5C)
    float Damage;                                      // +0x5C (Ego_r +0x60)
    CDefString InitSound;                              // +0x60 (Ego_r +0x64)
    CDefString LoopingSound;                           // +0x64 (Ego_r +0x68)
};

void CSpecialAbilitiesThunderLightningStormDef::Transfer(CPersistContext& context)
{
    context.Transfer(NumBeams);
    context.Transfer(StartRadius);
    context.Transfer(EndRadius);
    context.Transfer(SkyBeamStartTime);
    context.Transfer(MainBeamsStartTime);
    context.Transfer(BeamRotationStartSpeed);
    context.Transfer(BeamRotationEndSpeed);
    context.Transfer(TotalLifetime);
    context.Transfer(Damage);
    context.Transfer(InitSound);
    context.Transfer(LoopingSound);
}