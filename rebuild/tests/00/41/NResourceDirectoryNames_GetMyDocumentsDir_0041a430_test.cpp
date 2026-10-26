// Behaviour test for 0x0041A430 NResourceDirectoryNames::GetMyDocumentsDir
#include <cstdio>
typedef unsigned short wchar16;
static long g_strings = 0;
class CWideString { public:
    wchar16 m_text[128];
    long m_len;
    CWideString() : m_len(0) { m_text[0] = 0; ++g_strings; }
    explicit CWideString(const wchar16* t) : m_len(0) { while (t[m_len]) { m_text[m_len] = t[m_len]; ++m_len; } m_text[m_len] = 0; ++g_strings; }
    CWideString(const CWideString& o) : m_len(o.m_len) { for (long i = 0; i <= m_len; ++i) m_text[i] = o.m_text[i]; ++g_strings; }
    ~CWideString() { --g_strings; }
    long GetLength() const { return m_len; }
    operator const wchar16*() const { return m_text; }
    const CWideString& operator+=(const wchar16* t) { for (long i = 0; t[i]; ++i) m_text[m_len++] = t[i]; m_text[m_len] = 0; return *this; }
};
static CWideString operator+(const CWideString& a, const wchar16* b) { CWideString r(a); r += b; return r; }
static void Widen(wchar16* out, const char* in) { long i = 0; for (; in[i]; ++i) out[i] = (wchar16)in[i]; out[i] = 0; }
static bool SameText(const CWideString& s, const char* in) { long i = 0; for (; in[i]; ++i) if (s.m_text[i] != (wchar16)in[i]) return false; return s.m_len == i; }
static const char* g_folder = "C:\\Users\\Hero\\Documents";
static int g_folderId = -1;
long SHGetFolderPathW(void*, int folder, void*, unsigned long, wchar16* path) { g_folderId = folder; Widen(path, g_folder); return 0; }
CWideString GetMyDocumentsDir()
{
    wchar16 path[260];
    SHGetFolderPathW(0, 5, 0, 0, path);
    CWideString directory(path);
    long lastIndex = directory.GetLength() - 1;
    const wchar16* text = directory;
    if (text[lastIndex] != (wchar16)'\\') {
        wchar16 slash[2]; Widen(slash, "\\");
        directory += slash;
    }
    return directory;
}
int main()
{
    int ok = 1;
    {
        CWideString a = GetMyDocumentsDir();
        ok = ok && g_folderId == 5 && SameText(a, "C:\\Users\\Hero\\Documents\\");
        g_folder = "D:\\Docs\\";
        CWideString b = GetMyDocumentsDir();
        ok = ok && SameText(b, "D:\\Docs\\");
    }
    ok = ok && g_strings == 0;
    if (!ok) { std::printf("0041a430_TEST FAIL\n"); return 1; }
    std::printf("0041a430_TEST PASS\n");
    return 0;
}