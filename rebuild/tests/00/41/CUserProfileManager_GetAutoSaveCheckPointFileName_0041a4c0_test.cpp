// Behaviour test for the static CWideString getter at 0x0041A4C0 (documents folder + L"My Games\\Fable\\").
#include <cstdio>
#include <stddef.h>

struct CWideString {
    wchar_t m_text[64];
    CWideString() { m_text[0] = 0; }
    explicit CWideString(const wchar_t* text) { int i = 0; while (text[i] && i < 63) { m_text[i] = text[i]; ++i; } m_text[i] = 0; }
};

CWideString operator+(const CWideString& left, const wchar_t* right)
{
    CWideString r(left.m_text);
    int i = 0; while (r.m_text[i]) ++i;
    int j = 0; while (right[j] && i < 63) r.m_text[i++] = right[j++];
    r.m_text[i] = 0;
    return r;
}

struct CUserProfileManager {
    static CWideString GetDocumentsFolder() { return CWideString(L"C:\\Docs\\"); }
    static CWideString GetAutoSaveCheckPointFileName();
};

CWideString CUserProfileManager::GetAutoSaveCheckPointFileName()
{
    return GetDocumentsFolder() + L"My Games\\Fable\\";
}

int main()
{
    CWideString s = CUserProfileManager::GetAutoSaveCheckPointFileName();
    const wchar_t* want = L"C:\\Docs\\My Games\\Fable\\";
    for (int i = 0; ; ++i) {
        if (s.m_text[i] != want[i]) { std::printf("0041a4c0_TEST FAIL\n"); return 1; }
        if (want[i] == 0) break;
    }
    std::printf("0041a4c0_TEST PASS\n");
    return 0;
}