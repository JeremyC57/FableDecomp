#include <cstdio>
struct CIEngine { int id; };
struct C3DBoundingSphere { float Centre[3]; float Radius; };
struct CEnginePrimitivePrepareInfo { int id; };
struct CEnginePrimitiveHandle { int id; };
struct CParticleSystem;
struct CParticleUpdateParameters { unsigned char m_data[0x24]; };

// Mock component: records what the system forwarded to it.
struct CPSCBase {
    int   m_calls;
    float m_length;
    bool  m_result;
    const void* m_arg[5];
    float m_time;
    CPSCBase() : m_calls(0), m_length(0.0f), m_result(false), m_time(0.0f) { for (int i = 0; i < 5; ++i) m_arg[i] = 0; }
    virtual void PreparePrimitives(float time, CEnginePrimitivePrepareInfo* info, CParticleSystem* system, CParticleUpdateParameters* params, CEnginePrimitiveHandle* handle)
    { ++m_calls; m_time = time; m_arg[0] = info; m_arg[1] = system; m_arg[2] = params; m_arg[3] = handle; }
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float time, CParticleSystem* system, CParticleUpdateParameters* params)
    { ++m_calls; m_time = time; m_arg[0] = sphere; m_arg[1] = system; m_arg[2] = params; return m_result; }
    virtual void EnterTimelineEnd() { ++m_calls; }
    virtual bool Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters* params, CEnginePrimitiveHandle* handle)
    { ++m_calls; m_arg[0] = engine; m_arg[1] = system; m_arg[2] = params; m_arg[3] = handle; return m_result; }
    virtual void ClearAllPrimitives(CIEngine* engine, CEnginePrimitiveHandle* handle) { ++m_calls; m_arg[0] = engine; m_arg[1] = handle; }
    virtual float GetApproximateSplineLength() { ++m_calls; return m_length; }
};

struct SCountBlock;
struct CComponentPointer { CPSCBase* Object; SCountBlock* Block; CPSCBase* operator->() const { return Object; } };
struct SComponentState { CComponentPointer Component; bool Enabled; unsigned long InstanceName; };
struct SComponentEntry { long first; void* m_instanceVtable; SComponentState second; };

struct CParticleSystem {
    void* m_vtable; void* PParentEmitter; CIEngine* PEngine;
    SComponentEntry* m_componentsFirst; SComponentEntry* m_componentsLast; SComponentEntry* m_componentsEnd;
    unsigned char m_componentsCompare; bool m_componentsDirty;
    void* Particles; void* Decals;
    CParticleUpdateParameters UpdateParameters;
    long NoActiveParticles; long NoActiveDecals;
    bool Initialised : 1; bool Enabled : 1; bool Dead : 1; bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1; bool Visible : 1; bool StayWithEmitter : 1; bool ScaleParticles : 1;
    bool Initialise(CIEngine* engine, CParticleUpdateParameters* params, CEnginePrimitiveHandle* handle)
    {
        if (engine) {
            PEngine = engine;
            Initialised = true;
        }
    SComponentEntry* it = m_componentsFirst;
    SComponentEntry* last = m_componentsLast;
    if (it != last) {
        SComponentState* state = &it->second;
        do {
            if (state->Enabled) Initialised &= state->Component->Initialise(PEngine, this, &UpdateParameters, handle);
            last = m_componentsLast;
            ++it;
            state = (SComponentState*)((char*)state + sizeof(SComponentEntry));
        } while (it != last);
    }
        return Initialised;
    }
};
static CPSCBase g_c[3];
static SComponentEntry g_e[3];
static void Setup(CParticleSystem& s, CIEngine* engine)
{
    for (int i = 0; i < 3; ++i) { g_e[i].first = i; g_e[i].second.Component.Object = &g_c[i]; g_e[i].second.Component.Block = 0; g_e[i].second.Enabled = (i != 1); }
    s.m_componentsFirst = g_e; s.m_componentsLast = g_e + 3; s.m_componentsEnd = g_e + 3; s.PEngine = engine;
}
int main()
{
    CParticleSystem s; CIEngine e, e2; Setup(s, &e); CEnginePrimitiveHandle h;
    s.Initialised = false; s.Dead = true;
    g_c[0].m_result = true; g_c[1].m_result = false; g_c[2].m_result = true;
    if (!s.Initialise(&e2, 0, &h) || s.PEngine != &e2 || !s.Dead || g_c[0].m_arg[0] != &e2 || g_c[0].m_arg[1] != &s || g_c[2].m_arg[3] != &h) { std::printf("00af4010_TEST FAIL\n"); return 1; }
    g_c[2].m_result = false;
    if (s.Initialise(0, 0, &h) || s.PEngine != &e2 || s.Initialised || !s.Dead) { std::printf("00af4010_TEST FAIL\n"); return 1; }
    std::printf("00af4010_TEST PASS\n");
    return 0;
}