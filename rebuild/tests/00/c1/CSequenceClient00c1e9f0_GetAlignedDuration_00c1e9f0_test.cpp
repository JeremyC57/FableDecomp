// Behaviour test for 0x00C1E9F0 CSequenceClient00c1e9f0::GetAlignedDuration
#include <cstdio>
class CCombatSequenceBase { public: long period; };
static long CombatSequence_AlignDuration(CCombatSequenceBase* sequence, long time, long extra)
{
    long t = time + extra;
    long p = sequence->period;
    return (t + p - 1) / p * p - t;
}
struct CSequenceOwner00c1e9f0 { CCombatSequenceBase* m_pSequence; };
struct CSequenceClient00c1e9f0 {
    long m_time;
    CSequenceOwner00c1e9f0* m_pOwner;
    long m_extra;
    virtual long GetTime() { return m_time; }
    long GetAlignedDuration()
    {
        if (m_pOwner->m_pSequence == 0)
            return 0;
        return CombatSequence_AlignDuration(m_pOwner->m_pSequence, GetTime(), m_extra);
    }
};
int main()
{
    CCombatSequenceBase seq; seq.period = 10;
    CSequenceOwner00c1e9f0 owner; owner.m_pSequence = 0;
    CSequenceClient00c1e9f0 c; c.m_time = 13; c.m_pOwner = &owner; c.m_extra = 4;
    bool ok = c.GetAlignedDuration() == 0;
    owner.m_pSequence = &seq;
    if (c.GetAlignedDuration() != 3) ok = false;    // 17 -> 20
    if (!ok) { std::printf("00c1e9f0_TEST FAIL\n"); return 1; }
    std::printf("00c1e9f0_TEST PASS\n");
    return 0;
}