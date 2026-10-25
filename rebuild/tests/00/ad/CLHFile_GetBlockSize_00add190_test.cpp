#include <cstdio>
#include <cstring>
enum EThreadedFileDeviceID { };
class CThreadedFile {
public:
    int _padding_;
    EThreadedFileDeviceID DeviceID;
    void* FileHandle;
    void* Filename;
    unsigned long Length;
    unsigned long PhysicalSortKey;
    bool OpenedForWrite;
    bool OpenFlag;
};
namespace LHA {
class CFile {
public:
    virtual bool GetSize(unsigned long* size) = 0;
    virtual bool Read(void* dest, long block, long count, void* callback, void* context, bool high) = 0;
    virtual long GetBlockSize() = 0;
};
}
class CLHFile : public LHA::CFile {
public:
    virtual bool GetSize(unsigned long* size);
    virtual bool Read(void* dest, long block, long count, void* callback, void* context, bool high);
    virtual long GetBlockSize();
    CThreadedFile* File;
    void* FileCount;
};
bool CLHFile::GetSize(unsigned long*) { return false; }
bool CLHFile::Read(void*, long, long, void*, void*, bool) { return false; }

static int fail(int n) { std::printf("00add190_TEST FAIL %d\n", n); return 1; }
int main() {
    if (sizeof(CThreadedFile) != 0x1c) return fail(0);
    CThreadedFile t; std::memset(&t, 0, sizeof t);
    CLHFile f; f.File = &t; f.FileCount = 0;
    CLHFile* pf = &f;
    t.OpenFlag = true;
    t.DeviceID = (EThreadedFileDeviceID)0; if (pf->CLHFile::GetBlockSize() != 0x200) return fail(1);
    t.DeviceID = (EThreadedFileDeviceID)1; if (pf->CLHFile::GetBlockSize() != 0x800) return fail(2);
    t.DeviceID = (EThreadedFileDeviceID)2; if (pf->CLHFile::GetBlockSize() != 1) return fail(3);
    t.DeviceID = (EThreadedFileDeviceID)0; t.OpenFlag = false; if (pf->CLHFile::GetBlockSize() != 1) return fail(4);
    f.File = 0; if (pf->CLHFile::GetBlockSize() != 1) return fail(5);
    std::printf("00add190_TEST PASS\n");
    return 0;
}
