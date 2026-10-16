// CThreadedFileManager::Uninitialise() -- shuts the file-loading thread down: clears Initialised, raises ExitThread and signals the event under the lock, spins (Sleep(0) with the lock released) until the thread reports ThreadFinished, closes the event and deletes the critical section
// Leaf: Uninitialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098E0D0, length 150 bytes, `ret`.
// Callees (KERNEL32 imports): EnterCriticalSection (IAT 0x0143FE0C), LeaveCriticalSection (0x0143FE10), SetEvent
// (0x0143FDE0), Sleep (0x0143FE1C), CloseHandle (0x0143FE30), DeleteCriticalSection (0x0143FDE4).
// Layout (retail; the PDB containers are larger, so Objects and everything after it sit 0x38 below Ego_r.pdb):
// DeviceQueue[2][7] +0x04 (16 bytes each), CurrentDeviceAtPriorityLevel[7] +0xe4, CurrentDeviceEndOfTimeSlice[7]
// +0x100, CurrentlyReadingObject +0x138, Objects (12-byte map) +0x13c, Initialised +0x148, ExitThread +0x149,
// ThreadFinished +0x14a, CriticalSection +0x14c, Event +0x164 (offsets shared with the landed 0x0098E770).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);   // IAT 0x0143FDE4
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);                // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);               // IAT 0x0143FE30
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CThreadedFileManager {
    unsigned char _pad00[0x148];                   // vtable, DeviceQueue, device levels, time slices, current object, Objects
    bool  Initialised;                             // +0x148
    bool  ExitThread;                              // +0x149
    bool  ThreadFinished;                          // +0x14a
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void* Event;                                   // +0x164
    void Uninitialise();
};
void CThreadedFileManager::Uninitialise()
{
    if (Initialised)
    {
        Initialised = false;
        EnterCriticalSection(&CriticalSection);
        ExitThread = true;
        SetEvent(Event);
        while (!ThreadFinished)
        {
            LeaveCriticalSection(&CriticalSection);
            Sleep(0);
            EnterCriticalSection(&CriticalSection);
        }
        if (Event)
        {
            CloseHandle(Event);
            Event = 0;
        }
        LeaveCriticalSection(&CriticalSection);
        DeleteCriticalSection(&CriticalSection);
    }
}