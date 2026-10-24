// LHNetworkLib::LTimer::Stop() -- a running timer saves its current speed-up factor and freezes itself with factor 0
// Leaf: ?Stop@LTimer@LHNetworkLib@@
// Retail 0x008F9CA0, 0x52 bytes, thiscall `ret`. Manifest label "MusicTimer_CheckAndTrigger" (_global) is a propagated
// descriptive name. PDB name set: `?Stop@LTimer@LHNetworkLib@@QAEXXZ`, with the inline helpers Running()
// (`?Running@LTimer@LHNetworkLib@@QAEHXZ`, int) and GetSpeedUpFactor(). Ego_r layout as in the landed MSeconds 0x008F8030.
// Callers: 0x008F8A57 (LSocket construction), 0x008F9E7B, the LTimer constructor 0x008FA02F/0x008FA036.
// Callee: LTimer::SetSpeedUpFactor @ 0x008F80D0 (called with 0.0f, pushed as `push 0`).
// Shape: two `fucompp` tests against 0.0f (0x0122DEDC): Running() in Stop, then Running() again inside the inlined
// GetSpeedUpFactor() (`Running() ? SpeedUpFactor : SaveSpeedUpFactor`); the store and the SetSpeedUpFactor call are
// tail-duplicated into both arms of that ternary.
namespace LHNetworkLib {

class LTimer {
public:
    char          buf[256];                     // +0x000
    unsigned long SystemClockAtLastAdjust;      // +0x100
    unsigned long TimerTimeAtLastAdjust;        // +0x104
    float         SpeedUpFactor;                // +0x108
    float         SaveSpeedUpFactor;            // +0x10c
    int Running() { return SpeedUpFactor != 0.0f; }
    float GetSpeedUpFactor() { return Running() ? SpeedUpFactor : SaveSpeedUpFactor; }
    void SetSpeedUpFactor(float factor);        // 0x008F80D0
    void Stop();
};

void LTimer::Stop()
{
    if (Running()) {
        SaveSpeedUpFactor = GetSpeedUpFactor();
        SetSpeedUpFactor(0.0f);
    }
}

}