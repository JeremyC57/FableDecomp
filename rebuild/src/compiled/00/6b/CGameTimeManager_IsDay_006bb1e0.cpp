// CGameTimeManager::IsDay() -- true while the time of day (+8) lies in (0.1667, 0.875]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB1E0, length 48 bytes.
// Callees: none. Constants 0.1667 @ 0x0125D640, 0.875 @ 0x0124F6F4. Shape: `fcomp ; fnstsw ; test ah,41h ; jne` (not above the low bound) then `... jp` (above the high bound),
// `mov al,1` / `xor al,al`.
extern const float g_dayStart_0125d640;   // 0x0125D640 (0.1667)
extern const float g_dayEnd_0124f6f4;   // 0x0124F6F4 (0.875)
class CGameTimeManager { public:
    unsigned char _pad[8];
    float         m_timeOfDay;                     // +0x08 (a fraction of the day)
    bool IsDay();
};

bool CGameTimeManager::IsDay()
{
    if (m_timeOfDay > g_dayStart_0125d640 && m_timeOfDay <= g_dayEnd_0124f6f4)
        return true;
    return false;
}