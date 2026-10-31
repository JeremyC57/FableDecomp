// Behaviour test for PKCDefaultFileManager::CloseFile (0x00C1A370): an unknown key traces line 65 and changes nothing; a
// known file loses one reference and is removed, closed and deleted on the last one; a zero count traces line 68 and
// breaks before the decrement.
#include <cstdio>
#include <cstring>
#include <map>

struct OpenFile { int handle; unsigned int refCount; };   // 32-bit count, as retail
static int g_traces = 0, g_lastLine = 0, g_breaks = 0, g_closed = 0, g_deleted = 0;
static std::map<unsigned long, OpenFile*> g_files;
static void trace(int line) { ++g_traces; g_lastLine = line; }

static void CloseFile(unsigned long key)
{
    std::map<unsigned long, OpenFile*>::iterator it = g_files.find(key);
    if (it == g_files.end()) { trace(65); return; }
    OpenFile* file = it->second;
    if (file->refCount <= 0) { trace(68); ++g_breaks; }
    if (--file->refCount == 0) {
        g_files.erase(key);
        g_closed = file->handle;
        delete file;
        ++g_deleted;
    }
}

int main()
{
    OpenFile* f = new OpenFile; f->handle = 77; f->refCount = 2;
    g_files[5] = f;
    CloseFile(9);
    bool ok = g_traces == 1 && g_lastLine == 65 && g_files.size() == 1;
    CloseFile(5);
    ok = ok && f->refCount == 1 && g_closed == 0 && g_files.size() == 1;
    CloseFile(5);
    ok = ok && g_closed == 77 && g_deleted == 1 && g_files.empty() && g_traces == 1;
    OpenFile* z = new OpenFile; z->handle = 3; z->refCount = 0;
    g_files[6] = z;
    CloseFile(6);   // out of sync: traced and broken, the count wraps and the record stays
    ok = ok && g_traces == 2 && g_lastLine == 68 && g_breaks == 1 && g_files.size() == 1 && z->refCount == 0xFFFFFFFFu;
    delete z;
    std::printf(ok ? "00c1a370_TEST PASS\n" : "00c1a370_TEST FAIL\n");
    return 0;
}