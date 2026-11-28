// Leaf: ?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ
// Flags: /O2 /Oy /W3 /d2inlT64
// Inline-budget: T64 preserves the three hot inlined LowerBound calls and the final out-of-line call. The 004BA4E0 push_back owner expands insert.
// Ego_d names the mutable float MIN_DIST_FROM_SELLER. FableWin uses it in the distance checks and both moves.
#define FABLE_RETAIL_BUY_FROM_SELLER_LIST_INSERT_PASS30
#define FABLE_RETAIL_BUY_FROM_SELLER_SIZE_OWNERS_PASS30
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CREATUREBASE_SIZE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_INTERACTION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_BUY_FROM_SELLER_UPDATE_PASS30
#define FABLE_RETAIL_CAISTATEGROUP_WORKBASE_ISINWORKHOURS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ISWORKTIME
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCSTOCKITEM
#define FABLE_RETAIL_CTCAISCRATCHPAD_GETTHANKINGPHRASE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE

#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "engine/CIntelligentPointer_CreatureSizes.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CCreatureInteraction.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTCStockItem.h"
#include "engine/CTCThingOwner.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CAIStateGroup_BuyFromSeller.h"
#include "engine/ThingAlive.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/GFRandom.h"
#include "fable_assert.h"

extern float MIN_DIST_FROM_SELLER;

void CAIStateGroup_BuyFromSeller::Update()
{
    CCountedPointer<CCreatureInteraction>& interaction = GetCreature().GetPCreatureInteraction();
    const std::list<CThingCreatureBase*>& participants = interaction->GetParticipants();
    std::list<CThingCreatureBase*>::const_iterator participant;
    CIntelligentPointer<CThingAICreature> seller;
    for (participant = participants.begin(); participant != participants.end(); participant++) {
        if (*participant != &GetCreature())
            seller = GFDebugPDynamicCast(*participant, (CThingAICreature*)0);
    }
    EAIActionReturn ret = AI_ACTION_NULL;
    if (seller == 0) {
        Terminate();
        return;
    }
    if ((seller->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude() > MIN_DIST_FROM_SELLER * MIN_DIST_FROM_SELLER) {
        do {
            if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
                Terminate();
                return;
            }
            bool nav_route_calculated;
            ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, seller->GetPos(), MIN_DIST_FROM_SELLER,
                nav_route_calculated, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS);
    }
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    CTCShopKeeper* tc_shopkeeper;
    if (!seller->GetTC(&tc_shopkeeper)) {
        Terminate();
        return;
    }
    CTCShop* tc_shop = tc_shopkeeper->GetShopTC();
    if (!tc_shop) {
        Terminate();
        return;
    }

    std::list<CIntelligentPointer<CThing> > objects_for_sale;
    CTCThingOwner* owner;
    if (tc_shop->GetParentThing().GetTC(&owner)) {
        const std::list<CIntelligentPointer<CThing> >& objects = owner->GetObjectList();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator object_it = objects.begin();
             object_it != objects.end(); object_it++) {
            if (!IS_THING_ALIVE(*object_it))
                continue;
            if ((*object_it)->HasTC(TCI_STOCK_ITEM))
                objects_for_sale.push_back(*object_it);
        }
    }
    FABLE_RETAIL_ASSERT(objects_for_sale.size() != 0);
    if (objects_for_sale.size() == 0) {
        Terminate();
        return;
    }
    long index = GFRandom(objects_for_sale.size(), GetPWorldSeed());
    CThing* chosen_object = 0;
    for (std::list<CIntelligentPointer<CThing> >::iterator object_it = objects_for_sale.begin();
         object_it != objects_for_sale.end(); object_it++) {
        if (index == 0) {
            chosen_object = (*object_it).Get();
            break;
        }
        index--;
    }
    CTCStockItem* stock_item;
    if (!chosen_object->GetTC(&stock_item)) { FABLE_RETAIL_ASSERT(false); }
    unsigned long some_words = stock_item->GetBuyersPhrase();

    ret = AI_ACTION_NULL;
    CMessageEvent* message = GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_BUY_ITEM, 1000,
        C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), seller.Get(), 0.0f, 0.0f, 0);
    AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_CALLBACK, &GetCreature(), seller, some_words, true, 0x93);
    long time_to_wait_until = CWorld::GetFrame() + 20 * CMainGameComponent::GetConstantFPS();
    const CMessageEvent* reply;
    while ((reply = GetSellersReply(seller.Get())) == 0) {
        if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
            GetWorld().GetMessageEventManager().RemoveMessage(message);
            Terminate();
            return;
        }
        if (time_to_wait_until < CWorld::GetFrame()) {
            GetWorld().GetMessageEventManager().RemoveMessage(message);
            break;
        }
        if ((seller->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude() > MIN_DIST_FROM_SELLER * MIN_DIST_FROM_SELLER) {
            do {
                if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
                    Terminate();
                    return;
                }
                bool nav_route_calculated;
                ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, seller->GetPos(), MIN_DIST_FROM_SELLER,
                    nav_route_calculated, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
            } while (ret == AI_ACTION_IN_PROGRESS);
            if (ret == AI_ACTION_TERMINATE_PROCESS) {
                GetWorld().GetMessageEventManager().RemoveMessage(message);
                Terminate();
                return;
            }
        } else if (AIAction_DoNothing() == AI_ACTION_TERMINATE_PROCESS) {
            GetWorld().GetMessageEventManager().RemoveMessage(message);
            return;
        }
    }
    bool answer;
    if (reply) {
        GetWorld().GetMessageEventManager().RemoveMessage(reply);
        answer = reply->ExtraData != 0;
    } else
        answer = false;
    CTCAIScratchpad* ai_scratchpad;
    if (!GetCreature().GetTC(&ai_scratchpad)) { FABLE_RETAIL_ASSERT(false); }
    unsigned long some_other_words;
    if (answer)
        some_other_words = ai_scratchpad->GetThankingPhrase();
    else
        some_other_words = ai_scratchpad->GetIgnoringPhrase();
    ret = AI_ACTION_NULL;
    do {
        if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
            Terminate();
            return;
        }
        ret = AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_CALLBACK, &GetCreature(), seller, some_other_words, false, 0x93);
    } while (ret == AI_ACTION_IN_PROGRESS);
    BoughtSomething();
    GetCreature().RemoveFromCreatureInteraction();
    Terminate();
}