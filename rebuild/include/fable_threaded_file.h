#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

class CWideString;

#ifdef FABLE_RETAIL_CTHREADEDFILE
// Retail opt-in (chunk 775, row 0x009A8840): Ego_r.pdb CThreadedFile (0x1C bytes, not polymorphic: DeviceID +0x04 follows an
// opaque first word), FableWin.pdb ??0CThreadedFile@@QAE@XZ (0x0098DFD0) and ?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z
// (0x0098E1E0). No CCountedPointer<CThreadedFile> specialization here: engine/CCountedPointer.h owns the template.
class CThreadedFile
{
public:
    CThreadedFile();                                                    // 0x0098DFD0
    bool Open(const CWideString& filename, bool noCaching);             // 0x0098E1E0
    unsigned char _data[0x1C];
};
FABLE_STATIC_ASSERT(sizeof(CThreadedFile) == 0x1C);
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

FABLE_STATIC_ASSERT(sizeof(CThreadedFile) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, fileHandle_) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, filenameStorage_) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, openedForWrite_) == 0x18);

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

FABLE_STATIC_ASSERT(sizeof(CCountedPointer<CThreadedFile>) == 0x08);
#endif // FABLE_RETAIL_CTHREADEDFILE
