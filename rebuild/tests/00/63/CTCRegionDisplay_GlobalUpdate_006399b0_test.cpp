// Behaviour test for 0x006399B0 CTCRegionDisplay::GlobalUpdate(bool): the row's logic over a flat model (frame counter and
// constant FPS as test globals); checks the fade-in, the hold, the fade-out and the timed-out cut.
#include <cstdio>
static long g_frame = 0;
static long g_fps = 30;
struct CTCRegionDisplay {
    long FrameNameChanged, CurrentAlpha, LastAlpha;
    float DisplayTime;
    bool TimeOut, InfoDisplay, UpdateWhilePaused;
    unsigned int GetFrameNameChanged() const { return (unsigned int)FrameNameChanged; }
    void GlobalUpdate(bool paused);
};
void CTCRegionDisplay::GlobalUpdate(bool paused)
{
    (void)paused;
    if (!UpdateWhilePaused)
        return;
    LastAlpha = CurrentAlpha;
    float fps = (float)g_fps;
    float displayFrames = fps * DisplayTime;
    float fadeFrames = fps * 0.2f;
    float elapsed = (float)(unsigned int)(g_frame - (long)GetFrameNameChanged());
    if (!(elapsed < displayFrames) && TimeOut) {
        CurrentAlpha = 0;
        return;
    }
    if (elapsed < fadeFrames) {
        CurrentAlpha = (long)(elapsed / fadeFrames * 255.0f);
        return;
    }
    if (TimeOut) {
        float remaining = displayFrames - elapsed;
        if (remaining < fadeFrames)
            CurrentAlpha = (long)((remaining / fadeFrames) * 255.0f);
    }
}
static long Run(long changed, long now, float display, bool timeOut, long start, bool update)
{
    CTCRegionDisplay d; d.FrameNameChanged = changed; d.CurrentAlpha = start; d.LastAlpha = -1; d.DisplayTime = display;
    d.TimeOut = timeOut; d.InfoDisplay = false; d.UpdateWhilePaused = update;
    g_frame = now;
    d.GlobalUpdate(false);
    if (update && d.LastAlpha != start) return -999;
    return d.CurrentAlpha;
}
int main()
{
    bool ok = true;
    ok = ok && Run(100, 103, 2.0f, true, 7, true) == 127;     // 3 of 6 fade frames: 127
    ok = ok && Run(100, 130, 2.0f, true, 7, true) == 7;       // holding: unchanged
    ok = ok && Run(100, 157, 2.0f, true, 7, true) == 127;     // 3 frames left of 60: fading out
    ok = ok && Run(100, 170, 2.0f, true, 7, true) == 0;       // past the display time: cut
    ok = ok && Run(100, 170, 2.0f, false, 7, true) == 7;      // no time-out: held
    ok = ok && Run(100, 103, 2.0f, true, 7, false) == 7;      // not updating
    if (!ok) { std::printf("006399b0_TEST FAIL\n"); return 1; }
    std::printf("006399b0_TEST PASS\n");
    return 0;
}