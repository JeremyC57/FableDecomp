// CSpecialAbilitiesUnholyPowerDef::operator=(const CSpecialAbilitiesUnholyPowerDef&) -- memberwise copy: CParentDefClassBase part, the six sound-name vectors, the damage-hit timing, the per-level float vectors, the lift settings and the ring radii; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004608C6, 309 bytes, `ret 4`.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (as the landed sibling 0x00460256).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
//   CSpecialAbilitiesUnholyPowerDef's vtable 0x012348E4 is the 5-byte `jmp 0x004608C6` at 0x004608C1, and the Ego_r.pdb
//   layout sits at the copied offsets with the retail 12-byte vectors (PDB +0x40.. -> retail +0x3c..).
// Callees: CParentDefClassBase::operator= 0x00431ED0, vector<CDefString>::operator= 0x0046078C, vector<float>::operator=
//   0x0045EF2D.
// Shape: the compiler-style memberwise assignment written out in declaration order; scalars as moves through eax.
// NF48 (round 7): the member vectors are STLport 4.6.2 std::vector<float> (was CFloatVector; operator= 0x0045EF2D), std::vector<CDefString> (was CDefStringVector; operator= 0x0046078C) from the shared stlport/fable_containers.h
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

class CDefString { public: long Index; };

class CSpecialAbilitiesUnholyPowerDef : public CParentDefClassBase { public:   // vtable 0x012348E4
    std::vector<CDefString> StartPowerupSound;                   // +0x3c
    std::vector<CDefString> LoopingPowerupSound;                 // +0x48
    std::vector<CDefString> ReleaseSound;                        // +0x54
    std::vector<CDefString> ReleaseLoopSound;                    // +0x60
    std::vector<CDefString> FinishSound;                         // +0x6c
    std::vector<CDefString> CancelSound;                         // +0x78
    float            RandomFactorForNumSecsBetweenDamageHit;   // +0x84
    std::vector<float>     NumSecsBetweenDamageHit;             // +0x88
    std::vector<float>     DamagePerHit;                        // +0x94
    std::vector<float>     DurationOfSpellSecs;                 // +0xa0
    std::vector<float>     RadiusOfEffect;                      // +0xac
    std::vector<float>     EnvChangeSecs;                       // +0xb8
    std::vector<float>     EnvReturnSecs;                       // +0xc4
    bool             DoLifting;                           // +0xd0
    float            VelocityOfLift;                      // +0xd4
    float            DistanceToLift;                      // +0xd8
    std::vector<float>     StaminaCostPerSec;                   // +0xdc
    std::vector<float>     DamageTakenMultiplier;               // +0xe8
    float            SmallRingRadius;                     // +0xf4
    float            LargeRingRadius;                     // +0xf8
    CSpecialAbilitiesUnholyPowerDef& operator=(const CSpecialAbilitiesUnholyPowerDef& other);
};

CSpecialAbilitiesUnholyPowerDef& CSpecialAbilitiesUnholyPowerDef::operator=(const CSpecialAbilitiesUnholyPowerDef& other)
{
    CParentDefClassBase::operator=(other);
    StartPowerupSound = other.StartPowerupSound;
    LoopingPowerupSound = other.LoopingPowerupSound;
    ReleaseSound = other.ReleaseSound;
    ReleaseLoopSound = other.ReleaseLoopSound;
    FinishSound = other.FinishSound;
    CancelSound = other.CancelSound;
    RandomFactorForNumSecsBetweenDamageHit = other.RandomFactorForNumSecsBetweenDamageHit;
    NumSecsBetweenDamageHit = other.NumSecsBetweenDamageHit;
    DamagePerHit = other.DamagePerHit;
    DurationOfSpellSecs = other.DurationOfSpellSecs;
    RadiusOfEffect = other.RadiusOfEffect;
    EnvChangeSecs = other.EnvChangeSecs;
    EnvReturnSecs = other.EnvReturnSecs;
    DoLifting = other.DoLifting;
    VelocityOfLift = other.VelocityOfLift;
    DistanceToLift = other.DistanceToLift;
    StaminaCostPerSec = other.StaminaCostPerSec;
    DamageTakenMultiplier = other.DamageTakenMultiplier;
    SmallRingRadius = other.SmallRingRadius;
    LargeRingRadius = other.LargeRingRadius;
    return *this;
}