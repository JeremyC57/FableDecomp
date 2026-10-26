// CThingPhysical::SetMaxHealth(float, bool) -- sets MaxHealth; optionally grows Health by the increase, then clamps Health to MaxHealth
// Retail: Fable.exe 0x006A5260, 96 bytes, no callees.
// Retail CThingPhysical sits 0x18 below the PDB layout here (CThing 0x10 narrower, the
// HealthOverTimeEffects list 4 bytes instead of 12): MaxHealth +0xb0 (PDB +0xc8), Health +0xb4 (PDB
// +0xcc). The generated CThingPhysical.h keeps the PDB offsets, so the class is modelled locally.
// Shape: the old maximum is a float local (the `push ecx` slot); the increase is added with
// `Health += new - old` (difference first, `fadd [Health]`). The final clamp re-reads MaxHealth from
// memory: it is an inline helper taking the maximum by const reference (a plain `if (Health > MaxHealth)`
// forwards the stored parameter instead, `fcomp [esp+8]`).
// Layout (supersedes any local-model note above): engine/CThingPhysical.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CThingPhysical CThingPhysicalDonor
#include "engine/CThingPhysical.h"
#undef CThingPhysical
template <class T> inline void ClampMax(T& value, const T& maximum)
{
    if (value > maximum)
        value = maximum;
}

class CThingPhysical : public CThingPhysicalDonor {
 public:
    void SetMaxHealth(float maxHealth, bool adjustHealth);
};

void CThingPhysical::SetMaxHealth(float maxHealth, bool adjustHealth)
{
    float oldMaxHealth = MaxHealth;
    MaxHealth = maxHealth;
    if (adjustHealth && maxHealth > oldMaxHealth)
        Health += maxHealth - oldMaxHealth;
    ClampMax(Health, MaxHealth);
}