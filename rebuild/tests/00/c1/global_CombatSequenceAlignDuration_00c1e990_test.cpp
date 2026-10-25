// Behaviour test for 00c1e990: CombatSequence_AlignDuration -- with t = phase(time) + extra, the wait from t up to the next multiple of the period
#include <cstdio>

struct CSequence {
    unsigned int m_period;
    virtual unsigned int v0() { return 0; }
    virtual unsigned int v1() { return 0; }
    virtual unsigned int GetPeriod() { return m_period; }
};

static unsigned int Phase(CSequence* sequence, unsigned int value) { return value % sequence->GetPeriod(); }

static unsigned int AlignDuration(CSequence* sequence, unsigned int time, unsigned int extra)
{
    unsigned int t = Phase(sequence, time) + extra;
    unsigned int period = sequence->GetPeriod();
    return (t + period - 1) / period * period - t;
}

int main()
{
    CSequence s; s.m_period = 100;
    if (AlignDuration(&s, 250, 0) != 50) { std::printf("00c1e990_TEST FAIL\n"); return 1; }    // phase 50 -> 50 to go
    if (AlignDuration(&s, 250, 50) != 0) { std::printf("00c1e990_TEST FAIL\n"); return 1; }    // exactly on a boundary
    if (AlignDuration(&s, 230, 90) != 80) { std::printf("00c1e990_TEST FAIL\n"); return 1; }   // 30 + 90 = 120 -> 200
    if (AlignDuration(&s, 300, 0) != 0) { std::printf("00c1e990_TEST FAIL\n"); return 1; }
    s.m_period = 7;
    if (AlignDuration(&s, 15, 3) != 3) { std::printf("00c1e990_TEST FAIL\n"); return 1; }     // 1 + 3 = 4 -> 7
    std::printf("00c1e990_TEST PASS\n");
    return 0;
}