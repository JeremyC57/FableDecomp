// 00844bc0 CCreatureAction_GiveItemToThing::PerformAction
// Leaf: ?PerformAction@CCreatureAction_GiveItemToThing@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_GIVE_ITEM_ACTION_PASS26
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PBASEDEF
#define FABLE_RETAIL_CTHING_GETDEFGLOBALINDEX
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_NICEGIFT
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionPostDeedType.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureAction_GiveItemToThing.h"
#include "engine/CTCGift.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGiftDef.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCWife.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventReceivedItem.h"
#include "engine/ThingAlive.h"
#include "engine/GFDebugPDynamicCast.h"

void CCreatureAction_GiveItemToThing::PerformAction()
{
    if(IS_THING_ALIVE(PReceivingThing) && IS_THING_ALIVE(PItemBeingGiven)) {
        bool give_item=true;
        CTCHeroOpinionDeedLog* ptc_deedlog=0;
        const CTCGift* ptc_gift=0;
        const CTCCreatureOpinionOfHero* ptc_opinion=0;
        if(PCreature->GetTC(&ptc_deedlog) && PItemBeingGiven->PeekTC(&ptc_gift) && PReceivingThing->PeekTC(&ptc_opinion)) {
            EOpinionDeedType deed_type=OPINION_DEED_TYPE_NONE;
            if(ptc_gift->IsFriendly()) deed_type=OPINION_DEED_TYPE_RECEIVE_GIFT_FRIENDLY;
            else if(ptc_gift->IsRomantic()) deed_type=OPINION_DEED_TYPE_RECEIVE_GIFT_ROMANTIC;
            else if(ptc_gift->IsOffensive()) deed_type=OPINION_DEED_TYPE_RECEIVE_GIFT_OFFENSIVE;
            bool post_deed=true;
            CTCWife* ptc_wife=0;
            if(PCreature->IsPlayerCharacter() && PReceivingThing->GetTC(&ptc_wife)) {
                if(deed_type==OPINION_DEED_TYPE_RECEIVE_GIFT_FRIENDLY ||
                   deed_type==OPINION_DEED_TYPE_RECEIVE_GIFT_ROMANTIC)
                    post_deed=ptc_wife->ReceivedNiceGiftIsOpinionReactionHappening(PItemBeingGiven->GetDefGlobalIndex());
            }
            if(post_deed) {
                ptc_deedlog->PostDeed(deed_type,PCreature->GetPos(),PReceivingThing.Get(),
                    GFDebugPDynamicCast(PReceivingThing.Get(),(CThingAICreature*)0),OPINION_POST_DEED_TYPE_RECIPIENT,false,false,0,false,PItemBeingGiven->GetDefGlobalIndex());
            } else {
                CDefPointer<const CGiftDef> pgift_subdef(NULL_DEF);
                if(PItemBeingGiven->GetPBaseDef()->GetSubDef(&pgift_subdef) && pgift_subdef->IsWeddingRing) give_item=false;
            }
        }
        if(give_item) {
            CTCInventoryItem* ptc_inventory_item=0;
            if(PItemBeingGiven->GetTC(&ptc_inventory_item) && ptc_inventory_item->GetPInventory())
                ptc_inventory_item->RemoveFromInventory();
            PItemBeingGiven->SetInLimbo(true);
            CTCSoundPlayer* ptc_sound_player=0;
            if(PCreature->GetTC(&ptc_sound_player))
                ptc_sound_player->PlayAttachedCriteria("HERO_GIVE_ITEM",false,false,false,-1);
            CTCEntityEvents* ptc_receiver_events=0;
            PReceivingThing->GetTC(&ptc_receiver_events);
            ptc_receiver_events->AddEvent(new NEntityEvents::CEventReceivedItem(*PCreature,*PItemBeingGiven,true));
        } else {
            CTCInventoryItem* ptc_inventory_item=0;
            if(PItemBeingGiven->GetTC(&ptc_inventory_item) && ptc_inventory_item->GetPInventory())
                ptc_inventory_item->ReturnToInventory();
        }
    } else if(IS_THING_ALIVE(PItemBeingGiven)) {
        CTCInventoryItem* ptc_inventory_item=0;
        if(PItemBeingGiven->GetTC(&ptc_inventory_item) && ptc_inventory_item->GetPInventory())
            ptc_inventory_item->ReturnToInventory();
    }
}