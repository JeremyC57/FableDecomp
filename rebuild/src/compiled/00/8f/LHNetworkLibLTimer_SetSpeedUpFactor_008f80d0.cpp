// LHNetworkLib::LTimer::SetSpeedUpFactor(float) -- running timer: re-base on the scaled clock and take the new factor; stopped timer: remember it for Start
// Leaf: ?SetSpeedUpFactor@LTimer@LHNetworkLib@@
// Retail 0x008F80D0, 0x4B bytes, thiscall `ret 4`. Manifest label "CMusicManager_OnMusicEntryTimeout" (_global) is a
// propagated descriptive name. PDB name set: `?SetSpeedUpFactor@LTimer@LHNetworkLib@@QAEXM@Z`; Ego_r layout (0x110 bytes):
// buf[256], SystemClockAtLastAdjust +0x100, TimerTimeAtLastAdjust +0x104, SpeedUpFactor +0x108, SaveSpeedUpFactor +0x10C.
// Callers: LTimer::Stop 0x008F9CA0 (twice). Callees: LTimer::MSeconds @ 0x008F8030 (landed), GetTickCount (KERNEL32
// import 0x0143FDFC).
// Shape: `fld [0.0f] ; fld [this+108h] ; fucompp ; test ah,44h ; jnp` is the inline Running() test
// `SpeedUpFactor != 0.0f` (0x0122DEDC = 0.0f); the float argument is moved to the members with integer moves.
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);   // KERNEL32 import 0x0143FDFC

namespace LHNetworkLib {

class LTimer {
public:
    char          buf[256];                     // +0x000
    unsigned long SystemClockAtLastAdjust;      // +0x100
    unsigned long TimerTimeAtLastAdjust;        // +0x104
    float         SpeedUpFactor;                // +0x108
    float         SaveSpeedUpFactor;            // +0x10c
    unsigned long MSeconds();                   // 0x008F8030
    int Running() { return SpeedUpFactor != 0.0f; }
    void SetSpeedUpFactor(float factor);
};

void LTimer::SetSpeedUpFactor(float factor)
{
    if (Running()) {
        TimerTimeAtLastAdjust = MSeconds();
        SystemClockAtLastAdjust = GetTickCount();
        SpeedUpFactor = factor;
    } else {
        SaveSpeedUpFactor = factor;
    }
}

}