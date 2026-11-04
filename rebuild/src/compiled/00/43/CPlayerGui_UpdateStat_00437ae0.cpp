// CPlayerGui::UpdateStat(float value, float jumpThreshold, float step, float settleTime, float holdTime, NPlayerGui::CStatBase& stat) const -- feed a new value into a stat bar: jump or animate the displayed value towards it, track the recent maximum, and time the bar's settle / fully-open phases before closing it (mode 3); a value change while open reopens it (mode 2)
// Leaf: ?UpdateStat@CPlayerGui@@ABEXMMMMMAAVCStatBase@NPlayerGui@@@Z
// Naming wave 35c: Ego_r.pdb exact symbol (private const; the stat is NPlayerGui::CStatBase&, was NPlayerGui::CStatBar*); queue land0411j. Callers 0x00439E00 / 0x00439F20 respelled with it.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437AE0, 483 bytes + padding (`ret 18h`; `this` unused). Callees: the frame counter 0x0049D870 (descriptive name GetCurrentFrame).
// Data: epsilon 0x0129BA3C, frames-per-second long 0x01375550 (descriptive), 0.0f 0x0122DEDC, 2^32 0x0122DCB4 (unsigned->float fix-up).
// Layout: CStatBase per Ego_r.pdb (CurrentValue +0xc ... GuiMode +0x28), modelled locally (no engine header).
// Shape: every `if (a < b) a = b` clamp is an inline helper taking references (ClampMin/ClampMax): the store-then-reload of OldValue and the
// integer copy of RecentMaxValue only come from the helper boundary; the two frame-timeout tests are a __forceinline helper that reads the
// frames-per-second global into a local BEFORE the frame call (retail homes it in the dead `value` slot) and takes the start frame by
// const reference (re-read after the call).
#include <math.h>
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                        // 0x0049D870
__forceinline bool FramesExceed(const unsigned long& since, float seconds)
{
    long fps = CMainGameComponent::GetConstantFPS();
    return (float)(CWorld::GetFrame() - since) > (float)fps * seconds;
}
template<class T> inline void ClampMin(T& v, const T& lo) { if (v < lo) v = lo; }
template<class T> inline void ClampMax(T& v, const T& hi) { if (v > hi) v = hi; }
namespace NPlayerGui {
class CStatBase { public:                                      // Ego_r.pdb NPlayerGui::CStatBase offsets
    void*         __vftable;
    void*         GuiDef;                               // +0x04
    bool          ValueInitialised;                     // +0x08
    float         CurrentValue;                         // +0x0c
    float         OldValue;                             // +0x10
    float         LastOldValue;                         // +0x14
    float         RecentMaxValue;                       // +0x18
    float         OldRecentMaxValue;                    // +0x1c
    unsigned long FrameCurrentValueLastMoved;           // +0x20
    unsigned long FrameFullyOpenedOn;                   // +0x24
    long          GuiMode;                              // +0x28
    void UpdateRecentMax()
    {
        ClampMin(RecentMaxValue, CurrentValue);
    }
};
}
class CPlayerGui { public:
private:
    void UpdateStat(float value, float jumpThreshold, float step, float settleTime, float holdTime, NPlayerGui::CStatBase& stat) const;
};

void CPlayerGui::UpdateStat(float value, float jumpThreshold, float step, float settleTime, float holdTime, NPlayerGui::CStatBase& stat) const
{
    stat.LastOldValue = stat.OldValue;
    if (stat.GuiMode == 0 || fabs(value - stat.CurrentValue) > jumpThreshold)
        stat.CurrentValue = value;
    stat.OldRecentMaxValue = stat.RecentMaxValue;
    stat.UpdateRecentMax();
    bool moving = fabs(stat.CurrentValue - stat.OldValue) > DELTA_NOTIONAL_ZERO;
    if (stat.GuiMode == 0) {
        if (stat.CurrentValue < stat.OldValue) {
            stat.OldValue -= step;
            ClampMin(stat.OldValue, stat.CurrentValue);
        } else if (stat.CurrentValue > stat.OldValue) {
            stat.OldValue += step;
            ClampMax(stat.OldValue, stat.CurrentValue);
        }
        if (moving) {
            stat.FrameFullyOpenedOn = 0;
            stat.FrameCurrentValueLastMoved = 0;
            return;
        }
        if (stat.FrameCurrentValueLastMoved == 0) {
            stat.FrameCurrentValueLastMoved = CWorld::GetFrame();
            return;
        }
        if (FramesExceed(stat.FrameCurrentValueLastMoved, settleTime)) {
            stat.RecentMaxValue = stat.CurrentValue;
            stat.UpdateRecentMax();
            if (fabs(stat.RecentMaxValue - stat.OldRecentMaxValue) < DELTA_NOTIONAL_ZERO) {
                if (stat.FrameFullyOpenedOn == 0) {
                    stat.FrameFullyOpenedOn = CWorld::GetFrame();
                    return;
                }
                if (holdTime > 0.0f
                    && FramesExceed(stat.FrameFullyOpenedOn, holdTime)) {
                    stat.FrameFullyOpenedOn = 0;
                    stat.GuiMode = 3;
                }
            }
        }
        return;
    }
    if (stat.GuiMode != 2 && moving)
        stat.GuiMode = 2;
    else if (stat.GuiMode == 1)
        stat.RecentMaxValue = stat.CurrentValue;
}