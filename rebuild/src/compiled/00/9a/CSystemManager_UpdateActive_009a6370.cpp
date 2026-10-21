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
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
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