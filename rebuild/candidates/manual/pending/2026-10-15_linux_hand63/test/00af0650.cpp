// Behaviour test for CParticleEmitter::DatabaseClone (0x00AF0650): a new emitter is built and the
// source's database state copied into it.
#include <cstdio>

struct CParticleEmitter {
    virtual void v00() {}
    int m_state;
    int m_constructed;
    CParticleEmitter() : m_state(0), m_constructed(1) {}
    void DatabaseCopy(const CParticleEmitter& other) { m_state = other.m_state; }
    CParticleEmitter* DatabaseClone() const;
};

CParticleEmitter* CParticleEmitter::DatabaseClone() const
{
    CParticleEmitter* clone = new CParticleEmitter;
    clone->DatabaseCopy(*this);
    return clone;
}

int main()
{
    CParticleEmitter source;
    source.m_state = 1234;
    CParticleEmitter* clone = source.DatabaseClone();
    bool ok = clone != 0 && clone != &source && clone->m_state == 1234 && clone->m_constructed == 1;
    delete clone;
    if (!ok) {
        std::printf("00af0650_TEST FAIL\n");
        return 1;
    }
    std::printf("00af0650_TEST PASS\n");
    return 0;
}
