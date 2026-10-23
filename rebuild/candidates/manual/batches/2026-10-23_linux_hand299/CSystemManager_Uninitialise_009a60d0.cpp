// CSystemManager::Uninitialise() -- tears the system layer down: releases the manager counted pointers, the global
// drive manager and the global registered-entry table, unhooks the mesh bank from the display's reset callbacks,
// shuts the scratch buffer, destroys the owned window, deletes the critical section and drops the init count.
// Retail 0x009A60D0, 0x29B bytes. Callees: CInitBaseClass::IsInitialised 0x0099A330 (`return Valid;`, the
// manifest label CTCInventoryBase::GetCanItemBeQuickAssigned is propagated), CInitBaseClass::Uninitialise 0x0099A360
// (tail jump), CDisplayManager::RemoveDeviceResetCallback 0x009BF5A0 (label LSocketServer::CloseSocket is
// propagated; the landed 0x00B25D80 names it), CScratchBuffer::Uninitialise 0x009F4950, operator delete 0x00BFE9BC,
// DestroyWindow [0x01440340], DeleteCriticalSection [0x0143FDE4].
// Shape: every CCountedPointer release is the inline `--count; if (count == 0) { destroy(object); delete block; }`
// with the block re-read through the member, then `Info = 0; Data = 0;`.
// Layout: retail CSystemManager has one extra CCountedPointer at +0x84 (Ego_r.pdb has none): every member from
// WindowInitialised on sits 8 bytes above the PDB offset (ApplicationWinHandle +0x94, WindowValid +0xe8,
// AttachedToExternalWindow +0xec, ProfileManager +0xf8, SystemColours +0x100 as in 0x009A5490).
struct SCountBlock {
    long RefCount;                              // +0
    void (__fastcall* Destroy)(void* object);  // +4
    void* Object;                               // +8
};

template <class T> struct CCountedPointer {
    T* Data;                                    // +0
    SCountBlock* Info;                          // +4
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};

struct CDeviceResetCallback { virtual void OnDeviceReset(); };
struct CDebugManager; struct CInputManager; struct CGraphicBankManager; struct CFontManager; struct CDriveManager;
struct CProfileManager; struct CSystemObject;
struct CMeshBankManager { void* Base0; CDeviceResetCallback ResetCallback; };   // the reset-callback base sits at +4
struct CDisplayManager { void RemoveDeviceResetCallback(CDeviceResetCallback* callback); };   // 0x009BF5A0

struct CInitBaseClass {
    virtual void v00();
    bool Valid;                                 // +0x04
    bool IsInitialised() const;                 // 0x0099A330
    void Uninitialise();                        // 0x0099A360
};

struct CScratchBuffer : CInitBaseClass {
    void Uninitialise();                        // 0x009F4950
};

struct SRegisteredEntry {                      // 16-byte entries of the global table
    CCountedPointer<CSystemObject> Object;      // +0
    long Key;                                   // +8
    long Flags;                                 // +0xc
};

struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) int __stdcall DestroyWindow(void* window);                       // [0x01440340]
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* cs);  // [0x0143FDE4]

extern CCountedPointer<CDriveManager> g_DriveManager;   // 0x013CAA28
extern SRegisteredEntry* g_RegisteredEntries;          // 0x0138E074
extern unsigned long g_RegisteredEntryCount;           // 0x0129AB30
extern void* g_RegisteredEntryCursor;                  // 0x013C5A74
extern void* g_RegisteredEntryLast;                    // 0x013C5A78
extern CScratchBuffer g_ScratchBuffer;                 // 0x013CB498
extern unsigned char g_SystemInitialiseCount;          // 0x013CA608

class CSystemManager : public CInitBaseClass {
public:
    bool WindowsQuit;                                   // +0x08
    bool ApplicationActive;                             // +0x09
    bool ApplicationHasInputFocus;                      // +0x0a
    unsigned char _pad0b[0x34 - 0x0b];
    _RTL_CRITICAL_SECTION CriticalSection;              // +0x34
    void* MainFibre;                                    // +0x4c
    CCountedPointer<CDebugManager> LibDebugManager;     // +0x50
    CCountedPointer<CInputManager> PInputManager;       // +0x58
    CCountedPointer<CDisplayManager> PDisplayManager;   // +0x60
    CDebugManager* DebugManager;                        // +0x68
    CCountedPointer<CGraphicBankManager> PGraphicBankManager; // +0x6c
    CCountedPointer<CMeshBankManager> PMeshBankManager; // +0x74
    CCountedPointer<CFontManager> PFontManager;         // +0x7c
    CCountedPointer<CSystemObject> PExtraManager;       // +0x84 (retail only)
    bool WindowInitialised;                             // +0x8c
    bool DDrawInitialised;                              // +0x8d
    bool ExclusiveMode;                                 // +0x8e
    void* WinInstanceHandle;                            // +0x90
    void* ApplicationWinHandle;                         // +0x94
    unsigned char _pad98[0xe8 - 0x98];
    bool WindowValid;                                   // +0xe8
    bool RestoreExclusive;                              // +0xe9
    bool Restore;                                       // +0xea
    bool WaitWhileInactive;                             // +0xeb
    bool AttachedToExternalWindow;                      // +0xec
    unsigned char _padED[0xf8 - 0xed];
    CCountedPointer<CProfileManager> ProfileManager;    // +0xf8
    void Uninitialise();
};

void CSystemManager::Uninitialise()
{
    if (IsInitialised() == true)
    {
        PInputManager.Release();
        PFontManager.Release();
        g_DriveManager.Release();
        for (unsigned long i = 0; i != g_RegisteredEntryCount; ++i)
        {
            SRegisteredEntry& entry = g_RegisteredEntries[i];
            entry.Object.Release();
            entry.Flags = 0;
        }
        g_RegisteredEntryCursor = 0;
        g_RegisteredEntryLast = 0;
        PExtraManager.Release();
        PGraphicBankManager.Release();
        if (PDisplayManager.Data)
            PDisplayManager.Data->RemoveDeviceResetCallback(PMeshBankManager.Data ? &PMeshBankManager.Data->ResetCallback : 0);
        PMeshBankManager.Release();
        if (g_ScratchBuffer.IsInitialised())
            g_ScratchBuffer.Uninitialise();
        LibDebugManager.Release();
        PDisplayManager.Release();
        if (AttachedToExternalWindow)
            ApplicationWinHandle = 0;
        else if (WindowValid == true)
        {
            DestroyWindow(ApplicationWinHandle);
            WindowValid = false;
            ApplicationWinHandle = 0;
        }
        ProfileManager.Release();
        DeleteCriticalSection(&CriticalSection);
        --g_SystemInitialiseCount;
        CInitBaseClass::Uninitialise();
    }
}
