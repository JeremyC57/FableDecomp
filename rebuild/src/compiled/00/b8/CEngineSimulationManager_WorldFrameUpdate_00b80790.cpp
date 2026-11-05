// CEngineSimulationManager::WorldFrameUpdate -- refresh wind and advance simulation counters.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail00b80790,172bytes. Slots1 of provider198 and environment yield wind14/strength20.
// Shared PDB header retained for identity; native STLport list size4 vs donor12 shifts wind/counters -0x1c.
#ifdef _MSC_VER
#include "engine/CEngineSimulationManager.h"
#endif
#include "engine/C3DVector.h"

struct SWindRecord {
    unsigned char _pad[0x14];
    C3DVector     Direction;                       // +0x14
    float         Strength;                        // +0x20
};

class CEnvironment { public:
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