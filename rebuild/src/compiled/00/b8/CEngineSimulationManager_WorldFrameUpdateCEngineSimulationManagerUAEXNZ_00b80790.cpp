// CEngineSimulationManager::WorldFrameUpdate -- refresh wind and advance simulation counters.
// Retail00b80790,172bytes. Slots1 of provider198 and environment yield wind14/strength20.
// Shared PDB header retained for identity; native STLport list size4 vs donor12 shifts wind/counters -0x1c.
#ifdef _MSC_VER
#include "engine/CEngineSimulationManager.h"
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

struct SWindRecord {
    unsigned char _pad[0x14];
    C3DVector     Direction;                       // +0x14
    float         Strength;                        // +0x20
};

struct CEnvironment {
    virtual void v00();
    virtual SWindRecord* GetWind();                // slot 1
};

struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};

struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

struct SimulationManagerNative {
    unsigned char _pad[0x104c];                     // vptr at +0x00
    C3DVector     WindVector;                      // +0x1050
    unsigned char _pad105c[0xc];
    unsigned long FullUpdateOnlyStepsPending;
    unsigned long AlwaysUpdateStepsPending;              // +0x1070
    virtual void WorldFrameUpdate(bool full);
};

void SimulationManagerNative::WorldFrameUpdate(bool full)
{
    C3DVector direction = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetWind()->Direction;
    WindVector = direction * g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetWind()->Strength;
    if (full) ++FullUpdateOnlyStepsPending;
    ++AlwaysUpdateStepsPending;
}