// Behaviour test for the empty-string getter at 0x007D7C10.
#include <cstdio>
#include <stddef.h>

struct CWideString {
    wchar_t m_text[8];
    explicit CWideString(const wchar_t* text) { int i = 0; while (text[i] && i < 7) { m_text[i] = text[i]; ++i; } m_text[i] = 0; }
};

struct CUserProfileManager {
    CWideString GetAutoSaveFileName();
};

CWideString CUserProfileManager::GetAutoSaveFileName()
{
    return CWideString(L"");
}

int main()
{
    CUserProfileManager m;
    CWideString s = m.GetAutoSaveFileName();
    if (s.m_text[0] != 0) { std::printf("007d7c10_TEST FAIL\n"); return 1; }
    std::printf("007d7c10_TEST PASS\n");
    return 0;
}