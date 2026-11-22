#include <map>
#include <stdio.h>
#include <string.h>
#include "engine/EThreadedFilePriority.h"

#define TRUE 1
#define INFINITE 0xffffffffUL
enum EThreadedFileDeviceID { THREADED_FILE_DEVICE_HARD_DISK, THREADED_FILE_DEVICE_DVD, THREADED_FILE_DEVICE_COUNT };
enum EFileLoadingStatus { FILE_LOADING_PENDING, FILE_LOADING_READING, FILE_LOADING_READ_ISSUED, FILE_LOADING_READ_COMPLETE };
struct _OVERLAPPED {
    unsigned long Internal, InternalHigh, Offset, OffsetHigh;
    void* hEvent;
};
typedef void (*Completion)(unsigned long, unsigned long, _OVERLAPPED*);
struct CRITICAL_SECTION {};
struct CThreadedFile {
    bool OpenedForWrite;
    void* FileHandle;
    bool IsOpenForWrite() const { return OpenedForWrite; }
};
struct CThreadedFileCallback {
    bool permit;
    bool OnFileReadBegin(void**, EThreadedFilePriority);
};
struct CThreadedFileObjectDesc {
    void* PDst;
    unsigned long FilePos, Size;
    CThreadedFileCallback* PCallback;
    EThreadedFilePriority Priority;
};
struct CThreadedFileManager {
    struct CFileObject {
        CThreadedFileObjectDesc Desc;
        EFileLoadingStatus LoadingStatus;
        _OVERLAPPED* Overlapped;
        CThreadedFile* PFile;
    };
    struct CPendingObjectMap {
        std::map<long, CFileObject*> Objects;
        std::map<long, CFileObject*>::iterator CurrentPosition;
    };
    void ProcessObjects();
    void OnFinishObject(CFileObject*);
    void RemoveObjectFromPendingMap(CFileObject*);
    static void OnFileReadCompletion(unsigned long, unsigned long, _OVERLAPPED*) {}
    static void DoHardDiskErrorCallback(const char*);
    CPendingObjectMap DeviceQueue[THREADED_FILE_DEVICE_COUNT][THREADED_FILE_PRIORITY_COUNT];
    EThreadedFileDeviceID CurrentDeviceAtPriorityLevel[THREADED_FILE_PRIORITY_COUNT];
    double CurrentDeviceEndOfTimeSlice[THREADED_FILE_PRIORITY_COUNT];
    CFileObject* CurrentlyReadingObject;
    CRITICAL_SECTION CriticalSection;
    void* Event;
    long BlockPendingObjectProcessingRefCount;
    float DeviceTimeSlice[THREADED_FILE_DEVICE_COUNT];
    CThreadedFileManager() : CurrentlyReadingObject(0), Event(this), BlockPendingObjectProcessingRefCount(0)
    {
        DeviceTimeSlice[0] = 3.0f;
        DeviceTimeSlice[1] = 4.0f;
        for (long pri = 0; pri < THREADED_FILE_PRIORITY_COUNT; ++pri) {
            CurrentDeviceAtPriorityLevel[pri] = THREADED_FILE_DEVICE_HARD_DISK;
            CurrentDeviceEndOfTimeSlice[pri] = 0.0;
            for (long dev = 0; dev < THREADED_FILE_DEVICE_COUNT; ++dev)
                DeviceQueue[dev][pri].CurrentPosition = DeviceQueue[dev][pri].Objects.end();
        }
    }
};
typedef CThreadedFileManager::CFileObject FileObject;
static CThreadedFileManager* manager;
static FileObject* expected;
static char trace[64];
static int traceLen;
static int lockCount;
static bool valid;
static bool allocated;
static bool writeSucceeds;
static bool errorCallback;
static const double currentTime = 10.0;
static void Trace(char event, bool check)
{
    trace[traceLen++] = event;
    trace[traceLen] = 0;
    valid = valid && check;
}
static void EnterCriticalSection(CRITICAL_SECTION* section)
{
    Trace('E', section == &manager->CriticalSection && lockCount == 0);
    ++lockCount;
}
static void LeaveCriticalSection(CRITICAL_SECTION* section)
{
    Trace('L', section == &manager->CriticalSection && lockCount == 1);
    --lockCount;
}
static void WaitForSingleObjectEx(void* event, unsigned long timeout, int alertable)
{
    Trace('W', event == manager->Event && timeout == INFINITE && alertable && lockCount == 0);
}
static void SetEvent(void* event)
{
    Trace('S', event == manager->Event && lockCount == 0);
}
static void SleepEx(unsigned long duration, int alertable)
{
    Trace('Z', duration == 1 && alertable && lockCount == 0);
}
static double GFGetTime()
{
    Trace('T', lockCount == 1);
    return currentTime;
}
static bool CheckFile(void* handle, const void* data, unsigned long length, _OVERLAPPED* ov, Completion done)
{
    return expected && lockCount == 0 && manager->CurrentlyReadingObject == expected &&
           expected->LoadingStatus == FILE_LOADING_READ_ISSUED && handle == expected->PFile->FileHandle &&
           data == expected->Desc.PDst && length == expected->Desc.Size && ov == expected->Overlapped &&
           ov->Internal == 0 && ov->InternalHigh == 0 && ov->OffsetHigh == 0 &&
           ov->Offset == expected->Desc.FilePos && ov->hEvent == expected &&
           done == &CThreadedFileManager::OnFileReadCompletion;
}
static int ReadFileEx(void* handle, void* data, unsigned long length, _OVERLAPPED* ov, Completion done)
{
    Trace('r', CheckFile(handle,data,length,ov,done));
    return 1;
}
static int WriteFileEx(void* handle, const void* data, unsigned long length, _OVERLAPPED* ov, Completion done)
{
    Trace('w', CheckFile(handle,data,length,ov,done));
    return writeSucceeds;
}
struct CFileInstaller {
    static bool IsAllocated() { return allocated; }
    static CFileInstaller& Get()
    {
        static CFileInstaller instance;
        return instance;
    }
    void Pause() { Trace('P', lockCount == 0); }
    void Unpause() { Trace('U', lockCount == 0); }
};
bool CThreadedFileCallback::OnFileReadBegin(void** dst, EThreadedFilePriority priority)
{
    Trace('C', lockCount == 0 && dst == &expected->Desc.PDst && priority == expected->Desc.Priority &&
               expected->LoadingStatus == FILE_LOADING_READING);
    return permit;
}
void CThreadedFileManager::OnFinishObject(CFileObject* object)
{
    Trace('F', object == CurrentlyReadingObject && lockCount == 1);
    CurrentlyReadingObject = 0;
}
void CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject* object)
{
    Trace('R', lockCount == 1 && object == expected);
}
void CThreadedFileManager::DoHardDiskErrorCallback(const char* error)
{
    if (errorCallback) Trace('H', !*error && lockCount == 0);
}

void CThreadedFileManager::ProcessObjects()
{
    EnterCriticalSection(&CriticalSection);
    bool is_waiting_for_memory_to_be_freed = false;
    if (CurrentlyReadingObject) {
        if (CurrentlyReadingObject->LoadingStatus == FILE_LOADING_READ_ISSUED) {
            LeaveCriticalSection(&CriticalSection);
            WaitForSingleObjectEx(Event, INFINITE, TRUE);
            return;
        }
        OnFinishObject(CurrentlyReadingObject);
    }
    if (!BlockPendingObjectProcessingRefCount) {
        CFileObject* pending_object = 0;
        double this_time = GFGetTime();
        for (long pri = THREADED_FILE_PRIORITY_BLOCKING; pri >= 0 && !pending_object; --pri) {
            EThreadedFileDeviceID curr_dev = CurrentDeviceAtPriorityLevel[pri];
            (void)curr_dev;
            if (this_time < CurrentDeviceEndOfTimeSlice[pri]) {
                CPendingObjectMap* pmap = &DeviceQueue[CurrentDeviceAtPriorityLevel[pri]][pri];
                if (pmap->CurrentPosition == pmap->Objects.end())
                    pmap->CurrentPosition = pmap->Objects.begin();
                if (pmap->CurrentPosition != pmap->Objects.end()) {
                    pending_object = pmap->CurrentPosition->second;
                    if (pending_object)
                        continue;
                }
            }
            CurrentDeviceAtPriorityLevel[pri] = (EThreadedFileDeviceID)(CurrentDeviceAtPriorityLevel[pri] + 1);
            if (CurrentDeviceAtPriorityLevel[pri] == THREADED_FILE_DEVICE_COUNT)
                CurrentDeviceAtPriorityLevel[pri] = THREADED_FILE_DEVICE_HARD_DISK;
            for (long dev_count = 0; dev_count < THREADED_FILE_DEVICE_COUNT; ++dev_count) {
                CPendingObjectMap* pmap = &DeviceQueue[CurrentDeviceAtPriorityLevel[pri]][pri];
                if (pmap->CurrentPosition == pmap->Objects.end())
                    pmap->CurrentPosition = pmap->Objects.begin();
                if (pmap->CurrentPosition != pmap->Objects.end()) {
                    pending_object = pmap->CurrentPosition->second;
                    CurrentDeviceEndOfTimeSlice[pri] = DeviceTimeSlice[CurrentDeviceAtPriorityLevel[pri]] + this_time;
                    break;
                }
                CurrentDeviceAtPriorityLevel[pri] = (EThreadedFileDeviceID)(CurrentDeviceAtPriorityLevel[pri] + 1);
                if (CurrentDeviceAtPriorityLevel[pri] == THREADED_FILE_DEVICE_COUNT)
                    CurrentDeviceAtPriorityLevel[pri] = THREADED_FILE_DEVICE_HARD_DISK;
            }
        }
        if (pending_object) {
            if (pending_object->PFile->IsOpenForWrite()) {
                memset(pending_object->Overlapped, 0, sizeof(_OVERLAPPED));
                pending_object->Overlapped->Offset = pending_object->Desc.FilePos;
                pending_object->Overlapped->hEvent = pending_object;
                RemoveObjectFromPendingMap(pending_object);
                CurrentlyReadingObject = pending_object;
                pending_object->LoadingStatus = FILE_LOADING_READ_ISSUED;
                LeaveCriticalSection(&CriticalSection);
                if (CFileInstaller::IsAllocated())
                    CFileInstaller::Get().Pause();
                int ret = WriteFileEx(pending_object->PFile->FileHandle, pending_object->Desc.PDst,
                                      pending_object->Desc.Size, pending_object->Overlapped, OnFileReadCompletion);
                if (!ret) {
                    DoHardDiskErrorCallback("");
                    if (CFileInstaller::IsAllocated())
                        CFileInstaller::Get().Unpause();
                }
            } else {
                bool callback_failed = false;
                if (pending_object->Desc.PCallback) {
                    pending_object->LoadingStatus = FILE_LOADING_READING;
                    EThreadedFilePriority priority = pending_object->Desc.Priority;
                    LeaveCriticalSection(&CriticalSection);
                    if (!pending_object->Desc.PCallback->OnFileReadBegin(&pending_object->Desc.PDst, priority))
                        callback_failed = true;
                    EnterCriticalSection(&CriticalSection);
                }
                if (callback_failed) {
                    pending_object->LoadingStatus = FILE_LOADING_PENDING;
                    is_waiting_for_memory_to_be_freed = true;
                    LeaveCriticalSection(&CriticalSection);
                } else {
                    memset(pending_object->Overlapped, 0, sizeof(_OVERLAPPED));
                    pending_object->Overlapped->Offset = pending_object->Desc.FilePos;
                    pending_object->Overlapped->hEvent = pending_object;
                    RemoveObjectFromPendingMap(pending_object);
                    CurrentlyReadingObject = pending_object;
                    pending_object->LoadingStatus = FILE_LOADING_READ_ISSUED;
                    LeaveCriticalSection(&CriticalSection);
                    if (CFileInstaller::IsAllocated())
                        CFileInstaller::Get().Pause();
                    ReadFileEx(pending_object->PFile->FileHandle, pending_object->Desc.PDst,
                               pending_object->Desc.Size, pending_object->Overlapped, OnFileReadCompletion);
                }
            }
        } else {
            LeaveCriticalSection(&CriticalSection);
        }
    } else {
        LeaveCriticalSection(&CriticalSection);
    }
    if (is_waiting_for_memory_to_be_freed) {
        SetEvent(Event);
        SleepEx(1, TRUE);
    } else {
        WaitForSingleObjectEx(Event, INFINITE, TRUE);
    }
}

struct Fixture {
    CThreadedFileManager files;
    CThreadedFile file;
    CThreadedFileCallback callback;
    _OVERLAPPED ov;
    FileObject object;
    Fixture()
    {
        manager = &files;
        expected = &object;
        traceLen = lockCount = 0;
        trace[0] = 0;
        valid = writeSucceeds = errorCallback = true;
        allocated = false;
        file.OpenedForWrite = false;
        file.FileHandle = &file;
        callback.permit = true;
        memset(&ov, 0xc7, sizeof(ov));
        object.Desc.PDst = &ov;
        object.Desc.FilePos = 32;
        object.Desc.Size = 16;
        object.Desc.PCallback = 0;
        object.Desc.Priority = THREADED_FILE_PRIORITY_HIGH;
        object.LoadingStatus = FILE_LOADING_PENDING;
        object.Overlapped = &ov;
        object.PFile = &file;
    }
    void Queue(long device, long priority, FileObject* item)
    {
        files.DeviceQueue[device][priority].Objects[1] = item;
    }
    bool Check(const char* wanted)
    {
        files.ProcessObjects();
        if (valid && !lockCount && !strcmp(trace,wanted)) return true;
        printf("Expected %s, got %s, valid %d, locks %d\n",wanted,trace,valid,lockCount);
        return false;
    }
};
#define CHECK(expr) do { if (!(expr)) { printf("0098ecd0_TEST failure at %d\n", __LINE__); return 1; } } while (0)
int main()
{
    {
        Fixture f;
        f.files.CurrentlyReadingObject = &f.object;
        f.object.LoadingStatus = FILE_LOADING_READ_ISSUED;
        CHECK(f.Check("ELW"));
        CHECK(f.files.CurrentlyReadingObject == &f.object);
    }
    {
        Fixture f;
        f.files.CurrentlyReadingObject = &f.object;
        f.object.LoadingStatus = FILE_LOADING_READ_COMPLETE;
        f.files.BlockPendingObjectProcessingRefCount = 1;
        CHECK(f.Check("EFLW"));
        CHECK(!f.files.CurrentlyReadingObject);
    }
    {
        Fixture f;
        f.files.BlockPendingObjectProcessingRefCount = 2;
        f.Queue(0, 6, &f.object);
        CHECK(f.Check("ELW"));
        CHECK(f.object.LoadingStatus == FILE_LOADING_PENDING);
    }
    {
        Fixture f;
        CHECK(f.Check("ETLW"));
        CHECK(!f.files.CurrentlyReadingObject);
    }
    {
        Fixture f;
        FileObject lower = f.object;
        f.Queue(1, 6, &f.object);
        f.Queue(0, 5, &lower);
        f.files.CurrentDeviceEndOfTimeSlice[6] = currentTime;
        CHECK(f.Check("ETRLrW"));
        CHECK(f.files.CurrentDeviceAtPriorityLevel[6] == THREADED_FILE_DEVICE_DVD);
        CHECK(f.files.CurrentDeviceEndOfTimeSlice[6] == currentTime + 4.0);
    }
    {
        Fixture f;
        f.Queue(0, 6, &f.object);
        f.files.CurrentDeviceEndOfTimeSlice[6] = currentTime + 1.0;
        CHECK(f.Check("ETRLrW"));
        CHECK(f.files.CurrentDeviceEndOfTimeSlice[6] == currentTime + 1.0);
    }
    {
        Fixture f;
        f.Queue(0, 6, 0);
        f.Queue(1, 6, &f.object);
        f.files.CurrentDeviceEndOfTimeSlice[6] = currentTime + 1.0;
        CHECK(f.Check("ETRLrW"));
        CHECK(f.files.CurrentDeviceEndOfTimeSlice[6] == currentTime + 4.0);
    }
    {
        Fixture f;
        f.Queue(0, 4, &f.object);
        f.files.CurrentDeviceEndOfTimeSlice[4] = currentTime - 1.0;
        f.object.Desc.PCallback = &f.callback;
        allocated = true;
        CHECK(f.Check("ETLCERLPrW"));
        CHECK(f.files.CurrentDeviceAtPriorityLevel[4] == THREADED_FILE_DEVICE_HARD_DISK);
        CHECK(f.files.CurrentDeviceEndOfTimeSlice[4] == currentTime + 3.0);
    }
    {
        Fixture f;
        f.Queue(0, 6, &f.object);
        f.object.Desc.PCallback = &f.callback;
        f.callback.permit = false;
        allocated = true;
        CHECK(f.Check("ETLCELSZ"));
        CHECK(f.object.LoadingStatus == FILE_LOADING_PENDING && !f.files.CurrentlyReadingObject);
    }
    {
        Fixture f;
        f.Queue(0, 6, &f.object);
        f.file.OpenedForWrite = true;
        allocated = true;
        CHECK(f.Check("ETRLPwW"));
    }
    {
        Fixture f;
        f.Queue(0, 6, &f.object);
        f.file.OpenedForWrite = true;
        allocated = true;
        writeSucceeds = false;
        CHECK(f.Check("ETRLPwHUW"));
    }
    {
        Fixture f;
        f.Queue(0, 6, &f.object);
        f.file.OpenedForWrite = true;
        writeSucceeds = false;
        errorCallback = false;
        CHECK(f.Check("ETRLwW"));
    }
    puts("0098ecd0_TEST PASS");
    return 0;
}