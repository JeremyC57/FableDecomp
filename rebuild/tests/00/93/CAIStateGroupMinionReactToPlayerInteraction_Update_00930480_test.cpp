#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#include "engine/C3DVector.h"
#include "engine/CFilter_Bind2.h"
unsigned long GFastOneOverSqrtTable[128]={0};
static void check(bool b){if(!b)exit(3);}
enum {TCI_SHOP_KEEPER=144,TCI_WIFE=215,INTERACTION_REACTION_TRADER=3};
struct CMessageEvent { EMessageEventType type; const void* target;long ExtraData; };
struct CMessageEventFilter_IsOfType {
    EMessageEventType t; CMessageEventFilter_IsOfType(EMessageEventType v):t(v){}
    bool operator()(const CMessageEvent& e)const{return t==e.type;}
};
struct CMessageEventFilter_IsTargettedAtThing {
    const void* target; template<class T> CMessageEventFilter_IsTargettedAtThing(const T& c):target(&c){}
    bool operator()(const CMessageEvent& e)const{return e.target==target;}
};
struct CMessageEventManager {
    std::list<CMessageEvent> messages;
    template<class T> const CMessageEvent* FindMostRecentMessage(const T& t)const{
        for(std::list<CMessageEvent>::const_iterator i=messages.begin();i!=messages.end();++i)if(t(*i))return &*i;
        return 0;
    }
    void RemoveMessage(const CMessageEvent* m){for(std::list<CMessageEvent>::iterator i=messages.begin();i!=messages.end();++i)if(m==&*i){messages.erase(i);return;}check(false);}
};
struct CWorld {
    bool loading; static long frame; CMessageEventManager events;
    bool IsPerformingRegionLoad()const{return loading;}
    const CMessageEventManager& GetMessageEventManager()const{return events;}
    CMessageEventManager& GetMessageEventManager(){return events;} static long GetFrame(){return frame++;}
}; long CWorld::frame;
struct CMainGameComponent {int font;static CMainGameComponent* Get(){static CMainGameComponent c;return &c;} int PeekCutSceneMainFont()const{return font;} static long GetConstantFPS(){return 2;}};
struct CPlayerGui {
    int subtitles;bool busy;
    static CPlayerGui& Get(){static CPlayerGui g;return g;}
    CPlayerGui& GetSubtitleRenderer(){return *this;}bool IsDisplayingText()const{return busy;}
    void SetText(int font,const char* text,unsigned long flags,float time,bool click,bool pause){check(font==11&&!strcmp(text,"TEXT_GUI_TRADE_START_TRADING")&&flags==4&&time==0&&!click&&pause);++subtitles;}
};
struct CTCBoastUI {bool active;bool IsActive()const{return active;}};
struct CTCTalk {int ticks;bool IsTalking(){return ticks-->0;}};
struct CTCWife {int greeted;void OnGreet(){++greeted;}};
struct CTCCreatureOpinionOfHero {int interactions;void SetHasInteracted(){++interactions;}};
struct CTCCreatureNavigation {
    float left,right;
    void CalculateNavigableWidthOfLine(const C3DVector& a,const C3DVector& b,float& l,float& r){
        check(a.X==4 && b.X==2 && a.Y==4 && b.Y==4);l=left;r=right;
    }
};
struct CThingCreatureBase {
    C3DVector position; bool freeToFinish; int finished;
    const C3DVector& GetPos()const{return position;}const C3DVector& PeekPos()const{return position;}
    bool IsFreeToFinishAction()const{return freeToFinish;}void FinishCurrentAction(){check(freeToFinish);++finished;}
    static float GetPersonalSpaceRadius(){return 2.0f;}
};
struct CThingPlayerCreature : CThingCreatureBase {
    bool alive;const CTCBoastUI* boast;
    bool PeekTC(const CTCBoastUI** out)const{*out=boast;return boast!=0;}
};
struct CThingAICreature : CThingCreatureBase {
    bool shop; CTCWife* wife; CTCCreatureNavigation* nav; CTCTalk* talk; CTCCreatureOpinionOfHero* opinion;
    bool HasTC(int n){check(n==TCI_SHOP_KEEPER||n==TCI_WIFE);return n==TCI_WIFE?wife!=0:shop;}
    bool GetTC(CTCWife** p){*p=wife;return wife!=0;}bool GetTC(CTCTalk** p){*p=talk;return talk!=0;}
    bool GetTC(CTCCreatureNavigation** p){*p=nav;return nav!=0;}bool GetTC(CTCCreatureOpinionOfHero** p){*p=opinion;return opinion!=0;}
};
struct CPlayer {CThingPlayerCreature* hero;CThingPlayerCreature* GetPPlayerCharacter()const{return hero;}const CPlayer& GetMainPlayer()const{return *this;}};
template<class T> struct CIntelligentPointer { T* p; CIntelligentPointer(T* x):p(x){}T* Get()const{return p;} T& operator*()const{return *p;} T* operator->()const{return p;} };
static bool IS_THING_ALIVE(const CIntelligentPointer<CThingPlayerCreature>& p){return p.Get()&&p->alive;}
struct CAIStateGroup_MinionReactToPlayerInteraction {
    int ReactionType,moveMode,lookMode,confirmationCalls,moves,looks,idles,ends,tests;
    bool WaitingForConfirmation,DidTraderInteraction,far,nearFlag,confirmClears,idleOK,interactOK;
    C3DVector PositionPlayerWasWhenWaitingForConfirmation;
    CThingAICreature creature;CWorld world;CPlayer players;
    CWorld& GetWorld(){return world;}const CWorld& PeekWorld()const{return world;}
    const CPlayer& PeekPlayerManager()const{return players;}CThingAICreature& GetCreature(){return creature;}
    const CThingAICreature& GetCreature()const{return creature;}
    void Terminate(){++ends;}
    void UpdateWaitingForConfirmation(CIntelligentPointer<CThingPlayerCreature>& p){check(p.Get()==players.hero);++confirmationCalls;if(confirmClears)WaitingForConfirmation=false;}
    bool IsTooFarFromThePlayer(const CThingPlayerCreature& p){check(&p==players.hero);return far;}
    bool IsTooNearToThePlayer(const CThingPlayerCreature& p){check(&p==players.hero);return nearFlag;}
    bool TestOrDoInteraction(bool test,CThingPlayerCreature* p){check(!test&&p==players.hero);++tests;return interactOK;}
    EAIActionReturn AIAction_DoNothing(){++idles;return idleOK?AI_ACTION_IN_PROGRESS:AI_ACTION_FAILED;}
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector pos,float distance,bool& route,EMovementType type,bool wasFar,bool b,bool c,CThingPlayerCreature* p,bool d,float end){
        check(repeat==ACTION_REPEAT_CALLBACK && type==AI_MOVEMENT_WALKING && distance==0.1f && p==players.hero && wasFar==far && !b && c && d && end==0);
        check(pos.X==3.375f && pos.Y==4.0f && pos.Z==8.0f);route=true;++moves;
        if(moveMode==1)return AI_ACTION_FAILED;
        if(moveMode==2 && moves<=2 || moveMode==3)return AI_ACTION_IN_PROGRESS;
        return AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat r,CIntelligentPointer<CThingPlayerCreature> p,int flags,float speed){
        check(r==ACTION_REPEAT_UNTIL_FINISHED&&p.Get()==players.hero&&flags==11&&speed==0.0027777f);++looks;return lookMode==0?AI_ACTION_FINISHED:AI_ACTION_FAILED;
    }
    const CMessageEvent* FindPlayerInteractionEvent()const;
    void Update();
};

inline const CMessageEvent* CAIStateGroup_MinionReactToPlayerInteraction::FindPlayerInteractionEvent() const
{
    return PeekWorld().GetMessageEventManager().FindMostRecentMessage(
        GFFilter_Bind2(CMessageEventFilter_IsOfType(MESSAGE_EVENT_PLAYER_INTERACTION),
                       CMessageEventFilter_IsTargettedAtThing(GetCreature())));
}
void CAIStateGroup_MinionReactToPlayerInteraction::Update()
{
    const CMessageEvent* pevent = FindPlayerInteractionEvent();
    if (pevent) {
        long event_extra_data = pevent->ExtraData;
        GetWorld().GetMessageEventManager().RemoveMessage(pevent);
        CIntelligentPointer<CThingPlayerCreature> player_character(PeekPlayerManager().GetMainPlayer().GetPPlayerCharacter());
        if (!IS_THING_ALIVE(player_character)) {
            Terminate();
            return;
        }
        if (ReactionType == INTERACTION_REACTION_TRADER && !GetWorld().IsPerformingRegionLoad() && !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            const CTCBoastUI* ptc_boast_ui;
            if (!player_character->PeekTC(&ptc_boast_ui) || !ptc_boast_ui->IsActive()) {
                CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(),"TEXT_GUI_TRADE_START_TRADING",4,0.0f,false,true);
                WaitingForConfirmation=true;
                PositionPlayerWasWhenWaitingForConfirmation=player_character->GetPos();
                UpdateWaitingForConfirmation(player_character);
            }
        }
        bool too_far=IsTooFarFromThePlayer(*player_character);
        bool too_near=IsTooNearToThePlayer(*player_character);
        if(GetCreature().HasTC(TCI_SHOP_KEEPER)) {
            CTCCreatureNavigation* tc_navigation;
            if(GetCreature().GetTC(&tc_navigation)) {
                float left,right;
                tc_navigation->CalculateNavigableWidthOfLine(GetCreature().GetPos(),player_character->GetPos(),left,right);
                if(left==0.0f || right==0.0f) { too_near=false; too_far=false; }
            }
        }
        if(GetCreature().IsFreeToFinishAction()) GetCreature().FinishCurrentAction();
        if(too_far || too_near) {
            C3DVector away_dir=GetCreature().PeekPos()-player_character->PeekPos();
            away_dir.Normalise();
            long time_to_give_up=CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*10;
            bool calculated_nav_route;
            EAIActionReturn ret;
            do {
                C3DVector move_to_pos=player_character->PeekPos()+away_dir*CThingCreatureBase::GetPersonalSpaceRadius();
                ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,move_to_pos,0.1f,calculated_nav_route,AI_MOVEMENT_WALKING,too_far,false,true,player_character.Get(),true,0.0f);
            } while(CWorld::GetFrame()<time_to_give_up && ret==AI_ACTION_IN_PROGRESS);
            if(ret!=AI_ACTION_FINISHED && event_extra_data==5) { Terminate();return; }
            long frame_started=CWorld::GetFrame();
            const long SECONDS_TO_WAIT_FOR_GIFT=5;
            do {
                if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,player_character,11,0.0027777f)!=AI_ACTION_FINISHED) {
                    Terminate();
                    return;
                }
                if(event_extra_data!=5)break;
            } while(CWorld::GetFrame()-frame_started<CMainGameComponent::GetConstantFPS()*SECONDS_TO_WAIT_FOR_GIFT);
        }
        CTCTalk* tc_talk;
        GetCreature().GetTC(&tc_talk);
        while(tc_talk->IsTalking()) {
            if(AIAction_DoNothing()!=AI_ACTION_IN_PROGRESS)break;
        }
        if(GetCreature().HasTC(TCI_WIFE)) {
            CTCWife* ptc_wife=0;
            GetCreature().GetTC(&ptc_wife);
            ptc_wife->OnGreet();
        }
        if(TestOrDoInteraction(false,player_character.Get()) || DidTraderInteraction) {
            CTCCreatureOpinionOfHero* ptc_opinion;
            if(GetCreature().GetTC(&ptc_opinion))ptc_opinion->SetHasInteracted();
        }
        if(WaitingForConfirmation)UpdateWaitingForConfirmation(player_character);
    }
    Terminate();
}
int main(){
    unsigned int timeouts=0,failedMoves=0,lookFailures=0,giftWaits=0;
    for(int choice=0;choice<4096;++choice)for(int mode=0;mode<24;++mode) {
        CAIStateGroup_MinionReactToPlayerInteraction s;
        CMainGameComponent::Get()->font=11;CPlayerGui& gui=CPlayerGui::Get();gui.subtitles=0;gui.busy=(choice&1)!=0;
        CTCWife wife={0};CTCCreatureOpinionOfHero opinion={0};CTCTalk talk={2};CTCBoastUI boast={(choice&2)!=0};
        CTCCreatureNavigation nav;nav.left=(choice&4)?0.0f:2.0f;nav.right=(choice&8)?0.0f:1.0f;
        CThingPlayerCreature hero;hero.alive=mode!=2;hero.boast=(choice&16)?&boast:0;hero.position=C3DVector(2,4,8);
        s.creature.position=C3DVector(4,4,8);s.creature.finished=0;s.creature.freeToFinish=(choice&32)!=0;
        s.creature.shop=(choice&64)!=0;s.creature.wife=(choice&128)?&wife:0;s.creature.talk=&talk;
        s.creature.nav=(choice&256)?&nav:0;s.creature.opinion=(choice&512)?&opinion:0;
        s.ReactionType=(choice&1024)?3:1;s.world.loading=(choice&2048)!=0;
        s.far=(choice&4)!=0;s.nearFlag=(choice&8)!=0;s.moveMode=mode%4;s.lookMode=(mode==4||mode==10);
        s.WaitingForConfirmation=mode%7==0;s.DidTraderInteraction=mode%3==0;
        s.confirmClears=mode%2==0;s.idleOK=mode%2==1;s.interactOK=mode%5==0;
        s.tests=s.moves=s.looks=s.ends=s.idles=s.confirmationCalls=0;s.players.hero=mode==1?0:&hero;
        CWorld::frame=200;CMessageEvent other={MESSAGE_EVENT_PLAYER_INTERACTION,&hero,1};
        CMessageEvent ignored={MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO,&s.creature,1};
        bool gift=mode%3!=1;CMessageEvent interaction={MESSAGE_EVENT_PLAYER_INTERACTION,&s.creature,gift?5:1};
        s.world.events.messages.push_back(other);s.world.events.messages.push_back(ignored);
        if(mode!=0)s.world.events.messages.push_back(interaction);
        bool run=mode>=3;bool trader=run && s.ReactionType==3 && !s.world.loading && !gui.busy && !(hero.boast&&hero.boast->active);
        bool wait=s.WaitingForConfirmation;int replies=0;
        if(trader){wait=!s.confirmClears;++replies;}
        const bool canMove=run && (s.far||s.nearFlag) && !(s.creature.shop&&s.creature.nav&&(nav.left==0||nav.right==0));
        int moves=0,looks=0;bool active=run;
        if(canMove){
            moves=s.moveMode==3?20:s.moveMode==2?3:1;
            active=!(gift && (s.moveMode==1||s.moveMode==3));
            if(active){looks=(!gift||s.lookMode)?1:10;active=s.lookMode==0;}
        }
        if(active && wait){++replies;if(s.confirmClears)wait=false;}
        s.Update();
        if(s.moves==20)++timeouts;
        if(s.moves && s.moveMode==1)++failedMoves;
        if(s.looks && s.lookMode)++lookFailures;
        if(s.looks==10)++giftWaits;
        check(s.ends==1 && s.moves==moves && s.looks==looks);
        check(gui.subtitles==(trader?1:0) && s.confirmationCalls==replies && s.WaitingForConfirmation==wait);
        if(trader)check(s.PositionPlayerWasWhenWaitingForConfirmation.X==2 && s.PositionPlayerWasWhenWaitingForConfirmation.Y==4 && s.PositionPlayerWasWhenWaitingForConfirmation.Z==8);
        check(s.creature.finished==(run && s.creature.freeToFinish?1:0));
        check(s.idles==(active?(s.idleOK?2:1):0) && wife.greeted==(active&&s.creature.wife?1:0) && s.tests==(active?1:0));
        check(opinion.interactions==(active&&(s.interactOK||s.DidTraderInteraction)&&s.creature.opinion?1:0));
        check(s.world.events.messages.size()==2 && s.world.events.messages.front().target==&hero && s.world.events.messages.back().type==MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO);
    }
    check(timeouts && failedMoves && lookFailures && giftWaits);
    puts("00930480_TEST PASS");return 0;
}