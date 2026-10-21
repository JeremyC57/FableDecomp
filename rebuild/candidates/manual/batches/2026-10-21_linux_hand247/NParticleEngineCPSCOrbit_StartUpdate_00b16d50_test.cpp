// Behaviour test for NParticleEngine::CPSCOrbit::StartUpdate @ 0x00B16D50: first frame seeds and warms up four steps, later frames one step.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33; C3DVector Pos; };
struct CParticleUpdateParameters { int dummy; };

static float g_frameTime = 0.5f;
static unsigned long g_seed = 1;
float GetParticleFrameTime() { return g_frameTime; }
unsigned long* GetParticleRandomSeed() { return &g_seed; }

static int g_inits = 0, g_updates = 0, g_badMatrix = 0;
static float g_times[32];
struct COrbitSettings { long Type; };
struct COrbitState {
    void Initialise(const COrbitSettings&, unsigned long* seed) { if (seed == &g_seed) ++g_inits; }
    void Update(const COrbitSettings&, CMatrix3x4& m, float time)
    {
        if (m.E11 != 1 || m.E22 != 1 || m.E33 != 1 || m.E12 != 0 || m.E13 != 0 || m.E21 != 0 || m.E23 != 0
            || m.E31 != 0 || m.E32 != 0 || m.Pos.X != 3 || m.Pos.Y != 4 || m.Pos.Z != 5) ++g_badMatrix;
        g_times[g_updates++] = time;
    }
};

struct CPSCOrbit {
    bool UpdateStarted; unsigned long CentreParam; COrbitSettings Orbits[3]; COrbitState OrbitStates[3];
    long LifeTimer; float LifeTimeSecs;
    C3DVector GetCurrentPosition(const CParticleUpdateParameters&, const unsigned long& param, float, bool, unsigned long) const
    {
        C3DVector v; v.X = 3; v.Y = 4; v.Z = (float)param; return v;
    }
    void StartUpdate(const CParticleUpdateParameters& params);
};

void CPSCOrbit::StartUpdate(const CParticleUpdateParameters& params)
{
    UpdateStarted = true;
    float frameTime = GetParticleFrameTime();
    unsigned long updates = 1;
    if (LifeTimer == 0) {
        for (int i = 0; i < 3; ++i)
            OrbitStates[i].Initialise(Orbits[i], GetParticleRandomSeed());
        updates = 4;
        LifeTimeSecs = -frameTime;
    }
    C3DVector centre = GetCurrentPosition(params, CentreParam, 1.0f, false, 0);
    for (unsigned long n = 0; n < updates; ++n) {
        CMatrix3x4 transform;
        transform.E12 = transform.E13 = transform.E21 = transform.E23 = transform.E31 = transform.E32 = 0.0f;
        transform.E11 = transform.E22 = transform.E33 = 1.0f;
        transform.Pos = centre;
        for (int i = 0; i < 3; ++i)
            OrbitStates[i].Update(Orbits[i], transform, LifeTimeSecs);
        LifeTimeSecs = frameTime + LifeTimeSecs;
    }
    ++LifeTimer;
}

int main()
{
    CPSCOrbit o;
    o.UpdateStarted = false; o.CentreParam = 5; o.LifeTimer = 0; o.LifeTimeSecs = 99.0f;
    CParticleUpdateParameters p;
    o.StartUpdate(p);
    int ok = o.UpdateStarted && g_inits == 3 && g_updates == 12 && g_badMatrix == 0 && o.LifeTimer == 1
          && g_times[0] == -0.5f && g_times[3] == 0.0f && g_times[11] == 1.0f && o.LifeTimeSecs == 1.5f;
    o.StartUpdate(p);
    ok = ok && g_inits == 3 && g_updates == 15 && g_times[14] == 1.5f && o.LifeTimer == 2 && o.LifeTimeSecs == 2.0f;
    if (!ok) { std::printf("00b16d50_TEST FAIL\n"); return 1; }
    std::printf("00b16d50_TEST PASS\n");
    return 0;
}
