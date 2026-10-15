// CEnvironment::TransitionToThemeAllChannels(long priority, long globalThemeIndex, float duration) -- unless the environment theme is forced, map the global theme index to the theme-set slot and, when it and the priority (0..4) are in range, start the transition to that theme on entry `priority` of all six priority-stack channels
// Leaf: TransitionToThemeAllChannels
// Label correction: the manifest repeats "SetThemeWeightAllChannels" here; the body calls StartThemeTransition (0x008861D0),
//   so this is the all-channels transition (the SetThemeWeight form is 0x006B3680).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B38D0, 120 bytes (+ int3 padding).
// Callees: CDefinitionManager::GetDefClassIndexFromGlobalIndex(long) @ 0x009ACCA0 through DefinitionManager (+4),
//          CEnvironmentThemePriorityStackEntry::StartThemeTransition(long theme, float duration) @ 0x008861D0.
// Layout: ForcedEnvironmentTheme +0x3e, DefinitionManager +4, PriorityStackChannels +0x48 (engine/CEnvironment.h); the retail
//   theme set is at +0x1954 (PDB +0x1a7c shifted by the 12-byte retail CArrays), modelled locally like the landed 0x006B37A0.
// Shape: `dec ebp ; js` is the `- 1` and `< 0` test; the theme-set count is (end - begin) / 12 (0x2AAAAAAB, sar 1); the
//   entry offset priority*48 is formed once (`lea edi,[edi+edi*2] ; shl edi,4`) and the channel loop is strength-reduced
//   to a byte offset stepping 12 through the six channels (`cmp esi,48h`), re-reading the channel array base each pass
//   (same loop as the landed TransitionToDefaultThemeAllChannels 0x006B3950).
struct CDefinitionManager_006b38d0 {
    long GetDefClassIndexFromGlobalIndex(long globalIndex);       // 0x009ACCA0
};

struct CEnvironmentThemePriorityStackEntry_006b38d0 {            // 48 bytes in retail
    unsigned char _body[0x30];
    void StartThemeTransition(long theme, float duration);       // 0x008861D0
};

struct CEnvironmentThemePriorityStack_006b38d0 {  // CArray<CEnvironmentThemePriorityStackEntry> Ents: begin, end, capacity
    CEnvironmentThemePriorityStackEntry_006b38d0* m_pBegin;
    CEnvironmentThemePriorityStackEntry_006b38d0* m_pEnd;
    CEnvironmentThemePriorityStackEntry_006b38d0* m_pCapacity;
};

struct SThemeRecord_006b38d0 { long m_a, m_b, m_c; };   // 12 bytes

struct CEnvironment {
    void*                         __vftable;
    CDefinitionManager_006b38d0*  DefinitionManager;       // +0x04
    unsigned char                 _pad0[0x3e - 0x8];
    bool                          ForcedEnvironmentTheme;  // +0x3e
    unsigned char                 _pad1[0x48 - 0x3f];
    CEnvironmentThemePriorityStack_006b38d0* PriorityStackChannels;   // +0x48 CArray begin
    unsigned char                 _pad2[0x1954 - 0x4c];
    SThemeRecord_006b38d0*        m_pThemesBegin;          // +0x1954
    SThemeRecord_006b38d0*        m_pThemesEnd;            // +0x1958
    void TransitionToThemeAllChannels(long priority, long globalThemeIndex, float duration);
};

void CEnvironment::TransitionToThemeAllChannels(long priority, long globalThemeIndex, float duration)
{
    if (ForcedEnvironmentTheme)
        return;
    long theme = DefinitionManager->GetDefClassIndexFromGlobalIndex(globalThemeIndex) - 1;
    if (theme < 0)
        return;
    if (theme >= m_pThemesEnd - m_pThemesBegin)
        return;
    if (priority < 0 || priority >= 5)
        return;
    for (long channel = 0; channel < 6; ++channel)
        PriorityStackChannels[channel].m_pBegin[priority].StartThemeTransition(theme, duration);
}
