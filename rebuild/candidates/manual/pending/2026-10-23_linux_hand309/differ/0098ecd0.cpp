// CThreadedFileManager::ProcessObjects() -- one pass of the file thread: under the lock, finish the object whose read completed (or wait while one is still reading / processing is blocked); pick the next pending object, highest priority first, round-robin over the two devices with per-device time slices; then run its callback (if any) and start its overlapped read or write, and wait alertably on the event
// Grade: DIFFER(777v786) -- logic complete; retail homes `now` in an 8-aligned frame (push ebp/and esp,-8) where ours keeps it on the x87 stack, caches &CriticalSection in esi, and lays the leave/wait block as the fall-through of the status==2 test
// Leaf: ProcessObjects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098ECD0, length 786 bytes (8-byte aligned frame, plain `ret`).
// Callees: Enter/LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), WaitForSingleObjectEx (IAT 0x0143FE08),
//   SetEvent (0x0143FDE0), SleepEx (0x0143FDDC), ReadFileEx (0x0143FDF4), the write-side ...FileEx (0x0143FDD8),
//   CThreadedFileManager::OnFinishObject 0x0098EBC0, RemoveObjectFromPendingMap 0x0098E770, GFGetTime 0x009E1BC0 (double),
//   CFileInstallerSingleton::Get 0x00404440 (static __fastcall, singleton pointer 0x013CA818), CFileInstaller::Pause
//   0x009C31E0 / Unpause 0x009C3250, the completion routine 0x0098E320, the error hook 0x013BC864 (called with "").
// Layout: Ego_r.pdb CThreadedFileManager less 0x38 past DeviceQueue[2][7] (16-byte entries in retail, as the landed
//   0x0098E770 / 0x0098EFF0): CurrentDeviceAtPriorityLevel +0xe4, CurrentDeviceEndOfTimeSlice +0x100, CurrentlyReadingObject
//   +0x138, CriticalSection +0x14c, Event +0x164, BlockPendingObjectProcessingRefCount +0x16c, DeviceTimeSlice +0x170.
//   CFileObject as 0x0098EBC0; CThreadedFile file handle +0x08, write flag +0x18.
#include <string.h>
#pragma intrinsic(memset)
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);    // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);    // IAT 0x0143FE10
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long ms, int alertable);   // IAT 0x0143FE08
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                   // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long ms, int alertable);   // IAT 0x0143FDDC
struct OVERLAPPED_Fable { unsigned long Internal; unsigned long InternalHigh; unsigned long Offset; unsigned long OffsetHigh; void* hEvent; };
typedef void (__stdcall *COMPLETION_ROUTINE_Fable)(unsigned long error, unsigned long bytes, OVERLAPPED_Fable* overlapped);
extern "C" __declspec(dllimport) int __stdcall ReadFileEx(void* file, void* buffer, unsigned long bytes, OVERLAPPED_Fable* overlapped,
                                                        COMPLETION_ROUTINE_Fable routine);   // IAT 0x0143FDF4
extern "C" __declspec(dllimport) int __stdcall WriteFileEx(void* file, const void* buffer, unsigned long bytes, OVERLAPPED_Fable* overlapped,
                                                         COMPLETION_ROUTINE_Fable routine);  // IAT 0x0143FDD8
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
double GFGetTime();                                                                      // 0x009E1BC0
struct CFileInstaller {
    void Pause();                                                                        // 0x009C31E0
    void Unpause();                                                                      // 0x009C3250
};
struct CFileInstallerSingleton {
    static CFileInstaller& __fastcall Get();                                             // 0x00404440
};
struct CFileInstallerPointer { CFileInstaller* object; };
extern CFileInstallerPointer g_CFileInstallerSingleton_013CA818;                         // 0x013CA818
extern void (__fastcall *g_pFileErrorHook)(const char* message);                         // 0x013BC864
struct CFileObject;
struct CThreadedFileCallback {
    virtual void v00();
    virtual bool OnProcess(CFileObject* object, long priority);                          // slot 1 (+4)
};
struct CThreadedFile {
    void*         m_vtable;                        // +0x00
    long          DeviceID;                        // +0x04
    void*         Handle;                          // +0x08
    unsigned char _pad0c[0x18 - 0x0c];
    bool          Write;                           // +0x18
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
    OVERLAPPED_Fable* Overlapped;                  // +0x20
    void*         PManager;                        // +0x24
    CThreadedFile* PFile;                          // +0x28
};
struct CPendingNode {
    unsigned char _color[4];
    CPendingNode* parent;
    CPendingNode* left;                            // +0x08
    CPendingNode* right;
    CFileObject*  first;                           // +0x10 (sort key)
    CFileObject*  second;                          // +0x14
};
struct CPendingMap {                               // 12 bytes
    CPendingNode* head;
    unsigned long size;
    unsigned long _compare;
    CPendingNode* begin() const { return head->left; }
    CPendingNode* end() const { return head; }
};
struct CPendingObjectMap {                         // 16 bytes in retail
    CPendingMap   Objects;                         // +0x00
    CPendingNode* CurrentPosition;                 // +0x0c
};
void __stdcall OnFileCompletion(unsigned long error, unsigned long bytes, OVERLAPPED_Fable* overlapped);   // 0x0098E320
struct CThreadedFileManager {
    void*             m_vtable;                    // +0x00
    CPendingObjectMap DeviceQueue[2][7];           // +0x04
    long              CurrentDeviceAtPriorityLevel[7];   // +0xe4
    double            CurrentDeviceEndOfTimeSlice[7];    // +0x100
    CFileObject*      CurrentlyReadingObject;      // +0x138
    unsigned char     _pad13c[0x14c - 0x13c];      // Objects map, flags
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
    void*             Event;                       // +0x164
    unsigned long     NextObjectUID;               // +0x168
    long              BlockPendingObjectProcessingRefCount;   // +0x16c
    float             DeviceTimeSlice[2];          // +0x170
    void OnFinishObject(CFileObject* object);              // 0x0098EBC0
    void RemoveObjectFromPendingMap(CFileObject* object);  // 0x0098E770
    void ProcessObjects();
};

void CThreadedFileManager::ProcessObjects()
{
    EnterCriticalSection(&CriticalSection);
    if (CurrentlyReadingObject) {
        if (CurrentlyReadingObject->LoadingStatus == 2) {
        leave:
            LeaveCriticalSection(&CriticalSection);
        wait:
            WaitForSingleObjectEx(Event, 0xffffffff, 1);
            return;
        }
        OnFinishObject(CurrentlyReadingObject);
    }
    if (BlockPendingObjectProcessingRefCount != 0)
        goto leave;
    CFileObject* object = 0;
    double now = GFGetTime();
    for (long p = 6; p >= 0; --p) {
        if (object)
            break;
        if (now < CurrentDeviceEndOfTimeSlice[p]) {
            CPendingObjectMap& queue = DeviceQueue[CurrentDeviceAtPriorityLevel[p]][p];
            if (queue.CurrentPosition == queue.Objects.end())
                queue.CurrentPosition = queue.Objects.begin();
            if (queue.CurrentPosition != queue.Objects.end()) {
                object = queue.CurrentPosition->second;
                if (object)
                    continue;
            }
        }
        if (++CurrentDeviceAtPriorityLevel[p] == 2)
            CurrentDeviceAtPriorityLevel[p] = 0;
        for (long i = 0; i < 2; ++i) {
            CPendingObjectMap& queue = DeviceQueue[CurrentDeviceAtPriorityLevel[p]][p];
            if (queue.CurrentPosition == queue.Objects.end()) {
                queue.CurrentPosition = queue.Objects.begin();
                if (queue.CurrentPosition == queue.Objects.end()) {
                    if (++CurrentDeviceAtPriorityLevel[p] == 2)
                        CurrentDeviceAtPriorityLevel[p] = 0;
                    continue;
                }
            }
            object = queue.CurrentPosition->second;
            CurrentDeviceEndOfTimeSlice[p] = DeviceTimeSlice[CurrentDeviceAtPriorityLevel[p]] + now;
            break;
        }
    }
    if (!object)
        goto leave;
    if (object->PFile->Write) {
        memset(object->Overlapped, 0, sizeof(OVERLAPPED_Fable));
        object->Overlapped->Offset = object->FilePos;
        object->Overlapped->hEvent = object;
        RemoveObjectFromPendingMap(object);
        CurrentlyReadingObject = object;
        object->LoadingStatus = 2;
        LeaveCriticalSection(&CriticalSection);
        if (g_CFileInstallerSingleton_013CA818.object)
            CFileInstallerSingleton::Get().Pause();
        if (!WriteFileEx(object->PFile->Handle, object->PDst, object->Size, object->Overlapped, OnFileCompletion)) {
            if (g_pFileErrorHook)
                g_pFileErrorHook("");
            if (g_CFileInstallerSingleton_013CA818.object)
                CFileInstallerSingleton::Get().Unpause();
        }
    } else {
        bool failed = false;
        if (object->PCallback) {
            long priority = object->Priority;
            object->LoadingStatus = 1;
            LeaveCriticalSection(&CriticalSection);
            if (!object->PCallback->OnProcess(object, priority))
                failed = true;
            EnterCriticalSection(&CriticalSection);
            if (failed) {
                object->LoadingStatus = 0;
                LeaveCriticalSection(&CriticalSection);
                SetEvent(Event);
                SleepEx(1, 1);
                return;
            }
        }
        memset(object->Overlapped, 0, sizeof(OVERLAPPED_Fable));
        object->Overlapped->Offset = object->FilePos;
        object->Overlapped->hEvent = object;
        RemoveObjectFromPendingMap(object);
        CurrentlyReadingObject = object;
        object->LoadingStatus = 2;
        LeaveCriticalSection(&CriticalSection);
        if (g_CFileInstallerSingleton_013CA818.object)
            CFileInstallerSingleton::Get().Pause();
        ReadFileEx(object->PFile->Handle, object->PDst, object->Size, object->Overlapped, OnFileCompletion);
    }
    goto wait;
}
