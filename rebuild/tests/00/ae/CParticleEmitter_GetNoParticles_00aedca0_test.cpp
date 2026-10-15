// Behaviour test for CParticleEmitter::GetNoParticles (0x00AEDCA0): the counts of the enabled systems are
// summed.
#include <cstdio>

struct CParticleSystem {
    unsigned char _state[0x50];
    bool          Initialised           : 1;
    bool          Enabled               : 1;
    bool          Dead                  : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors      : 1;
    bool          Visible               : 1;
    bool          StayWithEmitter       : 1;
    bool          ScaleParticles        : 1;
    long          m_count;
    long GetNoActiveParticles() { return m_count; }
};

struct CParticleSystemPtr { CParticleSystem* m_pObject; void* m_pCount; };
struct CParticleSystemPtrVector { CParticleSystemPtr* m_first; CParticleSystemPtr* m_last; CParticleSystemPtr* m_end; };

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    long GetNoParticles();
};

long CParticleEmitter::GetNoParticles()
{
    long total = 0;
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system->Enabled)
            total += system->GetNoActiveParticles();
    }
    return total;
}

int main()
{
    CParticleSystem s[3];
    CParticleSystemPtr p[3];
    for (int i = 0; i < 3; ++i) {
        s[i].Initialised = s[i].Dead = s[i].NormalisedTimeEnabled = s[i].UseAllAttractors = false;
        s[i].Visible = s[i].StayWithEmitter = s[i].ScaleParticles = false;
        s[i].m_count = 10 * (i + 1);
        p[i].m_pObject = &s[i]; p[i].m_pCount = 0;
    }
    s[0].Enabled = true; s[1].Enabled = false; s[2].Enabled = true;
    CParticleEmitter e;
    e.ParticleSystems.m_first = p; e.ParticleSystems.m_last = p + 3; e.ParticleSystems.m_end = p + 3;
    long total = e.GetNoParticles();
    e.ParticleSystems.m_last = p;
    long none = e.GetNoParticles();
    if (total != 40 || none != 0) {
        std::printf("00aedca0_TEST FAIL\n");
        return 1;
    }
    std::printf("00aedca0_TEST PASS\n");
    return 0;
}