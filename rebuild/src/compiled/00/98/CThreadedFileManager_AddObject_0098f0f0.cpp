// CThreadedFileManager::AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile* file) -- under the lock, create a file object for the request with the next UID and a fresh OVERLAPPED block, register it in the UID map and in its device/priority pending queue, wake the loader thread and return the UID
// Leaf: AddObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F0F0, length 239 bytes (`ret 8`; 240 with padding).
// Callees: EnterCriticalSection / LeaveCriticalSection / GlobalAlloc / SetEvent (IAT 0x0143FE0C / 0x0143FE10 /
// 0x0143FDD4 / 0x0143FDE0), operator new @ 0x00BFEA1A, map<unsigned long, CFileObject*>::operator[] @ 0x009913D0
// (manifest "CThreadedFileManager_AddObject" is a placeholder), map<CPendingObjectSortKey, CFileObject*>::operator[]
// @ 0x00991350.
// Layout: Ego_r.pdb CFileObject (Desc +0, UID +0x18, LoadingStatus +0x1c, Overlapped +0x20, PManager +0x24, PFile
// +0x28); CThreadedFileManager as the landed 0x0098E770 / 0x0098EB20 (DeviceQueue[2][7] of 16-byte retail entries at
// +4, Objects +0x13c, CriticalSection +0x14c, Event +0x164, NextObjectUID +0x168); CThreadedFile DeviceID +4.
// Shape: the UID is a named local (its slot is the map key); the object constructor (desc defaults: zeros, queue
// when finished, priority 3) is inlined behind `new`'s null test; the device ID is read into a named local first (as in
// the landed RemoveObjectFromPendingMap 0x0098E770) and the queue is indexed in place -- a `CPendingMap&` reference
// local to the queue swaps the index/key-address registers (eax/edx); the pending-queue key is a bare by-value sort-key
// temporary homed in the dead desc argument slot.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int flags, unsigned long bytes);   // IAT 0x0143FDD4
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
// operator new @ 0x00BFEA1A is the implicit global one
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };

class CThreadedFile { public:
    void* m_vtable;                                // +0x00
    long  DeviceID;                                // +0x04 EThreadedFileDeviceID
};
struct CThreadedFileCallback;
class CThreadedFileObjectDesc { public:                   // 24 bytes
    void*                  PDst;                   // +0x00
    unsigned long          FilePos;                // +0x04
    unsigned long          Size;                   // +0x08
    CThreadedFileCallback* PCallback;              // +0x0c
    bool                   QueueWhenFinished;      // +0x10
    long                   Priority;               // +0x14 EThreadedFilePriority
    CThreadedFileObjectDesc() : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true), Priority(3) { }
};
class CThreadedFileManager;
struct CFileObject {                               // CThreadedFileManager::CFileObject (0x40 bytes)
    CThreadedFileObjectDesc Desc;                  // +0x00
    unsigned long           UID;                   // +0x18
    long                    LoadingStatus;         // +0x1c EFileLoadingStatus
    void*                   Overlapped;            // +0x20 _OVERLAPPED*
    CThreadedFileManager*   PManager;              // +0x24
    CThreadedFile*          PFile;                 // +0x28
    double                  ReadStartTimeStamp;    // +0x30
    double                  RequestTimeStamp;      // +0x38
    CFileObject() : Overlapped(0) { }
};
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
struct CPendingObjectMap {                         // 16 bytes in retail
    CPendingMap Objects;                           // +0x00
    void*       NextObject;                        // +0x0c
};
struct CObjectMap {                                // Objects: map<unsigned long, CFileObject*> (12 bytes in retail)
    void*         head;
    unsigned long size;
    unsigned long _compare;
    CFileObject*& operator[](const unsigned long& key);   // 0x009913D0
};
class CThreadedFileManager { public:
    void*                 m_vtable;                // +0x000
    CPendingObjectMap     DeviceQueue[2][7];       // +0x004
    unsigned char         _padE4[0x13c - 0xe4];    // device levels, time slices, current object
    CObjectMap            Objects;                 // +0x13c
    bool                  Initialised;             // +0x148
    bool                  ExitThread;              // +0x149
    bool                  ThreadFinished;          // +0x14a
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void*                 Event;                   // +0x164
    unsigned long         NextObjectUID;           // +0x168
    unsigned long AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile* file);
};

unsigned long CThreadedFileManager::AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile* file)
{
    EnterCriticalSection(&CriticalSection);
    unsigned long uid = NextObjectUID;
    unsigned long key = uid;
    NextObjectUID = uid + 1;
    CFileObject* object = new CFileObject;
    object->UID = uid;
    object->Desc = desc;
    object->PFile = file;
    object->PManager = this;
    object->LoadingStatus = 0;
    object->Overlapped = GlobalAlloc(0x40, 0x14);
    Objects[key] = object;
    long device = object->PFile->DeviceID;
    DeviceQueue[device][object->Desc.Priority].Objects[CPendingObjectSortKey(object)] = object;
    SetEvent(Event);
    LeaveCriticalSection(&CriticalSection);
    return uid;
}