// CSystemManager::IsApplicationActive() -- refresh ApplicationActive from the foreground window and return it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A57B0, length 60 bytes.
// Callee: USER32!GetForegroundWindow through the import slot 0x01440378.
//
// Shape: the window handle is read BEFORE the import call (kept in edi), so it is a named local; the
// comparison is a bool held in al (`sete al`), and the constant 1 lives in cl because it serves both the
// `EverBeenActive == true` compare and the two `true` stores. Becoming active again after having been
// active once (and not already active) asks for a restore. The retail layout sits 8 bytes above the PDB
// from ApplicationWinHandle on (PDB +0x8c/+0xd4/+0xe2 -> retail +0x94/+0xdc/+0xea); the leading bools
// (+0x09) are unshifted. No engine header exists for CSystemManager, so the touched members are modelled
// locally with their PDB names.
struct HWND__;

extern "C" __declspec(dllimport) HWND__* __stdcall GetForegroundWindow();   // [0x01440378]

struct CSystemManager {
    void*         _vtable;                  // +0x00
    unsigned char _pad04[4];                // +0x04
    bool          WindowsQuit;              // +0x08
    bool          ApplicationActive;        // +0x09
    bool          ApplicationHasInputFocus; // +0x0a
    unsigned char _pad0b[0x89];             // +0x0b .. +0x93 (GUIDs, critical section, managers)
    HWND__*       ApplicationWinHandle;     // +0x94
    unsigned char _pad98[0x44];             // +0x98 .. +0xdb
    bool          EverBeenActive;           // +0xdc
    unsigned char _paddd[0xd];              // +0xdd .. +0xe9 (mouse buttons, MouseZMove, WindowValid, ...)
    bool          Restore;                  // +0xea

    bool IsApplicationActive();
};

bool CSystemManager::IsApplicationActive()
{
    HWND__* window = ApplicationWinHandle;
    bool active = (window == GetForegroundWindow());
    if (active && !ApplicationActive && EverBeenActive == true)
        Restore = true;
    ApplicationActive = active;
    EverBeenActive = true;
    return active;
}