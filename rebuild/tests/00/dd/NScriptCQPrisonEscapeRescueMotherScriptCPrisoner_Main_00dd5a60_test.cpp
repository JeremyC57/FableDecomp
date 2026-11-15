#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "engine/CCountedPointer.h"
#include "engine/EHeroAbility.h"
struct Event {int kind,a,b;};
struct TestState {
 int stop,checks,frames,randomValue,randCalls,doorAt,escapedAt,regionAt,busyLeft,commentMode;
 int direct,any,heal,timer,registers,deregisters,sets,resets,comments,nearest,usedReads,conversations,freeLines,hitLines;
 int allocated,freed,phase,followStep,conversationStep,pendingID,pendingGuard,count;
 bool nullDoor,pendingComment,terminated;bool* escaped;Event events[4096];
};
static TestState state;
static void EventAt(int k,int a=0,int b=0){assert(!state.terminated&&state.count<4096);state.events[state.count].kind=k;state.events[state.count].a=a;state.events[state.count++].b=b;}
static bool HitExpected(){return state.direct!=0||(state.any!=0&&state.heal==0);}
class CCharString {public:const char* Text;CCharString(const char*s,long n=-1):Text(s){assert(n==-1);}~CCharString(){}};
static void TextIs(const CCharString&s,const char*text){assert(strcmp(s.Text,text)==0);}
class CBaseClass {public:virtual ~CBaseClass(){}};
class CScriptThing:public CBaseClass {
public:CCountedPointer<CScriptThing>PImp;int Identity;
 CScriptThing():Identity(0){}virtual ~CScriptThing();
 CScriptThing(const CScriptThing&o):PImp(o.PImp),Identity(o.Identity){}
 CScriptThing&operator=(const CScriptThing&o){PImp=o.PImp;Identity=o.Identity;return *this;}
 virtual bool MsgIsUsedBy(const CCharString&)const;
 virtual bool MsgIsHitBy(const CCharString&s)const{assert(Identity==2&&state.phase>=1);TextIs(s,"SCRIPT_NAME_HERO");EventAt(21,state.direct);return state.direct!=0;}
 virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(Identity==2&&!state.direct);TextIs(s,"SCRIPT_NAME_HERO");EventAt(22,state.any);return state.any!=0;}
 virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(Identity==2&&!state.direct&&state.any&&a==HERO_ABILITY_HEAL_LIFE_SPELL);TextIs(s,"SCRIPT_NAME_HERO");EventAt(23,state.heal);return state.heal!=0;}
};
class DoorImp:public CScriptThing {public:DoorImp(){Identity=4;++state.allocated;}virtual~DoorImp(){++state.freed;}virtual bool MsgIsUsedBy(const CCharString&s)const{TextIs(s,"SCRIPT_NAME_HERO");++state.usedReads;bool used=state.doorAt>=0&&state.frames>=state.doorAt;EventAt(6,used?1:0);return used;}};
static CScriptThing Hero;
class CTimer {public:long TimerIndex;CTimer();~CTimer();CTimer&operator=(long);long GetTime()const;bool operator==(long)const;};
class TRetVoid {public:void Return(){}};
class CGameScriptInterfaceBase {
public:
 long RegisterTimer(){assert(!state.registers);++state.registers;return 13;}
 void DeregisterTimer(long n){assert(n==13&&state.registers==1&&!state.deregisters);++state.deregisters;}
 void SetTimer(long n,long t){assert(n==13&&!state.terminated);if(!state.sets)assert(t==state.randomValue%90&&state.randCalls==1);else{assert(t==30&&state.pendingComment&&state.checks==state.pendingGuard+1);state.pendingComment=false;++state.resets;}state.timer=(int)t;++state.sets;EventAt(2,(int)t);}
 long GetTimer(long n){assert(n==13&&!state.terminated);EventAt(3,state.timer);return state.timer;}
 void NewScriptFrame(){assert(!state.pendingComment);++state.frames;if(state.timer>0)--state.timer;if(state.escapedAt>=0&&state.frames>=state.escapedAt)*state.escaped=true;EventAt(1,state.frames,state.phase);}
 CScriptThing GetNearestWithScriptName(const CScriptThing&me,const CCharString&name){assert(me.Identity==2&&!state.nearest&&state.phase==0);TextIs(name,"CellDoor");++state.nearest;EventAt(4);CScriptThing v;v.Identity=3;if(!state.nullDoor)v.PImp=CCountedPointer<CScriptThing>(new DoorImp);return v;}
 bool IsConversationActive(long id){assert(id==71&&state.phase==0);bool active=state.busyLeft>0;if(active)--state.busyLeft;EventAt(7,active?1:0);return active;}
 CScriptThing&GetHero(){return Hero;}
 long AddNewConversation(const CScriptThing&me,bool a,bool b){assert(me.Identity==2&&!a&&!b&&!state.conversationStep);if(state.phase)assert(HitExpected());state.conversationStep=1;state.pendingID=100+state.conversations++;EventAt(8,state.pendingID,state.phase);return state.pendingID;}
 void AddPersonToConversation(long id,const CScriptThing&who){assert(state.conversationStep==1&&id==state.pendingID&&who.Identity==1);state.conversationStep=2;EventAt(9,(int)id);}
 void AddLineToConversation(long id,const CCharString&line,bool a,const CScriptThing&speaker,const CScriptThing&listener){assert(state.conversationStep==2&&id==state.pendingID&&!a&&speaker.Identity==2&&listener.Identity==1);if(!state.phase){TextIs(line,"TEXT_QST_055_PRISON_PRISONER_FREE");++state.freeLines;}else{TextIs(line,"TEXT_QST_055_PRISON_PRISONER_ON_HIT");++state.hitLines;}state.conversationStep=0;EventAt(10,(int)id,state.phase);}
 void EntitySetAsAllowedToFollowHero(const CScriptThing&me,bool yes){assert(me.Identity==2&&yes&&state.followStep==0);state.followStep=1;EventAt(11);}
 void EntityFollowThing(const CScriptThing&me,const CScriptThing&hero,float d,bool yes){assert(me.Identity==2&&hero.Identity==1&&d==3.0f&&yes&&state.followStep==1);state.followStep=2;EventAt(12);}
 void SetEntityAsRegionFollowing(const CScriptThing&hero,const CScriptThing&me,bool yes){assert(hero.Identity==1&&me.Identity==2);if(yes){assert(state.followStep==2);state.followStep=3;}else{assert(state.followStep==5&&state.phase==1);state.phase=2;}EventAt(13,yes?1:0);}
 void SetThingPersistent(const CScriptThing&me,bool yes){assert(me.Identity==2&&yes&&state.followStep==3);state.followStep=4;EventAt(14);}
 void EntitySetInFaction(const CScriptThing&me,const CCharString&s){assert(me.Identity==2&&state.followStep==4);TextIs(s,"FACTION_PRISONER");state.followStep=5;state.phase=1;EventAt(15);}
 bool IsRegionLoaded(const CCharString&s){assert(state.phase==1&&state.followStep==5);TextIs(s,"PrisonCourtyard");bool loaded=state.regionAt>=0&&state.frames>=state.regionAt;EventAt(16,loaded?1:0);return loaded;}
};
static CGameScriptInterfaceBase Interface;
extern "C" int rand(void){assert(!state.randCalls&&!state.sets);++state.randCalls;EventAt(17,state.randomValue);return state.randomValue;}
namespace NScript {class CQ_PrisonEscapeRescueMotherScript {public:bool HeroEscapedCell;long ConversationIndex;
 bool MakePrisonerComment(const CScriptThing&me,const CCharString&s){assert(me.Identity==2&&state.timer==0&&!state.pendingComment);const char*expected=state.phase==2?"GUARDS":state.phase==1?"COURTYARD":HeroEscapedCell?"RELEASE":"IDLE";TextIs(s,expected);bool yes=state.commentMode==1||(state.commentMode==2&&state.frames%2==0);++state.comments;EventAt(5,state.phase,yes?1:0);if(yes){state.pendingComment=true;state.pendingGuard=state.checks;}return yes;}
 class CPrisoner {public:CQ_PrisonEscapeRescueMotherScript&ParentClass;CGameScriptInterfaceBase&Game;CScriptThing Me;bool ReleasedAndFollowing;
 CPrisoner(CQ_PrisonEscapeRescueMotherScript&p):ParentClass(p),Game(Interface),ReleasedAndFollowing(false){}
 bool IsActiveThreadTerminating()const{assert(!state.terminated&&state.checks<state.stop);state.terminated=++state.checks==state.stop;return state.terminated;}
 void Main();
 };};}

extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main()
{
    TRetVoid _Fail_Val;
    CTimer TalkTimer;
    TalkTimer = rand() % 90;
    if (!ReleasedAndFollowing) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing CellDoor = Game.GetNearestWithScriptName(Me, "CellDoor");
        while (!ReleasedAndFollowing) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HeroEscapedCell && TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "RELEASE")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer = 30;
            }
            if (!ParentClass.HeroEscapedCell && TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "IDLE")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer = 30;
            }
            if (CellDoor.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (Game.IsConversationActive(ParentClass.ConversationIndex)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_FREE", false, Me, Game.GetHero());
                ReleasedAndFollowing = true;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    Game.EntitySetAsAllowedToFollowHero(Me, true);
    Game.EntityFollowThing(Me, Game.GetHero(), 3.0f, true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
    Game.SetThingPersistent(Me, true);
    Game.EntitySetInFaction(Me, "FACTION_PRISONER");
    while (!Game.IsRegionLoaded("PrisonCourtyard")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "COURTYARD")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
    for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (TalkTimer == 0 && ParentClass.MakePrisonerComment(Me, "GUARDS")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            TalkTimer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game.GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

CGameScriptInterfaceBase* pDSTGame=&Interface;

int main(){
 static const int randoms[]={0,1,29,89,90,179},times[]={-1,0,1,3,8},regions[]={-1,0,3,12,40};
 unsigned cases=0;int coverage[8]={0,0,0,0,0,0,0,0};Hero.Identity=1;
 for(int seed=0;seed<1024;++seed){Event previous[4096];int previousCount=0;
  for(int stop=1;stop<=96;++stop){
   memset(&state,0,sizeof(state));state.stop=stop;state.randomValue=randoms[seed%6];state.doorAt=times[seed/3%5];state.escapedAt=times[seed/7%5];state.regionAt=regions[seed/11%5];state.busyLeft=seed/13%3;state.commentMode=seed/17%3;state.direct=seed/19%2;state.any=seed/23%2;state.heal=seed/29%2;state.nullDoor=seed/31%2!=0;
   NScript::CQ_PrisonEscapeRescueMotherScript parent;parent.HeroEscapedCell=seed/37%2!=0;parent.ConversationIndex=71;state.escaped=&parent.HeroEscapedCell;
   bool initialReleased=seed/41%2!=0;
   {NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner script(parent);script.ReleasedAndFollowing=initialReleased;script.Me.Identity=2;script.Main();
    assert(state.terminated&&state.checks==stop&&state.registers==1&&state.deregisters==1&&state.randCalls==1&&state.sets==1+state.resets&&!state.conversationStep);
    assert(state.conversations==state.freeLines+state.hitLines&&state.freeLines<=1&&script.ReleasedAndFollowing==(initialReleased||state.freeLines!=0));
    assert(state.pendingComment==(state.comments>0&&state.pendingGuard==state.checks-1&&state.timer==0&&state.sets==1+state.resets));
    if(state.followStep){assert(state.followStep==5&&script.ReleasedAndFollowing);++coverage[0];}else assert(!state.phase&&!state.hitLines);
    assert(state.nearest==(initialReleased||stop==1?0:1));assert(state.allocated==state.freed&&state.allocated==(state.nearest&&!state.nullDoor?1:0));
    if(state.phase==2)++coverage[1];if(state.freeLines)++coverage[2];if(state.hitLines)++coverage[3];if(state.resets)++coverage[4];if(state.pendingComment)++coverage[5];if(state.nullDoor&&state.nearest)++coverage[6];if(state.frames>30)++coverage[7];
   }
   assert(state.allocated==state.freed&&previousCount<=state.count);for(int n=0;n<previousCount;++n)assert(previous[n].kind==state.events[n].kind&&previous[n].a==state.events[n].a&&previous[n].b==state.events[n].b);
   previousCount=state.count;memcpy(previous,state.events,previousCount*sizeof(Event));++cases;
  }
 }
 assert(cases==98304u);for(int k=0;k<8;++k)assert(coverage[k]>0);puts("00dd5a60_TEST PASS");return 0;
}