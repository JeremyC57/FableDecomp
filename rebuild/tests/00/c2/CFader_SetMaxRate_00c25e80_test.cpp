#include <cstdio>
struct CFader { float m_timeStep, m_level; unsigned char m_param[0x34]; float m_silentTime, m_maxRate, m_fadeInStep, m_fadeOutStep; long m_state;
    void SetMaxRate(float rate) { m_maxRate = rate; } };
int main()
{
    CFader f; f.m_maxRate = 0.0f; f.m_silentTime = 7.0f; f.m_fadeInStep = 9.0f;
    f.SetMaxRate(250.0f);
    bool ok = sizeof(CFader) == 0x50 && (char*)&f.m_maxRate - (char*)&f == 0x40 && f.m_maxRate == 250.0f && f.m_silentTime == 7.0f && f.m_fadeInStep == 9.0f;
    if (!ok) { std::printf("00c25e80_TEST FAIL\n"); return 1; }
    std::printf("00c25e80_TEST PASS\n");
    return 0;
}
