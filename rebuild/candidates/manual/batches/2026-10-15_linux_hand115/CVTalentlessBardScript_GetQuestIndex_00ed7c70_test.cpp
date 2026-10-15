#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_dtors = 0, g_termCalls = 0; static unsigned char g_terminating = 0;
struct CCharString {
    const char* m_pStringData;
    CCharString(const char* s) : m_pStringData(s) { }
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { }
    ~CCharString() { ++g_dtors; }
    bool operator!=(const CCharString& other) const { const char* a = m_pStringData; const char* b = other.m_pStringData; while (*a && *a == *b) { ++a; ++b; } return *a != *b; }
};
struct SQuestEntry { CCharString m_name; long m_state; SQuestEntry() : m_name(""), m_state(0) { } };
struct CV_TalentlessBardScript {
    virtual void v00() { } unsigned char _pad04[0x70]; SQuestEntry m_quests[12];
    unsigned char IsActiveThreadTerminating() { ++g_termCalls; return g_terminating; }
    long GetQuestIndex(CCharString name)
    {
        long i;
        for (i = 0; i < 12; ++i) { if (!(m_quests[i].m_name != name)) break; }
        if (i == 12) { if (IsActiveThreadTerminating()) return -1; i = -1; }
        return i;
    }
};
int main()
{
    CV_TalentlessBardScript s; const char* names[12] = { "alpha", "beta", "gamma", "delta", "eps", "zeta", "eta", "theta", "iota", "kappa", "lambda", "mu" };
    for (int k = 0; k < 12; ++k) s.m_quests[k].m_name = CCharString(names[k]);
    g_dtors = 0;
    if (s.GetQuestIndex(CCharString("alpha")) != 0 || s.GetQuestIndex(CCharString("mu")) != 11 || s.GetQuestIndex(CCharString("eta")) != 6) { std::printf("00ed7c70_TEST FAIL\n"); return 1; }
    if (s.GetQuestIndex(CCharString("nu")) != -1 || g_termCalls != 1) { std::printf("00ed7c70_TEST FAIL\n"); return 1; }
    g_terminating = 1;
    if (s.GetQuestIndex(CCharString("nu")) != -1 || g_termCalls != 2 || s.GetQuestIndex(CCharString("beta")) != 1 || g_termCalls != 2) { std::printf("00ed7c70_TEST FAIL\n"); return 1; }
    std::printf("00ed7c70_TEST PASS\n"); return 0;
}
