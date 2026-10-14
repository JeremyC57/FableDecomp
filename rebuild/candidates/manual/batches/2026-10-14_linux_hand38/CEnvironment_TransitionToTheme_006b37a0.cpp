// CEnvironment::TransitionToTheme(long globalThemeIndex, float duration) -- unless the environment theme is forced, map the global theme definition index to the theme-set slot (class index minus one) and, when it is in range, start the transition on the first entry of the first priority-stack channel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B37A0, length 82 bytes (the oracle row carries 14 bytes of int3 padding).
// Callees: CDefinitionManager::GetDefClassIndexFromGlobalIndex(long) @ 0x009ACCA0 through DefinitionManager (+4),
// CEnvironmentThemePriorityStackEntry::StartThemeTransition(long, float) @ 0x008861D0. Shape: `dec edi ; js` is the `- 1` and the `< 0` test,
// the theme-set count is `(end - begin) / 12` (the 0x2AAAAAAB multiply, 12-byte theme records), compared signed; the entry pointer is the
// first word of the first channel (`mov eax,[esi+48h] ; mov ecx,[eax]`): channels is a CArray of CEnvironmentThemePriorityStack whose only
// member is the CArray Ents. Offsets: ForcedEnvironmentTheme +0x3e, DefinitionManager +4, PriorityStackChannels +0x48 as in
// engine/CEnvironment.h; the theme set sits at +0x1954 in retail (the Ego PDB's +0x1a7c shifted by the 12-byte CArrays), modelled locally.
struct CDefinitionManager_006b37a0 {
    long GetDefClassIndexFromGlobalIndex(long globalIndex);   // 0x009ACCA0
};

struct CEnvironmentThemePriorityStackEntry_006b37a0 {
    void StartThemeTransition(long theme, float duration);     // 0x008861D0
};

struct CEnvironmentThemePriorityStack_006b37a0 {  // CArray<CEnvironmentThemePriorityStackEntry> Ents: begin, end, capacity
    CEnvironmentThemePriorityStackEntry_006b37a0* m_pBegin;
    CEnvironmentThemePriorityStackEntry_006b37a0* m_pEnd;
    CEnvironmentThemePriorityStackEntry_006b37a0* m_pCapacity;
};

struct SThemeRecord_006b37a0 { long m_a, m_b, m_c; };   // 12 bytes

struct CEnvironment {
    void*                         __vftable;
    CDefinitionManager_006b37a0*  DefinitionManager;       // +0x04
    unsigned char                 _pad0[0x3e - 0x8];
    bool                          ForcedEnvironmentTheme;  // +0x3e
    unsigned char                 _pad1[0x48 - 0x3f];
    CEnvironmentThemePriorityStack_006b37a0* PriorityStackChannels;   // +0x48 CArray begin
    unsigned char                 _pad2[0x1954 - 0x4c];
    SThemeRecord_006b37a0*        m_pThemesBegin;          // +0x1954
    SThemeRecord_006b37a0*        m_pThemesEnd;            // +0x1958
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
