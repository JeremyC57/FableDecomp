// CSpecialAbilitiesDivineWrathDef::operator= -- assign the complete spell definition.
// Retail00460657 length309; identity via Copy00460652. Actual donor headers
// included below. Native base0x39 vs donor0x3d, native vectors12 vs donor16
// move first member40->3c and final rings130/134->f4/f8.
// Callees431ed0 parent assignment,46078c vector<CDefString> assignment,
// 45ef2d vector<float> assignment; vector bodies and element4 sizes audited.
#pragma optimize("s",on)
// NF48 (round 7): the member vectors are STLport 4.6.2 std::vector<float> (was CFloatVector; operator= 0x0045EF2D), std::vector<CDefString> (was CDefStringVector; operator= 0x0046078C) from the shared stlport/fable_containers.h
// (local stand-ins before); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares the out-of-line operator=.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#define CSpecialAbilitiesDivineWrathDef CSpecialAbilitiesDivineWrathDefDonor
#include "engine/_quarantine/CSpecialAbilitiesDivineWrathDef.h"
#undef CSpecialAbilitiesDivineWrathDef
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#endif
#pragma pack(push,1)
class CParentDefClassBase { public:
    void* __vftable;
    unsigned char DefClassBase[0x21]; // donor named flattened base, actual bytes4..24
    S32 InstantiationName_TablePos;
    unsigned char SubDefInfoMap[0x10]; // native vector12 + comparison/flags4
    CParentDefClassBase& operator=(const CParentDefClassBase&); // 0x00431ed0
};
#pragma pack(pop)
class CDefString { public: S32 TablePos; };
class CSpecialAbilitiesDivineWrathDef : public CParentDefClassBase { public:
    std::vector<CDefString> StartPowerupSound,LoopingPowerupSound,ReleaseSound,ReleaseLoopSound,FinishSound,CancelSound;
    float RandomFactorForNumSecsBetweenDamageHit;
    std::vector<float> NumSecsBetweenDamageHit,DamagePerHit,DurationOfSpellSecs,RadiusOfEffect,EnvChangeSecs,EnvReturnSecs;
    bool DoLifting; float VelocityOfLift,DistanceToLift;
    std::vector<float> StaminaCostPerSec,DamageTakenMultiplier;
    float SmallRingRadius,LargeRingRadius;
    CSpecialAbilitiesDivineWrathDef& operator=(const CSpecialAbilitiesDivineWrathDef&);
};
CSpecialAbilitiesDivineWrathDef& CSpecialAbilitiesDivineWrathDef::operator=(const CSpecialAbilitiesDivineWrathDef& o) {
    CParentDefClassBase::operator=(o);
    StartPowerupSound=o.StartPowerupSound;LoopingPowerupSound=o.LoopingPowerupSound;
    ReleaseSound=o.ReleaseSound;ReleaseLoopSound=o.ReleaseLoopSound;
    FinishSound=o.FinishSound;CancelSound=o.CancelSound;
    RandomFactorForNumSecsBetweenDamageHit=o.RandomFactorForNumSecsBetweenDamageHit;
    NumSecsBetweenDamageHit=o.NumSecsBetweenDamageHit;DamagePerHit=o.DamagePerHit;
    DurationOfSpellSecs=o.DurationOfSpellSecs;RadiusOfEffect=o.RadiusOfEffect;
    EnvChangeSecs=o.EnvChangeSecs;EnvReturnSecs=o.EnvReturnSecs;
    DoLifting=o.DoLifting;VelocityOfLift=o.VelocityOfLift;DistanceToLift=o.DistanceToLift;
    StaminaCostPerSec=o.StaminaCostPerSec;DamageTakenMultiplier=o.DamageTakenMultiplier;
    SmallRingRadius=o.SmallRingRadius;LargeRingRadius=o.LargeRingRadius;
    return *this;
}