// 0090cc40 CAIStateGroup_MinionGossip::IsRunnable
// Leaf: ?IsRunnable@CAIStateGroup_MinionGossip@@MAE_NXZ
// Flags:
#define FABLE_RETAIL_MINION_GOSSIP_PREDICATES_PASS29
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_IN2DRANGE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
#define FABLE_RETAIL_CTHINGFILTER_ISVALIDGOSSIPEE_ORIGINAL_MEMBERS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/EMessageEventType.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCGossip.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionGossip.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsValidGossipee.h"
#include "engine/CThingFilter_IsNavLineOfSightToThing.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"
#include "engine/NCreatureMode.h"
#include "engine/CFilter_Bind3.h"

// The final lookup's original 140-byte specialization is visible in this unit.
// The two gossipee overloads have independently identified out-of-line owners.
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing,
    CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> > OwnGossipFilter;
template<> inline const CMessageEvent* CMessageEventManager::FindMostRecentMessage<OwnGossipFilter>(
    const OwnGossipFilter& filter_func) const
{
    for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
        const CMessageEvent& event = *it;
        if (filter_func(event))
            return &event;
    }
    return 0;
}

bool CAIStateGroup_MinionGossip::IsRunnable()
{
    CTCCreatureModeManager* ptc_mode_manager;
    GetCreature().GetTC(&ptc_mode_manager);
    unsigned long mode = ptc_mode_manager->GetCurrentMode().GetType();
    if (mode == NCreatureMode::MODE_CARRYING_GENERIC_BOX || mode == NCreatureMode::MODE_CARRYING_GOODS_STAND ||
        mode == NCreatureMode::MODE_CARRYING_PLATE_STAND || mode == NCreatureMode::MODE_HOLD_JUG)
        return false;
    CTCGossip* gossip;
    if (!GetCreature().GetTC(&gossip))
        return false;
    long time = (CWorld::GetFrame() - FrameOfLastGossip) / CMainGameComponent::GetConstantFPS();
    // Inferred: the line-of-sight temporary must survive through each lookup.
    if (time > GetTimeToRespond()) {
        const CMessageEvent* event;
        if (UseLineOfSight()) {
            CThingFilter_IsNavLineOfSightToThing line_of_sight(GetCreature());
            event = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind4(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f)),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>(line_of_sight)));
        } else
            event = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f))));
        if (event) return true;
    }
    const CMessageEvent* event_request;
    if (UseLineOfSight()) {
        CThingFilter_IsNavLineOfSightToThing line_of_sight(GetCreature());
        event_request = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind4(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f)),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>(line_of_sight)));
    } else
        event_request = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f))));
    if (time > GetTimeToInitiate() && event_request) {
        GetWorld().GetMessageEventManager().RemoveMessage(event_request);
        return true;
    }
    if (!GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(
            CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST),
            CMessageEventFilter_IsOriginatedByThing(GetCreature()),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f))))) {
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST,
            CMainGameComponent::GetConstantFPS()*4, GetCreature().PeekPos(), &GetCreature(), 0, 16.0f, 16.0f, 0);
    }
    return false;
}