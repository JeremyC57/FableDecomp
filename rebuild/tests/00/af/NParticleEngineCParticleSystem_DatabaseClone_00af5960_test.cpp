// Behaviour test for NParticleEngine::CParticleSystem::DatabaseClone (0x00AF5960): a new system is built and the
// source's database state copied into it.
#include <cstdio>

namespace NParticleEngine {
class CParticleSystem { public:
    int m_state;
    int m_constructed;
    CParticleSystem() : m_state(0), m_constructed(1) {}
    void DatabaseCopy(const CParticleSystem& other) { m_state = other.m_state; }
    CParticleSystem* DatabaseClone() const;
};

CParticleSystem* CParticleSystem::DatabaseClone() const
{
    CParticleSystem* clone = new CParticleSystem;
    clone->DatabaseCopy(*this);
    return clone;
}

} // namespace NParticleEngine

using NParticleEngine::CParticleSystem;

int main()
{
    CParticleSystem source;
    source.m_state = 1234;
    CParticleSystem* clone = source.DatabaseClone();
    bool ok = clone != 0 && clone != &source && clone->m_state == 1234 && clone->m_constructed == 1;
    delete clone;
    if (!ok) {
        std::printf("00af5960_TEST FAIL\n");
        return 1;
    }
    std::printf("00af5960_TEST PASS\n");
    return 0;
}