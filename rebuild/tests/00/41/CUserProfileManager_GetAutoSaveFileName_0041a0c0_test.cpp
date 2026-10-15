// Behaviour test for the static CWideString getter at 0x0041A0C0 (returns L"Data\\Misc\\").
#include <cstdio>
#include <stddef.h>

struct CWideString {
    wchar_t m_text[32];
    explicit CWideString(const wchar_t* text) { int i = 0; while (text[i] && i < 31) { m_text[i] = text[i]; ++i; } m_text[i] = 0; }
};

struct CUserProfileManager {
    static CWideString GetAutoSaveFileName();
};

CWideString CUserProfileManager::GetAutoSaveFileName()
{
    return CWideString(L"Data\\Misc\\");
}

int main()
{
    CWideString s = CUserProfileManager::GetAutoSaveFileName();
    const wchar_t* want = L"Data\\Misc\\";
    for (int i = 0; ; ++i) {
        if (s.m_text[i] != want[i]) { std::printf("0041a0c0_TEST FAIL\n"); return 1; }
        if (want[i] == 0) break;
    }
    std::printf("0041a0c0_TEST PASS\n");
    return 0;
}