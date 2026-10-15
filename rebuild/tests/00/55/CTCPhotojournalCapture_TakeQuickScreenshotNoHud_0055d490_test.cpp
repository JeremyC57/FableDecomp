#include <cstdio>
struct CPlayerGui {
    long state;
    bool IsOpen() const { return state != 1; }
    void Close(bool immediately) { state = immediately ? 1 : 3; }
};
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
struct CTCPhotojournalCapture {
    long FrameCounter;
    bool WasPlayerGuiOpen;
    void TakeQuickScreenshotNoHud()
    {
        if (FrameCounter > 18) {
            WasPlayerGuiOpen = g_pPlayerGui->IsOpen();
            FrameCounter = 0;
            g_pPlayerGui->Close(false);
        }
    }
};
int main()
{
    CTCPhotojournalCapture c; c.FrameCounter = 18; c.WasPlayerGuiOpen = false; g_gui.state = 2;
    c.TakeQuickScreenshotNoHud();
    if (c.FrameCounter != 18 || c.WasPlayerGuiOpen || g_gui.state != 2) { std::printf("0055d490_TEST FAIL\n"); return 1; }
    c.FrameCounter = 19; c.TakeQuickScreenshotNoHud();
    if (c.FrameCounter != 0 || !c.WasPlayerGuiOpen || g_gui.state != 3) { std::printf("0055d490_TEST FAIL\n"); return 1; }
    g_gui.state = 1; c.FrameCounter = 25; c.TakeQuickScreenshotNoHud();
    if (c.WasPlayerGuiOpen || g_gui.state != 3) { std::printf("0055d490_TEST FAIL\n"); return 1; }
    std::printf("0055d490_TEST PASS\n");
    return 0;
}