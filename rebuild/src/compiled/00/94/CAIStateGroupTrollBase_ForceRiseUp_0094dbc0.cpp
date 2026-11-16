// CAIStateGroup_TrollBase::ForceRiseUp() -- when the troll's creature-mode manager has mode 0x1E (lying down) active, make it rise up and wait for that action
// Leaf: ?ForceRiseUp@CAIStateGroup_TrollBase@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094DBC0. Name: Ego_r.pdb 0x009720D0 ?ForceRiseUp@CAIStateGroup_TrollBase@@IAEXXZ (protected; donor order
//   after Initialise, before ThrowRocks = retail 0x0094DC40), as engine/CAIStateGroup_TrollBase.h declares it; the landed
//   CAIStateGroup_EarthTroll::Update 0x0094E7B0 calls it with that spelling.
// Callees: the creature's GetTC<CTCCreatureModeManager> (expanded: interface 0x31), CTCCreatureModeManager::IsModeActive 0x0073A8A0,
//   CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase&) 0x008BD880, CAIStateGroupBase::AIAction_PerformActionAndWait
//   0x0097DB70, ~CCombatActionBase 0x0062E900.
// NF41 (2026-11-16): was a CAIStateGroup_TrollBase_Methods stand-in with local CBaseIntelligentPointer / CAIBrain /
//   CTCCreatureModeManager / CCreatureAction_RiseUp models; now the member of the shared engine/CAIStateGroup_TrollBase.h under the
//   include chain of the landed caller 0x0094E7B0, plus engine/CTCCreatureModeManager.h and engine/CCreatureAction_RiseUp.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAISTATEGROUP_TROLLBASE
#define FABLE_RETAIL_CAISTATEGROUP_EARTHTROLL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_TROLLGROUNDPOUND_CTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_RESETCOMBO
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_TrollBase.h"
#include "engine/CAIStateGroup_EarthTroll.h"
#include "engine/CCreatureAction_TrollGroundPound.h"
#include "engine/CTCCombat.h"
#include "engine/CTCPhysicsBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CTrollBattleDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCreatureAction_RiseUp.h"

void CAIStateGroup_TrollBase::ForceRiseUp()
{
    CTCCreatureModeManager* modes;
    if (GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &modes) && modes->IsModeActive(0x1E))
        AIAction_PerformActionAndWait(CCreatureAction_RiseUp(GetCreature()), false);
}
