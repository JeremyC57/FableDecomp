// Behaviour test for 0x00B80840 CEngineSimulationManager::PausedWorldFrameUpdate: wind = direction * strength, ++steps.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct SWindRecord { C3DVector Direction; float Strength; };
class CEnvironment { public: SWindRecord wind; int calls; virtual SWindRecord* GetWind() { ++calls; return &wind; } };
struct CEnvironmentProvider { CEnvironment* env; virtual CEnvironment* GetCurrentEnvironment() { return env; } };
struct SEngineState { CEnvironmentProvider* m_pEnvironmentProvider; };
static SEngineState g_state;
static SEngineState* g_pEngineState = &g_state;

class CEngineSimulationManager { public:
    C3DVector WindVector;
    unsigned long PausedStepsPending;
    virtual void PausedWorldFrameUpdate()
    {
        C3DVector direction = g_pEngineState->m_pEnvironmentProvider->GetCurrentEnvironment()->GetWind()->Direction;
        WindVector = direction * g_pEngineState->m_pEnvironmentProvider->GetCurrentEnvironment()->GetWind()->Strength;
        ++PausedStepsPending;
    }
};

int main()
{
    CEnvironment env; env.calls = 0;
    env.wind.Direction = C3DVector(1.0f, -2.0f, 0.5f); env.wind.Strength = 4.0f;
    CEnvironmentProvider prov; prov.env = &env;
    g_state.m_pEnvironmentProvider = &prov;
    CEngineSimulationManager m; m.PausedStepsPending = 7;
    m.PausedWorldFrameUpdate();
    if (m.WindVector.X != 4.0f || m.WindVector.Y != -8.0f || m.WindVector.Z != 2.0f || m.PausedStepsPending != 8 || env.calls != 2) {
        std::printf("00b80840_TEST FAIL\n"); return 1;
    }
    std::printf("00b80840_TEST PASS\n");
    return 0;
}