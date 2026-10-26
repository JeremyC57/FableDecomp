#include <cstdio>
// Behaviour test for CThreadedFile scalar deleting destructor (0x00991450): an open file is purged from the
// manager, closed and reset before the name and the base die; an already-closed file is left alone.
static int g_purged = 0; static int g_closed = 0; static int g_order[8]; static int g_n = 0;
class CThreadedFile;
class CThreadedFileManager { public: void RemovePendingRequests(CThreadedFile*) { ++g_purged; } };
static CThreadedFileManager g_ThreadedFileManager;
static int CloseHandle(void*) { ++g_closed; return 1; }
class CWideString { public: void* m_pData; ~CWideString() { g_order[g_n++] = 2; } };
namespace N_0099a300 { struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } }; } using N_0099a300::CBase;
class CThreadedFile : public CBase { public:
    long DeviceID; void* FileHandle; CWideString Filename; unsigned long Length, PhysicalSortKey;
    bool OpenedForWrite, OpenFlag;
    void Close()
    {
        if (FileHandle != (void*)-1) {
            g_ThreadedFileManager.RemovePendingRequests(this);
            CloseHandle(FileHandle);
            FileHandle = (void*)-1;
            OpenFlag = false;
            OpenedForWrite = false;
        }
    }
    CThreadedFile() : FileHandle((void*)-1) { }
    virtual ~CThreadedFile() { Close(); }
};
int main()
{
    CThreadedFile* f = new CThreadedFile;
    f->FileHandle = (void*)0x40; f->OpenFlag = true; f->OpenedForWrite = true;
    CBase* p = f;
    delete p;
    if (g_purged != 1 || g_closed != 1 || g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("00991450_TEST FAIL\n"); return 1; }
    g_n = 0;
    p = new CThreadedFile;     // never opened
    delete p;
    if (g_purged != 1 || g_closed != 1 || g_n != 2) { std::printf("00991450_TEST FAIL\n"); return 1; }
    std::printf("00991450_TEST PASS\n");
    return 0;
}