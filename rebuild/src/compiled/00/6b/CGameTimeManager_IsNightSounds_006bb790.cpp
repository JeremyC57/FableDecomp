// CGameTimeManager::IsNotDay_006bb790() -- FALSE while the time of day (+8) lies in (0.1667, 0.875]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB790, length 56 bytes.
// Callees: none. Constants 0.1667 @ 0x0125D640, 0.875 @ 0x0124F6F4. Shape: `fcomp ; fnstsw ; test ah,41h ; jne` (not above the low bound) then `... jp` (above the high bound),
// `mov al,1` / `xor al,al`. This twin returns the NEGATION: the bool is materialised then inverted (`sete`) on both tails.
extern const float g_dayStart_0125d640;   // 0x0125D640 (0.1667)
extern const float g_dayEnd_0124f6f4;   // 0x0124F6F4 (0.875)
class CGameTimeManager { public:
    unsigned char _pad[8];
    float         m_timeOfDay;                     // +0x08 (a fraction of the day)
    bool IsNotDay_006bb790();
};

bool CGameTimeManager::IsNotDay_006bb790()
{
    bool day;
    if (m_timeOfDay > g_dayStart_0125d640 && m_timeOfDay <= g_dayEnd_0124f6f4)
        day = true;
    else
        day = false;
    return !day;
}