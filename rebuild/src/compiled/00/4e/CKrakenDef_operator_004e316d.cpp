// CKrakenDef::operator=(const CKrakenDef&) -- def copy: CDefClassBase part, then the CKrakenDef fields memberwise in declaration order
// Leaf: ??4CKrakenDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E316D, 141 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E3168 in slot 19 of vtable 0x01241B2C = CKrakenDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<long>::operator= @ 0x00454886.
// Layout (engine/CKrakenDef.h, TentacleMaxRiseDelay 4 below the PDB): HeightOffset +0x28, PoolRadius +0x2c, HeadRiseDelay +0x30, HeadIdleTime +0x34, LowStrikesBeforeHigh +0x38, LowStrikeChargeLoops +0x3c, LowStrikeAttackLoops +0x40, HighStrikeAttackLoops +0x44, RoarLoops +0x48, DelayBeforeLowBeam +0x4c, DelayBeforeHighBeam +0x50, BeamExplosionRadius +0x54, BeamTrackSpeed +0x58, TargetingTrackSpeed +0x5c, BeamMouthDistLow +0x60, BeamMouthDistHigh +0x64, TicksBetweenBeamExplosions +0x68, TentacleDefIndex +0x6c, TentacleMaxRiseDelay +0x78.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
struct CLongVector {                       // retail std::vector<long> (retail 0xc bytes)
    unsigned long m_words[3];
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

// def vtable 0x01241B2C: slot 0 0x004E13C3 (implicit inline dtor); real overrides from the def vtable scan
class CKrakenDef : public CDefClassBase { public:
    float                    HeightOffset;                // +0x28
    float                    PoolRadius;                  // +0x2c
    float                    HeadRiseDelay;               // +0x30
    float                    HeadIdleTime;                // +0x34
    long                     LowStrikesBeforeHigh;        // +0x38
    long                     LowStrikeChargeLoops;        // +0x3c
    long                     LowStrikeAttackLoops;        // +0x40
    long                     HighStrikeAttackLoops;       // +0x44
    long                     RoarLoops;                   // +0x48
    float                    DelayBeforeLowBeam;          // +0x4c
    float                    DelayBeforeHighBeam;         // +0x50
    float                    BeamExplosionRadius;         // +0x54
    float                    BeamTrackSpeed;              // +0x58
    float                    TargetingTrackSpeed;         // +0x5c
    float                    BeamMouthDistLow;            // +0x60
    float                    BeamMouthDistHigh;           // +0x64
    long                     TicksBetweenBeamExplosions;  // +0x68
    CLongVector              TentacleDefIndex;            // +0x6c
    float                    TentacleMaxRiseDelay;        // +0x78
    CKrakenDef& operator=(const CKrakenDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E80E3
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E3168
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DEFA3
};

CKrakenDef& CKrakenDef::operator=(const CKrakenDef& other)
{
    CDefClassBase::operator=(other);
    HeightOffset = other.HeightOffset;
    PoolRadius = other.PoolRadius;
    HeadRiseDelay = other.HeadRiseDelay;
    HeadIdleTime = other.HeadIdleTime;
    LowStrikesBeforeHigh = other.LowStrikesBeforeHigh;
    LowStrikeChargeLoops = other.LowStrikeChargeLoops;
    LowStrikeAttackLoops = other.LowStrikeAttackLoops;
    HighStrikeAttackLoops = other.HighStrikeAttackLoops;
    RoarLoops = other.RoarLoops;
    DelayBeforeLowBeam = other.DelayBeforeLowBeam;
    DelayBeforeHighBeam = other.DelayBeforeHighBeam;
    BeamExplosionRadius = other.BeamExplosionRadius;
    BeamTrackSpeed = other.BeamTrackSpeed;
    TargetingTrackSpeed = other.TargetingTrackSpeed;
    BeamMouthDistLow = other.BeamMouthDistLow;
    BeamMouthDistHigh = other.BeamMouthDistHigh;
    TicksBetweenBeamExplosions = other.TicksBetweenBeamExplosions;
    TentacleDefIndex = other.TentacleDefIndex;
    TentacleMaxRiseDelay = other.TentacleMaxRiseDelay;
    return *this;
}