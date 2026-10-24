// CThreadedFileManager::OnFileReadCompletion(unsigned long errorCode, unsigned long bytesTransferred, _OVERLAPPED* overlapped) -- ReadFileEx completion routine: on a read error flag a dirty disc (DVD device) or report through the hard-disk error callback; then mark the object finished, signal the manager's event and resume a paused file installer
// Leaf: ?OnFileReadCompletion@CThreadedFileManager@@CGXKKPAU_OVERLAPPED@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098E320, length 112 bytes (102 + int3 padding), `ret 0Ch` (__stdcall).
// PDB (FableWin/Ego_r): ?OnFileReadCompletion@CThreadedFileManager@@CGXKKPAU_OVERLAPPED@@@Z (private static); its address
// is passed to ReadFileEx at 0x0098EE9D / 0x0098EFBD (ProcessObjects / SetObjectPriority).
// Callees: SetEvent (IAT 0x0143FDE0), CTBaseSingleton<CFileInstaller>::Get @ 0x00404440 (__fastcall, landed as
// CFileInstallerSingleton::Get), CFileInstaller::Unpause @ 0x009C3250.
// Globals: the dirty-disc flag @ 0x013BC868 (CThreadedFileManager::SetDirtyDiscError(bool) @ 0x0098DED0 stores it), the
// hard-disk error callback @ 0x013BC864 (SetHardDiskErrorCallback @ 0x00401470, GetHardDiskErrorCallback @ 0x0098DEE0),
// the file installer singleton's counted pointer @ 0x013CA818 (object word tested inline), "" @ 0x0122D70E.
// Layout: the OVERLAPPED's hEvent (+0x10) carries the CFileObject; CFileObject LoadingStatus +0x1C, PManager +0x24,
// PFile +0x28; CThreadedFile DeviceID +0x04 (1 = the DVD device); CThreadedFileManager Event +0x164 (retail, as the
// landed 0x0098EB20).
// Shape: the inline setters/getters; `mov eax,1` serves both the device compare and the stored `true`; the callback
// is loaded once for its null test and the call (`ecx` = the empty literal, __fastcall).
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);     // IAT 0x0143FDE0

struct _OVERLAPPED {
    unsigned long Internal;
    unsigned long InternalHigh;
    unsigned long Offset;
    unsigned long OffsetHigh;
    void* hEvent;                               // +0x10 (the CFileObject)
};

class CFileInstaller {
public:
    void Unpause();                             // 0x009C3250
};

template <class T> class CCountedPointer {
public:
    T*    Data;                                 // +0x00
    void* Info;                                 // +0x04
};

template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                 // 0x00404440
    static bool Exists() { return Instance.Data != 0; }
    static CCountedPointer<T> Instance;         // 0x013CA818
};

enum EThreadedFileDeviceID { THREADED_FILE_DEVICE_HARD_DISK, THREADED_FILE_DEVICE_DVD };
enum EFileLoadingStatus { FILE_LOADING_PENDING, FILE_LOADING_READING, FILE_LOADING_READ_ISSUED, FILE_LOADING_READ_COMPLETE };

class CThreadedFile {
public:
    void* __vftable;                            // +0x00
    EThreadedFileDeviceID DeviceID;             // +0x04
};

class CThreadedFileManager {
public:
    class CFileObject {
    public:
        unsigned char Desc[0x18];               // +0x00 CThreadedFileObjectDesc
        unsigned long UID;                      // +0x18
        EFileLoadingStatus LoadingStatus;       // +0x1c
        _OVERLAPPED* Overlapped;                // +0x20
        CThreadedFileManager* PManager;         // +0x24
        CThreadedFile* PFile;                   // +0x28
    };
    unsigned char _pad00[0x164];                // vtable, device queues, time slices, Objects, flags, critical section
    void* Event;                                // +0x164
    static void __fastcall SetDirtyDiscError(bool error) { DirtyDiscError = error; }
protected:
    static void (__fastcall* GetHardDiskErrorCallback())(const char*) { return HardDiskErrorCallback; }
private:
    static bool DirtyDiscError;                 // 0x013BC868
    static void (__fastcall* HardDiskErrorCallback)(const char*);   // 0x013BC864
    static void __stdcall OnFileReadCompletion(unsigned long errorCode, unsigned long bytesTransferred, _OVERLAPPED* overlapped);
};

void __stdcall CThreadedFileManager::OnFileReadCompletion(unsigned long errorCode, unsigned long bytesTransferred, _OVERLAPPED* overlapped)
{
    CFileObject* object = (CFileObject*)overlapped->hEvent;
    if (errorCode != 0) {
        if (object->PFile->DeviceID == THREADED_FILE_DEVICE_DVD)
            SetDirtyDiscError(true);
        else if (GetHardDiskErrorCallback())
            GetHardDiskErrorCallback()("");
    }
    object->LoadingStatus = FILE_LOADING_READ_COMPLETE;
    SetEvent(object->PManager->Event);
    if (CTBaseSingleton<CFileInstaller>::Exists())
        CTBaseSingleton<CFileInstaller>::Get().Unpause();
}