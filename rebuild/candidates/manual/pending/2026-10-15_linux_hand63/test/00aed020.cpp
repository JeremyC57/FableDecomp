// Behaviour test for CParticleEmitter::GetEmitterCost (0x00AED020): only enabled systems add
// their cost.
#include <cstdio>

struct CParticleSystemCost { float PolyCount; };

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
    float         m_cost;
    void GetSystemCost(CParticleSystemCost* cost) { cost->PolyCount += m_cost; }
};

struct CParticleSystemPtr { CParticleSystem* m_pObject; void* m_pCount; };
struct CParticleSystemPtrVector { CParticleSystemPtr* m_first; CParticleSystemPtr* m_last; CParticleSystemPtr* m_end; };

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    void GetEmitterCost(CParticleSystemCost* cost);
};

void CParticleEmitter::GetEmitterCost(CParticleSystemCost* cost)
{
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system->Enabled)
            system->GetSystemCost(cost);
    }
}

int main()
{
    CParticleSystem s[3];
    CParticleSystemPtr p[3];
    for (int i = 0; i < 3; ++i) {
        s[i].Initialised = s[i].Dead = s[i].NormalisedTimeEnabled = s[i].UseAllAttractors = false;
        s[i].Visible = s[i].StayWithEmitter = s[i].ScaleParticles = false;
        s[i].m_cost = (float)(1 << i);
        p[i].m_pObject = &s[i]; p[i].m_pCount = 0;
    }
    s[0].Enabled = true; s[1].Enabled = false; s[2].Enabled = true;
    CParticleEmitter e;
    e.ParticleSystems.m_first = p; e.ParticleSystems.m_last = p + 3; e.ParticleSystems.m_end = p + 3;
    CParticleSystemCost cost; cost.PolyCount = 0.0f;
    e.GetEmitterCost(&cost);
    bool ok = cost.PolyCount == 5.0f;
    e.ParticleSystems.m_last = p;
    e.GetEmitterCost(&cost);
    ok = ok && cost.PolyCount == 5.0f;
    if (!ok) {
        std::printf("00aed020_TEST FAIL\n");
        return 1;
    }
    std::printf("00aed020_TEST PASS\n");
    return 0;
}
