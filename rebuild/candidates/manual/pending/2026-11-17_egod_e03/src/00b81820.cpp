// CEngineSimulationManager::FrameStart(const CEngineComponentParams& params) -- per-frame cloth simulation: with the main mesh bank loaded, advance the wind-noise time and its sine offset, clamp the zoom to at least 1, point the particle memory pool at the main bank's cloth pool, clear the static particle sims, run every pending full-update / always-update / paused step over its list of simulation infos, then restore the pool
// Leaf: ?FrameStart@CEngineSimulationManager@@UAEXABVCEngineComponentParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00B81820, 433 bytes, thiscall `ret 4` (public virtual; a CEngineComponent override, so `this` is the
//   +4 subobject: the lists at [ecx+0Ch] = object +0x10).
// Names: Ego_r.pdb / FableWin.pdb ?FrameStart@CEngineSimulationManager@@UAEXABVCEngineComponentParams@@@Z; Ego_d.pdb
//   Engine_Simulation_Manager.obj locals old_cloth_pool (CMovableResourceMemoryPool*) and three blocks { itr; { sim } }; members from
//   struct_layouts_egor.tsv (overlay opt-in FABLE_RETAIL_CENGINESIMULATIONMANAGER_MEMBERS). FableWin inline accessors
//   CEngine::GetNonInterpolatedWorldTime (Ego_d: first used here), CEngineCamera::PeekNonHomogenousViewToViewScaleX,
//   CEngineResourceManager::GetMeshBank (Ego_r), CMeshDataBank::GetClothMemoryPool; Ego_d EMeshBankNo MAIN_MESH_BANK_NO.
//   Callees / globals: CEngineClothSimulationMeshBase::ClearStaticParticleSims 0x00B81450 (static __fastcall; no shared header, declared
//   here as the landed row does), NEngineInternal::GPResourceManager 0x01436E98 / GPEngine 0x01436EA4 / GPCamera 0x01436EA0,
//   TWO_PI 0x0124F2B8, g_ParticleMemoryPool 0x013D2D70 (bound spellings; the pool global's PDB name is not in the name sets).
// Same-TU body: CEnginePrimitiveSimulationInfo::RunSimulation (Ego_d locals itr / sim), defined before the row as in Ego_d's object
//   order and expanded three times; it grades MATCH at its retail out-of-line copy 0x00B80B40.
#include <math.h>
#pragma intrinsic(sin)
#define FABLE_RETAIL_CENGINESIMULATIONMANAGER
#define FABLE_RETAIL_CENGINESIMULATIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINE_GETNONINTERPOLATEDWORLDTIME
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETMESHBANK
#define FABLE_RETAIL_EMESHBANKNO_ENUMERATORS
#define FABLE_RETAIL_CMESHDATABANK_GETCLOTHMEMORYPOOL
#include "fable_compiler.h"
#include "engine/CEngineSimulationManager.h"
#include "engine/CEngine.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CMeshDataBank.h"

class CMovableResourceMemoryPool;
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;                // 0x013D2D70
extern const double TWO_PI;                                             // 0x0124F2B8
namespace NEngineInternal {
extern CEngine* GPEngine;                                               // 0x01436EA4
extern CEngineCamera* GPCamera;                                         // 0x01436EA0
extern CEngineResourceManager* GPResourceManager;                       // 0x01436E98
}
class CEngineClothSimulationMeshBase {
public:
    static void FABLE_FASTCALL ClearStaticParticleSims();               // 0x00B81450
};

void CEnginePrimitiveSimulationInfo::RunSimulation()
{
    for (CArray<CCountedPointer<CEngineSimulationBase> >::iterator itr = Simulations.begin(); itr != Simulations.end(); ++itr) {
        CCountedPointer<CEngineSimulationBase>& sim = *itr;
        if (!sim.IsNull())
            sim->RunSimulation();
    }
}

void CEngineSimulationManager::FrameStart(const CEngineComponentParams& params)
{
    if (NEngineInternal::GPResourceManager->GetMeshBank(MAIN_MESH_BANK_NO) == 0)
        return;
    WindNoiseTime = WindNoiseSpeed * NEngineInternal::GPEngine->GetNonInterpolatedWorldTime();
    WindNoiseOffset = sin(WindNoiseTime * TWO_PI);
    Zoom = 1.0f > NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleX() ? 1.0f : NEngineInternal::GPCamera->PeekNonHomogenousViewToViewScaleX();
    CMovableResourceMemoryPool* old_cloth_pool = g_ParticleMemoryPool;
    g_ParticleMemoryPool = NEngineInternal::GPResourceManager->GetMeshBank(MAIN_MESH_BANK_NO)->GetClothMemoryPool();
    CEngineClothSimulationMeshBase::ClearStaticParticleSims();
    while (FullUpdateOnlyStepsPending) {
        for (std::list<CIVCountedPointer<CEnginePrimitiveSimulationInfo> >::iterator itr = FullUpdateOnlySimulationInfoList.begin(); itr != FullUpdateOnlySimulationInfoList.end(); ++itr) {
            CEnginePrimitiveSimulationInfo& sim = **itr;
            sim.RunSimulation();
        }
        --FullUpdateOnlyStepsPending;
    }
    while (AlwaysUpdateStepsPending) {
        for (std::list<CIVCountedPointer<CEnginePrimitiveSimulationInfo> >::iterator itr = AlwaysUpdateSimulationInfoList.begin(); itr != AlwaysUpdateSimulationInfoList.end(); ++itr) {
            CEnginePrimitiveSimulationInfo& sim = **itr;
            sim.RunSimulation();
        }
        --AlwaysUpdateStepsPending;
    }
    while (PausedStepsPending) {
        for (std::list<CIVCountedPointer<CEnginePrimitiveSimulationInfo> >::iterator itr = PausedSimulationInfoList.begin(); itr != PausedSimulationInfoList.end(); ++itr) {
            CEnginePrimitiveSimulationInfo& sim = **itr;
            sim.RunSimulation();
        }
        --PausedStepsPending;
    }
    g_ParticleMemoryPool = old_cloth_pool;
}
