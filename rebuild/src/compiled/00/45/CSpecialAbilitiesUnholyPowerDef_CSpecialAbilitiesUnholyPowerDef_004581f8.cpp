// CSpecialAbilitiesUnholyPowerDef::CSpecialAbilitiesUnholyPowerDef() -- def base, own vtable, fifteen empty vectors (six CDefString sound lists, eight float tables, the stamina/damage tables); the scalars are left unset
// Retail: Fable.exe (TLC, VC7.1) @ 0x004581F8, 188 bytes. Vtable 0x012348E4. Callee: CParentDefClassBase::CParentDefClassBase @ 0x00430370.
// Layout: Ego_r.pdb CSpecialAbilitiesUnholyPowerDef minus 4 (0x3C-byte retail def base) with 12-byte retail vectors (the dtor 0x004582D6 frees the same offsets).
#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CDefString { public:
    long TablePos;
};
template <class T> struct CRetailVector {          // retail std::vector<T> (12 bytes)
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class CSpecialAbilitiesUnholyPowerDef : public CParentDefClassBase {   // vtable 0x012348E4
public:
    virtual void v000();
    CRetailVector<CDefString> StartPowerupSound;             // +0x3c
    CRetailVector<CDefString> LoopingPowerupSound;           // +0x48
    CRetailVector<CDefString> ReleaseSound;                  // +0x54
    CRetailVector<CDefString> ReleaseLoopSound;              // +0x60
    CRetailVector<CDefString> FinishSound;                   // +0x6c
    CRetailVector<CDefString> CancelSound;                   // +0x78
    float                     RandomFactorForNumSecsBetweenDamageHit;   // +0x84
    CRetailVector<float>      NumSecsBetweenDamageHit;       // +0x88
    CRetailVector<float>      DamagePerHit;                  // +0x94
    CRetailVector<float>      DurationOfSpellSecs;           // +0xa0
    CRetailVector<float>      RadiusOfEffect;                // +0xac
    CRetailVector<float>      EnvChangeSecs;                 // +0xb8
    CRetailVector<float>      EnvReturnSecs;                 // +0xc4
    bool                      DoLifting;                     // +0xd0
    float                     VelocityOfLift;                // +0xd4
    float                     DistanceToLift;                // +0xd8
    CRetailVector<float>      StaminaCostPerSec;             // +0xdc
    CRetailVector<float>      DamageTakenMultiplier;         // +0xe8
    float                     SmallRingRadius;               // +0xf4
    float                     LargeRingRadius;               // +0xf8
    CSpecialAbilitiesUnholyPowerDef();
};

CSpecialAbilitiesUnholyPowerDef::CSpecialAbilitiesUnholyPowerDef()
{
}
