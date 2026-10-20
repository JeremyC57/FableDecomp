// Behaviour test for 00639930: body copied from src/00639930.cpp (CTCRegionDisplay::FinishShowingRegionName), frame counter and globals stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
long g_framesPerSecond;
const float g_regionNameFadeTime = 0.2f;
static long g_frame; static int g_frameCalls;
long __fastcall GFGetCurrentFrame() { ++g_frameCalls; return g_frame; }
struct CWideString { void* m_pData; };
struct CTCRegionDisplay {
    virtual void v00() {}
    void*       ParentThing;
    bool        ToBeRemoved;
    bool        IsInGlobalUpdateTCs;
    CWideString CurrentAreaName;
    CWideString CurrentAreaNameExtra;
    long        FrameNameChanged;
    long        CurrentAlpha;
    long        LastAlpha;
    float       DisplayTime;
    bool        TimeOut;
    bool        InfoDisplay;
    bool        UpdateWhilePaused;
    void FinishShowingRegionName();
};
void CTCRegionDisplay::FinishShowingRegionName()
{
    long changed = FrameNameChanged;
    float displayFrames = g_framesPerSecond * DisplayTime;
    float fadeFrames = g_framesPerSecond * g_regionNameFadeTime;
    long elapsed = GFGetCurrentFrame() - changed;
    if (!TimeOut || elapsed < displayFrames - fadeFrames)
        FrameNameChanged = (long)(GFGetCurrentFrame() - (displayFrames - fadeFrames));
    TimeOut = true;
}
int main()
{
    g_framesPerSecond = 30;
    CTCRegionDisplay d; memset(&d, 0, sizeof(d)); new (&d) CTCRegionDisplay;
    d.DisplayTime = 3.0f;              // displayFrames 90, fadeFrames 6 -> fade point 84 frames after the change
    // 1. not timing out yet: back-dated so that 84 frames appear elapsed; TimeOut set
    d.TimeOut = false; d.FrameNameChanged = 1000; g_frame = 1010; g_frameCalls = 0;
    d.FinishShowingRegionName();
    CHECK(d.FrameNameChanged == 1010 - 84); CHECK(d.TimeOut == true); CHECK(g_frameCalls == 2);
    // 2. timing out, fade point still ahead (elapsed 10 < 84): back-dated again
    d.TimeOut = true; d.FrameNameChanged = 2000; g_frame = 2010; g_frameCalls = 0;
    d.FinishShowingRegionName();
    CHECK(d.FrameNameChanged == 2010 - 84); CHECK(d.TimeOut == true); CHECK(g_frameCalls == 2);
    // 3. timing out and already past the fade point (elapsed 90 >= 84): untouched
    d.TimeOut = true; d.FrameNameChanged = 3000; g_frame = 3090; g_frameCalls = 0;
    d.FinishShowingRegionName();
    CHECK(d.FrameNameChanged == 3000); CHECK(d.TimeOut == true); CHECK(g_frameCalls == 1);
    // 4. exactly at the fade point (elapsed 84): untouched (strict less-than)
    d.FrameNameChanged = 4000; g_frame = 4084; d.FinishShowingRegionName(); CHECK(d.FrameNameChanged == 4000);
    // 5. one frame short of it: back-dated
    d.FrameNameChanged = 5000; g_frame = 5083; d.FinishShowingRegionName(); CHECK(d.FrameNameChanged == 5083 - 84);
    if (fails == 0) printf("00639930_TEST PASS\n");
    else printf("00639930_TEST FAIL (%d)\n", fails);
    return fails;
}