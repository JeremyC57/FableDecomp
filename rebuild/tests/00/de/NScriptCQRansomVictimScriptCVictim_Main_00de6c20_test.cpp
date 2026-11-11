// Whole source Main body with deterministic callee/resource stubs.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers,Threads;
class CCharString{ public:std::string text;CCharString():text(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}};
static CCharString operator+(const CCharString&a,const CCharString&b){return CCharString((a.text+b.text).c_str());}static CCharString operator+(const char*a,const CCharString&b){return CCharString((std::string(a)+b.text).c_str());}static CCharString GFIntToCharString(long n){char b[32];std::sprintf(b,"%ld",n);return CCharString(b);}
struct GameStub;static GameStub*Current;
class CScriptThing{ public:int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}bool MsgIsTalkedToBy(const CCharString&)const;bool MsgIsHitBy(const CCharString&)const;bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&)const;bool MsgIsHitBySpecialAbilityFrom(int,const CCharString&)const;bool IsDead()const;};
class CTimer;static std::vector<CTimer*>TimerList;
class CTimer{ public:long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer&operator=(long t){time=t;return *this;}long GetTime()const{return time;}};
class CScriptGameResourceObjectScriptedThingBase{ public:int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release();CScriptThing GetScriptThing()const{return CScriptThing(id);}void Speak(const CScriptThing&,const char*,int,bool,bool,bool);bool IsPerformingScriptTask()const;void MoveToThing(const CScriptThing&,float,int,bool,bool,bool,bool);};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CRGBColour{ public:int R,G,B,A;CRGBColour(int r,int g,int b,int a):R(r),G(g),B(b),A(a){}};
struct ThreadBase{ThreadBase(){++Threads;}virtual~ThreadBase(){--Threads;}virtual bool Valid(void*)const=0;};
template<class T,class P1>struct CSpawnedFunc1:ThreadBase{CCharString Name;unsigned long Id;T*Owner;void(T::*Func)(P1);P1 Arg;CSpawnedFunc1(const CCharString&n,unsigned long i,T*p,void(T::*f)(P1),P1 a):Name(n),Id(i),Owner(p),Func(f),Arg(a){}bool Valid(void*p)const{return Name.text=="ParentClass.CheckVictimHit"&&Id==0&&Owner==p&&Func==&T::CheckVictimHit&&Arg.id==1;}};
struct Parent{CScriptGameResourceObjectScriptedThingBase seh_Victim;CCharString ActiveEnemyName;bool RescuedVictim,RansommerDied,ThanksCutsceneStarting;int Cutscenes;std::vector<ThreadBase*>Spawned;Parent():ActiveEnemyName("Bandit"),RescuedVictim(false),RansommerDied(false),ThanksCutsceneStarting(false),Cutscenes(0){}~Parent(){for(unsigned i=0;i<Spawned.size();++i)delete Spawned[i];}void AddSpawnedFunction(ThreadBase*t,const CCharString&n){assert(n.text.empty()&&t->Valid(this));Spawned.push_back(t);}void CheckVictimHit(CScriptThing){}void PlayCutscene(const CCharString&,const CCharString&);};typedef Parent CQ_RansomVictimScript;
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_RUN=1,HERO_ABILITY_HEAL_LIFE_SPELL=14};
struct GameStub{Parent*P;int Frame,TerminateAt,RescueAt,StartAt,FailAfter,Starts,SuccessfulStarts,TaskUntil,TaskLength,ReleaseCalls,CutsceneAt,WorldAt,EnemyNearAt,EnemyFarAt,EnemyDeadAt,HeroNearAt,HitKind,RandomValue,PauseDepth,Markers,ShowInfo,HealthBars,PushCalls,AllowedOn,AllowedOff,RegionOn,RegionOff,OpinionOff,FollowCalls,StopCalls,ScaredOn,ScaredOff,MoveCalls,AllyCalls;bool Alive,Cavern,PredicateOkay,Missing20;float Health;std::vector<int>TalkFrames,HitFrames;std::vector<std::string>Lines,TextChecks;
 GameStub():P(0),Frame(0),TerminateAt(20),RescueAt(3),StartAt(0),FailAfter(100),Starts(0),SuccessfulStarts(0),TaskUntil(0),TaskLength(1),ReleaseCalls(0),CutsceneAt(-1),WorldAt(-1),EnemyNearAt(-1),EnemyFarAt(-1),EnemyDeadAt(-1),HeroNearAt(-1),HitKind(0),RandomValue(0),PauseDepth(0),Markers(0),ShowInfo(0),HealthBars(0),PushCalls(0),AllowedOn(0),AllowedOff(0),RegionOn(0),RegionOff(0),OpinionOff(0),FollowCalls(0),StopCalls(0),ScaredOn(0),ScaredOff(0),MoveCalls(0),AllyCalls(0),Alive(true),Cavern(false),PredicateOkay(true),Missing20(false),Health(10){Current=this;}
 bool At(const std::vector<int>&v)const{for(unsigned i=0;i<v.size();++i)if(v[i]==Frame)return true;return false;}
 void NewScriptFrame(){++Frame;assert(Frame<400);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(P){if(RescueAt>=0&&Frame>=RescueAt)P->RescuedVictim=true;if(CutsceneAt>=0&&Frame>=CutsceneAt)P->ThanksCutsceneStarting=true;}}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int pri){assert(t.id==1&&pri==4);++Starts;if(Frame<StartAt||SuccessfulStarts>=FailAfter)return false;h.id=1;++SuccessfulStarts;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetNearestWithScriptName(const CScriptThing&t,const CCharString&n){assert(t.id==1&&n.text=="Bandit");return CScriptThing(4);}
 void MiniMapAddMarker(const CScriptThing&t,const CCharString&n){assert(t.id==1&&n.text=="HUD_ORB_GREEN_SMALL");++Markers;}void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}float GetHealth(const CScriptThing&t){assert(t.id==1);return Health;}
 void EntitySetAsAllowedToFollowHero(const CScriptThing&t,bool b){assert(t.id==1);b?++AllowedOn:++AllowedOff;}void SetEntityAsRegionFollowing(const CScriptThing&h,const CScriptThing&t,bool b){assert(h.id==2&&t.id==1);b?++RegionOn:++RegionOff;}void DisplayQuestInfo(bool b){assert(b);++ShowInfo;}void EntitySetOpinionReactionsEnabled(const CScriptThing&t,bool b){assert(t.id==1&&!b);++OpinionOff;}void AddQuestInfoBarHealth(const CScriptThing&t,const CRGBColour&c,const CCharString&n,float f){assert(t.id==1&&c.R==255&&c.G==0&&c.B==0&&c.A==255&&n.text=="HUD_QUEST_ICON_GRANDSON"&&f==1);++HealthBars;}void EntityFollowThing(const CScriptThing&t,const CScriptThing&h,float f,bool b){assert(t.id==1&&h.id==2&&f==1&&b);++FollowCalls;}void SetIsPushableByHero(const CScriptThing&t,bool b){assert(t.id==1&&b);++PushCalls;}
 bool IsRegionLoaded(const CCharString&n){if(n.text=="Witchwood2")return WorldAt>=0&&Frame>=WorldAt;assert(n.text=="WitchwoodCavern");return Cavern;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);return 7;}void AddPersonToConversation(long i,const CScriptThing&h){assert(i==7&&h.id==2);}void AddLineToConversation(long i,const CCharString&n,bool b,const CScriptThing&t,const CScriptThing&h){assert(i==7&&!b&&t.id==1&&h.id==2);Lines.push_back(n.text);}void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert((a.id==1&&b.id==2)||(a.id==2&&b.id==1));++AllyCalls;}bool TextEntryExists(const CCharString&n){TextChecks.push_back(n.text);return !(Missing20&&n.text=="TEXT_QST_B06_VICTIM_ON_HIT_20");}void EntityStopFollowing(const CScriptThing&t){assert(t.id==1);++StopCalls;}void EntitySetAsScared(const CScriptThing&t,bool b){assert(t.id==1);b?++ScaredOn:++ScaredOff;}
 bool EnemyNear()const{return EnemyNearAt>=0&&Frame>=EnemyNearAt&&(EnemyFarAt<0||Frame<EnemyFarAt);}bool EnemyDead()const{return EnemyDeadAt>=0&&Frame>=EnemyDeadAt;}
};
void Parent::PlayCutscene(const CCharString&a,const CCharString&b){assert(a.text=="CS_RANSOM_THANKS"&&b.text=="NULL"&&Current->PauseDepth==1);++Cutscenes;}
void CScriptGameResourceObjectScriptedThingBase::Release(){id=0;++Current->ReleaseCalls;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&h,const char*n,int g,bool a,bool b,bool c){assert(id==1&&h.id==2&&g==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+Current->TaskLength;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing&h,float f,int m,bool a,bool b,bool c,bool d){assert(h.id==2&&f==3&&m==1&&!a&&b&&!c&&d);++Current->MoveCalls;}
bool CScriptThing::MsgIsTalkedToBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->At(Current->TalkFrames);}bool CScriptThing::MsgIsHitBy(const CCharString&n)const{assert(id==1&&(n.text.empty()||n.text=="SCRIPT_NAME_HERO"));return Current->At(Current->HitFrames)&&(Current->HitKind==3||(n.text.empty()&&Current->HitKind==1));}bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&n)const{assert(id==1&&(n.text.empty()||n.text=="SCRIPT_NAME_HERO"));return Current->At(Current->HitFrames)&&(Current->HitKind>=4||(n.text.empty()&&Current->HitKind==2));}bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const CCharString&n)const{assert(id==1&&a==14&&n.text=="SCRIPT_NAME_HERO");return Current->At(Current->HitFrames)&&Current->HitKind==5;}bool CScriptThing::IsDead()const{assert(id==4);return Current->EnemyDead();}
struct CIsThingAlive{bool b;CIsThingAlive(const CScriptThing&t):b(Current->Alive){assert(t.id==1);}};struct TRetVoid{void Return(){}};struct CWideScreenMagicPauseEntities{GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float f){assert(a.id==1);if(b.id==2){assert(f==14);return Current->HeroNearAt>=0&&Current->Frame>=Current->HeroNearAt;}assert(b.id==4&&f==15);return Current->EnemyNear();}static bool IsDistanceBetweenThingsOver(const CScriptThing&a,const CScriptThing&b,float f){assert(a.id==1&&b.id==4&&f==15);return !Current->EnemyNear();}
static int StubRand(){return Current->RandomValue;}
#define rand StubRand
struct Victim{GameStub*Game;CScriptThing Me;Parent ParentClass;bool WaitingForThingsToDie,ToldHeroOfThingsAttackingHim,GotOutOfCave;Victim(GameStub&g):Game(&g),Me(1),WaitingForThingsToDie(false),ToldHeroOfThingsAttackingHim(true),GotOutOfCave(false){g.P=&ParentClass;}void SetPredicate(const CIsThingAlive&p){Game->PredicateOkay=p.b;}bool IsActiveThreadTerminating()const{return !Game->PredicateOkay||Game->Frame>=Game->TerminateAt;}void Main();};
void Victim::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
    while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_RansomVictimScript, CScriptThing>(CCharString("ParentClass.") + CCharString("CheckVictimHit"), 0, &ParentClass, &CQ_RansomVictimScript::CheckVictimHit, Me), CCharString(""));
    while (!ParentClass.RescuedVictim) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_HELP_10", GROUP_SELECT_NONE, false, true, false);
                while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass.RansommerDied) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        ParentClass.PlayCutscene("CS_RANSOM_THANKS", "NULL");
        GotOutOfCave = true;
    }
    Game.EntitySetAsAllowedToFollowHero(Me, true);
    Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
    Game.DisplayQuestInfo(true);
    Game.EntitySetOpinionReactionsEnabled(Me, false);
    Game.AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_GRANDSON", 1.0f);
    Game.EntityFollowThing(Me, Game.GetHero(), 1.0f, true);
    Game.SetIsPushableByHero(Me, true);
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
    CTimer BoyResponseTimer;
    long NextLine = 10;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!GotOutOfCave && Game.IsRegionLoaded("Witchwood2")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            GotOutOfCave = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.IsRegionLoaded("WitchwoodCavern") && !GotOutOfCave) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
                while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_THANKS", GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
                GotOutOfCave = true;
            } else if (WaitingForThingsToDie) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.RansommerDied) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_WAITING_FOR_WAY_CLEARED_BANDITS_10", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(ParentClass.seh_Victim.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Victim.Speak(Game.GetHero(), "TEXT_QST_B06_VICTIM_WAITING_FOR_WAY_CLEARED_CREATURES_10", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Victim.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_VICTIM_NAGGING_" + GFIntToCharString(rand() % 4), false, Me, Game.GetHero());
            }
        }
        if ((Me.MsgIsHitBy("") || (Me.MsgIsHitByAnySpecialAbilityFrom("") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.seh_Victim.MoveToThing(Game.GetHero(), 3.0f, ENTITY_MOVE_RUN, false, true, false, true);
            ToldHeroOfThingsAttackingHim = false;
        }
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && BoyResponseTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_VICTIM_ON_HERO_HIT", false, Me, Game.GetHero());
            BoyResponseTimer = 5;
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
        if (!ToldHeroOfThingsAttackingHim && IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 14.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (BoyResponseTimer.GetTime() == 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!Game.TextEntryExists("TEXT_QST_B06_VICTIM_ON_HIT_" + GFIntToCharString(NextLine))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    NextLine = 10;
                }
                long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_VICTIM_ON_HIT_" + GFIntToCharString(NextLine), false, Me, Game.GetHero());
                BoyResponseTimer = 5;
                NextLine += 10;
            }
            if (WaitingForThingsToDie) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityStopFollowing(Me);
            }
            ToldHeroOfThingsAttackingHim = true;
        }
        if (!WaitingForThingsToDie && IsDistanceBetweenThingsUnder(Me, Game.GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            WaitingForThingsToDie = true;
            Game.EntityStopFollowing(Me);
            Game.EntitySetAsAllowedToFollowHero(Me, false);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, false);
            Game.EntitySetAsScared(Me, true);
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (ParentClass.ThanksCutsceneStarting) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntityStopFollowing(Me);
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
            while (!Game.StartScriptingEntity(Me, ParentClass.seh_Victim, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            while (!IsActiveThreadTerminating()) {
                Game.NewScriptFrame();
            }
            return _Fail_Val.Return();
        }
        if (WaitingForThingsToDie && (Game.GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName).IsDead() || IsDistanceBetweenThingsOver(Me, Game.GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName), 15.0f))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            WaitingForThingsToDie = false;
            Game.EntitySetAsAllowedToFollowHero(Me, true);
            Game.SetEntityAsRegionFollowing(Game.GetHero(), Me, true);
            Game.EntitySetOpinionReactionsEnabled(Me, false);
            Game.EntityFollowThing(Me, Game.GetHero(), 1.0f, true);
            Game.EntitySetAsScared(Me, false);
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Victim.IsValid()) parent.seh_Victim.Release(); }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}

#undef rand
static int Count(const GameStub&g,const char*n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}
static bool Balanced(const GameStub&g,const Victim&o){int spawn=(int)o.ParentClass.Spawned.size();return Strings==1+spawn&&Things==1+spawn&&Handles==1&&Movies==0&&Timers==0&&Threads==spawn&&TimerList.empty()&&g.PauseDepth==0;}
static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 {GameStub g;Victim o(g);g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g,o)&&o.ParentClass.Spawned.size()==1&&g.Markers==1&&Count(g,"TEXT_QST_B06_VICTIM_HELP_10")==1&&g.ShowInfo==1&&g.HealthBars==1&&g.AllowedOn==1&&g.FollowCalls==1&&g.RegionOn==1&&g.OpinionOff==1&&g.PushCalls==1&&!o.GotOutOfCave&&g.ReleaseCalls==1,++n)&&ok;}
 {GameStub g;Victim o(g);o.ParentClass.RansommerDied=true;o.Main();ok=Check(Balanced(g,o)&&o.GotOutOfCave&&o.ParentClass.Cutscenes==1&&g.FollowCalls==1,++n)&&ok;}
 {GameStub g;Victim o(g);g.WorldAt=4;o.Main();ok=Check(Balanced(g,o)&&o.GotOutOfCave&&o.ParentClass.Cutscenes==0,++n)&&ok;}
 {GameStub g;Victim o(g);g.Cavern=true;g.TalkFrames.push_back(5);o.Main();ok=Check(Balanced(g,o)&&o.GotOutOfCave&&Count(g,"TEXT_QST_B06_VICTIM_THANKS")==1&&g.SuccessfulStarts==2&&o.ParentClass.seh_Victim.id==0,++n)&&ok;}
 for(int died=0;died<2;++died){GameStub g;Victim o(g);o.ParentClass.RansommerDied=died!=0;g.EnemyNearAt=3;g.TalkFrames.push_back(5);o.Main();ok=Check(Balanced(g,o)&&o.WaitingForThingsToDie&&Count(g,died?"TEXT_QST_B06_VICTIM_WAITING_FOR_WAY_CLEARED_BANDITS_10":"TEXT_QST_B06_VICTIM_WAITING_FOR_WAY_CLEARED_CREATURES_10")==1&&g.ScaredOn==1&&g.AllowedOff==1&&g.RegionOff==1&&g.StopCalls==1&&o.ParentClass.seh_Victim.id==1,++n)&&ok;}
 for(int random=0;random<4;++random){GameStub g;Victim o(g);g.RandomValue=random;g.TalkFrames.push_back(5);o.Main();char line[80];std::sprintf(line,"TEXT_QST_B06_VICTIM_NAGGING_%d",random);ok=Check(Balanced(g,o)&&Count(g,line)==1&&g.MoveCalls==0&&g.AllyCalls==0,++n)&&ok;}
 for(int hit=1;hit<=2;++hit){GameStub g;Victim o(g);g.HitKind=hit;g.HitFrames.push_back(5);g.HeroNearAt=5;o.Main();ok=Check(Balanced(g,o)&&g.MoveCalls==1&&g.AllyCalls==0&&Count(g,"TEXT_QST_B06_VICTIM_ON_HIT_10")==1&&o.ToldHeroOfThingsAttackingHim,++n)&&ok;}
 for(int hit=3;hit<=5;++hit){GameStub g;Victim o(g);g.HitKind=hit;g.HitFrames.push_back(4);g.HitFrames.push_back(5);g.HitFrames.push_back(10);o.Main();ok=Check(Balanced(g,o)&&g.MoveCalls==0&&g.AllyCalls==(hit==5?0:4)&&Count(g,"TEXT_QST_B06_VICTIM_ON_HERO_HIT")==int(hit==5?0:2),++n)&&ok;}
 {GameStub g;Victim o(g);g.HitKind=1;g.HitFrames.push_back(5);g.HitFrames.push_back(12);g.HeroNearAt=5;g.Missing20=true;o.Main();ok=Check(Balanced(g,o)&&g.MoveCalls==2&&Count(g,"TEXT_QST_B06_VICTIM_ON_HIT_10")==2&&Count(g,"TEXT_QST_B06_VICTIM_ON_HIT_20")==0&&g.TextChecks.size()==2&&g.TextChecks[1]=="TEXT_QST_B06_VICTIM_ON_HIT_20",++n)&&ok;}
 for(int death=0;death<2;++death){GameStub g;Victim o(g);g.EnemyNearAt=3;if(death){g.EnemyDeadAt=8;g.EnemyFarAt=9;}else g.EnemyFarAt=8;o.Main();ok=Check(Balanced(g,o)&&!o.WaitingForThingsToDie&&g.FollowCalls==2&&g.AllowedOn==2&&g.RegionOn==2&&g.ScaredOn==1&&g.ScaredOff==1&&o.ParentClass.seh_Victim.id==0,++n)&&ok;}
 {GameStub g;Victim o(g);g.EnemyNearAt=3;g.HeroNearAt=4;g.HitKind=1;g.HitFrames.push_back(4);o.Main();ok=Check(Balanced(g,o)&&o.WaitingForThingsToDie&&g.StopCalls==2&&Count(g,"TEXT_QST_B06_VICTIM_ON_HIT_10")==1&&o.ToldHeroOfThingsAttackingHim,++n)&&ok;}
 {GameStub g;Victim o(g);g.CutsceneAt=5;o.Main();ok=Check(Balanced(g,o)&&o.ParentClass.ThanksCutsceneStarting&&g.StopCalls==1&&g.SuccessfulStarts==2&&o.ParentClass.seh_Victim.id==1&&g.Frame==20,++n)&&ok;}
 {GameStub g;Victim o(g);g.Alive=false;o.Main();ok=Check(Balanced(g,o)&&g.Frame==1&&g.Starts==0&&o.ParentClass.Spawned.empty()&&g.Markers==0,++n)&&ok;}
 {GameStub g;Victim o(g);g.StartAt=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g,o)&&g.Frame==4&&g.SuccessfulStarts==0&&o.ParentClass.Spawned.empty()&&g.ShowInfo==0,++n)&&ok;}
 {GameStub g;Victim o(g);g.TalkFrames.push_back(2);g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g,o)&&g.ShowInfo==0&&Count(g,"TEXT_QST_B06_VICTIM_HELP_10")==1,++n)&&ok;}
 {GameStub g;Victim o(g);g.Cavern=true;g.TalkFrames.push_back(5);g.TaskLength=100;g.TerminateAt=7;o.Main();ok=Check(Balanced(g,o)&&!o.GotOutOfCave&&Count(g,"TEXT_QST_B06_VICTIM_THANKS")==1&&o.ParentClass.seh_Victim.id==1,++n)&&ok;}
 {GameStub g;Victim o(g);g.EnemyNearAt=3;g.FailAfter=1;g.TerminateAt=6;o.Main();ok=Check(Balanced(g,o)&&o.WaitingForThingsToDie&&g.ScaredOn==1&&g.SuccessfulStarts==1&&o.ParentClass.seh_Victim.id==0,++n)&&ok;}
 {GameStub g;Victim o(g);g.CutsceneAt=5;g.FailAfter=1;g.TerminateAt=7;o.Main();ok=Check(Balanced(g,o)&&g.StopCalls==1&&o.ParentClass.ThanksCutsceneStarting&&g.SuccessfulStarts==1,++n)&&ok;}
 {GameStub g;Victim o(g);g.Health=0;g.TalkFrames.push_back(2);g.Cavern=true;g.TalkFrames.push_back(5);o.Main();ok=Check(Balanced(g,o)&&g.Lines.empty()&&o.GotOutOfCave&&g.ShowInfo==1&&g.SuccessfulStarts==2,++n)&&ok;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0&&Threads==0&&TimerList.empty();std::printf(ok?"00de6c20_TEST PASS\n":"00de6c20_TEST FAIL\n");return ok?0:1;
}