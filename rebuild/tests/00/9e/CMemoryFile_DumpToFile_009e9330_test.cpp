// Test: CMemoryFile::DumpToFile -- opens a disk file on the same pathname with the same access type (flags 2) and writes the whole buffer
#include <cstdio>
#include <cstring>
struct DiskLog { const char* path; int access; unsigned int flags; unsigned char data[16]; long length; int closed; };
static DiskLog g_log;
struct CDiskFile {
    CDiskFile(const char* path, int access, unsigned int flags) { g_log.path = path; g_log.access = access; g_log.flags = flags; g_log.length = 0; g_log.closed = 0; }
    void Write(const void* d, long n, void*) { std::memcpy(g_log.data + g_log.length, d, n); g_log.length += n; }
    ~CDiskFile() { g_log.closed = 1; }
};
class CMemoryFile { public:
    int AccessType; unsigned char buf[8]; long size; const char* Pathname;
    void DumpToFile()
    {
        CDiskFile file(Pathname, AccessType, 2);
        file.Write(buf, size, 0);
    }
};
int main()
{
    CMemoryFile m = { 3, { 1, 2, 3, 4, 5, 6, 7, 8 }, 5, "save.dat" };
    m.DumpToFile();
    if (std::strcmp(g_log.path, "save.dat") || g_log.access != 3 || g_log.flags != 2 || g_log.length != 5 || g_log.data[4] != 5 || !g_log.closed) { std::printf("009e9330_TEST FAIL\n"); return 1; }
    std::printf("009e9330_TEST PASS\n");
    return 0;
}