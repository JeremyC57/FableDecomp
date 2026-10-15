// CGameTimeManager::FastForwardTimeTo(float time, float seconds) -- clamps the target time of day to [0,1]; with a positive duration starts a fast-forward (target, flag, seconds), otherwise jumps straight there (a new day when the time wraps backwards)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB720, length 99 bytes (padded to 112).
// Globals: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 0.0001f @ 0x0129BA3C.
// Shape: the clamp is an `if / else if` on the parameter kept on the FPU (`fstp st(0) ; fld const`); the fast-forward
// arm stores the clamped time with `fstp`, the flag byte, then the duration as an integer move; the immediate arm
// compares with Time (`fcom [ecx+8]`), bumps DayCount on `<` and stores the time.
// Layout: Ego_r.pdb CGameTimeManager (Time +0x08, DayCount +0x14, FastForwardingToTime +0x21,
// TimeFastForwardingTo +0x24, SecondsLeftToFastForwardTime +0x28).
struct CWorld;
struct CMainGameComponent;
struct CGameTimeManager {
    CWorld*             World;                     // +0x00
    CMainGameComponent* GameComponent;             // +0x04
    float               Time;                      // +0x08
    float               PrevTime;                  // +0x0c
    float               TimeStep;                  // +0x10
    long                DayCount;                  // +0x14
    bool                UsingVillageTime;          // +0x18
    float               VillageTime;               // +0x1c
    bool                VillageDay;                // +0x20
    bool                FastForwardingToTime;      // +0x21
    float               TimeFastForwardingTo;      // +0x24
    float               SecondsLeftToFastForwardTime;   // +0x28
    void FastForwardTimeTo(float time, float seconds);
};
extern const float g_zero_0122dedc;                // 0x0122DEDC (0.0f)
extern const float g_one_0122ded8;                 // 0x0122DED8 (1.0f)
extern const float g_minFastForwardSeconds;        // 0x0129BA3C (0.0001f)

void CGameTimeManager::FastForwardTimeTo(float time, float seconds)
{
    if (time < g_zero_0122dedc)
        time = g_zero_0122dedc;
    else if (time > g_one_0122ded8)
        time = g_one_0122ded8;
    if (seconds > g_minFastForwardSeconds) {
        TimeFastForwardingTo = time;
        FastForwardingToTime = true;
        SecondsLeftToFastForwardTime = seconds;
        return;
    }
    if (time < Time)
        ++DayCount;
    Time = time;
}
