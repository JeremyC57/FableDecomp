#include <cstdio>
// Behaviour test for CInputProcessMain::ProcessInput (0x00687FD0): action 0x35 pressed in normal play takes a quick screenshot
// (no-HUD with Shift) and stops; otherwise the console is created once, sees the event unless open, and action 0x47 on a
// non-press, non-type-1 event moves the GUI cursor by the mouse movement.
struct CEvent { long Type; bool Pressed; long Action; float DX, DY; };
struct CProcessed { int Handled; void SetHandled() { Handled++; } };
struct CCapture { int Shots, NoHud; };
struct CHero { bool HasCapture; CCapture* Capture; };
class CWorld { public: long PendingState, SaveGameLoadStatus; };
class CConsole { public: bool IsOpen; int Seen; bool Takes; bool ProcessInput(const CEvent&) { Seen++; return Takes; } };
struct CGui { float X, Y; void MoveCursor(const float* m) { X += m[0]; Y += m[1]; } };
static CConsole* g_console = 0;
static CConsole g_consoleStore;
static int g_consoleNews = 0;
struct CMain {
    CWorld* World; CHero* Hero; bool Shift; bool Banner; CGui* Gui;
    void ProcessInput(const CEvent& event, CProcessed& processed)
    {
        if (event.Action == 0x35 && event.Pressed && World) {
            if ((World->SaveGameLoadStatus == 0 || World->SaveGameLoadStatus == 9) && World->PendingState == 0 && !Banner) {
                if (Hero->HasCapture) {
                    if (Shift) { Hero->Capture->NoHud++; processed.SetHandled(); }
                    else { Hero->Capture->Shots++; processed.SetHandled(); }
                    return;
                }
            }
        }
        if (!g_console) { g_console = &g_consoleStore; g_consoleNews++; }
        if (g_console->IsOpen) return;
        if (g_console->ProcessInput(event)) processed.SetHandled();
        if (event.Type == 1) return;
        if (event.Pressed) { bool unusedAction = event.Action == 6; (void)unusedAction; }
        else if (event.Action == 0x47) { float m[2] = { event.DX, -event.DY }; Gui->MoveCursor(m); }
    }
};
static int Fail(int n) { std::printf("00687fd0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CCapture cap = { 0, 0 };
    CHero hero = { true, &cap };
    CWorld world = { 0, 9 };
    CGui gui = { 0.0f, 0.0f };
    CMain m = { &world, &hero, false, false, &gui };
    CProcessed p = { 0 };
    CEvent shot = { 2, true, 0x35, 0, 0 };
    m.ProcessInput(shot, p);
    if (cap.Shots != 1 || p.Handled != 1 || g_consoleNews != 0) return Fail(1);
    m.Shift = true; m.ProcessInput(shot, p);
    if (cap.NoHud != 1 || p.Handled != 2) return Fail(2);
    world.PendingState = 1; m.ProcessInput(shot, p);
    if (cap.NoHud != 1 || g_consoleNews != 1 || g_consoleStore.Seen != 1) return Fail(3);
    CEvent move = { 2, false, 0x47, 2.0f, 3.0f };
    m.ProcessInput(move, p);
    if (gui.X != 2.0f || gui.Y != -3.0f || g_consoleNews != 1) return Fail(4);
    CEvent type1 = { 1, false, 0x47, 2.0f, 3.0f };
    g_consoleStore.Takes = true; m.ProcessInput(type1, p);
    if (gui.X != 2.0f || p.Handled != 3) return Fail(5);
    g_consoleStore.IsOpen = true; m.ProcessInput(move, p);
    if (gui.X != 2.0f || g_consoleStore.Seen != 3) return Fail(6);
    std::printf("00687fd0_TEST PASS\n");
    return 0;
}