// CEngineSimulationManager::WorldFrameUpdate -- refresh wind and advance simulation counters.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail00b80790,172bytes. Slots1 of provider198 and environment yield wind14/strength20.
// Shared PDB header retained for identity; native STLport list size4 vs donor12 shifts wind/counters -0x1c.
#ifdef _MSC_VER
#include "engine/CEngineSimulationManager.h"
#endif
#include "engine/C3DVector.h"

// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A); the "environment provider at +0x198" is its GameWorld (FableWin.pdb inline
// PeekGameWorld, engine/CEngineWorldMap.h _GAMEWORLD), slot 1 CIDrawWorld::DrawGetEnvironment, the environment's slot 1
// CIDrawEnvironment::PeekInterpolatedParams (engine/CIDrawWorld.h / CIDrawEnvironment.h; CIDrawEnvironmentParams Ego_r.pdb members).
// Were local SEngineState_01436e8c-style / CEnvironmentProvider / CEnvironment / settings-record stand-ins.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

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
    C3DVector direction = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeWindDirection;
    WindVector = direction * NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeWindSpeed;
    if (full) ++FullUpdateOnlyStepsPending;
    ++AlwaysUpdateStepsPending;
}