#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionPostDeedType.h"

static void require(bool value) { if(!value) abort(); }
class CThing;
class CThingAICreature;
struct C3DVector { float X,Y,Z; };
static int sequence[12], used;
static void trace(int step) { require(used<12); sequence[used++]=step; }
struct CTCGift {
    int flags;
    bool IsFriendly() const { return (flags&1)!=0; }
    bool IsRomantic() const { return (flags&2)!=0; }
    bool IsOffensive() const { return (flags&4)!=0; }
};
struct CTCCreatureOpinionOfHero {};
struct CTCWife {
    bool reacts;
    bool ReceivedNiceGiftIsOpinionReactionHappening(long id) { require(id==415); trace(1); return reacts; }
};
struct CTCInventoryItem {
    void* owner;
    void* GetPInventory() const { return owner; }
    void RemoveFromInventory() { require(owner!=0); trace(3); }
    void ReturnToInventory() { require(owner!=0); trace(7); }
};
struct CTCSoundPlayer {
    void PlayAttachedCriteria(const char* name,bool a,bool b,bool c,long n) {
        require(!strcmp(name,"HERO_GIVE_ITEM") && !a && !b && !c && n==-1); trace(5);
    }
};
static CThing* expectedGiver;
static CThing* expectedReceiver;
static CThing* expectedItem;
static const C3DVector* expectedPosition;
static EOpinionDeedType observedDeed;
struct CTCHeroOpinionDeedLog {
    void PostDeed(EOpinionDeedType kind,const C3DVector& pos,const CThing* target,CThingAICreature* creature,
                  EOpinionPostDeedType type,bool a,bool b,void* faction,bool c,long objectIndex) {
        require(target==expectedReceiver && (void*)creature==(void*)expectedReceiver && &pos==expectedPosition);
        require(type==OPINION_POST_DEED_TYPE_RECIPIENT && !a && !b && !faction && !c && objectIndex==415);
        observedDeed=kind; trace(2);
    }
};
struct CGiftDef { bool IsWeddingRing; };
enum ENullDef { NULL_DEF };
template <class T> struct CDefPointer {
    T* p;
    explicit CDefPointer(ENullDef) : p(0) {}
    T* operator->() const { require(p!=0); return p; }
};
struct CThingBaseDef {
    const CGiftDef* gift;
    bool GetSubDef(CDefPointer<const CGiftDef>* dest) const { dest->p=gift; return gift!=0; }
};
namespace NEntityEvents {
    struct CEventReceivedItem {
        CEventReceivedItem(CThing& giver, CThing& item, bool kill) {
            require(&giver==expectedGiver && &item==expectedItem && kill); trace(6);
        }
    };
}
struct CTCEntityEvents {
    void AddEvent(NEntityEvents::CEventReceivedItem* e) { require(e!=0); delete e; trace(8); }
};
class CThing {
public:
    bool dead,player;
    C3DVector position;
    CThingBaseDef def;
    CTCHeroOpinionDeedLog* deedlog;
    CTCGift* gift;
    CTCCreatureOpinionOfHero* opinion;
    CTCWife* wife;
    CTCInventoryItem* inventory;
    CTCSoundPlayer* sound;
    CTCEntityEvents* events;
    CThing():dead(false),player(false),deedlog(0),gift(0),opinion(0),wife(0),inventory(0),sound(0),events(0){
        def.gift=0;position.X=1;position.Y=2;position.Z=3;
    }
    bool IsPlayerCharacter() const { return player; }
    unsigned GetDefGlobalIndex() const { require(this==expectedItem);return 415; }
    const CThingBaseDef* GetPBaseDef() const { return &def; }
    const C3DVector& GetPos() const { return position; }
    void SetInLimbo(bool flag) { require(flag && this==expectedItem); trace(4); }
    bool GetTC(CTCHeroOpinionDeedLog** out) { *out=deedlog;return deedlog!=0; }
    bool GetTC(CTCInventoryItem** out) { *out=inventory;return inventory!=0; }
    bool GetTC(CTCEntityEvents** out) { *out=events;return events!=0; }
    bool GetTC(CTCSoundPlayer** out) { *out=sound;return sound!=0; }
    bool GetTC(CTCWife** out) { *out=wife;return wife!=0; }
    bool PeekTC(const CTCGift** out) const { *out=gift;return gift!=0; }
    bool PeekTC(const CTCCreatureOpinionOfHero** out) const { *out=opinion;return opinion!=0; }
};
class CThingAICreature : public CThing {};
template<class T> class CIntelligentPointer {
public:
    T* pointer;
    T* Get() const { return pointer; }
    T* operator->() const { require(pointer!=0);return pointer; }
    T& operator*() const { require(pointer!=0);return *pointer; }
};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() && !p->dead; }
static CThingAICreature* GFDebugPDynamicCast(CThing* p,CThingAICreature*) { return static_cast<CThingAICreature*>(p); }
class CCreatureAction_GiveItemToThing {
public:
    CIntelligentPointer<CThing> PReceivingThing, PItemBeingGiven, PCreature;
    void PerformAction();
};
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

static void run(int receiverState,int itemState,int giftFlags,int flags)
{
    CThing giver,item;
    CThingAICreature receiver;
    CTCGift gift; gift.flags=giftFlags;
    CTCWife wife; wife.reacts=(flags&16)!=0;
    CTCInventoryItem inv; inv.owner=(flags&64)?&giver:0;
    CTCCreatureOpinionOfHero opinion;
    CTCHeroOpinionDeedLog log;
    CTCSoundPlayer sound;
    CTCEntityEvents events;
    CGiftDef def;def.IsWeddingRing=(flags&256)!=0;
    giver.deedlog=(flags&1)?&log:0;
    receiver.opinion=(flags&2)?&opinion:0;
    giver.player=(flags&4)!=0;
    receiver.wife=(flags&8)?&wife:0;
    item.gift=giftFlags<0?0:&gift;
    item.inventory=(flags&32)?&inv:0;
    giver.sound=(flags&128)?&sound:0;
    item.def.gift=(flags&512)?&def:0;
    receiver.events=&events;
    receiver.dead=receiverState==1;
    item.dead=itemState==1;
    CCreatureAction_GiveItemToThing row;
    row.PCreature.pointer=expectedGiver=&giver;
    row.PReceivingThing.pointer=expectedReceiver=receiverState?&receiver:0;
    row.PItemBeingGiven.pointer=expectedItem=itemState?&item:0;
    expectedPosition=&giver.position;
    used=0;observedDeed=OPINION_DEED_TYPE_NONE;
    row.PerformAction();
    int expected[9],count=0;
    const bool canGive=receiverState==2 && itemState==2;
    const bool hasInventory=(flags&96)==96;
    const bool examine=canGive && (flags&3)==3 && giftFlags>=0;
    const bool queryWife=examine && (flags&12)==12 && (giftFlags&3)!=0;
    const bool post=examine && (!queryWife || wife.reacts);
    const bool rejected=queryWife && !wife.reacts && (flags&768)==768;
    if(queryWife) expected[count++]=1;
    if(post) {
        expected[count++]=2;
        EOpinionDeedType expectedDeed=OPINION_DEED_TYPE_NONE;
        if(giftFlags&1) expectedDeed=OPINION_DEED_TYPE_RECEIVE_GIFT_FRIENDLY;
        else if(giftFlags&2) expectedDeed=OPINION_DEED_TYPE_RECEIVE_GIFT_ROMANTIC;
        else if(giftFlags&4) expectedDeed=OPINION_DEED_TYPE_RECEIVE_GIFT_OFFENSIVE;
        require(expectedDeed==observedDeed);
    }
    if(canGive && !rejected) {
        if(hasInventory) expected[count++]=3;
        expected[count++]=4;
        if(flags&128) expected[count++]=5;
        expected[count++]=6; expected[count++]=8;
    } else if(itemState==2 && hasInventory) expected[count++]=7;
    require(used==count);
    for(int i=0;i<count;++i)require(expected[i]==sequence[i]);
}
int main()
{
    for(int receiver=0;receiver<3;++receiver) for(int item=0;item<3;++item)
    for(int gift=-1;gift<8;++gift) for(int flags=0;flags<1024;++flags)
        run(receiver,item,gift,flags);
    puts("00844bc0_TEST PASS"); return 0;
}