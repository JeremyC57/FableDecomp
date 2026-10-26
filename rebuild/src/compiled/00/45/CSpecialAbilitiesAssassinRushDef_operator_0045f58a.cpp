// CSpecialAbilitiesAssassinRushDef::operator=(const CSpecialAbilitiesAssassinRushDef&) -- memberwise copy: CParentDefClassBase part, seventeen rush/blur tuning words, then the per-level dash-distance vector; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045F58A, length 141 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesAssassinRushDef's vtable 0x0123405C is the 5-byte `jmp 0x0045F58A` right before this
// body; PDB members +0x40..+0x84 sit at retail +0x3c..+0x80 (retail base 0x39 bytes, vector 12 bytes).
// Shape: memberwise assignment in declaration order; the last scalar's store sinks below the vector's
// argument setup (`add edi,80h ; push edi`) because the vector is the final member.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

class CSpecialAbilitiesAssassinRushDef : public CParentDefClassBase { public:   // vtable 0x0123405C
    float DistanceAwayFromtargetToAssassinRushTo;   // +0x3c
    float SafeCollideRadiusPercentage;              // +0x40
    float DistFromFloor;                            // +0x44
    float AssassinStartSeconds;                     // +0x48
    float AssassinEndSeconds;                       // +0x4c
    float AssassinThreshold;                        // +0x50
    float AssassinLength;                           // +0x54
    float AssassinIntensity;                        // +0x58
    float AssassinFade;                             // +0x5c
    float AssassinRange;                            // +0x60
    float AssassinTwist;                            // +0x64
    float RunSpeed;                                 // +0x68
    float TrailDelaySecs;                           // +0x6c
    float SlowTimeLength;                           // +0x70
    long  AssassinAlpha;                            // +0x74
    long  SlowTimeMultiplier;                       // +0x78
    float RushDelay;                                // +0x7c
    CFloatVector DashDistance;                      // +0x80
    CSpecialAbilitiesAssassinRushDef& operator=(const CSpecialAbilitiesAssassinRushDef& other);
};

CSpecialAbilitiesAssassinRushDef& CSpecialAbilitiesAssassinRushDef::operator=(const CSpecialAbilitiesAssassinRushDef& other)
{
    CParentDefClassBase::operator=(other);
    DistanceAwayFromtargetToAssassinRushTo = other.DistanceAwayFromtargetToAssassinRushTo;
    SafeCollideRadiusPercentage = other.SafeCollideRadiusPercentage;
    DistFromFloor = other.DistFromFloor;
    AssassinStartSeconds = other.AssassinStartSeconds;
    AssassinEndSeconds = other.AssassinEndSeconds;
    AssassinThreshold = other.AssassinThreshold;
    AssassinLength = other.AssassinLength;
    AssassinIntensity = other.AssassinIntensity;
    AssassinFade = other.AssassinFade;
    AssassinRange = other.AssassinRange;
    AssassinTwist = other.AssassinTwist;
    RunSpeed = other.RunSpeed;
    TrailDelaySecs = other.TrailDelaySecs;
    SlowTimeLength = other.SlowTimeLength;
    AssassinAlpha = other.AssassinAlpha;
    SlowTimeMultiplier = other.SlowTimeMultiplier;
    RushDelay = other.RushDelay;
    DashDistance = other.DashDistance;
    return *this;
}