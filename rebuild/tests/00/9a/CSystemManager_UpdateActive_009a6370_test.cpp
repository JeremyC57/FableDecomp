// Behaviour test for 009a6370: src/009a6370.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CSystemManager::UpdateActive()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CSystemManager::UpdateActive() -- pump window messages until the application may run: track foreground activation (input focus on/off), sleep while the display device is lost, and wait for messages while inactive when the app is set to wait
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A6370, 227 bytes (+ int3 padding), `ret`.
// Leaf: UpdateActive
// Callees: GFRecalibrateFastSubFrameTimer @ 0x009E24B0 (`__fastcall`, no arguments), CSystemManager::ProcessWindowMessages
//   @ 0x009A4F20, CInputManager::SetHasInputFocus(bool) @ 0x009F4E20, CDisplayManager::ProcessLostDevice() @ 0x009C00C0;
//   imports USER32!WaitMessage (IAT 0x01440380), USER32!GetForegroundWindow (IAT 0x01440378), KERNEL32!Sleep (IAT 0x0143FE1C).
// Shape: the two imports used inside the loop are hoisted into ebp/edi; the constant 1 lives in ebx for the `== true` flag
//   compares and the stores; the two activation arms tail-merge into one SetHasInputFocus call (`push 0 ; jmp` / `push ebx`);
//   the loop is a top-tested `while (!WindowsQuit) { ... }` (the do/while form lets VC7.1 fold the WindowsQuit re-reads
//   retail keeps at the loop top) with the lost-device sleep falling into that test and the active/quit/
//   no-wait checks breaking out.
// Retail layout (Ego_r.pdb CSystemManager: +8 up to the managers, then +8 bytes from the window block on): WindowsQuit +0x08,
//   ApplicationActive +0x09, ApplicationHasInputFocus +0x0A, PInputManager +0x58, PDisplayManager +0x60, ApplicationWinHandle
//   +0x94, EverBeenActive +0xDC, Restore +0xEA, WaitWhileInactive +0xEB, AttachedToExternalWindow +0xEC.
// No engine header exists for CSystemManager.
struct HWND__;
extern "C" __declspec(dllimport) int __stdcall WaitMessage();                  // IAT 0x01440380
extern "C" __declspec(dllimport) HWND__* __stdcall GetForegroundWindow();       // IAT 0x01440378
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);        // IAT 0x0143FE1C
void __fastcall GFRecalibrateFastSubFrameTimer();                              // 0x009E24B0

class CInputManager {
public:
    void SetHasInputFocus(bool focus);                     // 0x009F4E20
};
class CDisplayManager {
public:
    bool ProcessLostDevice();                              // 0x009C00C0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    operator bool() const { return Data != 0; }
};

class CSystemManager {
public:
    unsigned char                   _pad00[8];
    bool                            WindowsQuit;           // +0x08
    bool                            ApplicationActive;     // +0x09
    bool                            ApplicationHasInputFocus;   // +0x0a
    unsigned char                   _pad0b[0x58 - 0x0b];
    CCountedPointer<CInputManager>  PInputManager;         // +0x58
    CCountedPointer<CDisplayManager> PDisplayManager;      // +0x60
    unsigned char                   _pad68[0x94 - 0x68];
    HWND__*                         ApplicationWinHandle;  // +0x94
    unsigned char                   _pad98[0xdc - 0x98];
    bool                            EverBeenActive;        // +0xdc
    unsigned char                   _padDD[0xea - 0xdd];
    bool                            Restore;               // +0xea
    bool                            WaitWhileInactive;     // +0xeb
    bool                            AttachedToExternalWindow;   // +0xec
    void ProcessWindowMessages();                          // 0x009A4F20
    void UpdateActive();
};

void CSystemManager::UpdateActive()
{
    GFRecalibrateFastSubFrameTimer();
    if (AttachedToExternalWindow || WindowsQuit)
        return;
    while (!WindowsQuit) {
        if (!ApplicationActive && !WindowsQuit && WaitWhileInactive == true)
            WaitMessage();
        ProcessWindowMessages();
        if (GetForegroundWindow() != ApplicationWinHandle) {
            ApplicationActive = false;
            EverBeenActive = true;
            ApplicationHasInputFocus = false;
            if (PInputManager)
                PInputManager->SetHasInputFocus(false);
        } else {
            if (!ApplicationActive && EverBeenActive == true)
                Restore = true;
            ApplicationActive = true;
            EverBeenActive = true;
            ApplicationHasInputFocus = true;
            if (PInputManager)
                PInputManager->SetHasInputFocus(true);
        }
        if (!PDisplayManager->ProcessLostDevice())
            Sleep(10);
        else if (ApplicationActive || WindowsQuit || !WaitWhileInactive)
            break;
    }
}

// ---- stubs and driver ----

#pragma comment(lib, "user32.lib")
static int g_recal = 0;
void __fastcall GFRecalibrateFastSubFrameTimer() { ++g_recal; }
static bool g_focus[8]; static int g_nFocus = 0;
void CInputManager::SetHasInputFocus(bool focus) { g_focus[g_nFocus++] = focus; }
static int g_lostCalls = 0; static int g_lostUntil = 0;
bool CDisplayManager::ProcessLostDevice() { ++g_lostCalls; return g_lostCalls > g_lostUntil; }
static int g_pumps = 0; static CSystemManager* g_mgr = 0; static int g_quitAfter = 100;
void CSystemManager::ProcessWindowMessages() { ++g_pumps; if (g_pumps >= g_quitAfter) WindowsQuit = true; }
int main()
{
    static CSystemManager m; static CInputManager input; static CDisplayManager display;
    m.PInputManager.Data = &input; m.PDisplayManager.Data = &display; g_mgr = &m;
    m.AttachedToExternalWindow = true; m.UpdateActive(); CHECK(g_recal == 1); CHECK(g_pumps == 0);       // attached: nothing
    m.AttachedToExternalWindow = false; m.WindowsQuit = true; m.UpdateActive(); CHECK(g_pumps == 0);      // quitting: nothing
    // foreground window: becomes active, restore requested once (inactive but ever active), focus on
    m.WindowsQuit = false; m.ApplicationActive = false; m.EverBeenActive = true; m.Restore = false; m.WaitWhileInactive = false;
    m.ApplicationWinHandle = GetForegroundWindow();
    m.UpdateActive();
    CHECK(g_pumps == 1); CHECK(m.ApplicationActive == true); CHECK(m.Restore == true); CHECK(m.ApplicationHasInputFocus == true);
    CHECK(g_nFocus == 1); CHECK(g_focus[0] == true); CHECK(g_lostCalls == 1);
    // background: inactive, focus off; device lost twice (sleep) then not waiting -> leave
    m.ApplicationWinHandle = (HWND__*)((char*)GetForegroundWindow() + 1); m.Restore = false; g_lostCalls = 0; g_lostUntil = 2; g_pumps = 0;
    m.UpdateActive();
    CHECK(g_pumps == 3); CHECK(m.ApplicationActive == false); CHECK(m.ApplicationHasInputFocus == false); CHECK(m.Restore == false);
    CHECK(g_focus[g_nFocus - 1] == false); CHECK(g_lostCalls == 3);
    // background, waiting while inactive is off but WindowsQuit raised by the pump: the loop test ends it
    m.ApplicationWinHandle = GetForegroundWindow();   // stays active, so WaitMessage is never reached
    m.WaitWhileInactive = true; m.ApplicationActive = true; g_lostCalls = 0; g_lostUntil = 100; g_pumps = 0; g_quitAfter = 2;
    m.UpdateActive(); CHECK(g_pumps == 2); CHECK(m.WindowsQuit == true);
    m.ApplicationWinHandle = (HWND__*)((char*)GetForegroundWindow() + 1);
    m.PInputManager.Data = 0; m.WindowsQuit = false; g_quitAfter = 100; g_lostUntil = 0; g_lostCalls = 0; m.WaitWhileInactive = false; int before = g_nFocus;
    m.UpdateActive(); CHECK(g_nFocus == before);                                                        // no input manager: no focus call
    if (fails == 0) printf("009a6370_TEST PASS\n"); else printf("009a6370_TEST FAIL\n");
    return fails;
}