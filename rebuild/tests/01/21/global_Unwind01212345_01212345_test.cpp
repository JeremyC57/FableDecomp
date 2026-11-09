// Behaviour test for PKCDefaultFileManager::ReleaseFile (0x00C1A870): an unknown key fails; a known one drops a reference
// and, at zero, is removed from the map, its handle closed and the record freed; a non-positive count traces line 145.
#include <cstdio>
#include <cstring>

static int g_locked = 0, g_traces = 0, g_breaks = 0, g_closed = 0;
static char g_last[256];
static int CloseHandle(void* h) { g_closed = (int)(long)h; return 1; }
struct CString {
    char buf[256];
    CString() { buf[0] = 0; }
    CString& operator<<(const char* t) { std::strcat(buf, t); return *this; }
    CString& operator<<(long v) { char n[32]; std::sprintf(n, "%ld", v); std::strcat(buf, n); return *this; }
};
static void Hook(const char* m) { ++g_traces; std::strcpy(g_last, m); }
static void (*g_pfnAssertHook)(const char*) = Hook;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct SOpenFile { long m_refCount; void* m_hFile; };
struct Map {
    unsigned long keys[8]; void* values[8]; int n;
    long Find(const unsigned long* key, void** value)
    { for (int i = 0; i < n; ++i) if (keys[i] == *key) { *value = values[i]; return 0; } return 3; }
    void Remove(const unsigned long* key)
    { for (int i = 0; i < n; ++i) if (keys[i] == *key) { keys[i] = keys[n - 1]; values[i] = values[--n]; return; } }
};
struct Manager {
    Map m_files;
    bool ReleaseFile(unsigned long key)
    {
        Lock lock;
        void* value;
        if (m_files.Find(&key, &value) != 0) return false;
        SOpenFile* file = (SOpenFile*)value;
        if (!(file->m_refCount > 0)) {
            {
                CString line;
                line << ".\\PKCDefaultFileManager.cpp" << "(" << (long)145 << ") : " << "Reference count for file out of sync" << "\n";
                if (g_pfnAssertHook) g_pfnAssertHook(line.buf);
            }
            CDebugBreak();
        }
        if (--file->m_refCount == 0) {
            m_files.Remove(&key);
            CloseHandle(file->m_hFile);
            delete file;
        }
        return true;
    }
};

int main()
{
    Manager m; m.m_files.n = 0;
    SOpenFile* f = new SOpenFile; f->m_refCount = 2; f->m_hFile = (void*)0x44;
    SOpenFile* g = new SOpenFile; g->m_refCount = 0; g->m_hFile = (void*)0x55;
    m.m_files.keys[0] = 3; m.m_files.values[0] = f; m.m_files.keys[1] = 4; m.m_files.values[1] = g; m.m_files.n = 2;
    bool ok = !m.ReleaseFile(9) && g_locked == 0;
    ok = ok && m.ReleaseFile(3) && f->m_refCount == 1 && m.m_files.n == 2 && g_closed == 0;
    ok = ok && m.ReleaseFile(3) && m.m_files.n == 1 && g_closed == 0x44 && g_traces == 0;
    ok = ok && m.ReleaseFile(4) && g_traces == 1 && g_breaks == 1 && g->m_refCount == -1 && m.m_files.n == 1
            && std::strcmp(g_last, ".\\PKCDefaultFileManager.cpp(145) : Reference count for file out of sync\n") == 0;
    delete g;
    if (!ok) { std::printf("00c1a870_TEST FAIL\n"); return 1; }
    std::printf("01212345_TEST PASS\n");
    return 0;
}