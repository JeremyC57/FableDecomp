// Behaviour test for CParticleRef_00c25c70::Bind @ 0x00c25c70: store the particle, bump its lifetimer word, store three values.
#include <cstdio>
struct CParticle { unsigned char _pad[0x1e]; unsigned short m_lifetimer; void IncrementLifetimer() { ++m_lifetimer; } };
struct CParticleRef_00c25c70 {
    CParticle* m_pParticle; long m_a; long m_b; long m_c;
    void Bind(CParticle* particle, long a, long b, long c) { m_pParticle = particle; particle->IncrementLifetimer(); m_a = a; m_b = b; m_c = c; }
};
int main()
{
    CParticle p; p.m_lifetimer = 0xFFFF;
    CParticleRef_00c25c70 r; r.Bind(&p, 1, -2, 3);
    bool ok = r.m_pParticle == &p && p.m_lifetimer == 0 && r.m_a == 1 && r.m_b == -2 && r.m_c == 3 && sizeof(r) == 0x10;
    if (!ok) { std::printf("00c25c70_TEST FAIL\n"); return 1; }
    std::printf("00c25c70_TEST PASS\n");
    return 0;
}
