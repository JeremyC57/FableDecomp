// CEngineSimulationManager::WorldFrameUpdate -- refresh wind and advance simulation counters.
// Retail00b80790,172bytes. Slots1 of provider198 and environment yield wind14/strength20.
// Shared PDB header retained for identity; native STLport list size4 vs donor12 shifts wind/counters -0x1c.
#ifdef _MSC_VER
#include "engine/CEngineSimulationManager.h"
#endif
class C3DVector { public:
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
#include <stdio.h>
SWindRecord windData;
int providerCalls=0,windCalls=0;
int special=0,eventCount=0;int events[8];
SEngineState_01436e8c* replacementState=0;SimulationManagerNative* activeManager=0;
SWindRecord replacementWind;
extern SEngineState_01436e8c* g_pEngineState_01436e8c;

void CEnvironment::v00(){} SWindRecord* CEnvironment::GetWind(){++windCalls;if(special){events[eventCount++]=special==1?2:4;if(special==1){g_pEngineState_01436e8c=replacementState;special=2;return &windData;}windData.Direction.X=999;windData.Direction.Y=999;windData.Direction.Z=999;activeManager->FullUpdateOnlyStepsPending=40;activeManager->AlwaysUpdateStepsPending=80;return &replacementWind;}return &windData;}
void CEnvironmentProvider::v00(){} CEnvironment* CEnvironmentProvider::GetCurrentEnvironment(){++providerCalls;if(special){events[eventCount++]=special==1?1:3;if(special==2&&this!=replacementState->m_pEnvironmentProvider)return 0;}static CEnvironment e;return &e;}
SEngineState_01436e8c* g_pEngineState_01436e8c;
int main(){SEngineState_01436e8c state;CEnvironmentProvider provider;state.m_pEnvironmentProvider=&provider;g_pEngineState_01436e8c=&state;
SimulationManagerNative m;m.FullUpdateOnlyStepsPending=3;m.AlwaysUpdateStepsPending=8;
windData.Direction.X=2;windData.Direction.Y=-4;windData.Direction.Z=0.5f;windData.Strength=3;
m.WorldFrameUpdate(false);if(m.WindVector.X!=6||m.WindVector.Y!=-12||m.WindVector.Z!=1.5f||m.FullUpdateOnlyStepsPending!=3||m.AlwaysUpdateStepsPending!=9)return 1;
windData.Strength=-2;m.WorldFrameUpdate(true);if(m.WindVector.X!=-4||m.WindVector.Y!=8||m.WindVector.Z!=-1||m.FullUpdateOnlyStepsPending!=4||m.AlwaysUpdateStepsPending!=10||providerCalls!=4||windCalls!=4)return 2;
m.FullUpdateOnlyStepsPending=~0UL;m.AlwaysUpdateStepsPending=~0UL;windData.Strength=0;m.WorldFrameUpdate(true);if(m.FullUpdateOnlyStepsPending||m.AlwaysUpdateStepsPending||m.WindVector.X||m.WindVector.Y||m.WindVector.Z)return 3;
SEngineState_01436e8c second;CEnvironmentProvider secondProvider;second.m_pEnvironmentProvider=&secondProvider;replacementState=&second;activeManager=&m;g_pEngineState_01436e8c=&state;windData.Direction.X=3;windData.Direction.Y=-2;windData.Direction.Z=4;replacementWind.Strength=-5;special=1;
m.WorldFrameUpdate(true);if(g_pEngineState_01436e8c!=&second||m.WindVector.X!=-15||m.WindVector.Y!=10||m.WindVector.Z!=-20||m.FullUpdateOnlyStepsPending!=41||m.AlwaysUpdateStepsPending!=81||eventCount!=4||events[0]!=1||events[1]!=2||events[2]!=3||events[3]!=4)return 4;
printf("00b80790_TEST PASS\n");return 0;}