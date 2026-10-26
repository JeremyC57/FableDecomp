// CThreadedFileManager::OnFinishObject(CFileObject* object) -- a finished read: the object's callback is told with the lock released (slot 2, true), the callback dropped, the status set to 4 (finished) and the current-object slot cleared; unless the object is to be queued, it is erased from the Objects map, its OVERLAPPED block freed and the object deleted
// Leaf: OnFinishObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098EBC0, length 119 bytes (padded to 128), `ret 4`.
// Callees: LeaveCriticalSection / EnterCriticalSection (IAT 0x0143FE10 / 0x0143FE0C), CThreadedFileCallback slot 2
// (+8, one bool argument), the Objects map erase(const key&) @ 0x00990D60, GlobalFree (IAT 0x0143FDF8),
// operator delete @ 0x00BFE9BC.
// Layout: as 0x0098E0D0 (CurrentlyReadingObject +0x138, Objects +0x13c, CriticalSection +0x14c); CFileObject (PDB):
// PCallback +0x0c, QueueWhenFinished +0x10, UID +0x18, LoadingStatus +0x1c, Overlapped +0x20.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);   // IAT 0x0143FDE4
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);                // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);               // IAT 0x0143FE30
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* hMem);                // IAT 0x0143FDF8
void operator delete(void* p);                                                           // 0x00BFE9BC
struct CThreadedFileCallback {
    virtual void v00(); virtual void v01();
    virtual void OnFinished(bool success);         // slot 2 (+8)
};
struct CFileObject {                               // CThreadedFileManager::CFileObject (0x40 bytes)
    void*         PDst;                            // +0x00 Desc.PDst
    unsigned long FilePos;                         // +0x04 Desc.FilePos
    unsigned long Size;                            // +0x08 Desc.Size
    CThreadedFileCallback* PCallback;              // +0x0c Desc.PCallback
    bool          QueueWhenFinished;               // +0x10 Desc.QueueWhenFinished
    long          Priority;                        // +0x14 Desc.Priority
    unsigned long UID;                             // +0x18
    long          LoadingStatus;                   // +0x1c EFileLoadingStatus
    void*         Overlapped;                      // +0x20 (GlobalAlloc'd _OVERLAPPED)
    void*         PManager;                        // +0x24
    void*         PFile;                           // +0x28 CThreadedFile*
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
    CObjectNode* operator->() const { return current; }
};
struct CObjectMap {                                // Objects: map<unsigned long, CFileObject*> (12 bytes in retail)
    CObjectNode*  head;                            // +0x00
    unsigned long size;                            // +0x04
    unsigned long _compare;                        // +0x08
    CObjectIterator find(const unsigned long& key);   // 0x009902D0
    unsigned long erase(const unsigned long& key);   // 0x00990D60
    CObjectIterator end() { return CObjectIterator(head); }
};
class CThreadedFileManager { public:
    unsigned char _pad00[0x138];                   // vtable, DeviceQueue, device levels, time slices
    CFileObject* CurrentlyReadingObject;           // +0x138
    CObjectMap Objects;                            // +0x13c
    bool  Initialised;                             // +0x148
    bool  ExitThread;                              // +0x149
    bool  ThreadFinished;                          // +0x14a
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void* Event;                                   // +0x164
    void OnFinishObject(CFileObject* object);
};
void CThreadedFileManager::OnFinishObject(CFileObject* object)
{
    if (object->PCallback)
    {
        LeaveCriticalSection(&CriticalSection);
        object->PCallback->OnFinished(true);
        EnterCriticalSection(&CriticalSection);
        object->PCallback = 0;
    }
    object->LoadingStatus = 4;
    CurrentlyReadingObject = 0;
    if (!object->QueueWhenFinished)
    {
        Objects.erase(object->UID);
        if (object->Overlapped)
            GlobalFree(object->Overlapped);
        delete object;
    }
}