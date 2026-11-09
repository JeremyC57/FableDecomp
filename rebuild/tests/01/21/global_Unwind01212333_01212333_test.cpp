// Behaviour test for PKCDefaultFileManager::OpenFile (0x00C1A510): a missing file (attributes -1) or a failed CreateFile
// (INVALID_HANDLE_VALUE) traces and fails; otherwise a { refcount 1, handle } record is inserted under the next key, the
// key is returned and the counter advances; the lock is held around the whole call.
#include <cstdio>
#include <cstring>

static int g_locked = 0, g_lockCalls = 0, g_traces = 0;
static char g_last[256];
static unsigned long g_attributes = 0; static void* g_handle = (void*)0x1234;
static unsigned long GetFileAttributesW(const unsigned short*) { return g_attributes; }
static void* CreateFileW(const unsigned short*, unsigned long access, unsigned long share, void*, unsigned long disp, unsigned long, void*)
{ return (access == 0x80000000UL && share == 3 && disp == 4) ? g_handle : (void*)-1; }
static unsigned long GetLastError() { return 2; }
struct CString {
    char buf[256];
    CString() { buf[0] = 0; }
    CString& operator<<(const char* t) { std::strcat(buf, t); return *this; }
    CString& operator<<(long v) { char n[32]; std::sprintf(n, "%ld", v); std::strcat(buf, n); return *this; }
    CString& operator<<(unsigned int v) { char n[32]; std::sprintf(n, "%u", v); std::strcat(buf, n); return *this; }
    CString& operator<<(const void*) { std::strcat(buf, "<name>"); return *this; }
};
static void Report(const char* m) { ++g_traces; std::strcpy(g_last, m); }
static void (*g_pfnErrorReport)(const char*) = Report;
struct Lock { Lock() { ++g_locked; ++g_lockCalls; } ~Lock() { --g_locked; } };
struct SOpenFile { long m_refCount; void* m_hFile; };
struct Map {
    unsigned long keys[8]; void* values[8]; int n;
    long Insert(const unsigned long* key, void* const* value)
    { for (int i = 0; i < n; ++i) if (keys[i] == *key) return 1; keys[n] = *key; values[n++] = *value; return 0; }
};
struct Manager {
    Map m_files; unsigned long m_nextKey;
    bool OpenFile(const unsigned short* fileName, unsigned long* key)
    {
        Lock lock;
        if (GetFileAttributesW(fileName) == (unsigned long)-1) {
            CString s; s << ".\\PKCDefaultFileManager.cpp" << "(" << 101L << ") : " << "GetFileAttributes for file '" << (const void*)fileName << "' failed: " << (unsigned int)GetLastError() << "\n";
            if (g_pfnErrorReport) g_pfnErrorReport(s.buf);
            return false;
        }
        void* handle = CreateFileW(fileName, 0x80000000UL, 3, 0, 4, 0, 0);
        if (handle == (void*)-1) {
            CString s; s << ".\\PKCDefaultFileManager.cpp" << "(" << 116L << ") : " << "CreateFile '" << (const void*)fileName << "' failed: " << (unsigned int)GetLastError() << "\n";
            if (g_pfnErrorReport) g_pfnErrorReport(s.buf);
            return false;
        }
        SOpenFile* file = new SOpenFile;
        file->m_refCount = 1;
        file->m_hFile = handle;
        *key = m_nextKey;
        ++m_nextKey;
        m_files.Insert(key, (void* const*)&file);
        return true;
    }
};

int main()
{
    Manager m; m.m_files.n = 0; m.m_nextKey = 7;
    unsigned short name[] = { 'a', 0 };
    unsigned long key = 0;
    bool ok = m.OpenFile(name, &key) && key == 7 && m.m_nextKey == 8 && m.m_files.n == 1 && m.m_files.keys[0] == 7;
    SOpenFile* f = (SOpenFile*)m.m_files.values[0];
    ok = ok && f->m_refCount == 1 && f->m_hFile == g_handle && g_locked == 0 && g_traces == 0;
    g_attributes = (unsigned long)-1;
    ok = ok && !m.OpenFile(name, &key) && g_traces == 1 && std::strcmp(g_last, ".\\PKCDefaultFileManager.cpp(101) : GetFileAttributes for file '<name>' failed: 2\n") == 0;
    g_attributes = 0; g_handle = (void*)-1;
    ok = ok && !m.OpenFile(name, &key) && g_traces == 2 && std::strcmp(g_last, ".\\PKCDefaultFileManager.cpp(116) : CreateFile '<name>' failed: 2\n") == 0;
    ok = ok && m.m_nextKey == 8 && g_locked == 0 && g_lockCalls == 3;
    delete f;
    if (!ok) { std::printf("00c1a510_TEST FAIL\n"); return 1; }
    std::printf("01212333_TEST PASS\n");
    return 0;
}