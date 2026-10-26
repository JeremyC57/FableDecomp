// CEnvironmentThemePriorityStackEntry::TransitionToDefaultTheme(long a) -- target theme -1 at +0x14, the transition marked active, the start time stamped at +0x20, `a` kept at +0x28 and the current weight at +0 copied to +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00886400, length 36 bytes.
// Callees: GFGetTime @ 0x009E1BC0 (a double in st0, stored straight into +0x20). Shape: the -1 store is an immediate, the weight copy goes through ecx as a dword;
// `ret 4`.
double __fastcall GFGetTime();                        // 0x009E1BC0

struct CEnvironmentThemePriorityStackEntry {
    float         m_currentWeight;                 // +0x00
    unsigned char _pad[0x10];
    long          m_targetTheme;                   // +0x14
    float         m_weight;                        // +0x18
    bool          m_transitioning;                 // +0x1c
    unsigned char _pad1d[3];
    double        m_startTime;                     // +0x20
    long          m_transitionParam;               // +0x28
    void TransitionToDefaultTheme(long a);
};

void CEnvironmentThemePriorityStackEntry::TransitionToDefaultTheme(long a)
{
    m_targetTheme = -1;
    m_transitioning = true;
    m_startTime = GFGetTime();
    m_transitionParam = a;
    m_weight = m_currentWeight;
}