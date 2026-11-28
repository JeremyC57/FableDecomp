// Leaf: ?GetSellersReply@CAIStateGroup_BuyFromSeller@@AAEPBVCMessageEvent@@PBVCThingCreatureBase@@@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_BUY_FROM_SELLER_UPDATE_PASS30
#define FABLE_RETAIL_CMESSAGEEVENT

#define FABLE_RETAIL_CAISTATEGROUPBASE_WAITMESSAGE
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
class CMessageEvent;
#include "engine/EAIAction.h"
#include "engine/EMessageEventType.h"
#include "engine/EMovementType.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CMessageEvent.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CAIStateGroup_BuyFromSeller.h"

#include "engine/CFilter_Bind3.h"
#include "engine/CMessageEventFilters.h"

const CMessageEvent* CAIStateGroup_BuyFromSeller::GetSellersReply(const CThingCreatureBase* seller)
{
    const CMessageEvent* pevent;
    if (AIAction_WaitForMessageCallback(ACTION_REPEAT_CALLBACK,
        GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO),
            CMessageEventFilter_IsOriginatedByThing(*seller), CMessageEventFilter_IsTargettedAtThing(GetCreature())),
        &pevent) == AI_ACTION_FINISHED)
        return pevent;
    return 0;
}
