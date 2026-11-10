// CEnvironment::TransitionToTheme(long globalThemeIndex, float duration) -- unless the environment theme is forced, map the global theme definition index to the theme-set slot (class index minus one) and, when it is in range, start the transition on the first entry of the first priority-stack channel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B37A0, length 82 bytes (the oracle row carries 14 bytes of int3 padding).
// Callees: CDefinitionManager::GetDefClassIndexFromGlobalIndex(long) @ 0x009ACCA0 through DefinitionManager (+4),
// CEnvironmentThemePriorityStackEntry::StartThemeTransition(long, float) @ 0x008861D0. Shape: `dec edi ; js` is the `- 1` and the `< 0` test,
// the theme-set count is `(end - begin) / 12` (the 0x2AAAAAAB multiply, 12-byte theme records), compared signed; the entry pointer is the
// first word of the first channel (`mov eax,[esi+48h] ; mov ecx,[eax]`): channels is a CArray of CEnvironmentThemePriorityStack whose only
// member is the CArray Ents. Offsets: ForcedEnvironmentTheme +0x3e, DefinitionManager +4, PriorityStackChannels +0x48 as in
// engine/CEnvironment.h; the theme set sits at +0x1954 in retail (the Ego PDB's +0x1a7c shifted by the 12-byte CArrays), modelled locally.
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
    void TransitionToTheme(long globalThemeIndex, float duration);
};

void CEnvironment::TransitionToTheme(long globalThemeIndex, float duration)
{
    if (ForcedEnvironmentTheme)
        return;
    long theme = DefinitionManager->GetDefClassIndexFromGlobalIndex(globalThemeIndex) - 1;
    if (theme < 0)
        return;
    if (theme >= m_pThemesEnd - m_pThemesBegin)
        return;
    PriorityStackChannels[0].m_pBegin[0].StartThemeTransition(theme, duration);
}