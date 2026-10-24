// Behaviour test for 0x0041A540 NResourceDirectoryNames::GetSaveDir
#include <cstdio>
typedef unsigned short wchar16;
static long g_strings = 0;
struct CWideString {
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
static int g_docsCalls = 0;
static wchar16 g_lit1[32], g_lit2[16], g_lit3[4];
CWideString GetMyDocumentsDir() { ++g_docsCalls; wchar16 p[64]; Widen(p, "C:\\Docs\\"); return CWideString(p); }
CWideString GetUserDataDir() { return GetMyDocumentsDir() + g_lit1; }
CWideString GetSaveDirWithoutTrailingBlackslash() { return GetUserDataDir() + g_lit2; }
CWideString GetSaveDir() { return GetSaveDirWithoutTrailingBlackslash() + g_lit3; }
int main()
{
    int ok = 1;
    Widen(g_lit1, "My Games\\Fable\\"); Widen(g_lit2, "Saves"); Widen(g_lit3, "\\");
    {
        CWideString s = GetSaveDir();
        ok = SameText(s, "C:\\Docs\\My Games\\Fable\\Saves\\") && g_docsCalls == 1 && g_strings == 1;
    }
    ok = ok && g_strings == 0;
    if (!ok) { std::printf("0041a540_TEST FAIL\n"); return 1; }
    std::printf("0041a540_TEST PASS\n");
    return 0;
}