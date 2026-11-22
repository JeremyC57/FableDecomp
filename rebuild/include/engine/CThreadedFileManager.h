#ifndef FABLE_ENGINE_CTHREADEDFILEMANAGER_H
#define FABLE_ENGINE_CTHREADEDFILEMANAGER_H
#ifdef FABLE_RETAIL_CTHREADEDFILE_PROCESS_PASS27
#include "fable_threaded_file.h"
#include "engine/CThreadedFileObjectDesc.h"
#include "engine/Win32Types.h"
#include "stlport/fable_map.h"
struct _OVERLAPPED;
enum EFileLoadingStatus { FILE_LOADING_PENDING, FILE_LOADING_READING, FILE_LOADING_READ_ISSUED, FILE_LOADING_READ_COMPLETE };
class CThreadedFileManager {
public:
    void ProcessObjects();
    static void __fastcall DoHardDiskErrorCallback(const char* message) {
        if (HardDiskErrorCallback) HardDiskErrorCallback(message);
    }
private:
    class CFileObject {
    public:
        CThreadedFileObjectDesc Desc;
        unsigned long UID;
        EFileLoadingStatus LoadingStatus;
        _OVERLAPPED* Overlapped;
        CThreadedFileManager* PManager;
        CThreadedFile* PFile;
        double ReadStartTimeStamp;
        double RequestTimeStamp;
    };
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;
        bool operator<(const CPendingObjectSortKey&) const;
    };
    class CPendingObjectMap {
    public:
        std::map<CPendingObjectSortKey, CFileObject*> Objects;
        std::map<CPendingObjectSortKey, CFileObject*>::iterator CurrentPosition;
    };
    FABLE_STATIC_ASSERT(sizeof(CFileObject) == 0x40);
    FABLE_STATIC_ASSERT(sizeof(CPendingObjectMap) == 0x10);
    void OnFinishObject(CFileObject*);
    void RemoveObjectFromPendingMap(CFileObject*);
    static void __stdcall OnFileReadCompletion(unsigned long, unsigned long, _OVERLAPPED*);
    static void (__fastcall* HardDiskErrorCallback)(const char*);
    unsigned char Padding[4];
    CPendingObjectMap DeviceQueue[THREADED_FILE_DEVICE_COUNT][THREADED_FILE_PRIORITY_COUNT];
    EThreadedFileDeviceID CurrentDeviceAtPriorityLevel[THREADED_FILE_PRIORITY_COUNT];
    double CurrentDeviceEndOfTimeSlice[THREADED_FILE_PRIORITY_COUNT];
    CFileObject* CurrentlyReadingObject;
    std::map<unsigned long, CFileObject*> Objects;
    bool Initialised;
    bool ExitThread;
    bool ThreadFinished;
    CRITICAL_SECTION CriticalSection;
    void* Event;
    unsigned long NextObjectUID;
    long BlockPendingObjectProcessingRefCount;
    float DeviceTimeSlice[THREADED_FILE_DEVICE_COUNT];
};
FABLE_STATIC_ASSERT(sizeof(CThreadedFileManager)==0x178);
#endif
#endif
