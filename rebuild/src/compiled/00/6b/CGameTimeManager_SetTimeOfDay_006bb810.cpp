// CGameTimeManager::SetTimeOfDay(float time) -- set the time of day (a fraction of the day), clamped to [0, 1]
// Leaf: ?SetTimeOfDay@CGameTimeManager@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB810, length 59 bytes.
// Identity (NF166, round 10, q11765): FableWin.pdb 0x02461020 / Ego_r.pdb 0x006F70A0 ?SetTimeOfDay@CGameTimeManager@@QAEXM@Z. Retail
//   0x0057C83A calls it on the world's game time manager (CWorld PGameTimeManager +0x1C) with 0.875f / 10/24 after IsNightTime 0x006BB210 /
//   IsDay 0x006BB2C0, and CGameScriptInterface::SetTimeOfDay 0x0088FDC0 with hours / 24; the address sits in that class's run
//   (0x006BB210 .. 0x006BB810). The body writes Time (+0x08, engine/CGameTimeManager.h). Was CThingDrawInformation::SetBarPercentage, a
//   label propagated from a byte-identical body (FableWin 0x01D73730).
// Callees: none (float pool 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f).
// Shape: the clamp is an inline helper taking the member by REFERENCE (a direct `if` on the member compares the argument
// register instead); the argument is stored first with an integer move, then re-read from the member for each compare
// (`fld [ecx+8] ; fcomp [0.0]` / `test ah,5 ; jp` is `< 0`, `test ah,41h ; jne` is `> 1`); the clamp values are
// immediate stores (`mov dword ptr [ecx+8],0` / `3F800000h`), each arm returning on its own.
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_SETTIMEOFDAY
#include "engine/CGameTimeManager.h"

inline void ClampFloat(float& value, float lo, float hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}

void CGameTimeManager::SetTimeOfDay(float time)
{
    Time = time;
    ClampFloat(Time, 0.0f, 1.0f);
}
