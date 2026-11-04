// CThreadedFile::~CThreadedFile() -- threaded file dtor: Close() (drop the manager's streams for this file, close the handle, clear the open flags), then ~Filename and ~CBaseClassNonCopyable
// Leaf: ??1CThreadedFile@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098F090.
// Identity: Ego_r.pdb ??1CThreadedFile@@QAE@XZ; Close() inlined; the static manager 0x013BC870 is CThreadedFile::Manager
//   (Ego_r "dynamic initializer for 'CThreadedFile::Manager'").
// Callees: CThreadedFileManager::CloseAllStreamsForFile 0x0098E7D0, CloseHandle (IAT), ~CWideString, ~CBaseClassNonCopyable.
// Shape: Close() defined in the TU (FileHandle test, stream drop, CloseHandle, flags cleared).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"

extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);    // IAT 0x0143FE30
#define INVALID_HANDLE_VALUE ((void*)-1)

class CThreadedFile;
enum EThreadedFileDeviceID { };

class CThreadedFileManager : public CBaseClassNonCopyable {                  // vtable 0x0129A164
public:
    CThreadedFileManager();
    virtual ~CThreadedFileManager();
    void CloseAllStreamsForFile(CThreadedFile* file);                        // 0x0098E7D0
};

class CThreadedFile : public CBaseClassNonCopyable {                         // vtable 0x0129A158
public:
    CThreadedFile();
    virtual ~CThreadedFile();
    void Close();                                                            // 0x0098EC40
private:
    static CThreadedFileManager Manager;                                     // 0x013BC870
    EThreadedFileDeviceID DeviceID;                                          // +0x04
    void*                 FileHandle;                                        // +0x08
    CWideString           Filename;                                          // +0x0c
    unsigned long         Length;                                            // +0x10
    unsigned long         PhysicalSortKey;                                   // +0x14
    bool                  OpenedForWrite;                                    // +0x18
    bool                  OpenFlag;                                          // +0x19
};
FABLE_STATIC_ASSERT(sizeof(CThreadedFile) == 0x1c);

void CThreadedFile::Close()
{
    if (FileHandle != INVALID_HANDLE_VALUE) {
        Manager.CloseAllStreamsForFile(this);
        CloseHandle(FileHandle);
        FileHandle = INVALID_HANDLE_VALUE;
        OpenFlag = false;
        OpenedForWrite = false;
    }
}

CThreadedFile::~CThreadedFile()
{
    Close();
}