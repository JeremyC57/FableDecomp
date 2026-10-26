// CMainGameComponent::GetGameTimeOfNextPresentCompletion() -- predict when the next present completes (the vsync timer's last vsync time plus the pending vsync count times the refresh period), never earlier than now (or exactly now when the immediate-present flag is set), as game time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004166E2, length 104 bytes.
// Callees: CVsyncCallbackTimer::GetInfo @ 0x009F7050 (0x30-byte CVsyncCallbackTimerInfo by hidden pointer),
//          GFGetTime @ 0x009E1BC0 (`__fastcall`, double).
// Globals: the display device at 0x013B8390 (vsync timer +0x294, refresh period float +0x1d4), the immediate-present
// flag byte at 0x013B86A4. Game time base: the component's double at +0x60.
// Shape: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`); the pending count `a - b + 1` goes
// through a frame word for `fild`; `if (t < now) t = now;` keeps `now` on the FPU (`fst`), the flag test pops it.
#pragma optimize("s", on)
#pragma optimize("y", off)
double __fastcall GFGetTime();                     // 0x009E1BC0
struct CVsyncCallbackTimerInfo {                   // 0x30 bytes
    unsigned char _pad00[0x10];
    long          VsyncCount;                      // +0x10
    unsigned char _pad14[4];
    double        LastVsyncTime;                   // +0x18
    unsigned char _pad20[8];
    long          PresentedVsyncCount;             // +0x28
    unsigned char _pad2c[4];
};
class CVsyncCallbackTimer { public:
    CVsyncCallbackTimerInfo GetInfo();             // 0x009F7050
};
struct CDisplayDevice {
    unsigned char        _pad00[0x1d4];
    float                m_refreshPeriod;          // +0x1d4
    unsigned char        _pad1d8[0x294 - 0x1d8];
    CVsyncCallbackTimer* m_pVsyncTimer;            // +0x294
};
extern CDisplayDevice* g_pDisplayDevice;           // 0x013B8390
extern bool g_immediatePresent;                    // 0x013B86A4
class CMainGameComponent { public:
    unsigned char _pad00[0x60];
    double        m_gameTimeBase;                  // +0x60
    double GetGameTimeOfNextPresentCompletion();
};

double CMainGameComponent::GetGameTimeOfNextPresentCompletion()
{
    CVsyncCallbackTimerInfo info = g_pDisplayDevice->m_pVsyncTimer->GetInfo();
    double t = (info.VsyncCount - info.PresentedVsyncCount + 1) * g_pDisplayDevice->m_refreshPeriod + info.LastVsyncTime;
    double now = GFGetTime();
    if (t < now)
        t = now;
    if (g_immediatePresent)
        t = now;
    return t - m_gameTimeBase;
}