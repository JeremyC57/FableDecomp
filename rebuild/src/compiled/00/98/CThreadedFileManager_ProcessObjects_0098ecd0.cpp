// Process pending asynchronous file transfers in priority and device order.
// Leaf: ?ProcessObjects@CThreadedFileManager@@AAEXXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHREADEDFILE
#define FABLE_RETAIL_CTHREADEDFILE_PROCESS_PASS27
#define FABLE_RETAIL_CTHREADEDFILEMANAGER_PROCESSOBJECTS_PRIVATE   // NF293: Ego_r + FableWin AAE (private)
#define FABLE_RETAIL_WIN32_OVERLAPPED_FILE_API_PASS27
#define FABLE_RETAIL_CTBASESINGLETON_ISALLOCATED
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include <string.h>
#include "engine/CFileInstaller.h"
#include "engine/CThreadedFileManager.h"
#include "engine/CThreadedFileCallback.h"
#include "engine/GFGetTime.h"
#pragma intrinsic(memset)

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