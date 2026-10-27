// CEnvironment::RestoreDefaultWeightAllChannels(long priority, float weight) -- unless the environment theme is forced, and for a priority level in 1..4, set the default-theme weight on entry `priority` of all six priority-stack channels
// Leaf: ?RestoreDefaultWeightAllChannels@CEnvironment@@QAEXJM@Z
// Retail: Fable.exe @ 0x006B3750, 70 bytes, thiscall `ret 8`.
// Identity (probable): Ego_r.pdb `?RestoreDefaultWeightAllChannels@CEnvironment@@QAEXJM@Z` is the one (long, float)
// CEnvironment method between SetThemeWeightAllChannels (landed 0x006B3680) and TransitionToTheme(long, float) (landed
// 0x006B37A0) in both the PDB and retail order; the body has the AllChannels shape of the landed
// TransitionToDefaultThemeAllChannels 0x006B3950 (forced-theme guard, `jl 1` / `jge 5`, six 12-byte channels, 48-byte
// entries) and calls CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight(float, bool) 0x00886430 (Ego_r
// `?SetDefaultThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXM_N@Z`, the entry's one (float,
// bool) method) with true. Caller 0x00884E50 passes priority 2.
// Layout: engine/CEnvironment.h (ForcedEnvironmentTheme +0x3e, PriorityStackChannels +0x48 CArray begin).
#define CEnvironment DonorCEnvironment
#include "engine/CEnvironment.h"
#undef CEnvironment

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry { public:     // 48 bytes in retail
    unsigned char _body[0x30];
    void SetDefaultThemeWeight(float weight, bool unknown);  // 0x00886430
};
class CEnvironmentThemePriorityStack { public:          // CArray<CEnvironmentThemePriorityStackEntry> Ents
    CEnvironmentThemePriorityStackEntry* m_pBegin;
    CEnvironmentThemePriorityStackEntry* m_pEnd;
    CEnvironmentThemePriorityStackEntry* m_pCapacity;
};
}

class CEnvironment : public DonorCEnvironment { public:
    NEnvironmentInternal::CEnvironmentThemePriorityStack* GetChannels() const
    {
        return *(NEnvironmentInternal::CEnvironmentThemePriorityStack* const*)PriorityStackChannels;
    }
    void RestoreDefaultWeightAllChannels(long priority, float weight);
};

void CEnvironment::RestoreDefaultWeightAllChannels(long priority, float weight)
{
    if (ForcedEnvironmentTheme)
        return;
    if (priority < 1 || priority >= 5)
        return;
    for (long channel = 0; channel < 6; ++channel)
        GetChannels()[channel].m_pBegin[priority].SetDefaultThemeWeight(weight, true);
}