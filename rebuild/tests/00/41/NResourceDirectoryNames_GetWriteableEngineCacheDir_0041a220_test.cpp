#include <cstdio>
#include <stddef.h>
// Behaviour test for NResourceDirectoryNames::GetWriteableEngineCacheDir (retail 0x0041A220): the CSIDL_APPDATA
// folder from SHGetFolderPathW with "\Microsoft\Fable\" appended.
static int g_folderAsked = -1;
static long SHGetFolderPathW(void*, int folder, void*, unsigned long, wchar_t* path)
{
    const wchar_t* appData = L"C:\\Users\\Hero\\AppData";
    g_folderAsked = folder;
    int i = 0;
    for (; appData[i]; ++i) path[i] = appData[i];
    path[i] = 0;
    return 0;
}
class CWideString { public:
    wchar_t m_text[300];
    explicit CWideString(const wchar_t* text) { int i = 0; for (; text[i]; ++i) m_text[i] = text[i]; m_text[i] = 0; }
};
static CWideString operator+(const CWideString& left, const wchar_t* right)
{
    CWideString r(left.m_text);
    int n = 0; while (r.m_text[n]) ++n;
    int i = 0; for (; right[i]; ++i) r.m_text[n + i] = right[i];
    r.m_text[n + i] = 0;
    return r;
}
static CWideString GetWriteableEngineCacheDir()
{
    wchar_t path[260];
    SHGetFolderPathW(0, 0x1a, 0, 0, path);
    return CWideString(path) + L"\\Microsoft\\Fable\\";
}
static int Same(const wchar_t* a, const wchar_t* b)
{
    int i = 0;
    for (; a[i] && b[i]; ++i) if (a[i] != b[i]) return 0;
    return a[i] == b[i];
}
int main()
{
    CWideString dir = GetWriteableEngineCacheDir();
    int ok = g_folderAsked == 0x1a && Same(dir.m_text, L"C:\\Users\\Hero\\AppData\\Microsoft\\Fable\\");
    if (!ok) { std::printf("0041a220_TEST FAIL\n"); return 1; }
    std::printf("0041a220_TEST PASS\n");
    return 0;
}