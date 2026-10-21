// CPlayerGui::UpdateStat(float value, float jumpThreshold, float step, float settleTime, float holdTime, NPlayerGui::CStatBar* bar) -- feed a new value into a stat bar: jump or animate the displayed value towards it, track the recent maximum, and time the bar's settle / fully-open phases before closing it (mode 3); a value change while open reopens it (mode 2)
// Leaf: ?UpdateStat@CPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437AE0, 483 bytes + padding (`ret 18h`; `this` unused). Callees: the frame counter 0x0049D870 (descriptive name GetCurrentFrame).
// Data: epsilon 0x0129BA3C, frames-per-second long 0x01375550 (descriptive), 0.0f 0x0122DEDC, 2^32 0x0122DCB4 (unsigned->float fix-up).
// Layout: CStatBase per Ego_r.pdb (CurrentValue +0xc ... GuiMode +0x28), modelled locally (no engine header).
// Shape: every `if (a < b) a = b` clamp is an inline helper taking references (ClampMin/ClampMax): the store-then-reload of OldValue and the
// integer copy of RecentMaxValue only come from the helper boundary; the two frame-timeout tests are a __forceinline helper that reads the
// frames-per-second global into a local BEFORE the frame call (retail homes it in the dead `value` slot) and takes the start frame by
// const reference (re-read after the call).
#include <math.h>
extern float         g_fEpsilon;                        // 0x0129BA3C
extern long          g_FramesPerSecond;                 // 0x01375550 (descriptive)
unsigned long GetCurrentFrame();                        // 0x0049D870
__forceinline bool FramesExceed(const unsigned long& since, float seconds)
{
    long fps = g_FramesPerSecond;
    return (float)(GetCurrentFrame() - since) > (float)fps * seconds;
}
template<class T> inline void ClampMin(T& v, const T& lo) { if (v < lo) v = lo; }
template<class T> inline void ClampMax(T& v, const T& hi) { if (v > hi) v = hi; }
namespace NPlayerGui {
struct CStatBar {                                       // CStatBase (Ego_r.pdb offsets)
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
struct CPlayerGui {
    void UpdateStat(float value, float jumpThreshold, float step, float settleTime, float holdTime, NPlayerGui::CStatBar* bar);
};

void CPlayerGui::UpdateStat(float value, float jumpThreshold, float step, float settleTime, float holdTime, NPlayerGui::CStatBar* bar)
{
    bar->LastOldValue = bar->OldValue;
    if (bar->GuiMode == 0 || fabs(value - bar->CurrentValue) > jumpThreshold)
        bar->CurrentValue = value;
    bar->OldRecentMaxValue = bar->RecentMaxValue;
    bar->UpdateRecentMax();
    bool moving = fabs(bar->CurrentValue - bar->OldValue) > g_fEpsilon;
    if (bar->GuiMode == 0) {
        if (bar->CurrentValue < bar->OldValue) {
            bar->OldValue -= step;
            ClampMin(bar->OldValue, bar->CurrentValue);
        } else if (bar->CurrentValue > bar->OldValue) {
            bar->OldValue += step;
            ClampMax(bar->OldValue, bar->CurrentValue);
        }
        if (moving) {
            bar->FrameFullyOpenedOn = 0;
            bar->FrameCurrentValueLastMoved = 0;
            return;
        }
        if (bar->FrameCurrentValueLastMoved == 0) {
            bar->FrameCurrentValueLastMoved = GetCurrentFrame();
            return;
        }
        if (FramesExceed(bar->FrameCurrentValueLastMoved, settleTime)) {
            bar->RecentMaxValue = bar->CurrentValue;
            bar->UpdateRecentMax();
            if (fabs(bar->RecentMaxValue - bar->OldRecentMaxValue) < g_fEpsilon) {
                if (bar->FrameFullyOpenedOn == 0) {
                    bar->FrameFullyOpenedOn = GetCurrentFrame();
                    return;
                }
                if (holdTime > 0.0f
                    && FramesExceed(bar->FrameFullyOpenedOn, holdTime)) {
                    bar->FrameFullyOpenedOn = 0;
                    bar->GuiMode = 3;
                }
            }
        }
        return;
    }
    if (bar->GuiMode != 2 && moving)
        bar->GuiMode = 2;
    else if (bar->GuiMode == 1)
        bar->RecentMaxValue = bar->CurrentValue;
}