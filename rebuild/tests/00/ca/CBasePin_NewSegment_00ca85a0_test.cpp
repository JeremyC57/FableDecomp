#include <cstdio>
// CBasePin::NewSegment: store start, stop and rate; S_OK.
struct Pin {
    long long m_tStart, m_tStop; double m_dRate;
    int NewSegment(long long tStart, long long tStop, double dRate) { m_tStart = tStart; m_tStop = tStop; m_dRate = dRate; return 0; }
};
int main()
{
    Pin p; p.m_tStart = p.m_tStop = 0; p.m_dRate = 0;
    if (p.NewSegment(10000000LL, 0x123456789LL, 1.5) != 0 || p.m_tStart != 10000000LL || p.m_tStop != 0x123456789LL || p.m_dRate != 1.5) {
        std::printf("00ca85a0_TEST FAIL\n"); return 1;
    }
    std::printf("00ca85a0_TEST PASS\n");
    return 0;
}