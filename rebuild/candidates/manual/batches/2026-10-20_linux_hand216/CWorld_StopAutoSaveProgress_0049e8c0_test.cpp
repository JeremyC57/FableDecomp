// Behaviour test for 0x0049E8C0 CWorld::StopAutoSaveProgress: waits out the minimum time with sleeps and redisplays,
// stops (twice when a saved display exists), restores the old display, releases it and clears the shown flag.
#include <cstdio>
#include <cstring>
struct CCharString { char text[8]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0;
static void Destroy(void*) { ++g_destroyed; }
struct CAProgressDisplay { int id; };
struct CCountedPointer_CAProgressDisplay {
    CAProgressDisplay* Data; SCountBlock* Info;
    void Clear()
    {
        if (Info) { --Info->m_references; if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); delete Info; } }
        Info = 0; Data = 0;
    }
};
static int g_sleeps = 0, g_displays = 0, g_stops = 0; static double g_now = 0.0;
static CCountedPointer_CAProgressDisplay* g_restored = 0;
static void Sleep(unsigned long ms) { ++g_sleeps; g_now += ms / 1000.0; }
static double GFGetTime() { return g_now; }
static void DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{ if (text.text[0] == 0 && progress == -1.0f && !a && !b) ++g_displays; }
static void StopProgress(bool stopNow, bool keepScreen) { if (stopNow && !keepScreen) ++g_stops; }
static void SetProgressDisplay(CCountedPointer_CAProgressDisplay* d) { g_restored = d; }
static bool g_bAutoSaveProgressShown = false;
static double g_AutoSaveProgressEndTime = 0.25;
static CCountedPointer_CAProgressDisplay GOldAutoSaveProgressDisplay = { 0, 0 };
struct CWorld {
    void StopAutoSaveProgress()
    {
        if (g_bAutoSaveProgressShown) {
            while (GFGetTime() < g_AutoSaveProgressEndTime) { Sleep(100); DisplayProgress("", -1.0f, false, false); }
            StopProgress(true, false);
            SetProgressDisplay(&GOldAutoSaveProgressDisplay);
            if (GOldAutoSaveProgressDisplay.Data) StopProgress(true, false);
            GOldAutoSaveProgressDisplay.Clear();
            g_bAutoSaveProgressShown = false;
        }
    }
};
int main()
{
    CWorld w;
    w.StopAutoSaveProgress();
    bool ok = g_sleeps == 0 && g_stops == 0 && g_restored == 0;
    static CAProgressDisplay old = { 1 };
    SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = Destroy; block->m_pObject = &old;
    GOldAutoSaveProgressDisplay.Data = &old; GOldAutoSaveProgressDisplay.Info = block;
    g_bAutoSaveProgressShown = true;
    w.StopAutoSaveProgress();
    ok = ok && g_sleeps == 3 && g_displays == 3 && g_stops == 2 && g_restored == &GOldAutoSaveProgressDisplay
         && g_destroyed == 1 && GOldAutoSaveProgressDisplay.Data == 0 && GOldAutoSaveProgressDisplay.Info == 0
         && !g_bAutoSaveProgressShown;
    if (!ok) { std::printf("0049e8c0_TEST FAIL\n"); return 1; }
    std::printf("0049e8c0_TEST PASS\n");
    return 0;
}
