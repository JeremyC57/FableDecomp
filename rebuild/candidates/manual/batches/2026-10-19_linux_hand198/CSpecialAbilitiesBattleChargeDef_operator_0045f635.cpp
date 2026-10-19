// CSpecialAbilitiesBattleChargeDef::operator=(const CSpecialAbilitiesBattleChargeDef&) -- memberwise copy: CParentDefClassBase part, run speed, the dash/radius/damage per-level vectors, the radial-blur tuning floats, the people-hit vector and the dragon gumph radii; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045F635, length 195 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D (three times),
//          vector<long>::operator= @ 0x00454886.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesBattleChargeDef's vtable 0x012340C4 is the 5-byte `jmp 0x0045F635` right before this body.
// PDB members +0x40.. sit at retail +0x3c.. with 12-byte retail vectors.
// Shape: memberwise assignment in declaration order; vectors out of line, floats as dword moves.
#pragma pack(push, 1)
struct CParentDefClassBase {
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

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CSpecialAbilitiesBattleChargeDef : CParentDefClassBase {   // vtable 0x012340C4
    float        RunSpeed;                              // +0x3c
    CFloatVector DashDistance;                          // +0x40
    CFloatVector RadiusOfEffect;                        // +0x4c
    CFloatVector BattleChargeDamage;                    // +0x58
    float        BattleChargeCombatMultiplierFactor;    // +0x64
    float        BattleChargeRadialStartSeconds;        // +0x68
    float        BattleChargeRadialEndSeconds;          // +0x6c
    float        BattleChargeRadialThreshold;           // +0x70
    float        BattleChargeRadialLength;              // +0x74
    float        BattleChargeRadialIntensity;           // +0x78
    float        BattleChargeRadialFade;                // +0x7c
    float        BattleChargeRadialRange;               // +0x80
    float        BattleChargeRadialTwist;               // +0x84
    float        PoseDelaySecs;                         // +0x88
    float        ChargeDelay;                           // +0x8c
    CLongVector  NumPeopleHit;                          // +0x90
    float        DragonExtraGumphRadiusMultiplier;      // +0x9c
    float        DragonExtraGumphRadius;                // +0xa0
    CSpecialAbilitiesBattleChargeDef& operator=(const CSpecialAbilitiesBattleChargeDef& other);
};

CSpecialAbilitiesBattleChargeDef& CSpecialAbilitiesBattleChargeDef::operator=(const CSpecialAbilitiesBattleChargeDef& other)
{
    CParentDefClassBase::operator=(other);
    RunSpeed = other.RunSpeed;
    DashDistance = other.DashDistance;
    RadiusOfEffect = other.RadiusOfEffect;
    BattleChargeDamage = other.BattleChargeDamage;
    BattleChargeCombatMultiplierFactor = other.BattleChargeCombatMultiplierFactor;
    BattleChargeRadialStartSeconds = other.BattleChargeRadialStartSeconds;
    BattleChargeRadialEndSeconds = other.BattleChargeRadialEndSeconds;
    BattleChargeRadialThreshold = other.BattleChargeRadialThreshold;
    BattleChargeRadialLength = other.BattleChargeRadialLength;
    BattleChargeRadialIntensity = other.BattleChargeRadialIntensity;
    BattleChargeRadialFade = other.BattleChargeRadialFade;
    BattleChargeRadialRange = other.BattleChargeRadialRange;
    BattleChargeRadialTwist = other.BattleChargeRadialTwist;
    PoseDelaySecs = other.PoseDelaySecs;
    ChargeDelay = other.ChargeDelay;
    NumPeopleHit = other.NumPeopleHit;
    DragonExtraGumphRadiusMultiplier = other.DragonExtraGumphRadiusMultiplier;
    DragonExtraGumphRadius = other.DragonExtraGumphRadius;
    return *this;
}
