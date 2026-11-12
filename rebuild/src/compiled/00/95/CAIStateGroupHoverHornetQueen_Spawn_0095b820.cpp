// CAIStateGroup_HoverHornetQueen::Spawn() -- play the shoot-babies action at the target, then make every live CREATURE_HORNET_PICNIC creature within 60 units the boss's helper (copying the queen's level persistence onto it); terminate when no longer runnable
// Leaf: ?Spawn@CAIStateGroup_HoverHornetQueen@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095B820, 445 bytes, plain `ret` (Ego_r.pdb name above, protected).
// Callees: CAIStateGroup_HoverHornetQueen::IsStillRunnable 0x0095A420, CAIStateGroupBase::Terminate 0x008FCF40,
//   CCreatureAction_QueenHornetShootBabies(CThingCreatureBase&, CThingCreatureBase&) 0x0095CB80 and its compiler-generated destructor
//   (~CCombatActionBase 0x0062E900), CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName
//   0x009AD410, CThing::GetPos 0x004C73D0, __ftol2 (C3DVector::ToC2DCoordI), CAIStateGroupBase::PeekThingSearchTools 0x008FD000,
//   CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature, vector<CThingAICreature*>, CThingFilter_HasDefGlobalIndex> 0x008E3EB0
//   (Ego_r.pdb instance 0x00826060), CThing::IsLevelPersistent 0x004C82A0, CThing::GetTC<CTCBoss> (expanded; CVectorMap::LowerBound
//   0x0040F020), CTCBoss::SetHelperCreature 0x007AF310, the vector's free 0x00BFEA14.
// Shape: on the shared headers (the earlier draft carried a stand-in collector body). The collector's template body is visible
//   (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY, noinline: the instantiation retail's linker folded to 0x008E3EB0), so VC7.1 keeps the
//   vector start in ebp across SetHelperCreature. The def index is a named local and the filter a temporary of the collector call:
//   the filter then shares the name string's frame slot (0x0095B8AC `mov [esp+14h],edi` after ~CCharString), as does the boss
//   pointer; a named filter object gets its own slot (frame 0x13C v 0x138).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_LEVELPERSISTENCE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_QUEENHORNETSHOOTBABIES
#define FABLE_RETAIL_CTCBOSS_SETHELPER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBoss.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#include "engine/CCreatureAction_QueenHornetShootBabies.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "stlport/fable_containers.h"

void CAIStateGroup_HoverHornetQueen::Spawn()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    AIAction_PerformActionAndWait(CCreatureAction_QueenHornetShootBabies(GetCreature(), *PTargetThing), false);
    std::vector<CThingAICreature*> helpers;
    unsigned long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_HORNET_PICNIC");
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingAICreature>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), 60),
        CThingFilter_HasDefGlobalIndex(def_index), helpers);
    bool level_persistent = false;
    if (!GetCreature().IsToKillOnLevelUnload() || GetCreature().IsLevelPersistent())
        level_persistent = true;
    CTCBoss* boss;
    GetCreature().GetTC(&boss);
    for (unsigned long i = 0; i < helpers.size(); ++i) {
        if (IS_THING_ALIVE(helpers[i])) {
            helpers[i]->SetAsLevelPersistent(level_persistent);
            boss->SetHelperCreature(*helpers[i]);
        }
    }
    if (!IsStillRunnable())
        Terminate();
}