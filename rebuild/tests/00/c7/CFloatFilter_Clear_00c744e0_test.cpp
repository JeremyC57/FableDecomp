// Behaviour test for 00c744e0: CDspGain::Reset -- four running gains back to 1.0f
#include <cstdio>
class CDspObject { public: virtual ~CDspObject() { } };
class CDspGain : public CDspObject { public: float m_gain[4]; void Reset() { m_gain[0] = 1.0f; m_gain[1] = 1.0f; m_gain[2] = 1.0f; m_gain[3] = 1.0f; } };
int main()
{
    CDspGain g; for (int i = 0; i < 4; ++i) g.m_gain[i] = 0.25f * i;
    g.Reset();
    int ok = g.m_gain[0] == 1.0f && g.m_gain[1] == 1.0f && g.m_gain[2] == 1.0f && g.m_gain[3] == 1.0f && sizeof(CDspGain) == 0x14;
    std::printf(ok ? "00c744e0_TEST PASS\n" : "00c744e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
