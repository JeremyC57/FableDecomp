// Behaviour test for 00437ae0 (CPlayerGui::UpdateStat): jump vs animate, recent max, settle/hold timers, reopen.
#include <cstdio>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
float g_fEpsilon = 0.0001f;
long g_FramesPerSecond = 10;
static unsigned long s_frame = 100;
unsigned long GetCurrentFrame() { return s_frame; }
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

static int fails = 0;
static void check(bool c, int n) { if (!c) { std::printf("fail %d\n", n); ++fails; } }
int main()
{
    CPlayerGui gui;
    NPlayerGui::CStatBar bar;
    bar.__vftable = 0; bar.GuiDef = 0; bar.ValueInitialised = true;
    bar.CurrentValue = 0.0f; bar.OldValue = 0.0f; bar.LastOldValue = 0.0f;
    bar.RecentMaxValue = 0.0f; bar.OldRecentMaxValue = 0.0f;
    bar.FrameCurrentValueLastMoved = 0; bar.FrameFullyOpenedOn = 0; bar.GuiMode = 0;
    // mode 0: value always taken, Old animates by step towards it
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.CurrentValue == 10.0f && bar.OldValue == 3.0f && bar.RecentMaxValue == 10.0f, 1);
    check(bar.FrameCurrentValueLastMoved == 0 && bar.FrameFullyOpenedOn == 0, 2);
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.OldValue == 9.0f && bar.LastOldValue == 6.0f, 3);
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.OldValue == 10.0f, 4);                       // clamped
    // settled: first call stamps the frame
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.FrameCurrentValueLastMoved == 100, 5);
    s_frame = 105;                                          // 5 frames < 10 * 1.0
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.FrameFullyOpenedOn == 0, 6);
    s_frame = 111;                                          // 11 > 10: opened stamp
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.FrameFullyOpenedOn == 111 && bar.GuiMode == 0, 7);
    s_frame = 140;                                          // 29 > 20 hold: closes (mode 3)
    gui.UpdateStat(10.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.GuiMode == 3 && bar.FrameFullyOpenedOn == 0, 8);
    // mode 3, small change below threshold: ignored, not moving
    gui.UpdateStat(10.5f, 1.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.CurrentValue == 10.0f && bar.GuiMode == 3, 9);
    // big change: taken, moving -> reopen (mode 2)
    gui.UpdateStat(20.0f, 1.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.CurrentValue == 20.0f && bar.GuiMode == 2 && bar.RecentMaxValue == 20.0f, 10);
    // mode 1: recent max follows the current value
    bar.GuiMode = 1; bar.OldValue = 20.0f; bar.CurrentValue = 20.0f; bar.RecentMaxValue = 50.0f;
    gui.UpdateStat(20.0f, 1.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.RecentMaxValue == 20.0f && bar.GuiMode == 1, 11);
    // decreasing in mode 0
    bar.GuiMode = 0; bar.CurrentValue = 5.0f; bar.OldValue = 5.0f; bar.RecentMaxValue = 5.0f;
    gui.UpdateStat(1.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.OldValue == 2.0f && bar.RecentMaxValue == 5.0f, 12);
    gui.UpdateStat(1.0f, 100.0f, 3.0f, 1.0f, 2.0f, &bar);
    check(bar.OldValue == 1.0f, 13);
    if (fails == 0) std::printf("00437ae0_TEST PASS\n");
    return fails;
}
