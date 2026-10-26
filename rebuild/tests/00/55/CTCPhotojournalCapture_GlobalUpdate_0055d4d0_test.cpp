#include <cstdio>
static int g_opens = 0;
class CPlayerGui { public: void Open() { ++g_opens; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CTCPhotojournalCapture { public:
    long FrameCounter;
    bool WasPlayerGuiOpen;
    long TakeScreenshotAfterFrames;
    int shots;
    void TakeQuickScreenshot() { ++shots; }
    void GlobalUpdate(long frame)
    {
        if (FrameCounter < 20) {
            if (++FrameCounter == 18) {
                TakeQuickScreenshot();
                if (WasPlayerGuiOpen)
                    g_pPlayerGui->Open();
            }
        }
        if (TakeScreenshotAfterFrames > -1)
            --TakeScreenshotAfterFrames;
        if (TakeScreenshotAfterFrames == 0)
            TakeQuickScreenshot();
    }
};
int main()
{
    CTCPhotojournalCapture c; c.FrameCounter = 16; c.WasPlayerGuiOpen = true; c.TakeScreenshotAfterFrames = -1; c.shots = 0;
    c.GlobalUpdate(0);
    if (c.FrameCounter != 17 || c.shots != 0 || c.TakeScreenshotAfterFrames != -1) { std::printf("0055d4d0_TEST FAIL\n"); return 1; }
    c.GlobalUpdate(0);
    if (c.FrameCounter != 18 || c.shots != 1 || g_opens != 1) { std::printf("0055d4d0_TEST FAIL\n"); return 1; }
    c.FrameCounter = 20; c.TakeScreenshotAfterFrames = 2; c.GlobalUpdate(0);
    if (c.FrameCounter != 20 || c.TakeScreenshotAfterFrames != 1 || c.shots != 1) { std::printf("0055d4d0_TEST FAIL\n"); return 1; }
    c.GlobalUpdate(0);
    if (c.TakeScreenshotAfterFrames != 0 || c.shots != 2) { std::printf("0055d4d0_TEST FAIL\n"); return 1; }
    c.GlobalUpdate(0);
    if (c.TakeScreenshotAfterFrames != -1 || c.shots != 2) { std::printf("0055d4d0_TEST FAIL\n"); return 1; }
    std::printf("0055d4d0_TEST PASS\n");
    return 0;
}