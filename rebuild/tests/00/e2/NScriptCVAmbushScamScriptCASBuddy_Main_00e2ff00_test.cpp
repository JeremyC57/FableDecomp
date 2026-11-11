// Complete Main and its original macros, deterministic engine and owned spawned callees.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Resources,Movies,Pauses,Threads;
class CCharString{ public:std::string text;CCharString(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const std::string&s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}CCharString&operator+=(const CCharString&s){text+=s.text;return *this;}};static CCharString operator+(const CCharString&a,const CCharString&b){return CCharString(a.text+b.text);}static CCharString operator+(const char*a,const CCharString&b){return CCharString(std::string(a)+b.text);}static CCharString GFIntToCharString(long n){char b[40];std::sprintf(b,"%ld",n);return CCharString(b);}
class C3DVector{ public:int Id;C3DVector(int i=0):Id(i){}};struct GameStub;static GameStub*Current;enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2,ENTITY_MOVE_WALK=0,ESSG_NONE=0,ESSG_WANDER_NEAR=4};
class CScriptThing{ public:int Id;CScriptThing(int n=0):Id(n){}bool IsAlive()const;bool MsgIsTalkedToBy(const char*)const;C3DVector GetPos()const{return C3DVector(Id);}};struct TRetVoid{void Return(){}};struct Parent;typedef Parent CV_AmbushScamScript;struct Buddy;
struct ThreadBase{CCharString Name;ThreadBase(const CCharString&n):Name(n){++Threads;}virtual~ThreadBase(){--Threads;}virtual bool Valid(Parent*)const=0;};template<class T,class A>struct CSpawnedFunc1:ThreadBase{T*Owner;void(T::*Func)(A);A Par;unsigned long Id;CSpawnedFunc1(const CCharString&n,unsigned long id,T*p,void(T::*f)(A),A a):ThreadBase(n),Owner(p),Func(f),Par(a),Id(id){}bool Valid(Parent*)const;};
struct GameStub{Buddy*Owner;int Frame,TerminateAt,DeadAt,TricksterAliveAt,TricksterDeadAt,Starts,StartFailures,FailAfter,StopAcquire,Releases,TaskUntil,TaskLength,StopSpeech,SpeechCount,FollowAt,FollowCalls,StopFollow,DoAmbushAt,DoAmbushOffAt,AllDeadAt,TricksterCompleteAt,QuestFinishedAt,AttackBuddyAt,AttackTricksterAt,Conversations,People,InfoClear,AttackCount,WanderCentre,WanderMin,WanderMax,StateWander,StateNone,MoveCount,MoveUntil,FadeCount,AmbusherCount;bool Stop,TricksterAlive,ExitAlive,Following,ExplicitStopped;float HeroDistance;std::vector<int>TalkAt;std::vector<std::string>Speech,ConvoLines,SpawnNames;std::vector<int>Selections;std::vector<std::pair<int,int> >Allies,Unallies,Enemies;
 GameStub():Owner(0),Frame(0),TerminateAt(24),DeadAt(-1),TricksterAliveAt(1),TricksterDeadAt(-1),Starts(0),StartFailures(0),FailAfter(-1),StopAcquire(-1),Releases(0),TaskUntil(0),TaskLength(1),StopSpeech(-1),SpeechCount(0),FollowAt(-1),FollowCalls(0),StopFollow(0),DoAmbushAt(-1),DoAmbushOffAt(-1),AllDeadAt(-1),TricksterCompleteAt(-1),QuestFinishedAt(-1),AttackBuddyAt(-1),AttackTricksterAt(-1),Conversations(0),People(0),InfoClear(0),AttackCount(0),WanderCentre(0),WanderMin(0),WanderMax(0),StateWander(0),StateNone(0),MoveCount(0),MoveUntil(999),FadeCount(0),AmbusherCount(3),Stop(false),TricksterAlive(true),ExitAlive(true),Following(false),ExplicitStopped(false),HeroDistance(2){Current=this;}
 void NewScriptFrame();CScriptThing GetHero(){return CScriptThing(2);}bool StartScriptingEntity(const CScriptThing&,class CScriptGameResourceObjectScriptedThingBase&,int);bool TakeTalk(){if(!TalkAt.empty()&&TalkAt.front()<=Frame){TalkAt.erase(TalkAt.begin());return true;}return false;}bool IsEntityFollowingHero(const CScriptThing&t){assert(t.Id==1);return Following||(!ExplicitStopped&&FollowAt>=0&&Frame>=FollowAt);}void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){Allies.push_back(std::make_pair(a.Id,b.Id));}void EntityUnsetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){Unallies.push_back(std::make_pair(a.Id,b.Id));}void EntitySetThingAsEnemyOfThing(const CScriptThing&a,const CScriptThing&b){Enemies.push_back(std::make_pair(a.Id,b.Id));}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.Id==1&&!a&&!b);return ++Conversations;}void AddPersonToConversation(long c,const CScriptThing&t){assert(c==Conversations&&t.Id==2);++People;}void AddLineToConversation(long c,const CCharString&s,bool a,const CScriptThing&t,const CScriptThing&h){assert(c==Conversations&&!a&&t.Id==1&&h.Id==2);ConvoLines.push_back(s.text);}long GetAllThingsWithScriptName(const char*s,std::vector<CScriptThing>&v){assert(std::strcmp(s,"AS_Ambusher")==0);v.clear();for(int n=0;n<AmbusherCount;++n)v.push_back(CScriptThing(10+n));return AmbusherCount;}void SetWanderCentrePoint(const CScriptThing&t,const C3DVector&p){assert(t.Id==1&&p.Id==7);++WanderCentre;}void SetWanderMinDistance(const CScriptThing&t,float f){assert(t.Id==1&&f==0);++WanderMin;}void SetWanderMaxDistance(const CScriptThing&,float);void SetScriptingStateGroup(const CScriptThing&t,int s){assert(t.Id==1);if(s==ESSG_WANDER_NEAR)++StateWander;else{assert(s==ESSG_NONE);++StateNone;}}void GiveThingBestEnemyTarget(const CScriptThing&a,const CScriptThing&b){assert(a.Id==1&&b.Id==2);++AttackCount;}void StartMovieSequence(const CCharString&,class CScriptGameResourceObjectMovieBase&);float GetHealth(const CScriptThing&);CScriptThing GetThingWithScriptName(const char*s){assert(std::strcmp(s,"M_AS_BuddyExitPoint")==0);return CScriptThing(4);}void ClearThingHasInformation(const CScriptThing&t){assert(t.Id==1);++InfoClear;}
};
class CScriptGameResourceObjectMovieBase{ public:bool Active;CScriptGameResourceObjectMovieBase():Active(false){}~CScriptGameResourceObjectMovieBase(){if(Active)--Movies;}};void GameStub::StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&m){assert(s.text.empty()&&!m.Active);m.Active=true;++Movies;}struct CWideScreenMagicPauseEntities{CWideScreenMagicPauseEntities(GameStub&){++Pauses;}~CWideScreenMagicPauseEntities(){--Pauses;}};
class CScriptGameResourceObjectScriptedThingBase{ public:bool Active;CScriptGameResourceObjectScriptedThingBase():Active(false){++Resources;}~CScriptGameResourceObjectScriptedThingBase(){--Resources;}bool IsValid()const{return Active;}void Release(){assert(Active);Active=false;++Current->Releases;}bool IsFollowingThing(){return Current->Following;}bool IsFollowActionRunning(const CScriptThing&t){assert(t.Id==2);return Current->Following;}void FollowThing(const CScriptThing&t,float f,bool b){assert(t.Id==2&&f==1&&b&&Active);Current->Following=true;Current->ExplicitStopped=false;++Current->FollowCalls;}void StopFollowingThing(const CScriptThing&t){assert(t.Id==2&&Active);Current->Following=false;Current->ExplicitStopped=true;++Current->StopFollow;}bool IsPerformingScriptTask(){return Current->Frame<Current->TaskUntil;}CScriptThing GetScriptThing(){return CScriptThing(1);}void Speak(const CScriptThing&h,const CCharString&s,int selection,bool a,bool b,bool c){assert(Active&&h.Id==2&&!a&&b&&!c);++Current->SpeechCount;Current->Speech.push_back(s.text);Current->Selections.push_back(selection);Current->TaskUntil=Current->Frame+Current->TaskLength;if(Current->SpeechCount==Current->StopSpeech)Current->Stop=true;}void MoveToPosition(const C3DVector&p,float f,int type,bool a,bool b){assert(p.Id==4&&f==1&&type==ENTITY_MOVE_WALK&&!a&&b&&Active);++Current->MoveCount;Current->MoveUntil=Current->Frame+2;}};
struct Parent{bool MoralitySwitch,TricksterComplete,DoAmbush,AllAmbushersDead,HeroAttackOnTricksterAfterAmbush,TricksterResetAllegience,HeroAttackOnBuddy,BuddyHelped,TricksterApproachComplete,BuddyIntroComplete,QuestFinished,TricksterKilledBeforeAmbush,ExpressionDetectedWait,ExpressionDetectedFollow;long BuddyCounter;std::vector<ThreadBase*>Owned;Parent():MoralitySwitch(true),TricksterComplete(false),DoAmbush(false),AllAmbushersDead(false),HeroAttackOnTricksterAfterAmbush(false),TricksterResetAllegience(false),HeroAttackOnBuddy(false),BuddyHelped(false),TricksterApproachComplete(false),BuddyIntroComplete(false),QuestFinished(false),TricksterKilledBeforeAmbush(false),ExpressionDetectedWait(false),ExpressionDetectedFollow(false),BuddyCounter(0){}~Parent(){for(unsigned n=0;n<Owned.size();++n)delete Owned[n];}void AddSpawnedFunction(ThreadBase*t,const CCharString&n){assert(n.text.empty()&&t->Valid(this));Owned.push_back(t);Current->SpawnNames.push_back(t->Name.text);}CScriptThing GetTrickster(){return CScriptThing(3);}void WatchForExpressions(CScriptThing){}void WatchForHeroAttack(CScriptThing){}void WatchForTricksterComplete(CScriptThing){}};
template<class T,class A>bool CSpawnedFunc1<T,A>::Valid(Parent*p)const{return Owner==p&&Id==0&&Par.Id==1&&((Name.text=="ParentClass.WatchForExpressions"&&Func==&T::WatchForExpressions)||(Name.text=="ParentClass.WatchForHeroAttack"&&Func==&T::WatchForHeroAttack)||(Name.text=="ParentClass.WatchForTricksterComplete"&&Func==&T::WatchForTricksterComplete));}
struct Buddy{GameStub*Game;CScriptThing Me;Parent ParentClass;bool BuddyFollowSpeechComplete,BuddyHeroApproachComplete,BuddyHeroApproachSpeechComplete;C3DVector BuddyStartPos;int Health;Buddy(GameStub&g):Game(&g),Me(1),BuddyFollowSpeechComplete(false),BuddyHeroApproachComplete(false),BuddyHeroApproachSpeechComplete(false),BuddyStartPos(7),Health(10){g.Owner=this;}bool IsActiveThreadTerminating(){return Game->Stop||Game->Frame>=Game->TerminateAt||(Game->DeadAt>=0&&Game->Frame>=Game->DeadAt);}void Main();};
void GameStub::NewScriptFrame(){assert(++Frame<300);if(DoAmbushAt>=0&&Frame>=DoAmbushAt)Owner->ParentClass.DoAmbush=true;if(DoAmbushOffAt>=0&&Frame>=DoAmbushOffAt)Owner->ParentClass.DoAmbush=false;if(AllDeadAt>=0&&Frame>=AllDeadAt)Owner->ParentClass.AllAmbushersDead=true;if(TricksterCompleteAt>=0&&Frame>=TricksterCompleteAt)Owner->ParentClass.TricksterComplete=true;if(QuestFinishedAt>=0&&Frame>=QuestFinishedAt)Owner->ParentClass.QuestFinished=true;if(AttackBuddyAt>=0&&Frame>=AttackBuddyAt)Owner->ParentClass.HeroAttackOnBuddy=true;if(AttackTricksterAt>=0&&Frame>=AttackTricksterAt)Owner->ParentClass.HeroAttackOnTricksterAfterAmbush=true;}
bool GameStub::StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(t.Id==1&&p==4);++Starts;if(Starts==StopAcquire)Stop=true;if(Starts<=StartFailures||(FailAfter>=0&&Starts>=FailAfter))return false;r.Active=true;return true;}float GameStub::GetHealth(const CScriptThing&t){assert(t.Id==1);return(float)Owner->Health;}bool CScriptThing::IsAlive()const{return Id==3?Current->TricksterAlive&&Current->Frame>=Current->TricksterAliveAt&&(Current->TricksterDeadAt<0||Current->Frame<Current->TricksterDeadAt):Id==4?Current->ExitAlive:Id!=0;}bool CScriptThing::MsgIsTalkedToBy(const char*s)const{assert(Id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return Current->TakeTalk();}static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.Id==1);return b.Id==2?Current->HeroDistance<d:b.Id==4?Current->Frame>=Current->MoveUntil:false;}static void FadeOutAndKillEntity(const CScriptThing&t,float f,bool a,bool b){assert(t.Id==1&&f==1&&a&&b);++Current->FadeCount;Current->Stop=true;}
struct ScriptDef{float AmbushScamBuddyWanderMax;ScriptDef():AmbushScamBuddyWanderMax(10){}};static ScriptDef GlobalDef;static ScriptDef*pGlobalScriptDef=&GlobalDef;void GameStub::SetWanderMaxDistance(const CScriptThing&t,float f){assert(t.Id==1&&f==10);++WanderMax;}
#define T IsActiveThreadTerminating()
#define NSF Game.NewScriptFrame()
#define AB_START \
    if (sehMe.IsValid()) sehMe.Release(); \
    while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return _Fail_Val.Return(); \
    } \
    if (T) return _Fail_Val.Return();
#define AB_SAY(TEXT,SELECTION) \
    if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) { \
        sehMe.Speak(Game.GetHero(),TEXT,SELECTION,false,true,false); \
        while (sehMe.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return _Fail_Val.Return(); \
        } \
        if (T) return _Fail_Val.Return(); \
    }
#define AB_MOVIE_SAY(GUARD,BANDIT) \
    CScriptGameResourceObjectMovieBase speech_movie; \
    Game.StartMovieSequence("",speech_movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game); \
    if (ParentClass.MoralitySwitch) { \
        if (T) return _Fail_Val.Return(); \
        AB_SAY(GUARD,GROUP_SELECT_RANDOM_NO_REPEAT) \
    } \
    else { \
        if (T) return _Fail_Val.Return(); \
        AB_SAY(BANDIT,GROUP_SELECT_RANDOM_NO_REPEAT) \
    }
#define AB_SPAWN(FUNC) \
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_AmbushScamScript,CScriptThing>(CCharString("ParentClass.")+CCharString(#FUNC),0,&ParentClass,&CV_AmbushScamScript::FUNC,Me),CCharString(""));
void Buddy::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    AB_SPAWN(WatchForExpressions)
    AB_SPAWN(WatchForHeroAttack)
    AB_SPAWN(WatchForTricksterComplete)
    CScriptThing trickster;
    for (; !false; NSF) {
        if (T) return _Fail_Val.Return();
        trickster=ParentClass.GetTrickster();
        if (trickster.IsAlive()) {
            if (T) return _Fail_Val.Return();
            break;
        }
    }
    if (T) return _Fail_Val.Return();
    Game.EntitySetThingAsAllyOfThing(Me,trickster);
    Game.EntitySetThingAsAllyOfThing(trickster,Me);
    for (; !false; NSF) {
        if (T) return _Fail_Val.Return();
        if (!BuddyFollowSpeechComplete && Game.IsEntityFollowingHero(Me)) {
            // Native E304A1 -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            BuddyFollowSpeechComplete=true;
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            if (ParentClass.MoralitySwitch) {
                if (T) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_002_GUARD_FOLLOW",false,Me,Game.GetHero());
            }
            else {
                if (T) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_002_BANDIT_FOLLOW",false,Me,Game.GetHero());
            }
        }
        if (ParentClass.TricksterComplete && sehMe.IsValid() && sehMe.IsFollowingThing()) {
            if (T) return _Fail_Val.Return();
            AB_START
            sehMe.StopFollowingThing(Game.GetHero());
            if (sehMe.IsValid()) sehMe.Release();
        }
        if (ParentClass.DoAmbush && !BuddyHeroApproachComplete) {
            // Native E30657 -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            std::vector<CScriptThing> ambushers;
            long numAmbushers=Game.GetAllThingsWithScriptName("AS_Ambusher",ambushers);
            if (!false) {
            for (long idx=0; idx<numAmbushers; ++idx) {
                Game.EntitySetThingAsEnemyOfThing(Me,ambushers[idx]);
                Game.EntitySetThingAsEnemyOfThing(ambushers[idx],Me);
            }
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            AB_START
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            sehMe.FollowThing(Game.GetHero(),1.0f,true);
            BuddyHeroApproachComplete=true;
        }
        else if (BuddyHeroApproachComplete && !BuddyHeroApproachSpeechComplete && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),3.0f)) {
            if (T) return _Fail_Val.Return();
            BuddyHeroApproachSpeechComplete=true;
            sehMe.StopFollowingThing(Game.GetHero());
            if (sehMe.IsValid()) sehMe.Release();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            long suffix=10;
            CCharString assistText="TEXT_QST_002_";
            if (ParentClass.BuddyCounter>1) {
                if (T) return _Fail_Val.Return();
                suffix=20;
            }
            else {
                if (T) return _Fail_Val.Return();
                ParentClass.BuddyCounter++;
            }
            if (ParentClass.MoralitySwitch) {
                if (T) return _Fail_Val.Return();
                assistText+="GUARD_ASSIST_"+GFIntToCharString(suffix);
            }
            else {
                if (T) return _Fail_Val.Return();
                assistText+="BANDIT_ASSIST_"+GFIntToCharString(suffix);
            }
            Game.AddLineToConversation(HLStyleConversationIndex,assistText,false,Me,Game.GetHero());
        }
        else if (ParentClass.AllAmbushersDead && BuddyHeroApproachSpeechComplete) {
            if (T) return _Fail_Val.Return();
            Game.SetWanderCentrePoint(Me,BuddyStartPos);
            Game.SetWanderMinDistance(Me,0.0f);
            Game.SetWanderMaxDistance(Me,pGlobalScriptDef->AmbushScamBuddyWanderMax);
            Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        }
        if (ParentClass.HeroAttackOnTricksterAfterAmbush && trickster.IsAlive() && !ParentClass.TricksterResetAllegience) {
            if (T) return _Fail_Val.Return();
            Game.EntityUnsetThingAsAllyOfThing(Me,trickster);
            Game.EntityUnsetThingAsAllyOfThing(trickster,Me);
            Game.EntitySetThingAsEnemyOfThing(Me,trickster);
            Game.EntitySetThingAsEnemyOfThing(trickster,Me);
            ParentClass.TricksterResetAllegience=true;
        }
        else if (ParentClass.HeroAttackOnBuddy) {
            // Native E31EBD -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.SetScriptingStateGroup(Me,ESSG_NONE);
            Game.EntityUnsetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(),Me);
            Game.EntitySetThingAsEnemyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsEnemyOfThing(Game.GetHero(),Me);
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (sehMe.IsValid()) sehMe.Release();
            for (;;) {
                NSF;
                // Native E31F8A -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return _Fail_Val.Return();
            AB_START
            if (ParentClass.TricksterComplete) {
                if (T) return _Fail_Val.Return();
                if (ParentClass.BuddyHelped) {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_HELPED","TEXT_QST_002_BANDIT_HELPED")
                }
                else {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_END","TEXT_QST_002_BANDIT_END")
                }
            }
            else if (ParentClass.DoAmbush) {
                if (T) return _Fail_Val.Return();
                if (ParentClass.AllAmbushersDead) {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_AMBUSH_FINISHED","TEXT_QST_002_BANDIT_AMBUSH_FINISHED")
                }
                else {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_AMBUSH_STARTED","TEXT_QST_002_BANDIT_AMBUSH_STARTED")
                }
            }
            else if (ParentClass.TricksterApproachComplete) {
                if (T) return _Fail_Val.Return();
                AB_MOVIE_SAY("TEXT_QST_002_GUARD_CAUTION","TEXT_QST_002_BANDIT_CAUTION")
            }
            else {
                if (T) return _Fail_Val.Return();
                if (!ParentClass.BuddyIntroComplete) {
                    if (T) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (ParentClass.MoralitySwitch) {
                        if (T) return _Fail_Val.Return();
                        AB_SAY("TEXT_QST_002_GUARD_NOTHING",GROUP_SELECT_NONE)
                        AB_SAY("TEXT_QST_002_GUARD_BORED",GROUP_SELECT_NONE)
                    }
                    else {
                        if (T) return _Fail_Val.Return();
                        AB_SAY("TEXT_QST_002_BANDIT_NOTHING",GROUP_SELECT_NONE)
                        AB_SAY("TEXT_QST_002_BANDIT_BORED",GROUP_SELECT_NONE)
                    }
                    ParentClass.BuddyIntroComplete=true;
                }
                else {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_OFFER","TEXT_QST_002_BANDIT_OFFER")
                }
            }
            if (sehMe.IsValid()) sehMe.Release();
        }
        if (ParentClass.QuestFinished || ParentClass.TricksterKilledBeforeAmbush) {
            if (T) return _Fail_Val.Return();
            CScriptThing Exit=Game.GetThingWithScriptName("M_AS_BuddyExitPoint");
            AB_START
            Game.ClearThingHasInformation(Me);
            if (sehMe.IsValid() && sehMe.IsFollowActionRunning(Game.GetHero())) {
                if (T) return _Fail_Val.Return();
                sehMe.StopFollowingThing(Game.GetHero());
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (Exit.IsAlive()) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                if (T) return _Fail_Val.Return();
                if (sehMe.IsValid()) {
                    if (T) return _Fail_Val.Return();
                    sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_WALK,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,Exit,2.0f)) {
                    NSF;
                    if (T) return _Fail_Val.Return();
                }
                if (T) return _Fail_Val.Return();
                Game.ClearThingHasInformation(Me);
                FadeOutAndKillEntity(Me,1.0f,true,true);
            }
        }
        else if (ParentClass.ExpressionDetectedWait) {
            if (T) return _Fail_Val.Return();
            ParentClass.ExpressionDetectedWait=false;
            Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        }
        else if (ParentClass.ExpressionDetectedFollow) {
            if (T) return _Fail_Val.Return();
            ParentClass.ExpressionDetectedFollow=false;
            Game.SetScriptingStateGroup(Me,ESSG_NONE);
        }
    }
    if (T) return _Fail_Val.Return();
    while (true) {
        NSF;
        if (T) return _Fail_Val.Return();
    }
    if (T) return _Fail_Val.Return();
}
}

static bool Has(const std::vector<std::string>&v,const char*s){for(unsigned i=0;i<v.size();++i)if(v[i]==s)return true;return false;}static bool Pair(const std::vector<std::pair<int,int> >&v,int a,int b){for(unsigned i=0;i<v.size();++i)if(v[i]==std::make_pair(a,b))return true;return false;}static bool Balanced(const Buddy&o){return Resources==0&&Movies==0&&Pauses==0&&Threads==3&&Strings==3&&o.ParentClass.Owned.size()==3&&Current->SpawnNames.size()==3&&Current->SpawnNames[0]=="ParentClass.WatchForExpressions"&&Current->SpawnNames[1]=="ParentClass.WatchForHeroAttack"&&Current->SpawnNames[2]=="ParentClass.WatchForTricksterComplete";}static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 {GameStub g;Buddy o(g);o.Main();ok=Check(Balanced(o)&&g.Allies.size()==2&&Pair(g.Allies,1,3)&&Pair(g.Allies,3,1)&&g.Speech.empty()&&g.Starts==0,++n)&&ok;}
 {GameStub g;Buddy o(g);g.TricksterAliveAt=100;o.Main();ok=Check(Balanced(o)&&g.Allies.empty()&&g.Starts==0&&g.Frame==24,++n)&&ok;}
 for(int morality=0;morality<2;++morality){GameStub g;Buddy o(g);o.ParentClass.MoralitySwitch=morality!=0;g.FollowAt=3;o.Main();ok=Check(Balanced(o)&&o.BuddyFollowSpeechComplete&&g.Conversations==1&&Has(g.ConvoLines,morality?"TEXT_QST_002_GUARD_FOLLOW":"TEXT_QST_002_BANDIT_FOLLOW"),++n)&&ok;}
 for(int morality=0;morality<2;++morality){for(int count=0;count<3;++count){GameStub g;Buddy o(g);o.ParentClass.MoralitySwitch=morality!=0;o.ParentClass.BuddyCounter=count;g.DoAmbushAt=3;g.DoAmbushOffAt=4;g.AllDeadAt=7;o.Main();const char*expected=morality?(count>1?"TEXT_QST_002_GUARD_ASSIST_20":"TEXT_QST_002_GUARD_ASSIST_10"):(count>1?"TEXT_QST_002_BANDIT_ASSIST_20":"TEXT_QST_002_BANDIT_ASSIST_10");ok=Check(Balanced(o)&&o.BuddyHeroApproachComplete&&o.BuddyHeroApproachSpeechComplete&&o.ParentClass.BuddyCounter==(count>1?count:count+1)&&g.Enemies.size()==6&&Pair(g.Enemies,1,10)&&Pair(g.Enemies,12,1)&&g.FollowCalls==1&&g.StopFollow==1&&Has(g.ConvoLines,expected)&&g.WanderCentre>0&&g.WanderCentre==g.WanderMin&&g.WanderCentre==g.WanderMax,++n)&&ok;}}
 {GameStub g;Buddy o(g);g.DoAmbushAt=3;g.HeroDistance=4;g.AmbusherCount=0;o.Main();ok=Check(Balanced(o)&&o.BuddyHeroApproachComplete&&!o.BuddyHeroApproachSpeechComplete&&g.Enemies.empty()&&g.FollowCalls==1&&g.StopFollow==0,++n)&&ok;}
 {GameStub g;Buddy o(g);g.DoAmbushAt=3;g.HeroDistance=4;g.TricksterCompleteAt=6;o.Main();ok=Check(Balanced(o)&&g.StopFollow==1&&!g.Following&&g.Releases==2,++n)&&ok;}
 {GameStub g;Buddy o(g);g.AttackTricksterAt=3;o.Main();ok=Check(Balanced(o)&&o.ParentClass.TricksterResetAllegience&&g.Unallies.size()==2&&g.Enemies.size()==2&&Pair(g.Enemies,1,3)&&Pair(g.Enemies,3,1),++n)&&ok;}
 {GameStub g;Buddy o(g);g.AttackBuddyAt=3;o.Main();ok=Check(Balanced(o)&&g.AttackCount==1&&g.StateNone==1&&Pair(g.Enemies,1,2)&&Pair(g.Enemies,2,1)&&g.Speech.empty(),++n)&&ok;}
 {GameStub g;Buddy o(g);g.AttackBuddyAt=3;g.AttackTricksterAt=3;o.Main();ok=Check(Balanced(o)&&o.ParentClass.TricksterResetAllegience&&g.AttackCount==1&&g.Unallies.size()==4&&g.Enemies.size()==4,++n)&&ok;}
 {GameStub g;Buddy o(g);g.TricksterDeadAt=3;g.AttackTricksterAt=3;o.Main();ok=Check(Balanced(o)&&!o.ParentClass.TricksterResetAllegience&&g.Enemies.empty(),++n)&&ok;}
 for(int morality=0;morality<2;++morality){for(int branch=0;branch<7;++branch){GameStub g;Buddy o(g);o.ParentClass.MoralitySwitch=morality!=0;const char*tail="";switch(branch){case 0:o.ParentClass.TricksterComplete=true;o.ParentClass.BuddyHelped=true;tail="HELPED";break;case 1:o.ParentClass.TricksterComplete=true;tail="END";break;case 2:o.ParentClass.DoAmbush=true;o.ParentClass.AllAmbushersDead=true;tail="AMBUSH_FINISHED";break;case 3:o.ParentClass.DoAmbush=true;tail="AMBUSH_STARTED";break;case 4:o.ParentClass.TricksterApproachComplete=true;tail="CAUTION";break;case 5:o.ParentClass.BuddyIntroComplete=true;tail="OFFER";break;case 6:tail="NOTHING";break;}g.TalkAt.push_back(2);o.Main();std::string expected=std::string(morality?"TEXT_QST_002_GUARD_":"TEXT_QST_002_BANDIT_")+tail;ok=Check(Balanced(o)&&Has(g.Speech,expected.c_str())&&g.Speech.size()==(branch==6?2u:1u)&&g.Selections[0]==(branch==6?GROUP_SELECT_NONE:GROUP_SELECT_RANDOM_NO_REPEAT)&&(branch!=6||(o.ParentClass.BuddyIntroComplete&&g.Selections[1]==GROUP_SELECT_NONE)),++n)&&ok;}}
 {GameStub g;Buddy o(g);g.TalkAt.push_back(2);g.TalkAt.push_back(8);o.Main();ok=Check(Balanced(o)&&g.Speech.size()==3&&g.Speech.back()=="TEXT_QST_002_GUARD_OFFER"&&o.ParentClass.BuddyIntroComplete,++n)&&ok;}
 {GameStub g;Buddy o(g);o.Health=0;g.TalkAt.push_back(2);o.Main();ok=Check(Balanced(o)&&o.ParentClass.BuddyIntroComplete&&g.Speech.empty()&&g.Releases==1,++n)&&ok;}
 for(int killed=0;killed<2;++killed){GameStub g;Buddy o(g);if(killed)o.ParentClass.TricksterKilledBeforeAmbush=true;else g.QuestFinishedAt=5;o.Main();ok=Check(Balanced(o)&&g.MoveCount==1&&g.FadeCount==1&&g.InfoClear==2&&g.Stop,++n)&&ok;}
 {GameStub g;Buddy o(g);o.ParentClass.QuestFinished=true;g.ExitAlive=false;o.Main();ok=Check(Balanced(o)&&g.MoveCount==0&&g.FadeCount==0&&g.InfoClear>1,++n)&&ok;}
 {GameStub g;Buddy o(g);o.ParentClass.ExpressionDetectedWait=true;o.ParentClass.ExpressionDetectedFollow=true;o.Main();ok=Check(Balanced(o)&&!o.ParentClass.ExpressionDetectedWait&&!o.ParentClass.ExpressionDetectedFollow&&g.StateWander==1&&g.StateNone==1,++n)&&ok;}
 {GameStub g;Buddy o(g);g.DoAmbushAt=3;g.HeroDistance=4;g.QuestFinishedAt=5;o.Main();ok=Check(Balanced(o)&&g.FollowCalls==1&&g.StopFollow==1&&g.FadeCount==1&&!g.Following,++n)&&ok;}
 {GameStub g;Buddy o(g);g.DoAmbushAt=3;g.StartFailures=100;g.TerminateAt=6;o.Main();ok=Check(Balanced(o)&&!o.BuddyHeroApproachComplete&&g.FollowCalls==0&&g.Enemies.size()==6&&g.Starts==3,++n)&&ok;}
 {GameStub g;Buddy o(g);g.TalkAt.push_back(2);g.StopSpeech=1;o.Main();ok=Check(Balanced(o)&&g.SpeechCount==1&&!o.ParentClass.BuddyIntroComplete,++n)&&ok;}
 {GameStub g;Buddy o(g);g.TalkAt.push_back(2);g.StopSpeech=2;o.Main();ok=Check(Balanced(o)&&g.SpeechCount==2&&!o.ParentClass.BuddyIntroComplete,++n)&&ok;}
 {GameStub g;Buddy o(g);g.QuestFinishedAt=3;g.TerminateAt=4;o.Main();ok=Check(Balanced(o)&&g.MoveCount==1&&g.FadeCount==0&&g.InfoClear==1,++n)&&ok;}
 {GameStub g;Buddy o(g);g.DoAmbushAt=3;g.StopAcquire=1;o.Main();ok=Check(Balanced(o)&&!o.BuddyHeroApproachComplete&&g.Enemies.size()==6&&g.FollowCalls==0,++n)&&ok;}
 ok=ok&&Strings==0&&Threads==0;std::printf(ok?"00e2ff00_TEST PASS\n":"00e2ff00_TEST FAIL\n");return ok?0:1;}