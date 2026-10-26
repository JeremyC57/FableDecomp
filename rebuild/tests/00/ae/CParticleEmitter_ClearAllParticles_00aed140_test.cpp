// Behaviour test for CParticleEmitter::ClearAllParticles (0x00AED140): runs once, passes the emitter's
// primitive group to every system, then sets ClearedParticles (the other flag untouched).
#include <cstdio>

class CEnginePrimitiveHandle { public: void* m_pGroup; long m_index; };

static const CEnginePrimitiveHandle* g_lastGroup = 0;

struct CParticleSystem {
    int m_cleared;
    void ClearAllParticles(const CEnginePrimitiveHandle& group) { ++m_cleared; g_lastGroup = &group; }
};

struct CParticleSystemPtr { CParticleSystem* m_pObject; void* m_pCount; };
struct CParticleSystemPtrVector { CParticleSystemPtr* m_first; CParticleSystemPtr* m_last; CParticleSystemPtr* m_end; };

class CIEngine;

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    unsigned char            UpdateParameters[0x24];
    CEnginePrimitiveHandle   PrimitiveGroupHandle;
    unsigned short           Priority                   : 3;
    unsigned short           IsScreenDisplacement       : 1;
    unsigned short           ReadZBuffer                : 1;
    unsigned short           Dead                       : 1;
    unsigned short           ParticleCreationDisabled   : 1;
    unsigned short           ContinuousEmitter          : 1;
    unsigned short           Initialised                : 1;
    unsigned short           WaitingToSleep             : 1;
    unsigned short           Sleeping                   : 1;
    unsigned short           Emitter2D                  : 1;
    unsigned short           Emitter2DLayer             : 7;
    unsigned char            PreWaterEffect             : 1;
    unsigned char            WaterEffect                : 1;
    unsigned char            ZBufferWriteable           : 1;
    unsigned char            Visible                    : 1;
    unsigned char            AlwaysUpdate               : 1;
    unsigned char            DieIfOffsceen              : 1;
    unsigned char            OffscreenUpdate            : 1;
    unsigned char            ClipEffectToWeatherMask    : 1;
    unsigned char            EnableDithering            : 1;
    unsigned char            CalcBoundingSphereOnceOnly : 1;
    unsigned char            BoundingSphereValid        : 1;
    unsigned char            ClearedParticles           : 1;
    unsigned char            ClearedDecals              : 1;
    void ClearAllParticles(CIEngine* engine);
};

void CParticleEmitter::ClearAllParticles(CIEngine* engine)
{
    if (!ClearedParticles) {
        for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it)
            it->m_pObject->ClearAllParticles(PrimitiveGroupHandle);
        ClearedParticles = 1;
    }
}

int main()
{
    CParticleSystem s[2];
    s[0].m_cleared = 0; s[1].m_cleared = 0;
    CParticleSystemPtr p[2];
    p[0].m_pObject = &s[0]; p[0].m_pCount = 0;
    p[1].m_pObject = &s[1]; p[1].m_pCount = 0;
    CParticleEmitter e;
    e.ParticleSystems.m_first = p; e.ParticleSystems.m_last = p + 2; e.ParticleSystems.m_end = p + 2;
    e.EnableDithering = 1; e.CalcBoundingSphereOnceOnly = 0; e.BoundingSphereValid = 1;
    e.ClearedParticles = 0; e.ClearedDecals = 0;
    e.ClearAllParticles(0);
    bool ok = s[0].m_cleared == 1 && s[1].m_cleared == 1 && g_lastGroup == &e.PrimitiveGroupHandle && e.ClearedParticles == 1
        && e.EnableDithering == 1 && e.BoundingSphereValid == 1 && e.CalcBoundingSphereOnceOnly == 0
        && (e.ClearedParticles + e.ClearedDecals) == 1;
    e.ClearAllParticles(0);
    ok = ok && s[0].m_cleared == 1 && s[1].m_cleared == 1;
    CParticleEmitter empty;
    empty.ParticleSystems.m_first = empty.ParticleSystems.m_last = empty.ParticleSystems.m_end = 0;
    empty.ClearedParticles = 0; empty.ClearedDecals = 0;
    empty.ClearAllParticles(0);
    ok = ok && empty.ClearedParticles == 1;
    if (!ok) {
        std::printf("00aed140_TEST FAIL\n");
        return 1;
    }
    std::printf("00aed140_TEST PASS\n");
    return 0;
}