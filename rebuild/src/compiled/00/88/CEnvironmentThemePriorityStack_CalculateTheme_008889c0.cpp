// CEnvironmentThemePriorityStack::CalculateTheme(CBlendedEnvironmentTheme* theme, float time, CEnvironmentThemeResourcePallette* pallette) -- take the bottom entry's level theme, then blend in every higher-priority entry whose base weight leaves room (1 - weight above 1/1024)
// Retail: Fable.exe (TLC, VC7.1) @ 0x008889C0, 150 bytes (+ int3 padding).
// Callees: CEnvironmentThemePriorityStackEntry::GetLevelTheme(CBlendedEnvironmentTheme*, float, CEnvironmentThemeResourcePallette*)
//   @ 0x008884D0, CEnvironmentThemePriorityStackEntry::BlendInLevel(CBlendedEnvironmentTheme* out, const CBlendedEnvironmentTheme* in,
//   float, CEnvironmentThemeResourcePallette*) @ 0x008886E0 (called with the same theme as source and destination).
//   Float pool: 0x0122DED8 = 1.0f, 0x01278080 = 0.0009765625f (1/1024).
// Layout (Ego_r.pdb NEnvironmentInternal::CEnvironmentThemePriorityStack): Ents, a retail 12-byte CArray of 48-byte entries
//   (BaseWeight at +0 of each entry).
// Shape: the entry count `(end - begin) / 48` (0x2AAAAAAB, sar 3) is re-derived each pass, the loop starts at 1 with a
//   strength-reduced byte offset (ebp += 0x30); `1.0f - BaseWeight` is compared with `test ah,41h ; jne` (`>`).
struct CBlendedEnvironmentTheme;
struct CEnvironmentThemeResourcePallette;

struct CEnvironmentThemePriorityStackEntry {     // 48 bytes in retail
    float         BaseWeight;                    // +0x00
    unsigned char _body[0x2c];
    void GetLevelTheme(CBlendedEnvironmentTheme* theme, float time, CEnvironmentThemeResourcePallette* pallette);   // 0x008884D0
    void BlendInLevel(CBlendedEnvironmentTheme* out, const CBlendedEnvironmentTheme* in, float time,
                      CEnvironmentThemeResourcePallette* pallette);                                              // 0x008886E0
};

template <class T> class CArray { public:               // retail CArray: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

struct CEnvironmentThemePriorityStack {
    CArray<CEnvironmentThemePriorityStackEntry> Ents;   // +0x00
    void CalculateTheme(CBlendedEnvironmentTheme* theme, float time, CEnvironmentThemeResourcePallette* pallette);
};

void CEnvironmentThemePriorityStack::CalculateTheme(CBlendedEnvironmentTheme* theme, float time, CEnvironmentThemeResourcePallette* pallette)
{
    Ents[0].GetLevelTheme(theme, time, pallette);
    for (unsigned long i = 1; i < Ents.Size(); ++i) {
        CEnvironmentThemePriorityStackEntry& entry = Ents[i];
        if (1.0f - entry.BaseWeight > 0.0009765625f)
            entry.BlendInLevel(theme, theme, time, pallette);
    }
}