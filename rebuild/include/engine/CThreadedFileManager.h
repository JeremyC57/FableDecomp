#ifndef FABLE_ENGINE_CTHREADEDFILEMANAGER_H
#define FABLE_ENGINE_CTHREADEDFILEMANAGER_H
#ifdef FABLE_RETAIL_CTHREADEDFILE_PROCESS_PASS27
#include "fable_threaded_file.h"
#include "engine/CThreadedFileObjectDesc.h"
#include "engine/Win32Types.h"
#include "stlport/fable_map.h"
struct _OVERLAPPED;
enum EFileLoadingStatus { FILE_LOADING_PENDING, FILE_LOADING_READING, FILE_LOADING_READ_ISSUED, FILE_LOADING_READ_COMPLETE };
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
#include "engine/CBaseClassNonCopyable.h"
// FableWin and Ego_r TPI both give CBaseClassNonCopyable as the public base at offset zero.
class CThreadedFileManager : public CBaseClassNonCopyable {
public:
    CThreadedFileManager();
    virtual ~CThreadedFileManager();
#else
class CThreadedFileManager {
public:
#endif
#ifdef FABLE_RETAIL_CTHREADEDFILEMANAGER_PROCESSOBJECTS_PRIVATE
    // Add-only opt-in (NF293, 2026-11-23; row 0x0098ECD0): Ego_r.pdb + FableWin.pdb ?ProcessObjects@CThreadedFileManager@@AAEXXZ (private);
    // the landed binder 0x0098F2A0 binds AAE. Declaration only.
private:
#endif
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
public:
    void CloseAllStreamsForFile(CThreadedFile* pfile);
#ifdef FABLE_RETAIL_CTHREADEDFILE_REMOVE_OBJECT_PASS30
    // Ego_r 00AE97C0, FableWin 0300EC10, retail 0098EB20.
    bool RemoveObject(unsigned long uid);
#endif
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
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
    // TPI in Ego_d, Ego_r and FableWin agrees on the scope and all five enumerators.
    enum EFileLoadingStatus {
        FILE_LOADING_STATUS_PENDING, FILE_LOADING_STATUS_PENDING_LOCKED, FILE_LOADING_STATUS_READING,
        FILE_LOADING_STATUS_READ_COMPLETED, FILE_LOADING_STATUS_FINISHED
    };
#endif
    class CFileObject {
    public:
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
        // FableWin lib_threaded_file.obj and the retail conditional on Overlapped.
        ~CFileObject() { if (Overlapped) GlobalFree(Overlapped); }
#endif
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
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
        // FableWin lib_threaded_file.obj compares physical position, file offset, then pointer identity.
        bool operator<(const CPendingObjectSortKey& rhs) const {
            if (Object->PFile->PeekPhysicalSortKey() < rhs.Object->PFile->PeekPhysicalSortKey()) return true;
            if (Object->PFile->PeekPhysicalSortKey() > rhs.Object->PFile->PeekPhysicalSortKey()) return false;
            if (Object->Desc.FilePos < rhs.Object->Desc.FilePos) return true;
            if (Object->Desc.FilePos > rhs.Object->Desc.FilePos) return false;
            return Object < rhs.Object;
        }
#else
        bool operator<(const CPendingObjectSortKey&) const;
#endif
    };
    class CPendingObjectMap {
    public:
        std::map<CPendingObjectSortKey, CFileObject*> Objects;
        std::map<CPendingObjectSortKey, CFileObject*>::iterator CurrentPosition;
    };
    FABLE_STATIC_ASSERT_AT(38, sizeof(CFileObject) == 0x40);
    FABLE_STATIC_ASSERT_AT(39, sizeof(CPendingObjectMap) == 0x10);
    void OnFinishObject(CFileObject*);
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
    // The editor body reads the device through PeekDeviceID and compares the stored iterator.
#if defined(FABLE_RETAIL_CTHREADEDFILE_REMOVE_OBJECT_PASS30) && defined(_MSC_VER)
    __declspec(noinline)
#endif
    void RemoveObjectFromPendingMap(CFileObject* object) {
        EThreadedFileDeviceID device = object->PFile->PeekDeviceID();
        CPendingObjectSortKey sort_key;
        sort_key.Object = object;
        std::map<CPendingObjectSortKey, CFileObject*>::iterator itr =
            DeviceQueue[device][object->Desc.Priority].Objects.find(sort_key);
        if (DeviceQueue[device][object->Desc.Priority].CurrentPosition == itr)
            ++DeviceQueue[device][object->Desc.Priority].CurrentPosition;
        DeviceQueue[device][object->Desc.Priority].Objects.erase(sort_key);
    }
#else
    void RemoveObjectFromPendingMap(CFileObject*);
#endif
    static void __stdcall OnFileReadCompletion(unsigned long, unsigned long, _OVERLAPPED*);
    static void (__fastcall* HardDiskErrorCallback)(const char*);
#ifndef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
    unsigned char Padding[4];
#endif
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
