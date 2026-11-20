// CGameTimeManager::IsDay() const -- true while the time of day (Time, +0x08) lies in (0.3333, 0.7083]
// Leaf: ?IsDay@CGameTimeManager@@QBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB2C0, length 48 bytes.
// Identity (two sources): Ego_r.pdb 0x006F6A60 / FableWin.pdb 0x02460170 ?IsDay@CGameTimeManager@@QBE_NXZ; Ego_r's member order
//   IsDaySounds / IsNightTime / IsEvening / IsDawn / IsDay / SetAsUsingVillageTime / GetTimeAs24HourClock runs the same way in retail
//   (0x006BB1E0 / 0x006BB210 / .. / 0x006BB2C0 / 0x006BB2F0 / 0x006BB300); the callers reach it through CWorld +0x1C
//   (PGameTimeManager): CTCVillage torches / tavern lights 0x0065C781, CGameScriptInterface::MsgOnHeroSlept 0x00893EC0, the firefly
//   wander state 0x00911F80. NF122 (2026-11-20, q4490): was the non-const stand-in IsDay_006bb2c0 on a local class model.
// Callees: none. Constants 0.3333 @ 0x0125D624, 0.7083 @ 0x0125D628. Shape: `fcomp ; fnstsw ; test ah,41h ; jne` (not above the low
//   bound) then `... jp` (above the high bound), `mov al,1` / `xor al,al`.
// Header: engine/CGameTimeManager.h (FABLE_RETAIL_CGAMETIMEMANAGER + _ISDAY: Time +0x08).
extern const float g_dayStart_0125d624;   // 0x0125D624 (0.3333)
extern const float g_dayEnd_0125d628;   // 0x0125D628 (0.7083)
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_ISDAY
#include "engine/CGameTimeManager.h"

bool CGameTimeManager::IsDay() const
{
    if (Time > g_dayStart_0125d624 && Time <= g_dayEnd_0125d628)
        return true;
    return false;
}
