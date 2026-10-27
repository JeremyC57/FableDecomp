#include <cstdio>

// Behaviour model of CParticle::IncrementLifetimer (0x00C1EA30): a 16-bit increment that wraps.
struct CParticle {
    unsigned char  _pad00[0x1e];
    unsigned short m_lifetimer;
    void IncrementLifetimer() { ++m_lifetimer; }
};

int main()
{

    CParticle p; p.m_lifetimer = 0;
    p.IncrementLifetimer(); p.IncrementLifetimer();
    if (p.m_lifetimer != 2) { std::printf("00c1ea30_TEST FAIL\n"); return 1; }
    p.m_lifetimer = 0xFFFF; p.IncrementLifetimer();
    if (p.m_lifetimer != 0) { std::printf("00c1ea30_TEST FAIL\n"); return 1; }
    std::printf("00c1ea30_TEST PASS\n");
    return 0;
}