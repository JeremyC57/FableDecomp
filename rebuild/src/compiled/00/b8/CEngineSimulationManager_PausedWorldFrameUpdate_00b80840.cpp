// CEngineSimulationManager::PausedWorldFrameUpdate() -- while the world is paused: refresh the wind vector from the current environment's wind (direction * strength) and count one more pending paused step
// Leaf: PausedWorldFrameUpdate
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80840, length 156 bytes (virtual, `?PausedWorldFrameUpdate@CEngineSimulationManager@@UAEXXZ`).
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C (returns the current
// environment), virtual slot 1 of that (returns the wind record: direction C3DVector at +0x14, strength float at +0x20);
// the chain is walked twice (once per field).
// Layout: a CEngineComponent virtual, so `this` is the CEngineComponent subobject at +4 of the object (the landed constructor
// 0x00B81BC0 stores its vtable at +4): this+0x1050 / +0x1070 are engine/CEngineSimulationManager.h WindVector +0x1054 and
// PausedStepsPending +0x1074 (retail STLport 4.6 offsets, which the constructor confirms). Modelled locally as that subobject.
// Shape: the direction is copied into a C3DVector local through a memberwise user copy constructor (the two first
// words are read at +0x14/+0x18 before `add eax,14h`; the implicit copy forms the address first), `direction * strength`
// builds a by-value C3DVector temporary (user constructor) in the second frame vector that is then copied into the
// member through ecx/edx; the strength stays on the FPU across the three products; `++PausedStepsPending` last.
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

class CEngineSimulationManager { public:
    unsigned char _pad[0x104c];                     // CEngineComponent vptr at subobject +0x00 (object +0x04)
    C3DVector     WindVector;                      // subobject +0x1050 = object +0x1054
    unsigned char _pad105c[0x14];
    unsigned long PausedStepsPending;              // subobject +0x1070 = object +0x1074
    virtual void PausedWorldFrameUpdate();
};

void CEngineSimulationManager::PausedWorldFrameUpdate()
{
    C3DVector direction = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeWindDirection;
    WindVector = direction * NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeWindSpeed;
    ++PausedStepsPending;
}