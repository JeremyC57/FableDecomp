// Behaviour test for CDefStringTable::LoadTable (0x009D4C30): only an existing file is opened (read access, flags 4)
// and read; afterwards the table is locked and ReadOnly follows the argument; the file is closed on scope exit.
#include <cstdio>
#include <cstring>
class CWideString { public: const char* path; };
static bool g_exists = false;
static int g_opens = 0, g_closes = 0, g_reads = 0, g_lastAccess = 0; static unsigned long g_lastFlags = 0, g_bufferSize = 0;
static bool PathExists(const CWideString& p) { return g_exists && p.path != 0; }
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 { public:
    bool Opened;
    CDiskFileWin32() { Opened = false; }
    ~CDiskFileWin32() { Close(); }
    void Open(const CWideString&, FileAccessType access, unsigned long flags) { ++g_opens; g_lastAccess = access; g_lastFlags = flags; Opened = true; }
    void Close() { if (Opened) ++g_closes; Opened = false; }
};
class CFileDataInputStream { public:
    CDiskFileWin32* file;
    CFileDataInputStream(CDiskFileWin32* f, unsigned long size) : file(f) { g_bufferSize = size; }
    ~CFileDataInputStream() {}
};
class CDefStringTable {
public:
    CWideString TableFilename; bool Locked; bool ReadOnly;
    void ReadTable(CFileDataInputStream* s) { if (s->file->Opened) ++g_reads; }
    void LoadTable(bool readOnly);
};
void CDefStringTable::LoadTable(bool readOnly)
{
    if (PathExists(TableFilename)) {
        CDiskFileWin32 file;
        file.Open(TableFilename, FILE_ACCESS_READ, 4);
        CFileDataInputStream stream(&file, 0x4000);
        ReadTable(&stream);
        Locked = true;
        ReadOnly = readOnly;
    }
}
int main()
{
    CDefStringTable t; t.TableFilename.path = "defs.str"; t.Locked = false; t.ReadOnly = false;
    int bad = 0;
    t.LoadTable(true);
    if (g_opens || g_reads || t.Locked) bad = 1;
    g_exists = true;
    t.LoadTable(true);
    if (g_opens != 1 || g_reads != 1 || g_closes != 1 || !t.Locked || !t.ReadOnly) bad = 2;
    if (g_lastAccess != FILE_ACCESS_READ || g_lastFlags != 4 || g_bufferSize != 0x4000) bad = 3;
    t.LoadTable(false);
    if (t.ReadOnly || g_reads != 2) bad = 4;
    if (bad) { std::printf("009d4c30_TEST FAIL\n"); return 1; }
    std::printf("009d4c30_TEST PASS\n");
    return 0;
}