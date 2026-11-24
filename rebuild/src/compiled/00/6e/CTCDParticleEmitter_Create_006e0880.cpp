// Leaf: ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z
// Flags:
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_MOVEPLAYERPOS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGMANAGER_WORLDMAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTC_DRIVER_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CParticleEmitter.h"

CThing* FABLE_FASTCALL CTCDParticleEmitter::Create(long particle_type_id, const C3DVector& pos, bool force)
{
    if (!force && CMainGameComponent::Get()->PeekWorld().PeekWorldMap().IsLoadingAnyMap())
        force = true;
    CThing* out_pthing = 0;
    long map_no = CThing::PeekPCurrentThingManager()->PeekWorldMap().GetMapNumberAt(pos);
    if (map_no > 0) {
        long def_global_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("PARTICLE_EMITTER_NORMAL");
        CThing* pthing = CThing::Create(def_global_index, 4, map_no, true);
        CTCDParticleEmitter* ptcd = 0;
        pthing->GetTCDriver(&ptcd);
        ptcd->Construct(particle_type_id, pos, false, force);
        pthing->DoActivationSequence();
        if (ptcd->PEmitter.Get() == 0 && force)
            pthing->Kill(false);
        else
            out_pthing = pthing;
    }
    return out_pthing;
}