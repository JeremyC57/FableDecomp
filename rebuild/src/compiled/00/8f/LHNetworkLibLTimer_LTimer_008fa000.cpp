// LHNetworkLib::LTimer::LTimer() -- construct the network timer: unit speed-up factor, no saved factor, the current
// tick count as the system clock at the last adjustment, timer time 0, then Stop() twice; returns `this`.
// Leaf: ??0LTimer@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA000, length 62 bytes (+ int3 padding).
// Renamed (rename1, 2026-10-24) from CMusicTimer_008fa000::Reset @ 00/8f/CMusicTimer008fa000_Reset_008fa000.cpp:
//   the PDB (Ego_r) class LHNetworkLib::LTimer is 0x110 bytes {char buf[256]; ulong SystemClockAtLastAdjust +0x100;
//   ulong TimerTimeAtLastAdjust +0x104; float SpeedUpFactor +0x108; float SaveSpeedUpFactor +0x10c} -- exactly the
//   fields this body initialises; its only callee 0x008F9CA0 is the landed LHNetworkLib::LTimer::Stop (PDB name);
//   its only caller (0x008F898D, LSocket construction) builds a stack object at [esp+0x11c] with it before calling
//   LTimer members; the PDB name set has `??0LTimer@LHNetworkLib@@QAE@XZ`. The manifest's
//   "CAIStateGroup_MinionHasInformation::OnActivate" is a propagated label.
// Callees: KERNEL32 GetTickCount through its import slot (call [0x0143FDFC]); LTimer::Stop @ 0x008F9CA0 twice.
// Shape: stores at +0x108 (1.0f), +0x10c (0), the tick call, +0x100 (ticks), +0x104 (0) in statement order,
// then the two member calls and `mov eax,esi` (the constructor returns `this`).
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);   // KERNEL32 import 0x0143FDFC

namespace LHNetworkLib {

class LTimer {
public:
    char          buf[256];                     // +0x000
    unsigned long SystemClockAtLastAdjust;      // +0x100
    unsigned long TimerTimeAtLastAdjust;        // +0x104
    float         SpeedUpFactor;                // +0x108
    float         SaveSpeedUpFactor;            // +0x10c
    LTimer();
    void Stop();                                // 0x008F9CA0
};

LTimer::LTimer()
{
    SpeedUpFactor = 1.0f;
    SaveSpeedUpFactor = 0;
    SystemClockAtLastAdjust = GetTickCount();
    TimerTimeAtLastAdjust = 0;
    Stop();
    Stop();
}

}
