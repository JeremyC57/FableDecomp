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
#ifdef FABLE_RETAIL_CTHREADEDFILEMANAGER_PROCESSOBJECTS_PRIVATE
    // Add-only opt-in (NF293, 2026-11-23; row 0x0098ECD0): Ego_r.pdb + FableWin.pdb ?ProcessObjects@CThreadedFileManager@@AAEXXZ (private);
    // the landed binder 0x0098F2A0 binds AAE. Declaration only.
private:
#endif
    void ProcessObjects();
#ifdef FABLE_RETAIL_CTHREADEDFILEMANAGER_PROCESSOBJECTS_PRIVATE
public:
#endif
    static void __fastcall DoHardDiskErrorCallback(const char* message) {
        if (HardDiskErrorCallback) HardDiskErrorCallback(message);
    }
#ifdef FABLE_RETAIL_CTHREADEDFILEMANAGER_SETCALLBACKS
    // b24c4 opt-in (row 0x00402510 GFMain): FableWin.pdb ?SetErrorCallback@CThreadedFileManager@@SIXP6IXXZ@Z /
    // ?SetHardDiskErrorCallback@CThreadedFileManager@@SIXP6IXPBD@Z@Z, inline in retail (GFMain stores 0x013BC860 / 0x013BC864 in
    // place, 0x0040256B / 0x0040257E). m_errorCallback 0x013BC860 is the Ego_d.pdb name (?m_errorCallback@CThreadedFileManager@@0P6IXXZA).
    static void __fastcall SetErrorCallback(void (__fastcall* callback)()) { m_errorCallback = callback; }
    static void __fastcall SetHardDiskErrorCallback(void (__fastcall* callback)(const char*)) { HardDiskErrorCallback = callback; }
private:
    static void (__fastcall* m_errorCallback)();                     // 0x013BC860
public:
#endif
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
    FABLE_STATIC_ASSERT_AT(38, sizeof(CFileObject) == 0x40);
    FABLE_STATIC_ASSERT_AT(39, sizeof(CPendingObjectMap) == 0x10);
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
FABLE_STATIC_ASSERT_AT(59, sizeof(CThreadedFileManager)==0x178);
#endif
#endif
