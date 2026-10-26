// CThreadedFileManager::SetObjectPriority(unsigned long uid, long priority) -- under the lock, finds the file object by UID; when its priority changes while it is still pending or queued (status 0/1) it is taken off its old device queue, given the new priority and re-inserted into DeviceQueue[device][priority]
// Leaf: SetObjectPriority
// Grade: RELOCATION_MATCH (retry helper C) -- same-TU bodies for the Objects map find 0x009902D0 (real lower-bound walk behind an inline find wrapper) and a by-value-key stand-in for the pending map's operator[] 0x00991350: the sort-key temporary then lives in the dead uid slot as retail. The named `device` local, read after the Priority store, gives retail's edx/eax/ecx rotation. Was DIFFER(151v151).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098EFF0, length 149 bytes (padded to 160), `ret 8`.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), the Objects map find
// @ 0x009902D0, CThreadedFileManager::RemoveObjectFromPendingMap @ 0x0098E770, the pending map's
// operator[](const CPendingObjectSortKey&) @ 0x00991350 (returns CFileObject*&).
// Layout: as 0x0098E0D0 / 0x0098E770: DeviceQueue[2][7] +0x04 (16-byte entries, the map first), Objects +0x13c,
// CriticalSection +0x14c; CFileObject Priority +0x14, LoadingStatus +0x1c, PFile +0x28 (CThreadedFile DeviceID +4).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);   // IAT 0x0143FDE4
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);                // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);               // IAT 0x0143FE30
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* hMem);                // IAT 0x0143FDF8
void operator delete(void* p);                                                           // 0x00BFE9BC
class CThreadedFile { public:
    void* m_vtable;                                // +0x00
    long  DeviceID;                                // +0x04 EThreadedFileDeviceID
};
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
    CThreadedFile* PFile;                          // +0x28
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
    CObjectIterator TreeFind(const unsigned long& key);   // 0x009902D0 (_Rb_tree::find)
    CObjectIterator find(const unsigned long& key) { return TreeFind(key); }
    void erase(CObjectIterator where);                // 0x00990430
    CObjectIterator end() { return CObjectIterator(head); }
};
#pragma auto_inline(off)
// Same-TU body of the map<unsigned long, CFileObject*>::find instantiation 0x009902D0 (the landed 0x006699C0 shape).
__declspec(noinline) CObjectIterator CObjectMap::TreeFind(const unsigned long& key)
{
    CObjectNode* result = head;
    CObjectNode* n = head->parent;
    while (n) {
        if (!(n->first < key)) {
            result = n;
            n = n->left;
        }
        else
            n = n->right;
    }
    CObjectNode* found = result;
    if (found == head || key < found->first)
        found = head;
    return CObjectIterator(found);
}
#pragma auto_inline(on)
struct CPendingObjectSortKey {
    CFileObject* Object;
    CPendingObjectSortKey(CFileObject* object) : Object(object) { }
};
struct CPendingMap {                               // map<CPendingObjectSortKey, CFileObject*>, 12 bytes
    unsigned char m_allocator;
    void*         m_head;
    unsigned long m_size;
    CFileObject*& operator[](const CPendingObjectSortKey& key);   // 0x00991350
};
CFileObject** PendingSlot_00991350(CPendingMap* map, CFileObject* object);
#pragma auto_inline(off)
// Same-TU stand-in for map<CPendingObjectSortKey, CFileObject*>::operator[] 0x00991350 (reads the key by value).
__declspec(noinline) CFileObject*& CPendingMap::operator[](const CPendingObjectSortKey& key)
{
    return *PendingSlot_00991350(this, key.Object);
}
#pragma auto_inline(on)
struct CPendingObjectMap {                         // 16 bytes in retail
    CPendingMap Objects;                           // +0x00
    void*       NextObject;                        // +0x0c (iterator)
};
class CThreadedFileManager { public:
    void*             m_vtable;                    // +0x00
    CPendingObjectMap DeviceQueue[2][7];           // +0x04
    unsigned char     _padE4[0x13c - 0xe4];        // device levels, time slices, current object
    CObjectMap Objects;                            // +0x13c
    bool  Initialised;                             // +0x148
    bool  ExitThread;                              // +0x149
    bool  ThreadFinished;                          // +0x14a
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void* Event;                                   // +0x164
    void RemoveObjectFromPendingMap(CFileObject* object);   // 0x0098E770
    void SetObjectPriority(unsigned long uid, long priority);
};
void CThreadedFileManager::SetObjectPriority(unsigned long uid, long priority)
{
    EnterCriticalSection(&CriticalSection);
    CObjectIterator it = Objects.find(uid);
    if (!(it != Objects.end()))
    {
        LeaveCriticalSection(&CriticalSection);
        return;
    }
    CFileObject* object = it->second;
    if (object->Priority != priority)
    {
        if (object->LoadingStatus == 0 || object->LoadingStatus == 1)
        {
            RemoveObjectFromPendingMap(object);
            object->Priority = priority;
            long device = object->PFile->DeviceID;
            DeviceQueue[device][priority].Objects[CPendingObjectSortKey(object)] = object;
        }
    }
    LeaveCriticalSection(&CriticalSection);
}