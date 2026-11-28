#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

class CWideString;

#ifdef FABLE_RETAIL_CTHREADEDFILE
#ifdef FABLE_RETAIL_CTHREADEDFILE_PROCESS_PASS27
#include "fable_string.h"
// Descriptive: THREADED_FILE_DEVICE_COUNT denotes the two device queues.
enum EThreadedFileDeviceID { THREADED_FILE_DEVICE_HARD_DISK, THREADED_FILE_DEVICE_DVD, THREADED_FILE_DEVICE_COUNT };
#endif
// Retail opt-in (chunk 775, row 0x009A8840): Ego_r.pdb CThreadedFile (0x1C bytes, not polymorphic: DeviceID +0x04 follows an
// opaque first word), FableWin.pdb ??0CThreadedFile@@QAE@XZ (0x0098DFD0) and ?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z
// (0x0098E1E0). No CCountedPointer<CThreadedFile> specialization here: engine/CCountedPointer.h owns the template.
class CThreadedFile
{
public:
    CThreadedFile();                                                    // 0x0098DFD0
    bool Open(const CWideString& filename, bool noCaching);             // 0x0098E1E0
#ifdef FABLE_RETAIL_CTHREADEDFILE_PROCESS_PASS27
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
    // FableWin lib_threaded_file.obj calls this accessor before indexing DeviceQueue.
    EThreadedFileDeviceID PeekDeviceID() const { return DeviceID; }
#endif
#ifdef FABLE_RETAIL_CTHREADEDFILE_CLOSE_STREAMS_PASS30
    unsigned long PeekPhysicalSortKey() const { return PhysicalSortKey; }
#endif
    bool IsOpenForWrite() const { return OpenedForWrite; }
private:
    friend class CThreadedFileManager;
    unsigned char Padding[4];
    EThreadedFileDeviceID DeviceID;
    void* FileHandle;
    CWideString Filename;
    unsigned long Length;
    unsigned long PhysicalSortKey;
    bool OpenedForWrite;
    bool OpenFlag;
#else
    unsigned char _data[0x1C];
#endif
};
FABLE_STATIC_ASSERT_AT(20, sizeof(CThreadedFile) == 0x1C);
#else
class CThreadedFile
{
public:
    CThreadedFile();
    bool Open(const CWideString& name, bool noCaching);

    fable_u32 vtable_;
    fable_u32 deviceId_;
    void* fileHandle_;
    void* filenameStorage_;
    fable_u32 length_;
    fable_u32 physicalSortKey_;
    bool openedForWrite_;
    bool openFlag_;
    fable_u8 padding1A_[2];
};

FABLE_STATIC_ASSERT_AT(39, sizeof(CThreadedFile) == 0x1C);
FABLE_STATIC_ASSERT_AT(40, offsetof(CThreadedFile, fileHandle_) == 0x08);
FABLE_STATIC_ASSERT_AT(41, offsetof(CThreadedFile, filenameStorage_) == 0x0C);
FABLE_STATIC_ASSERT_AT(42, offsetof(CThreadedFile, openedForWrite_) == 0x18);

template<typename T>
class CCountedPointer;

template<>
class CCountedPointer<CThreadedFile>
{
public:
    void Reset(CThreadedFile* data);

    CThreadedFile* data_;
    void* control_;
};

FABLE_STATIC_ASSERT_AT(57, sizeof(CCountedPointer<CThreadedFile>) == 0x08);
#endif // FABLE_RETAIL_CTHREADEDFILE
