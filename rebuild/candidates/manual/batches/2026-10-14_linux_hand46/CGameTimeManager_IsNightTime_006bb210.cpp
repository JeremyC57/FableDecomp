// CGameTimeManager::IsNightTime() -- true while the time of day (+8) lies in (0.7917, 1.0] or in [0.0, 0.25]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB210, length 70 bytes.
// Callees: none. Constants 0.7917 @ 0x0125D62C, 1.0 @ 0x0122DED8, 0.25 @ 0x0125D620, 0.0 @ 0x0122DEDC. Shape: four `fcomp ; fnstsw` tests (`test ah,41h ; jne` /
// `jnp` / `jp`, then `test ah,1 ; jne` for the final `>= 0`), `mov al,1` / `xor al,al`.
extern const float g_nightStart_0125d62c;   // 0x0125D62C (0.7917)
extern const float g_one_0122ded8;          // 0x0122DED8 (1.0)
extern const float g_nightEnd_0125d620;     // 0x0125D620 (0.25)
extern const float g_zero_0122dedc;         // 0x0122DEDC (0.0)
struct CGameTimeManager {
    unsigned char _pad[8];
    float         m_timeOfDay;                     // +0x08 (a fraction of the day)
    bool IsNightTime();
};

bool CGameTimeManager::IsNightTime()
{
    if (m_timeOfDay > g_nightStart_0125d62c && m_timeOfDay <= g_one_0122ded8)
        return true;
    if (m_timeOfDay <= g_nightEnd_0125d620 && m_timeOfDay >= g_zero_0122dedc)
        return true;
    return false;
}
