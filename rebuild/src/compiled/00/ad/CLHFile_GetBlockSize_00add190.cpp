// CLHFile::GetBlockSize() (descriptive; CLHFile / LHA::CFile have no PDB symbols) -- vtable 0x0129F094
// (RTTI .?AVCLHFile@@ : LHA::CFile) slot 2. Maps the open CThreadedFile's device to its block size:
// device 0 -> 0x200, device 1 -> 0x800, anything else (or no / closed file) -> 1. The slot-1 block read
// (0xADD240) multiplies this by its block index and block count to build the byte offset / length.
// CThreadedFile members from Ego_r.pdb (DeviceID +0x04, Length +0x10, OpenFlag +0x19; sizeof 0x1C).
// Leaf: ?GetBlockSize@CLHFile@@UAEJXZ
enum EThreadedFileDeviceID { };

class CThreadedFile {
public:
    int _padding_;                                  // +0x00
    EThreadedFileDeviceID DeviceID;                 // +0x04
    void* FileHandle;                               // +0x08
    void* Filename;                                 // +0x0c CWideString
    unsigned long Length;                           // +0x10
    unsigned long PhysicalSortKey;                  // +0x14
    bool OpenedForWrite;                            // +0x18
    bool OpenFlag;                                  // +0x19
};

namespace LHA {
class CFile {
public:
    virtual bool GetSize(unsigned long* size) = 0;  // slot 0
    virtual bool Read(void* dest, long block, long count, void* callback, void* context, bool high) = 0;  // slot 1
    virtual long GetBlockSize() = 0;                 // slot 2
};
}

class CLHFile : public LHA::CFile {
public:
    virtual bool GetSize(unsigned long* size);
    virtual bool Read(void* dest, long block, long count, void* callback, void* context, bool high);
    virtual long GetBlockSize();

    CThreadedFile* File;                            // +0x04 CCountedPointer<CThreadedFile> (object, then count)
    void* FileCount;                                // +0x08
};

long CLHFile::GetBlockSize()
{
    CThreadedFile* file = File;
    if (file && file->OpenFlag) {
        if (file->DeviceID == 0)
            return 0x200;
        if (file->DeviceID == 1)
            return 0x800;
    }
    return 1;
}
