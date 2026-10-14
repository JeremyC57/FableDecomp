// CEnvironmentThemePriorityStackEntry::StartThemeTransition(long theme, long a) -- record the target theme at +0x14, mark the transition active, stamp the start time at +0x20, keep `a` at +0x28 and the theme's weight at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008861D0, length 46 bytes.
// Callees: GFGetTime @ 0x009E1BC0 (a double in st0, stored straight into +0x20), GetThemeWeight @ 0x00886160 (on `this`, the theme; a float in st0 stored into +0x18).
// Shape: the theme stays in edi for the weight call, the argument `a` is stored between the push and the call; `ret 8`.
double __cdecl GFGetTime();                        // 0x009E1BC0

struct CEnvironmentThemePriorityStackEntry {
    unsigned char _pad[0x14];
    long          m_targetTheme;                   // +0x14
    float         m_weight;                        // +0x18
    bool          m_transitioning;                 // +0x1c
    unsigned char _pad1d[3];
    double        m_startTime;                     // +0x20
    long          m_transitionParam;               // +0x28
    float GetThemeWeight(long theme);              // 0x00886160
    void StartThemeTransition(long theme, long a);
};

void CEnvironmentThemePriorityStackEntry::StartThemeTransition(long theme, long a)
{
    m_targetTheme = theme;
    m_transitioning = true;
    m_startTime = GFGetTime();
    m_transitionParam = a;
    m_weight = GetThemeWeight(theme);
}