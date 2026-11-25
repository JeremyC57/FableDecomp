// Behaviour test for 0x008B3980 CCombatSequence_BanditIdleMiddle::GetName: "COMBAT_SEQUENCE_BANDIT_IDLE_MIDDLE" (retail 0x01278824), length -1.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CCharString
{
public:
    CCharString(const char* str, long len);
private:
    char* m_pData;
    unsigned long m_Length;
    unsigned long m_Capacity;
};
class CCombatSequence_BanditIdleMiddle
{
public:
    virtual CCharString GetName() const;
};
static const char* g_lastStr = 0; static long g_lastLen = 999; static int g_calls = 0; static const void* g_this = 0;
CCharString::CCharString(const char* str, long len) { g_lastStr = str; g_lastLen = len; ++g_calls; g_this = this; m_pData = 0; m_Length = 0; m_Capacity = 0; }
int main()
{
    CCombatSequence_BanditIdleMiddle obj;
    CCharString r = obj.GetName();
    bool ok = g_calls == 1 && g_lastLen == -1 && g_this == &r && g_lastStr != 0 && std::strcmp(g_lastStr, "COMBAT_SEQUENCE_BANDIT_IDLE_MIDDLE") == 0;
    if (ok) std::printf("BANDIT_GETNAME_OK\n");
    else std::printf("FAIL str=%s len=%ld\n", g_lastStr ? g_lastStr : "(null)", g_lastLen);
    return ok ? 0 : 1;
}
