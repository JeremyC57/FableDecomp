#include <cstdio>
// Behaviour test for CThreadedFile::Close (0x0098EC40): an open handle is purged, closed, reset to
// INVALID_HANDLE_VALUE and both flags cleared; a second Close does nothing.
static int g_purged = 0; static int g_closed = 0; static void* g_lastClosed = 0;
struct CThreadedFile;
struct CThreadedFileManager { void RemovePendingRequests(CThreadedFile*) { ++g_purged; } };
static CThreadedFileManager g_ThreadedFileManager;
static int CloseHandle(void* h) { ++g_closed; g_lastClosed = h; return 1; }
struct CThreadedFile {
    void* m_vtable; long DeviceID; void* FileHandle; void* Filename; unsigned long Length, PhysicalSortKey;
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
};
int main()
{
    CThreadedFile f;
    f.FileHandle = (void*)0x1234; f.OpenFlag = true; f.OpenedForWrite = true;
    f.Close();
    if (g_purged != 1 || g_closed != 1 || g_lastClosed != (void*)0x1234 || f.FileHandle != (void*)-1 || f.OpenFlag || f.OpenedForWrite) { std::printf("0098ec40_TEST FAIL\n"); return 1; }
    f.OpenFlag = true;
    f.Close();
    if (g_purged != 1 || g_closed != 1 || !f.OpenFlag) { std::printf("0098ec40_TEST FAIL\n"); return 1; }
    std::printf("0098ec40_TEST PASS\n");
    return 0;
}