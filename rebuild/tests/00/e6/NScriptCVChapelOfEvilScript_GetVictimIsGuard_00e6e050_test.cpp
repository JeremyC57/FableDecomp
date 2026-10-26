// Behaviour test for 00e6e050: the model below is a copy of src/00e6e050.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_terminating = 0, g_termCalls = 0, g_ctors = 0, g_dtors = 0;
struct CCharStringData { const char* m_pText; unsigned long m_length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length) { ++g_ctors; if (text) { PStringData = new CCharStringData; PStringData->m_pText = text; PStringData->m_length = (unsigned long)std::strlen(text); } else PStringData = 0; (void)length; }
    CCharString(const CCharString& o) { ++g_ctors; PStringData = o.PStringData ? new CCharStringData(*o.PStringData) : 0; }
    ~CCharString() { ++g_dtors; delete PStringData; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pText; }
};
using std::strstr;
struct CScriptBase {
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();
};
bool CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_terminating != 0; }

__forceinline bool NameContains(const CCharString& name, const char* text)
{
    CCharString pattern(text, -1);
    return strstr(name.GetPString(), pattern.GetPString()) ? true : false;
}

struct CV_ChapelOfEvilScript : CScriptBase {
    bool GetVictimIsGuard(CCharString name);
};

bool CV_ChapelOfEvilScript::GetVictimIsGuard(CCharString name)
{
    bool found = NameContains(name, "BS_SHERIFF") || NameContains(name, "BS_GUARD");
    if (found) {
        if (!IsActiveThreadTerminating())
            return true;
        return false;
    }
    return false;
}

static bool Run(const char* text, int terminating, bool expect, int expectTermCalls)
{
    CV_ChapelOfEvilScript s; std::memset(&s, 0, sizeof(s));
    g_terminating = terminating; g_termCalls = g_ctors = g_dtors = 0;
    bool r = s.GetVictimIsGuard(CCharString(text, -1));
    if (r != expect || g_termCalls != expectTermCalls) return false;
    if (g_ctors != g_dtors) return false;                 // every temporary (patterns, the by-value name) destroyed once
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run("BS_SHERIFF_01", 0, true, 1);          // first pattern hits, not terminating
    ok = ok && Run("CREATURE_BS_GUARD_X", 0, true, 1);    // second pattern hits
    ok = ok && Run("BS_GUARD", 1, false, 1);              // found but terminating
    ok = ok && Run("CREATURE_BANDIT", 0, false, 0);       // not found: the terminating test is never made
    ok = ok && Run(0, 0, false, 0);                       // null block: empty text, not found
    if (!ok) { std::printf("00e6e050_TEST FAIL\n"); return 1; }
    std::printf("00e6e050_TEST PASS\n");
    return 0;
}