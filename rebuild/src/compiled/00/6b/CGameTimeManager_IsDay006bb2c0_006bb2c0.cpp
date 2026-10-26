// CGameTimeManager::IsDay_006bb2c0() -- true while the time of day (+8) lies in (0.3333, 0.7083]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB2C0, length 48 bytes.
// Callees: none. Constants 0.3333 @ 0x0125D624, 0.7083 @ 0x0125D628. Shape: `fcomp ; fnstsw ; test ah,41h ; jne` (not above the low bound) then `... jp` (above the high bound),
// `mov al,1` / `xor al,al`. The chunk label "IsDay" is shared by three twins with different bounds.
extern const float g_dayStart_0125d624;   // 0x0125D624 (0.3333)
extern const float g_dayEnd_0125d628;   // 0x0125D628 (0.7083)
class CGameTimeManager { public:
    unsigned char _pad[8];
    float         m_timeOfDay;                     // +0x08 (a fraction of the day)
    bool IsDay_006bb2c0();
};

bool CGameTimeManager::IsDay_006bb2c0()
{
    if (m_timeOfDay > g_dayStart_0125d624 && m_timeOfDay <= g_dayEnd_0125d628)
        return true;
    return false;
}