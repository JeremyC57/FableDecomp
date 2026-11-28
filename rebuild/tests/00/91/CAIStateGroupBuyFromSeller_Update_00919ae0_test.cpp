// Executes the checked-in Update statements with deterministic engine service doubles.
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#else
#include <list>
#endif
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/EMessageEventType.h"

static int failures;
static const char* scenario;
static void check(bool result, const char* expression)
{
    if (!result) { printf("%s: %s\n", scenario, expression); ++failures; }
}
#define CHECK(expression) check((expression), #expression)
#define FABLE_RETAIL_ASSERT(expression) ((void)0)
enum ETCInterfaceType { TCI_STOCK_ITEM = 0x54 };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x=0, float y=0, float z=0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const {return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
    float GetSquaredMagnitude() const {return X*X+Y*Y+Z*Z;}
};
class CTCShopKeeper; class CTCAIScratchpad; class CTCThingOwner; class CTCStockItem;
class CTCVillage {
public:
    bool working;
    bool IsWorkTime() const { return working; }
};
class CTCVillageMember {
public:
    CTCVillage* village;
    CTCVillage* GetPVillage() const { return village; }
};
class CThing {
public:
    C3DVector pos;
    bool dead, present;
    CTCVillageMember* member;
    CTCShopKeeper* keeper;
    CTCThingOwner* owner;
    CTCStockItem* stock;
    CTCAIScratchpad* scratchpad;
    CThing() : dead(false),present(true),member(0),keeper(0),owner(0),stock(0),scratchpad(0) {}
    const C3DVector& GetPos() const {return pos;}
    bool HasTC(ETCInterfaceType type) const {CHECK(type == TCI_STOCK_ITEM); return stock != 0;}
    bool GetTC(CTCVillageMember** p) {*p=member;return member!=0;}
    bool GetTC(CTCShopKeeper** p) {*p=keeper;return keeper!=0;}
    bool GetTC(CTCThingOwner** p) {*p=owner;return owner!=0;}
    bool GetTC(CTCStockItem** p) {*p=stock;return stock!=0;}
    bool GetTC(CTCAIScratchpad** p) {*p=scratchpad;return scratchpad!=0;}
};

template<class T> class CCountedPointer {
public:
    T* value;
    T* operator->() const {return value;}
};
template<class T> class CIntelligentPointer {
    T* pointer;
public:
    CIntelligentPointer(T* value=0) : pointer(value) {}
    template<class U> CIntelligentPointer(CIntelligentPointer<U>& other) : pointer(other.Get()) {}
    T* Get() const {return pointer && pointer->present ? pointer : 0;}
    T* operator->() const {return Get();}
    bool operator==(int zero) const {CHECK(zero == 0);return Get()==0;}
};
template<class T> bool IS_THING_ALIVE(const T& p) {return p.Get() && !p->dead;}
template<class From,class To> To GFDebugPDynamicCast(From from,To) {return static_cast<To>(from);}

class CThingCreatureBase;
class CCreatureInteraction {
public:
    std::list<CThingCreatureBase*> participants;
    const std::list<CThingCreatureBase*>& GetParticipants() const {return participants;}
};
class CThingCreatureBase : public CThing {
public:
    CCountedPointer<CCreatureInteraction> interaction;
    CCountedPointer<CCreatureInteraction>& GetPCreatureInteraction() {return interaction;}
    void RemoveFromCreatureInteraction();
};
class CThingAICreature : public CThingCreatureBase {};
class CTCStockItem {
public:
    unsigned long phrase;
    unsigned long GetBuyersPhrase() {return phrase;}
};
class CTCAIScratchpad {
public:
    unsigned long GetThankingPhrase() const {return 0x91;}
    unsigned long GetIgnoringPhrase() const {return 0x72;}
};
class CTCThingOwner {
public:
    std::list<CIntelligentPointer<CThing> > objects;
    std::list<CIntelligentPointer<CThing> >& GetObjectList() {return objects;}
};
class CTCShop {
public:
    CThing parent;
    CThing& GetParentThing() {return parent;}
};
class CTCShopKeeper {
public:
    CTCShop* shop;
    CTCShop* GetShopTC() const {return shop;}
};
class CMessageEvent {
public:
    long ExtraData;
};
class CMessageEventManager {
public:
    CMessageEvent* AddMessage(EMessageEventType type,unsigned long lifespan,const C3DVector& pos,
                              CThing* origin,CThing* target,float sight,float hearing,long extra);
    void RemoveMessage(const CMessageEvent* event);
};
class CWorld {
public:
    CMessageEventManager manager;
    CMessageEventManager& GetMessageEventManager() {return manager;}
    static long GetFrame();
};
class CMainGameComponent {
public:
    static long GetConstantFPS();
};
class CAIStateGroup_BuyFromSeller {
public:
    long FrameLastBought;
    CThingAICreature& GetCreature() const;
    bool IsInWorkHours() const;
    CWorld& GetWorld() const;
    unsigned long* GetPWorldSeed() const;
    void Terminate();
    void BoughtSomething() {FrameLastBought = CWorld::GetFrame();}
    const CMessageEvent* GetSellersReply(const CThingCreatureBase* seller);
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat,C3DVector,float,bool&,EMovementType,
                                               bool,bool,bool,const CThing*,bool,float);
    EAIActionReturn AIAction_SpeakAISpeechToTarget(EAIActionRepeat,CIntelligentPointer<CThingCreatureBase>,
                                                   CIntelligentPointer<const CThing>,unsigned long,bool,long);
    EAIActionReturn AIAction_DoNothing();
    void Update();
};

struct Fixture {
    CWorld world;
    CAIStateGroup_BuyFromSeller state;
    CCreatureInteraction interaction;
    CThingAICreature buyer,other,seller;
    CTCVillage village;
    CTCVillageMember member;
    CTCShopKeeper keeper;
    CTCShop shop;
    CTCThingOwner owner;
    CThing items[4];
    CTCStockItem stock[4];
    CTCAIScratchpad scratch;
    CMessageEvent request,reply;
    long frame,fps;
    unsigned long seed,pick,range;
    bool snap;
    int replyAt,polled,moveCount,speechCount,wordCount,sleepCount,stopAfterMove,killAfterMove,stopAtPoll,killAtPoll,loseAtPoll;
    int stopAfterSpeech,killAfterSpeech,firstReact,afterRemoveExtra;
    EAIActionReturn moves[6],speeches[6],idle;
    unsigned long words[6];
    bool interrupts[6];
    char log[256]; int logLength;
    Fixture() : frame(100),fps(2),seed(0x219),pick(0),range(0),snap(true),replyAt(0),polled(0),
        moveCount(0),speechCount(0),wordCount(0),sleepCount(0),stopAfterMove(-1),killAfterMove(-1),stopAtPoll(-1),
        killAtPoll(-1),loseAtPoll(-1),stopAfterSpeech(-1),killAfterSpeech(-1),firstReact(-1),afterRemoveExtra(-1),
        idle(AI_ACTION_FINISHED),logLength(0) {
        state.FrameLastBought = -1;
        log[0]=0;
        village.working=true; member.village=&village; buyer.member=&member;
        buyer.interaction.value=&interaction; buyer.scratchpad=&scratch;
        interaction.participants.push_back(&other); interaction.participants.push_back(&buyer);
        interaction.participants.push_back(&seller);
        seller.keeper=&keeper;seller.pos=C3DVector(0,0,5);keeper.shop=&shop;shop.parent.owner=&owner;
        for (int i=0;i<4;++i) {
            stock[i].phrase=0x20+i;items[i].stock=&stock[i];
            owner.objects.push_back(&items[i]);
        }
        items[1].dead=true;items[2].stock=0;owner.objects.push_back(CIntelligentPointer<CThing>());
        for(int j=0;j<6;++j){moves[j]=AI_ACTION_FINISHED;speeches[j]=AI_ACTION_FINISHED;}
        request.ExtraData=0;reply.ExtraData=1;
    }
    void event(char c) {CHECK(logLength+1 < (int)sizeof(log));if(logLength+1<(int)sizeof(log)){log[logLength++]=c;log[logLength]=0;}}
};
static Fixture* active;
static float MIN_DIST_FROM_SELLER=1.0f;
long CWorld::GetFrame() {return active->frame;}
long CMainGameComponent::GetConstantFPS() {return active->fps;}
CWorld& CAIStateGroup_BuyFromSeller::GetWorld() const {return active->world;}
CThingAICreature& CAIStateGroup_BuyFromSeller::GetCreature() const {return active->buyer;}
unsigned long* CAIStateGroup_BuyFromSeller::GetPWorldSeed() const {return &active->seed;}
unsigned long GFRandom(unsigned long range,unsigned long* seed) {
    CHECK(seed==&active->seed);CHECK(active->pick<range);active->range=range;return active->pick;
}
bool CAIStateGroup_BuyFromSeller::IsInWorkHours() const {
    CTCVillageMember* member;
    if (GetCreature().GetTC(&member)) {
        CTCVillage* village = member->GetPVillage();
        if (village && village->IsWorkTime())
            return true;
    }
    return false;
}
void CAIStateGroup_BuyFromSeller::Terminate(){active->event('T');}
void CThingCreatureBase::RemoveFromCreatureInteraction(){CHECK(this==&active->buyer);active->event('X');}
CMessageEvent* CMessageEventManager::AddMessage(EMessageEventType type,unsigned long lifespan,const C3DVector& pos,
    CThing* origin,CThing* target,float sight,float hearing,long extra) {
    CHECK(this==&active->world.manager && type==MESSAGE_EVENT_BUY_ITEM && lifespan==1000);
    CHECK(pos.X==0 && pos.Y==0 && pos.Z==0 && sight==0 && hearing==0 && extra==0);
    CHECK(origin==&active->buyer && target==&active->seller);active->event('A');return &active->request;
}
void CMessageEventManager::RemoveMessage(const CMessageEvent* event) {
    CHECK(this==&active->world.manager && (event==&active->request || event==&active->reply));
    active->event(event==&active->request?'q':'r');
    if(event==&active->reply && active->afterRemoveExtra>=0)active->reply.ExtraData=active->afterRemoveExtra;
}
const CMessageEvent* CAIStateGroup_BuyFromSeller::GetSellersReply(const CThingCreatureBase* seller) {
    Fixture& f=*active;CHECK(seller==&f.seller);const int i=f.polled++;
    if(i==f.stopAtPoll)f.village.working=false;
    if(i==f.killAtPoll)f.seller.dead=true;
    if(i==f.loseAtPoll)f.seller.present=false;
    if(i==f.replyAt)return &f.reply;
    f.frame+=10;return 0;
}
EAIActionReturn CAIStateGroup_BuyFromSeller::AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector pos,
    float radius,bool& nav,EMovementType movement,bool a,bool b,bool c,const CThing* ignore,bool d,float e) {
    Fixture& f=*active;
    CHECK(repeat==ACTION_REPEAT_CALLBACK && movement==AI_MOVEMENT_WALKING && radius==MIN_DIST_FROM_SELLER);
    CHECK(a && !b && c && !ignore && d && e==0.0f);
    CHECK(pos.X==f.seller.pos.X && pos.Y==f.seller.pos.Y && pos.Z==f.seller.pos.Z);
    f.event('M');nav=true;CHECK(f.moveCount<6);
    EAIActionReturn ret=f.moveCount<6?f.moves[f.moveCount++]:AI_ACTION_TERMINATE_PROCESS;
    if(f.village.working && f.moveCount==f.stopAfterMove)f.village.working=false;
    if(f.moveCount==f.killAfterMove)f.seller.dead=true;
    if(f.snap && ret==AI_ACTION_FINISHED)f.buyer.pos=pos;
    return ret;
}
EAIActionReturn CAIStateGroup_BuyFromSeller::AIAction_DoNothing() {active->event('N');++active->sleepCount;return active->idle;}
EAIActionReturn CAIStateGroup_BuyFromSeller::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat,CIntelligentPointer<CThingCreatureBase> speaker,
        CIntelligentPointer<const CThing> target,unsigned long word,bool interrupt,long priority) {
    Fixture& f=*active;CHECK(repeat==ACTION_REPEAT_CALLBACK && speaker.Get()==&f.buyer && target.Get()==&f.seller && priority==0x93);
    CHECK(f.speechCount<6);if(f.speechCount>=6)return AI_ACTION_TERMINATE_PROCESS;
    const int at=f.speechCount++;f.event('S');f.words[at]=word;f.interrupts[at]=interrupt;CHECK(interrupt==(at==0));
    if(f.speechCount==f.stopAfterSpeech)f.village.working=false;
    if(f.speechCount==f.killAfterSpeech)f.seller.dead=true;
    return f.speeches[at];
}

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
static void run(Fixture& f,const char* name,const char* expected) {
    active=&f;scenario=name;f.state.Update();
    if(strcmp(f.log,expected)){printf("%s: expected %s, observed %s\n",name,expected,f.log);++failures;}
    if(strchr(expected,'X'))CHECK(f.state.FrameLastBought==f.frame);
    else CHECK(f.state.FrameLastBought==-1);
}
int main() {
    {Fixture f;f.interaction.participants.clear();run(f,"empty participants","T");}
    {Fixture f;f.interaction.participants.clear();f.interaction.participants.push_back(&f.buyer);run(f,"only buyer","T");}
    {Fixture f;f.seller.pos=C3DVector(0,0,1);run(f,"at proximity boundary","T");}
    {Fixture f;MIN_DIST_FROM_SELLER=6;run(f,"mutable proximity","T");MIN_DIST_FROM_SELLER=1;}
    {Fixture f;f.buyer.member=0;run(f,"no village membership","T");}
    {Fixture f;f.member.village=0;run(f,"no village","T");}
    {Fixture f;f.village.working=false;run(f,"first move off hours","T");}
    {Fixture f;f.seller.dead=true;run(f,"dead before move","T");}
    {Fixture f;f.moves[0]=AI_ACTION_IN_PROGRESS;f.stopAfterMove=1;run(f,"work ends while moving","MT");}
    {Fixture f;f.moves[0]=AI_ACTION_FAILED;run(f,"initial move failed","MT");}
    {Fixture f;f.seller.keeper=0;run(f,"no shopkeeper","MT");}
    {Fixture f;f.keeper.shop=0;run(f,"no shop","MT");}
    {Fixture f;f.shop.parent.owner=0;run(f,"no owner","MT");}
    {Fixture f;f.owner.objects.clear();run(f,"empty inventory","MT");}
    {Fixture f;f.items[0].dead=true;f.items[3].present=false;run(f,"all filtered","MT");}
    {Fixture f;f.moves[0]=AI_ACTION_IN_PROGRESS;f.pick=1;f.replyAt=1;f.speeches[1]=AI_ACTION_IN_PROGRESS;
     run(f,"accept second live stock item","MMASNrSSXT");CHECK(f.range==2 && f.words[0]==0x23 && f.words[1]==0x91 && f.words[2]==0x91);}
    {Fixture f;f.reply.ExtraData=0;run(f,"refusal","MASrSXT");CHECK(f.words[0]==0x20 && f.words[1]==0x72);}
    {Fixture f;f.afterRemoveExtra=0;run(f,"read response after removal","MASrSXT");CHECK(f.words[1]==0x72);}
    {Fixture f;f.replyAt=-1;run(f,"strict frame timeout","MASNNNNqSXT");CHECK(f.frame==150 && f.sleepCount==4 && f.words[1]==0x72);}
    {Fixture f;f.replyAt=-1;f.fps=1;run(f,"constant FPS controls timeout","MASNNqSXT");CHECK(f.sleepCount==2);}
    {Fixture f;f.replyAt=-1;f.stopAtPoll=0;run(f,"shop closes while waiting","MASqT");}
    {Fixture f;f.replyAt=-1;f.killAtPoll=0;run(f,"seller dies while waiting","MASqT");}
    {Fixture f;f.replyAt=-1;f.loseAtPoll=0;run(f,"seller pointer invalidated","MASqT");}
    {Fixture f;f.replyAt=-1;f.idle=AI_ACTION_TERMINATE_PROCESS;run(f,"idle termination","MASNq");}
    {Fixture f;f.replyAt=1;f.snap=false;f.moves[1]=AI_ACTION_TERMINATE_PROCESS;run(f,"return walk terminated","MASMqT");}
    {Fixture f;f.replyAt=1;f.snap=false;f.moves[1]=AI_ACTION_IN_PROGRESS;f.stopAfterMove=2;run(f,"shop closes on return walk","MASMT");}
    {Fixture f;f.replyAt=1;f.snap=false;f.moves[1]=AI_ACTION_IN_PROGRESS;f.killAfterMove=2;run(f,"seller dies on return walk","MASMT");}
    {Fixture f;f.replyAt=1;f.snap=false;f.moves[1]=AI_ACTION_IN_PROGRESS;f.moves[2]=AI_ACTION_FAILED;
     run(f,"return walk resumes polling after failure","MASMMrSXT");}
    {Fixture f;f.stopAfterSpeech=1;run(f,"shop closed before reaction","MASrT");}
    {Fixture f;f.killAfterSpeech=1;run(f,"dead before reaction","MASrT");}
    {Fixture f;f.speeches[1]=AI_ACTION_IN_PROGRESS;f.stopAfterSpeech=2;run(f,"shop closes mid-reaction","MASrST");}
    {Fixture f;f.speeches[0]=AI_ACTION_TERMINATE_PROCESS;f.speeches[1]=AI_ACTION_FAILED;run(f,"speech results ignored on these paths","MASrSXT");}
    if(failures)return 1;
    puts("00919ae0_TEST PASS");return 0;
}