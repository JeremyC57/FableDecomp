// Behaviour test for CParticleEmitter::GetParticleStats (0x00AED290): SystemsActive grows by the
// system count, each visible system counts as visible, otherwise (emitter sleeping) as sleeping,
// and every system adds its own stats.
#include <cstdio>

struct CParticleStats {
    long EmittersVisible;
    long EmittersActive;
    long EmittersSleeping;
    long SystemsVisible;
    long SystemsActive;
    long SystemsSleeping;
};

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
    void GetParticleStats(CParticleStats* stats) { ++stats->EmittersVisible; }
};

struct CParticleSystemPtr { CParticleSystem* m_pObject; void* m_pCount; };

struct CParticleSystemPtrVector {
    CParticleSystemPtr* m_first;
    CParticleSystemPtr* m_last;
    CParticleSystemPtr* m_end;
    long size() const { return m_last - m_first; }
};

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    unsigned char            UpdateParameters[0x24];
    unsigned char            PrimitiveGroupHandle[8];
    unsigned short           Priority                 : 3;
    unsigned short           IsScreenDisplacement     : 1;
    unsigned short           ReadZBuffer              : 1;
    unsigned short           Dead                     : 1;
    unsigned short           ParticleCreationDisabled : 1;
    unsigned short           ContinuousEmitter        : 1;
    unsigned short           Initialised              : 1;
    unsigned short           WaitingToSleep           : 1;
    unsigned short           Sleeping                 : 1;
    unsigned short           Emitter2D                : 1;
    bool IsSleeping() const { return Sleeping; }
    void GetParticleStats(CParticleStats* stats);
};

void CParticleEmitter::GetParticleStats(CParticleStats* stats)
{
    stats->SystemsActive += ParticleSystems.size();
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        CParticleSystem* system = it->m_pObject;
        if (system->Visible)
            ++stats->SystemsVisible;
        else if (IsSleeping())
            ++stats->SystemsSleeping;
        system->GetParticleStats(stats);
    }
}

int main()
{
    CParticleSystem s[3];
    CParticleSystemPtr p[3];
    for (int i = 0; i < 3; ++i) {
        s[i].Initialised = s[i].Enabled = s[i].Dead = s[i].NormalisedTimeEnabled = false;
        s[i].UseAllAttractors = s[i].StayWithEmitter = s[i].ScaleParticles = false;
        p[i].m_pObject = &s[i]; p[i].m_pCount = 0;
    }
    s[0].Visible = true; s[1].Visible = false; s[2].Visible = false;
    CParticleEmitter e;
    e.ParticleSystems.m_first = p; e.ParticleSystems.m_last = p + 3; e.ParticleSystems.m_end = p + 3;
    e.Priority = 0; e.IsScreenDisplacement = 0; e.ReadZBuffer = 0; e.Dead = 0; e.ParticleCreationDisabled = 0;
    e.ContinuousEmitter = 0; e.Initialised = 1; e.WaitingToSleep = 0; e.Emitter2D = 0;
    CParticleStats st = { 0, 0, 0, 0, 0, 0 };
    e.Sleeping = 0;
    e.GetParticleStats(&st);
    bool ok = st.SystemsActive == 3 && st.SystemsVisible == 1 && st.SystemsSleeping == 0 && st.EmittersVisible == 3;
    e.Sleeping = 1;
    e.GetParticleStats(&st);
    ok = ok && st.SystemsActive == 6 && st.SystemsVisible == 2 && st.SystemsSleeping == 2 && st.EmittersVisible == 6;
    if (!ok) {
        std::printf("00aed290_TEST FAIL\n");
        return 1;
    }
    std::printf("00aed290_TEST PASS\n");
    return 0;
}