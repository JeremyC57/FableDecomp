// CThreadedFileManager::RemoveObject(unsigned long uid) -- under the lock, finds the file object by UID; a still-pending object is first taken off its device queue; an object being read (status 1..3) is kept and false returned; otherwise its OVERLAPPED block is freed, the object deleted and the map entry erased (true)
// Leaf: RemoveObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098EB20, length 155 bytes (padded to 160), `ret 4`.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), the Objects map find @ 0x009902D0
// (landed CRetailTree_ulongkey::find, iterator through a hidden pointer), CThreadedFileManager::
// RemoveObjectFromPendingMap @ 0x0098E770, GlobalFree (IAT 0x0143FDF8), operator delete @ 0x00BFE9BC, the map's
// erase(iterator) @ 0x00990430 (iterator by value: retail rebuilds it in delete's dead argument slot).
// Layout: as 0x0098E0D0 (Objects +0x13c, CriticalSection +0x14c); CFileObject (PDB, no container): LoadingStatus
// +0x1c, Overlapped +0x20; the map node keeps the key at +0x10 and the object at +0x14.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);   // IAT 0x0143FDE4
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);                // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);               // IAT 0x0143FE30
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* hMem);                // IAT 0x0143FDF8
void operator delete(void* p);                                                           // 0x00BFE9BC
struct CFileObject {                               // CThreadedFileManager::CFileObject (0x40 bytes)
    void*         PDst;                            // +0x00 Desc.PDst
    unsigned long FilePos;                         // +0x04 Desc.FilePos
    unsigned long Size;                            // +0x08 Desc.Size
    void*         PCallback;                       // +0x0c Desc.PCallback
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
    void erase(CObjectIterator where);                // 0x00990430
    CObjectIterator end() { return CObjectIterator(head); }
};
class CThreadedFileManager { public:
    unsigned char _pad00[0x13c];                   // vtable, DeviceQueue, device levels, time slices, current object
    CObjectMap Objects;                            // +0x13c
    bool  Initialised;                             // +0x148
    bool  ExitThread;                              // +0x149
    bool  ThreadFinished;                          // +0x14a
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void* Event;                                   // +0x164
    void RemoveObjectFromPendingMap(CFileObject* object);   // 0x0098E770
    bool RemoveObject(unsigned long uid);
};
bool CThreadedFileManager::RemoveObject(unsigned long uid)
{
    EnterCriticalSection(&CriticalSection);
    CObjectIterator it = Objects.find(uid);
    if (it != Objects.end())
    {
        CFileObject* object = it->second;
        if (object->LoadingStatus == 0)
            RemoveObjectFromPendingMap(object);
        if (object->LoadingStatus == 1 || object->LoadingStatus == 2 || object->LoadingStatus == 3)
        {
            LeaveCriticalSection(&CriticalSection);
            return false;
        }
        if (object->Overlapped)
            GlobalFree(object->Overlapped);
        delete object;
        Objects.erase(it);
    }
    LeaveCriticalSection(&CriticalSection);
    return true;
}