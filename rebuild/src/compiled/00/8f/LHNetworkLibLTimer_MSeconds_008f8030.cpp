// LHNetworkLib::LTimer::MSeconds() -- milliseconds on the scaled timer: real ticks since the last adjust times SpeedUpFactor, plus the timer time at that adjust
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F8030, length 73 bytes.
// Leaf: MSeconds
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// Ego_r.pdb lists ?MSeconds@LTimer@LHNetworkLib@@QAEKXZ and lays LTimer out as buf[256], SystemClockAtLastAdjust
// +0x100, TimerTimeAtLastAdjust +0x104, SpeedUpFactor +0x108, SaveSpeedUpFactor +0x10C (0x110 bytes). Callers are the
// timer's own setters: 0x008F80D0 (stores MSeconds() into +0x104, GetTickCount() into +0x100, the new factor into
// +0x108; manifest "CMusicManager_OnMusicEntryTimeout") and 0x008F9D00 (Stop-like, factor 1e-5); the LTimer
// constructor 0x008FA000 (factor 1.0f) is built on the stack of LSocket::Bind.
// Callees: GetTickCount (KERNEL32 import 0x0143FDFC), __ftol2 @ 0x00BFEA70 (tail jump).
// Shape: both unsigned longs convert with the `fild ; jge ; fadd 2^32` fix-up (constant 0x0122DCB4); the factor
// multiply is `fmul [esi+108h]` straight after the first fild, then the second conversion and `faddp`; the float to
// unsigned long return is the tail-jumped __ftol2.
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);   // KERNEL32 import 0x0143FDFC
namespace LHNetworkLib {
class LTimer {
public:
    char          buf[256];                  // +0x000
    unsigned long SystemClockAtLastAdjust;   // +0x100
    unsigned long TimerTimeAtLastAdjust;     // +0x104
    float         SpeedUpFactor;             // +0x108
    float         SaveSpeedUpFactor;         // +0x10c
    unsigned long MSeconds();
};
unsigned long LTimer::MSeconds()
{
    return (unsigned long)((float)(GetTickCount() - SystemClockAtLastAdjust) * SpeedUpFactor + (float)TimerTimeAtLastAdjust);
}
}