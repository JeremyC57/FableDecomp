// CKrakenDef::Transfer(CPersistContext& context) -- persist the Kraken boss definition's tuning fields in declaration order
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E80E3, length 222 bytes (`ret 4`).
// Callees: CPersistContext::Transfer(float&) @ 0x00431061, CPersistContext::Transfer(long&) @ 0x00431102,
// CPersistContext::Transfer(vector<long>&) @ 0x00466A47 (the per-type instantiation also used by the landed 0x00467553).
// Layout (Ego_r.pdb CKrakenDef, 0x80 bytes): HeightOffset +0x28 .. TicksBetweenBeamExplosions +0x68, TentacleDefIndex (vector<long>) +0x6C;
// retail's STLport vector is 12 bytes (PDB 16), so TentacleMaxRiseDelay sits at +0x78 (PDB +0x7C). No engine header exists for CKrakenDef.
// Size TU (`#pragma optimize("s", on)`: every address goes through eax). Shape: one call per member, the context kept in edi, the last member's address formed with `add esi,78h` (the natural `&m_last`).
#pragma optimize("s", on)
struct CLongVector {                            // retail STLport vector<long>: three pointers
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
class CPersistContext { public:
    void Transfer(float& value);                // 0x00431061
    void Transfer(long& value);                 // 0x00431102
    void Transfer(CLongVector& value);          // 0x00466A47
};
class CKrakenDef { public:
    void*         __vftable;                    // +0x00 (CDefClassBase)
    unsigned char _base[0x24];                  // +0x04 .. +0x27
    float HeightOffset;                         // +0x28
    float PoolRadius;                           // +0x2c
    float HeadRiseDelay;                        // +0x30
    float HeadIdleTime;                         // +0x34
    long  LowStrikesBeforeHigh;                 // +0x38
    long  LowStrikeChargeLoops;                 // +0x3c
    long  LowStrikeAttackLoops;                 // +0x40
    long  HighStrikeAttackLoops;                // +0x44
    long  RoarLoops;                            // +0x48
    float DelayBeforeLowBeam;                   // +0x4c
    float DelayBeforeHighBeam;                  // +0x50
    float BeamExplosionRadius;                  // +0x54
    float BeamTrackSpeed;                       // +0x58
    float TargetingTrackSpeed;                  // +0x5c
    float BeamMouthDistLow;                     // +0x60
    float BeamMouthDistHigh;                    // +0x64
    long  TicksBetweenBeamExplosions;           // +0x68
    CLongVector TentacleDefIndex;               // +0x6c
    float TentacleMaxRiseDelay;                 // +0x78
    void Transfer(CPersistContext& context);
};

void CKrakenDef::Transfer(CPersistContext& context)
{
    context.Transfer(HeightOffset);
    context.Transfer(PoolRadius);
    context.Transfer(HeadRiseDelay);
    context.Transfer(HeadIdleTime);
    context.Transfer(LowStrikesBeforeHigh);
    context.Transfer(LowStrikeChargeLoops);
    context.Transfer(LowStrikeAttackLoops);
    context.Transfer(HighStrikeAttackLoops);
    context.Transfer(RoarLoops);
    context.Transfer(DelayBeforeLowBeam);
    context.Transfer(DelayBeforeHighBeam);
    context.Transfer(BeamExplosionRadius);
    context.Transfer(BeamTrackSpeed);
    context.Transfer(TargetingTrackSpeed);
    context.Transfer(BeamMouthDistLow);
    context.Transfer(BeamMouthDistHigh);
    context.Transfer(TicksBetweenBeamExplosions);
    context.Transfer(TentacleDefIndex);
    context.Transfer(TentacleMaxRiseDelay);
}