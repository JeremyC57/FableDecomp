// Behaviour test (0x00c1e9c0): GetSequencePhase returns 0 without an owner sequence, else the sequence query of the client's time
#include <cstdio>
class CCombatSequenceBase { public: unsigned long Base; };
static int g_queries = 0;
static unsigned long CCombatSequenceBase_IsBoastSequence(CCombatSequenceBase* sequence, unsigned long value) { ++g_queries; return sequence->Base + value; }
struct CSequenceOwner { CCombatSequenceBase* m_pSequence; };
struct CSequenceClient {
    CSequenceOwner* m_pOwner;
    long            m_time;
    int             m_timeCalls;
    virtual long GetTime() { ++m_timeCalls; return m_time; }
    virtual ~CSequenceClient() { }
    long GetSequencePhase()
    {
        if (m_pOwner->m_pSequence == 0)
            return 0;
        return CCombatSequenceBase_IsBoastSequence(m_pOwner->m_pSequence, GetTime());
    }
};
int main()
{
    CCombatSequenceBase seq; seq.Base = 100;
    CSequenceOwner owner; owner.m_pSequence = 0;
    CSequenceClient c; c.m_pOwner = &owner; c.m_time = 7; c.m_timeCalls = 0;
    if (c.GetSequencePhase() != 0 || g_queries != 0 || c.m_timeCalls != 0) { std::printf("00c1e9c0_TEST FAIL\n"); return 1; }
    owner.m_pSequence = &seq;
    if (c.GetSequencePhase() != 107 || g_queries != 1 || c.m_timeCalls != 1) { std::printf("00c1e9c0_TEST FAIL\n"); return 1; }
    std::printf("00c1e9c0_TEST PASS\n");
    return 0;
}