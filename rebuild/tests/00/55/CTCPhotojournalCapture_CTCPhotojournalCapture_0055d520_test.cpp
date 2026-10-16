#include <cstdio>
// Behaviour model of CTCPhotojournalCapture::CTCPhotojournalCapture (0x0055D520)
struct CWideString { void* m_data; CWideString() : m_data(0) {} };
struct CTCBase { void* ParentThing; int registered; CTCBase(void* p) : ParentThing(p), registered(0) {} void AddToGlobalUpdateTCs() { ++registered; } };
struct CComponentVector { void* a; void* b; void* c; CComponentVector() : a(0), b(0), c(0) {} };
struct CTCPhotojournalCapture : CTCBase {
    CWideString Description; bool DiscardScreenShot; long FrameCounter; bool FirstRenderFrame; bool ScreenOpen;
    double LastRenderTime; CWideString FileName; CComponentVector Components; void* PMannequin; bool WasPlayerGuiOpen; long TakeScreenshotAfterFrames;
    CTCPhotojournalCapture(void* parent) : CTCBase(parent), FrameCounter(30), FirstRenderFrame(true), ScreenOpen(false),
        LastRenderTime(0.0), PMannequin(0), WasPlayerGuiOpen(false), TakeScreenshotAfterFrames(-1) { AddToGlobalUpdateTCs(); }
};
int main()
{
    int thing;
    CTCPhotojournalCapture c(&thing);
    if (c.ParentThing != &thing || c.registered != 1 || c.FrameCounter != 30 || !c.FirstRenderFrame || c.ScreenOpen
        || c.LastRenderTime != 0.0 || c.PMannequin || c.WasPlayerGuiOpen || c.TakeScreenshotAfterFrames != -1 || c.Components.a || c.Description.m_data)
    { std::printf("0055d520_TEST FAIL\n"); return 1; }
    std::printf("0055d520_TEST PASS\n");
    return 0;
}