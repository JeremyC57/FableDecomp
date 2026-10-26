// Behaviour test for 0098f0f0: src/0098f0f0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CThreadedFileManager::AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile* file)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
#pragma comment(lib, "kernel32.lib")
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* attributes, int manualReset, int initialState, const char* name);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long ms);
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);
// stubs: the two map operator[]s record their map and key and hand back a static slot
static int g_objCalls = 0, g_pendCalls = 0;
static const void* g_objThis = 0; static const void* g_pendThis = 0;
static unsigned long g_objKey = 0; static CFileObject* g_pendKey = 0;
static CFileObject* g_objSlot = 0; static CFileObject* g_pendSlot = 0;
CFileObject*& CObjectMap::operator[](const unsigned long& key) { ++g_objCalls; g_objThis = this; g_objKey = key; return g_objSlot; }
CFileObject*& CPendingMap::operator[](const CPendingObjectSortKey& key) { ++g_pendCalls; g_pendThis = this; g_pendKey = key.Object; return g_pendSlot; }
int main()
{
    static CThreadedFileManager m;
    InitializeCriticalSection(&m.CriticalSection);
    m.Event = CreateEventA(0, 1, 0, 0);
    m.NextObjectUID = 41;
    static CThreadedFile file; file.DeviceID = 1;
    static CThreadedFileCallback* cb = (CThreadedFileCallback*)0x1234;
    CThreadedFileObjectDesc desc;
    desc.PDst = (void*)0x5000; desc.FilePos = 77; desc.Size = 99; desc.PCallback = cb; desc.QueueWhenFinished = false; desc.Priority = 5;
    unsigned long uid = m.AddObject(desc, &file);
    CHECK(uid == 41 && m.NextObjectUID == 42);
    CFileObject* o = g_objSlot;
    CHECK(g_objCalls == 1 && g_objThis == &m.Objects && g_objKey == 41 && o != 0);
    CHECK(o->UID == 41 && o->PFile == &file && o->PManager == &m && o->LoadingStatus == 0 && o->Overlapped != 0);
    CHECK(o->Desc.PDst == (void*)0x5000 && o->Desc.FilePos == 77 && o->Desc.Size == 99 && o->Desc.PCallback == cb);
    CHECK(o->Desc.QueueWhenFinished == false && o->Desc.Priority == 5);
    CHECK(((unsigned char*)o->Overlapped)[0] == 0 && ((unsigned char*)o->Overlapped)[0x13] == 0);   // GPTR: zeroed
    CHECK(g_pendCalls == 1 && g_pendThis == &m.DeviceQueue[1][5].Objects && g_pendKey == o && g_pendSlot == o);
    CHECK(WaitForSingleObject(m.Event, 0) == 0);      // signalled
    CHECK(m.CriticalSection.RecursionCount == 0);     // left
    // a second request on device 0, priority 0 gets the next UID and its own queue
    ResetEvent(m.Event);
    desc.Priority = 0; file.DeviceID = 0;
    uid = m.AddObject(desc, &file);
    CHECK(uid == 42 && m.NextObjectUID == 43 && g_objKey == 42 && g_objSlot != o && g_objSlot->UID == 42);
    CHECK(g_pendCalls == 2 && g_pendThis == &m.DeviceQueue[0][0].Objects && g_pendKey == g_objSlot);
    CHECK(WaitForSingleObject(m.Event, 0) == 0);
    if (fails == 0) printf("0098f0f0_TEST PASS\n");
    return fails;
}
