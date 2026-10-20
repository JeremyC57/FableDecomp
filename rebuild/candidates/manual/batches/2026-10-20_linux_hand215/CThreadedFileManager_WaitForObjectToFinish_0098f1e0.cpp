// CThreadedFileManager::WaitForObjectToFinish(unsigned long uid) -- under the lock, when the file object is known and not finished: raise it to priority 6, then keep releasing the lock, waking the loader thread and yielding until the object is gone or finished
// Leaf: WaitForObjectToFinish
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F1E0, length 179 bytes (`ret 4`; 192 with padding).
// Callees: EnterCriticalSection / LeaveCriticalSection / SetEvent / SleepEx (IAT 0x0143FE0C / 0x0143FE10 /
// 0x0143FDE0 / 0x0143FDDC), the Objects map find @ 0x009902D0 (iterator through a hidden pointer, key by reference),
// CThreadedFileManager::SetObjectPriority(unsigned long, EThreadedFilePriority) @ 0x0098EFF0.
// Layout: as the landed RemoveObject 0x0098EB20 (Objects +0x13c, CriticalSection +0x14c, Event +0x164; CFileObject
// LoadingStatus +0x1c, 4 = finished).
// Shape: both lookups go through one `__forceinline` IsObjectUnfinished helper (its iterator local shares one frame
// word; an iterator reassigned in the loop needs a second word for the hidden-pointer result); the `while` keeps its
// test at the top; the imports are
// dllimport (Enter cached in ebp, Leave in ebx inside the found arm).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long ms, int alertable);   // IAT 0x0143FDDC
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CFileObject {                               // CThreadedFileManager::CFileObject (0x40 bytes)
    unsigned char _pad00[0x1c];
    long          LoadingStatus;                   // +0x1c EFileLoadingStatus (4 = finished)
};
struct CObjectNode {                               // map<unsigned long, CFileObject*> node
    unsigned char _color[4];
    CObjectNode*  parent;
    CObjectNode*  left;
    CObjectNode*  right;
    unsigned long first;                           // +0x10
    CFileObject*  second;                          // +0x14
};
struct CObjectIterator {
    CObjectNode* current;
    CObjectIterator(CObjectNode* n) : current(n) { }
    CObjectIterator(const CObjectIterator& o) : current(o.current) { }
    bool operator!=(const CObjectIterator& o) const { return current != o.current; }
    bool operator==(const CObjectIterator& o) const { return current == o.current; }
    CObjectNode* operator->() const { return current; }
};
struct CObjectMap {                                // Objects: map<unsigned long, CFileObject*> (12 bytes in retail)
    CObjectNode*  head;                            // +0x00
    unsigned long size;                            // +0x04
    unsigned long _compare;                        // +0x08
    CObjectIterator find(const unsigned long& key);   // 0x009902D0
    CObjectIterator end() { return CObjectIterator(head); }
};
struct CThreadedFileManager {
    unsigned char _pad00[0x13c];                   // vtable, DeviceQueue, device levels, time slices, current object
    CObjectMap Objects;                            // +0x13c
    bool  Initialised;                             // +0x148
    bool  ExitThread;                              // +0x149
    bool  ThreadFinished;                          // +0x14a
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void* Event;                                   // +0x164
    void SetObjectPriority(unsigned long uid, long priority);   // 0x0098EFF0
    void WaitForObjectToFinish(unsigned long uid);
    __forceinline bool IsObjectUnfinished(const unsigned long& uid)
    {
        CObjectIterator it = Objects.find(uid);
        return it != Objects.end() && it->second->LoadingStatus != 4;
    }
};

void CThreadedFileManager::WaitForObjectToFinish(unsigned long uid)
{
    EnterCriticalSection(&CriticalSection);
    if (IsObjectUnfinished(uid)) {
        SetObjectPriority(uid, 6);
        while (IsObjectUnfinished(uid)) {
            LeaveCriticalSection(&CriticalSection);
            SetEvent(Event);
            SleepEx(0, 1);
            EnterCriticalSection(&CriticalSection);
        }
        LeaveCriticalSection(&CriticalSection);
        return;
    }
    LeaveCriticalSection(&CriticalSection);
}
