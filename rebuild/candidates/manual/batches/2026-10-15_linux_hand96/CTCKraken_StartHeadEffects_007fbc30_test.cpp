#include <cstdio>
#include <cstring>
static int g_live;
struct CCharString {
    char m_text[32];
    CCharString(const char* t, long) { std::strcpy(m_text, t); ++g_live; }
    ~CCharString() { --g_live; }
};
struct CTCKraken {
    char m_last[32]; int m_liveDuringCall;
    void CreateHeadEffect(const CCharString& name) { std::strcpy(m_last, name.m_text); m_liveDuringCall = g_live; }
    void StartHeadEffects() { CCharString effectName("MEETS_WATER", -1); CreateHeadEffect(effectName); }
};
int main()
{
    CTCKraken k; k.m_last[0] = 0; k.m_liveDuringCall = 0;
    k.StartHeadEffects();
    if (std::strcmp(k.m_last, "MEETS_WATER") != 0 || k.m_liveDuringCall != 1 || g_live != 0) { std::printf("007fbc30_TEST FAIL\n"); return 1; }
    std::printf("007fbc30_TEST PASS\n");
    return 0;
}
