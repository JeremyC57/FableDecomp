// CSpecialAbilitiesDivineWrathDef::operator= -- assign the complete spell definition.
// Retail00460657 length309; identity via Copy00460652. Actual donor headers
// included below. Native base0x39 vs donor0x3d, native vectors12 vs donor16
// move first member40->3c and final rings130/134->f4/f8.
// Callees431ed0 parent assignment,46078c vector<CDefString> assignment,
// 45ef2d vector<float> assignment; vector bodies and element4 sizes audited.
#pragma optimize("s",on)
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
struct CDefStringVector {
    CDefString *Start,*Finish,*End;
    CDefStringVector& operator=(const CDefStringVector&); // 0x0046078c
};
struct CFloatVector {
    float *Start,*Finish,*End;
    CFloatVector& operator=(const CFloatVector&); // 0x0045ef2d
};
class CSpecialAbilitiesDivineWrathDef : public CParentDefClassBase { public:
    CDefStringVector StartPowerupSound,LoopingPowerupSound,ReleaseSound,ReleaseLoopSound,FinishSound,CancelSound;
    float RandomFactorForNumSecsBetweenDamageHit;
    CFloatVector NumSecsBetweenDamageHit,DamagePerHit,DurationOfSpellSecs,RadiusOfEffect,EnvChangeSecs,EnvReturnSecs;
    bool DoLifting; float VelocityOfLift,DistanceToLift;
    CFloatVector StaminaCostPerSec,DamageTakenMultiplier;
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