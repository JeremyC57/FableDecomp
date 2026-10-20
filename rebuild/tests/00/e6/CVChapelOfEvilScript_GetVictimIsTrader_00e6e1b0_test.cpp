// Behaviour test for 00e6e1b0: the model below is a copy of src/00e6e1b0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_terminating = 0, g_termCalls = 0, g_ctors = 0, g_dtors = 0;
using std::strstr;
struct CCharStringData { char* m_pData; long m_length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1) { ++g_ctors; if (text) { PStringData = new CCharStringData; PStringData->m_pData = (char*)text; PStringData->m_length = (long)std::strlen(text); } else PStringData = 0; (void)length; }
    CCharString(const CCharString& o) { ++g_ctors; PStringData = o.PStringData ? new CCharStringData(*o.PStringData) : 0; }
    ~CCharString() { ++g_dtors; delete PStringData; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
};
struct CScriptBase {
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating() const;
};
bool CScriptBase::IsActiveThreadTerminating() const { ++g_termCalls; return g_terminating != 0; }
struct CV_ChapelOfEvilScript : CScriptBase {
    bool GetVictimIsTrader(CCharString victim_def);
};
bool CV_ChapelOfEvilScript::GetVictimIsTrader(CCharString victim_def)
{
    bool found;
    {
        CCharString pattern("CREATURE_BANDIT");
        found = strstr(victim_def.GetPString(), pattern.GetPString()) != 0;
    }
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
    bool r = s.GetVictimIsTrader(CCharString(text));
    if (r != expect || g_termCalls != expectTermCalls) return false;
    if (g_ctors != g_dtors) return false;
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run("X_CREATURE_BANDIT_Y", 0, true, 1);    // contains, not terminating
    ok = ok && Run("CREATURE_BANDIT", 1, false, 1);       // contains but terminating
    ok = ok && Run("CREATURE_TRADER", 0, false, 0);       // not found: no terminating test
    ok = ok && Run(0, 0, false, 0);                       // null block: empty text
    if (!ok) { std::printf("00e6e1b0_TEST FAIL\n"); return 1; }
    std::printf("00e6e1b0_TEST PASS\n");
    return 0;
}