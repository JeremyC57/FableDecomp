// CEnvironment::TransitionToDefaultThemeAllChannels(long priority, float duration) -- unless the environment theme is forced, and for a priority level in 1..4, start the default-theme transition on entry `priority` of all six priority-stack channels
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B3950, length 68 bytes.
// Callees: CEnvironmentThemePriorityStackEntry::TransitionToDefaultTheme(float) @ 0x00886400. Shape: the range test is `jl 1` / `jge 5`; the entry
// offset `priority * 48` is formed once (`lea edi,[eax+eax*2] ; shl edi,4`: the retail entry is 48 bytes -- the PDB's 56 with a 12-byte CArray),
// the channel loop is strength-reduced to a byte offset stepping 12 through the six 12-byte channels (`cmp esi,48h`), the channel array
// base is re-read each iteration and the duration is pushed as a dword. Offsets: ForcedEnvironmentTheme +0x3e, PriorityStackChannels +0x48
// as in engine/CEnvironment.h.
struct CEnvironmentThemePriorityStackEntry_006b3950 {   // 48 bytes in retail
    unsigned char _body[0x30];
    void TransitionToDefaultTheme(float duration); // 0x00886400
};

struct CEnvironmentThemePriorityStack_006b3950 {  // CArray<CEnvironmentThemePriorityStackEntry> Ents: begin, end, capacity
    CEnvironmentThemePriorityStackEntry_006b3950* m_pBegin;
    CEnvironmentThemePriorityStackEntry_006b3950* m_pEnd;
    CEnvironmentThemePriorityStackEntry_006b3950* m_pCapacity;
};

struct CEnvironment {
    unsigned char                 _pad0[0x3e];
    bool                          ForcedEnvironmentTheme;  // +0x3e
    unsigned char                 _pad1[0x48 - 0x3f];
    CEnvironmentThemePriorityStack_006b3950* PriorityStackChannels;   // +0x48 CArray begin
    void TransitionToDefaultThemeAllChannels(long priority, float duration);
};

void CEnvironment::TransitionToDefaultThemeAllChannels(long priority, float duration)
{
    if (ForcedEnvironmentTheme)
        return;
    if (priority < 1 || priority >= 5)
        return;
    for (long channel = 0; channel < 6; ++channel)
        PriorityStackChannels[channel].m_pBegin[priority].TransitionToDefaultTheme(duration);
}
