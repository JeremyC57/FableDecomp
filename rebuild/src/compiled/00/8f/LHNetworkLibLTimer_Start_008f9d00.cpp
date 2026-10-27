// LHNetworkLib::LTimer::Start() -- restart the timer at its saved speed, folding the stopped interval into the timer time
// Leaf: ?Start@LTimer@LHNetworkLib@@QAEXXZ
// Retail: Fable.exe @ 0x008F9D00, 60 bytes, thiscall plain `ret`.
// Identity (proven): Ego_r.pdb LTimer = { char buf[256]; ulong SystemClockAtLastAdjust +0x100; ulong TimerTimeAtLastAdjust
// +0x104; float SpeedUpFactor +0x108; float SaveSpeedUpFactor +0x10C } (size 0x110), exactly the offsets used here.
// FableWin.pdb's LHNetworkLib::LTimer methods are the ctor, Reset(ulong), Start(), Stop(), MSeconds(), Running(),
// GetSpeedUpFactor() and SetSpeedUpFactor(float). The siblings are pinned by body: 0x008FA000 ctor (returns this),
// 0x008F8030 MSeconds (GetTickCount delta * SpeedUpFactor + TimerTimeAtLastAdjust, _ftol2 tail), 0x008F8080 Running
// (SpeedUpFactor != 0), 0x008F80A0 GetSpeedUpFactor, 0x008F80D0 SetSpeedUpFactor(float), 0x008F9CA0 Stop (saves the speed
// and sets 0). This is the remaining no-argument void method, restoring SaveSpeedUpFactor: Start. Its caller 0x008F8999
// (in the LSocket receive-with-timeout path) builds an LTimer on the stack (ctor 0x008FA000) and starts it.
// Callees: LTimer::MSeconds 0x008F8030 (out of line), GetTickCount (IAT 0x0143FDFC).
// Layout: no engine header for LTimer: local model from Ego_r.pdb.
// Shape: written store-first (the read of the saved factor is hoisted above the store, its spill stays below); a tiny non-zero speed (1e-5f, pool 0x3727C5AC) is stored so
// MSeconds() counts the stopped interval as ~0, then the clock pair is rebased and the saved speed written back.
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();

namespace LHNetworkLib {
class LTimer {
public:
    void Start();
    unsigned long MSeconds();                               // 0x008F8030

    char buf[256];                                          // +0x000
    unsigned long SystemClockAtLastAdjust;                  // +0x100
    unsigned long TimerTimeAtLastAdjust;                    // +0x104
    float SpeedUpFactor;                                    // +0x108
    float SaveSpeedUpFactor;                                // +0x10C
};
}

void LHNetworkLib::LTimer::Start()
{
    SpeedUpFactor = 0.00001f;
    float saved = SaveSpeedUpFactor;
    TimerTimeAtLastAdjust = MSeconds();
    SystemClockAtLastAdjust = GetTickCount();
    SpeedUpFactor = saved;
}