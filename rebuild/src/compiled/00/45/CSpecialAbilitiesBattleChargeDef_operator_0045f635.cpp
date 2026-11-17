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
// NF48 (round 7): the member vectors are STLport 4.6.2 std::vector<long> (was CLongVector; operator= 0x00454886), std::vector<float> (was CFloatVector; operator= 0x0045EF2D) from the shared stlport/fable_containers.h
// (local stand-ins before); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares the out-of-line operator=.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)



class CSpecialAbilitiesBattleChargeDef : public CParentDefClassBase { public:   // vtable 0x012340C4
    float        RunSpeed;                              // +0x3c
    std::vector<float> DashDistance;                          // +0x40
    std::vector<float> RadiusOfEffect;                        // +0x4c
    std::vector<float> BattleChargeDamage;                    // +0x58
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
    std::vector<long>  NumPeopleHit;                          // +0x90
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