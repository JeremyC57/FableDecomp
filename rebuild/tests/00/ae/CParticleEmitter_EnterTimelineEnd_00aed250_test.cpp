// Behaviour test for CParticleEmitter::EnterTimelineEnd (0x00AED250): present, enabled, not-dead
// systems only.
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
    int           m_ended;
    void EnterTimelineEnd() { ++m_ended; }
};

struct CParticleSystemPtr { CParticleSystem* m_pObject; void* m_pCount; };
struct CParticleSystemPtrVector { CParticleSystemPtr* m_first; CParticleSystemPtr* m_last; CParticleSystemPtr* m_end; };

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    void EnterTimelineEnd();
};

void CParticleEmitter::EnterTimelineEnd()
{
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system && system->Enabled && !system->Dead)
            system->EnterTimelineEnd();
    }
}

int main()
{
    CParticleSystem s[3];
    CParticleSystemPtr p[4];
    for (int i = 0; i < 3; ++i) {
        s[i].Initialised = s[i].NormalisedTimeEnabled = s[i].UseAllAttractors = false;
        s[i].Visible = s[i].StayWithEmitter = s[i].ScaleParticles = false;
        s[i].m_ended = 0;
        p[i].m_pObject = &s[i]; p[i].m_pCount = 0;
    }
    p[3].m_pObject = 0; p[3].m_pCount = 0;
    s[0].Enabled = true;  s[0].Dead = false;
    s[1].Enabled = true;  s[1].Dead = true;
    s[2].Enabled = false; s[2].Dead = false;
    CParticleEmitter e;
    e.ParticleSystems.m_first = p; e.ParticleSystems.m_last = p + 4; e.ParticleSystems.m_end = p + 4;
    e.EnterTimelineEnd();
    if (s[0].m_ended != 1 || s[1].m_ended != 0 || s[2].m_ended != 0) {
        std::printf("00aed250_TEST FAIL\n");
        return 1;
    }
    std::printf("00aed250_TEST PASS\n");
    return 0;
}