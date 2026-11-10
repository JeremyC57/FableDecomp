// CEnvironment::SetThemeWeight(long channel, long priority, long globalThemeIndex, float weight) -- unless the environment theme is forced, map the global theme index to the theme-set slot and, when slot, channel (0..5) and priority (0..4) are all in range, set that theme's weight on the channel's priority entry
// Leaf: SetThemeWeight
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B3600, 124 bytes (+ int3 padding).
// Label correction: the manifest's "TransitionToTheme" is wrong -- the body calls SetThemeWeight, not StartThemeTransition
//   (the real 4-argument TransitionToTheme is 0x006B3800, the same body with the transition call).
// Callees: CDefinitionManager::GetDefClassIndexFromGlobalIndex(long) @ 0x009ACCA0 through DefinitionManager (+4),
//          CEnvironmentThemePriorityStackEntry::SetThemeWeight(long theme, float weight, bool) @ 0x00886D70 (called with true).
// Layout: ForcedEnvironmentTheme +0x3e, DefinitionManager +4, PriorityStackChannels +0x48 (engine/CEnvironment.h); the retail
//   theme set is at +0x1954 (the PDB's +0x1a7c shifted by the 12-byte retail CArrays), modelled locally like the landed
//   0x006B37A0. Channels are 12-byte CArrays of 48-byte entries (`lea ; shl 4` = *48, `[esi+edx*4]` with edx = 3*channel).
// Shape: `dec edi ; js` is the `- 1` and `< 0` test; the theme-set count is (end - begin) / 12 (0x2AAAAAAB, sar 1).
// Names (agentNF3, 2026-11-10): the address-suffixed local helper classes are gone; the callees bind the PDB spellings
//   ?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z and NEnvironmentInternal::CEnvironmentThemePriorityStackEntry members
//   (Ego_r.pdb / FableWin.pdb); the theme records are the retail 12-byte NEnvironmentInternal::CEnvironmentThemeSetDay.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX   // ?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z 0x009ACCA0 (hdr overlay)
#include "engine/CDefinitionManager.h"
#include "engine/CEnvironmentThemePriorityStackEntry.h"   // NEnvironmentInternal::CEnvironmentThemePriorityStackEntry (48 bytes) + CEnvironmentThemeSetDay

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStack { public:   // retail 12 bytes: its only member, the CArray<CEnvironmentThemePriorityStackEntry> of entries (begin, end, capacity)
    CEnvironmentThemePriorityStackEntry* m_pBegin;
    CEnvironmentThemePriorityStackEntry* m_pEnd;
    CEnvironmentThemePriorityStackEntry* m_pCapacity;
};
}

class CEnvironment { public:
    void*                         __vftable;
    CDefinitionManager*  DefinitionManager;       // +0x04
    unsigned char                 _pad0[0x3e - 0x8];
    bool                          ForcedEnvironmentTheme;  // +0x3e
    unsigned char                 _pad1[0x48 - 0x3f];
    NEnvironmentInternal::CEnvironmentThemePriorityStack* PriorityStackChannels;   // +0x48 CArray begin
    unsigned char                 _pad2[0x1954 - 0x4c];
    const NEnvironmentInternal::CEnvironmentThemeSetDay*        m_pThemesBegin;          // +0x1954
    const NEnvironmentInternal::CEnvironmentThemeSetDay*        m_pThemesEnd;            // +0x1958
    void SetThemeWeight(long channel, long priority, long globalThemeIndex, float weight);
};

void CEnvironment::SetThemeWeight(long channel, long priority, long globalThemeIndex, float weight)
{
    if (ForcedEnvironmentTheme)
        return;
    long theme = DefinitionManager->GetDefClassIndexFromGlobalIndex(globalThemeIndex) - 1;
    if (theme < 0)
        return;
    if (theme >= m_pThemesEnd - m_pThemesBegin)
        return;
    if (channel < 0 || channel >= 6)
        return;
    if (priority < 0 || priority >= 5)
        return;
    PriorityStackChannels[channel].m_pBegin[priority].SetThemeWeight(theme, weight, true);
}