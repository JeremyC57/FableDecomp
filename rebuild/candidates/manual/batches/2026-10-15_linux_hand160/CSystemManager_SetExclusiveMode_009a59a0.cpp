// CSystemManager::SetExclusiveMode(bool exclusive) -- unless mode changes are locked out, try the requested exclusive mode and then the previous one: set ExclusiveMode, reset the process priority, let the input manager accept the mode and switch the display manager; always returns false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A59A0, length 137 bytes (+7 int3 padding). `ret 4`.
// Callees: GetCurrentProcess @ [0x0143FD6C], SetPriorityClass @ [0x0143FD70] (NORMAL_PRIORITY_CLASS 0x20, kept in ebp),
// the input manager's virtual slot 1 (bool), CDisplayManager::SetExclusiveMode(bool) @ 0x009C0020 (result dropped).
// Shape: the previous mode is a bool local spilled to the frame (esp+7) before the lock-out test; a two-pass `for` whose first
// pass stores the argument and second the old mode (`test ebx,ebx ; jne` arms), `!input || input->Accept(mode)` guards the
// display call; the shared `xor al,al` exit.
// Model: PDB CSystemManager (+0x58 PInputManager, +0x60 PDisplayManager counted pointers' object words); retail sits 8 bytes
// above the PDB past the counted-pointer block (ExclusiveMode +0x8E, PreventExclusiveModeChanges +0xED).
extern "C" __declspec(dllimport) void* __stdcall GetCurrentProcess();
extern "C" __declspec(dllimport) int __stdcall SetPriorityClass(void* process, unsigned long priorityClass);

struct CInputManager {
    virtual void v00();
    virtual bool SetExclusiveMode(bool exclusive);  // slot 1
};
struct CDisplayManager {
    long SetExclusiveMode(bool exclusive);          // 0x009C0020 (ECreateDeviceSuccess)
};

struct CSystemManager {
    unsigned char _pad00[0x58];
    CInputManager* PInputManager;               // +0x58
    void* PInputManagerInfo;                    // +0x5c
    CDisplayManager* PDisplayManager;           // +0x60
    void* PDisplayManagerInfo;                  // +0x64
    unsigned char _pad68[0x8e - 0x68];
    bool ExclusiveMode;                         // +0x8e
    unsigned char _pad8f[0xed - 0x8f];
    bool PreventExclusiveModeChanges;           // +0xed
    bool SetExclusiveMode(bool exclusive);
};

bool CSystemManager::SetExclusiveMode(bool exclusive)
{
    bool oldMode = ExclusiveMode;
    if (!PreventExclusiveModeChanges) {
        void* process = GetCurrentProcess();
        for (long pass = 0; pass < 2; ++pass) {
            if (pass == 0)
                ExclusiveMode = exclusive;
            else
                ExclusiveMode = oldMode;
            SetPriorityClass(process, 0x20);
            if (!PInputManager || PInputManager->SetExclusiveMode(ExclusiveMode))
                PDisplayManager->SetExclusiveMode(ExclusiveMode);
        }
    }
    return false;
}
