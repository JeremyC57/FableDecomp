// CAIStateGroup_MinionReactToCorpse::Update()
// Leaf: ?Update@CAIStateGroup_MinionReactToCorpse@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGDEADCREATURE_KILLER_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_ORIGINAL_PASS26
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD
#define FABLE_RETAIL_CAISTATEGROUPBASE_CAIFILTERBASE_RESTORE_PROTECTED
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_DISTANCE_PASS26
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMessageEventDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingDeadCreature.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CTCDiscovery.h"
#include "engine/CTCTalk.h"
#include "engine/CAIStateGroup_MinionReactToCorpse.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_OrderSecuritySweep.h"
#include "engine/ThingAlive.h"

// Inline-budget: native calls the complete list destructor at 0x00936910.
// The visible STLport body preserves the lifetime and alias analysis of the original TU.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
std::list<CThingDeadCreature*>::~list()
{
    this->clear();
    this->_M_node.deallocate(this->_M_node._M_data, 1);
}

const float DISTANCE_FROM_CORPSE_TO_NOTICE_IT = 5.0f;
const float PROXIMITY_TO_LOOK_AT_BODY = 2.0f;

void CAIStateGroup_MinionReactToCorpse::Update()
{
    bool player_did_it = false;
    if (!IS_THING_ALIVE(PDeadThing)) {
        Terminate();
        return;
    }

    bool nav_route_calculated = false;
    EAIActionReturn ret = AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PDeadThing,
        PROXIMITY_TO_LOOK_AT_BODY, nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PDeadThing)) {
        Terminate();
        return;
    }

    CTCDiscovery* tc_discovery;
    if (IS_THING_ALIVE(PDeadThing) && PDeadThing->GetTC(&tc_discovery) && !tc_discovery->IsDiscovered()) {
        tc_discovery->SetDiscovered(true, &GetCreature());
        if (PDeadThing->GetKiller() && PDeadThing->GetKiller()->IsPlayerCharacter())
            player_did_it = true;
    }

    std::list<CThingDeadCreature*> dead_things;
    C2DBoxI area(PDeadThing->PeekPos().ToC2DCoordI(), (long)DISTANCE_FROM_CORPSE_TO_NOTICE_IT);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingDeadCreature>(area, CThingFilter_None(), dead_things);
    for (std::list<CThingDeadCreature*>::iterator it = dead_things.begin(); it != dead_things.end(); ++it) {
        CThingDeadCreature* dead_it = *it;
        if (IS_THING_ALIVE(dead_it) && !dead_it->GetTC(&tc_discovery) && !tc_discovery->IsDiscovered()) {
            if (GFGet2DDistanceSquared(C2DVector(PDeadThing->PeekPos()), C2DVector(dead_it->PeekPos())) <=
                    GFSqr(DISTANCE_FROM_CORPSE_TO_NOTICE_IT)) {
                tc_discovery->SetDiscovered(true, &GetCreature());
                if (dead_it->GetKiller() && dead_it->GetKiller()->IsPlayerCharacter())
                    player_did_it = true;
            }
        }
    }

    CTCTalk* ptc_lipsync;
    GetCreature().GetTC(&ptc_lipsync);
    if (GetCreature().GetProperty() == 3) {
        if (player_did_it) {
            if (AIAction_PerformActionAndWait(CCreatureAction_OrderSecuritySweep(GetCreature()), false) != AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
        }
    }
    else {
        CDefPointer<const CMessageEventDef> pdef;
        if (PeekDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_FOUND_CORPSE", &pdef)) {
            AIAction_FindAGuardAndGiveMessage(pdef, PDeadThing->PeekPos(), 0);
        }
    }
}