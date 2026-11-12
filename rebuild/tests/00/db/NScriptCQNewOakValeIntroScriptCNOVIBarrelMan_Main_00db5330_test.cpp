// Complete original Main body with deterministic callee stubs.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers;
class CCharString{ public:std::string text;CCharString():text(){++Strings;}CCharString(const char* s):text(s){++Strings;}CCharString(const CCharString& s):text(s.text){++Strings;}~CCharString(){--Strings;}};
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0):X(x),Y(0),Z(0){}};
struct GameStub;static GameStub* Current;
class CTimer;static std::vector<CTimer*>TimerList;
class CTimer{ public:long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer&operator=(long v){time=v;return *this;}long GetTime()const{return time;}};
class CScriptThing{ public:int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}C3DVector GetPos()const;C3DVector GetHomePos()const{assert(id==1);return C3DVector(1);}bool MsgIsHitBy(const CCharString&)const;bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&)const;bool MsgIsHitBySpecialAbilityFrom(int,const CCharString&)const;bool MsgIsTalkedToBy(const CCharString&)const;};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CScriptGameResourceObjectScriptedThingBase{ public:int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release(){id=0;}CScriptThing GetScriptThing()const{assert(id==1);return CScriptThing(id);}void MoveToPosition(const C3DVector&,float,int,bool,bool);bool IsPerformingScriptTask()const;void Speak(const CScriptThing&,const char*,int,bool,bool,bool);};
class CRGBColour{ public:int R,G,B,A;CRGBColour(int r,int g,int b,int a):R(r),G(g),B(b),A(a){}};
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,HERO_ABILITY_HEAL_LIFE_SPELL=14,CUTSCENE_BEHAVIOUR_PAUSED=1,CUTSCENE_BEHAVIOUR_NOT_PAUSED=2,ESSG_WANDER_NEAR=4};
typedef int EScriptEntityMoveType;typedef int EBadDeeds;enum{BD_DERELICTION_OF_DUTY=1,BD_VIOLENCE=2};
struct Parent{CTimer WatchTimer;bool BarrelManSpokenToHeroOnReturn,BarrelManLeftHeroInCharge,BarrelBrokenPersistent;int GoodDeeds;std::vector<int>BadDeeds;Parent():BarrelManSpokenToHeroOnReturn(false),BarrelManLeftHeroInCharge(false),BarrelBrokenPersistent(false),GoodDeeds(0){}void AddGoodDeed(){++GoodDeeds;}void AddBadDeed(EBadDeeds d){BadDeeds.push_back(d);}};
struct GameStub{int Frame,TerminateAt,StartAt,TaskUntil,TaskLength,TalkAt,HitAt,HitKind,InfoAt,InfoDelay,PauseDepth,RandomValue,RandomCalls,Starts,Releases,AllyCalls,ClearInfo,FadeOut,FadeIn,PauseCalls,Targetable,Cutscene,MoveCalls,HeroTeleport,MyTeleport,CameraCalls,QuestInfo;bool PredicateOkay,Conscious,CameraOnScreen,Visible,Near10,Near15,Approach;float Health,MePosition;std::vector<std::string>Lines,Brains,Events;std::vector<int>TeleportTargets;
 GameStub():Frame(0),TerminateAt(60),StartAt(0),TaskUntil(0),TaskLength(1),TalkAt(-1),HitAt(-1),HitKind(0),InfoAt(0),InfoDelay(1),PauseDepth(0),RandomValue(1),RandomCalls(0),Starts(0),Releases(0),AllyCalls(0),ClearInfo(0),FadeOut(0),FadeIn(0),PauseCalls(0),Targetable(-1),Cutscene(-1),MoveCalls(0),HeroTeleport(0),MyTeleport(0),CameraCalls(0),QuestInfo(0),PredicateOkay(true),Conscious(true),CameraOnScreen(false),Visible(true),Near10(false),Near15(false),Approach(false),Health(10),MePosition(1){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<500);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int p){assert(t.id==1&&p==4);++Starts;if(Frame<StartAt)return false;h.id=1;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetThingWithScriptName(const CCharString& n){int id=n.text=="M_WHouse_ManStart"?3:n.text=="M_WHouse_GuardPoint"?4:n.text=="M_BarrelManWalkOff"?5:n.text=="M_BarrelManWalkOffAlt"?6:n.text=="M_BarrelManHiddenPos"?7:0;assert(id);return CScriptThing(id);}
 void SetCreatureBrain(const CScriptThing&t,const CCharString&n){assert(t.id==1);Brains.push_back(n.text);}void SetWanderCentrePoint(CScriptThing t,const C3DVector&){assert(t.id==1);}void SetWanderMinDistance(CScriptThing t,float f){assert(t.id==1&&f==0);}void SetWanderMaxDistance(CScriptThing t,float f){assert(t.id==1&&f==1);}void SetScriptingStateGroup(CScriptThing t,int e){assert(t.id==1&&e==4);}
 void EntitySetTargetable(const CScriptThing&t,bool b){assert(t.id==1);Targetable=b?1:0;}void EntitySetCutsceneBehaviour(const CScriptThing&t,int e){assert(t.id==1&&(e==1||e==2));Cutscene=e;}void EntitySetFacingAngleTowardsThing(const CScriptThing&t,const CScriptThing&h,bool b){assert(t.id==1&&h.id==2&&!b);}bool CanThingBe_Seen_ByOtherThing(const CScriptThing&h,const CScriptThing&t){assert(h.id==2&&t.id==1);return Visible;}
 bool IsCameraPosOnScreen(const C3DVector&p){assert(p.X==5);++CameraCalls;return CameraOnScreen;}void EntityTeleportToThing(const CScriptThing&t,const CScriptThing&p,bool b){assert(!b);TeleportTargets.push_back(p.id);if(t.id==1){MePosition=p.id;++MyTeleport;}else{assert(t.id==2&&p.id==4);++HeroTeleport;}}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);return 1;}void AddLineToConversation(long i,const CCharString&n,bool b,const CScriptThing&t,const CScriptThing&to){assert(i==1&&!b&&t.id==1&&to.id==0);Lines.push_back(n.text);}
 void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}void DisplayGameInfo(const CCharString&n){assert(n.text=="TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED");InfoAt=Frame+InfoDelay;++QuestInfo;}bool MsgIsGameInfoClickedPast(){return Frame>=InfoAt;}float GetHealth(const CScriptThing&t){assert(t.id==1);return Health;}
 void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert((a.id==1&&b.id==2)||(a.id==2&&b.id==1));++AllyCalls;}void FadeScreenOut(float a,float b,const CRGBColour&c){assert(a==1&&b==1&&c.R==0&&c.G==0&&c.B==0&&c.A==255);++FadeOut;}void FadeScreenIn(){++FadeIn;}void Pause(float f){assert(f==2);++PauseCalls;}void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);++ClearInfo;}
};
C3DVector CScriptThing::GetPos()const{return C3DVector(id==1?Current->MePosition:(float)id);}
bool CScriptThing::MsgIsHitBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->HitKind==1&&Current->Frame==Current->HitAt;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->HitKind>=2&&Current->Frame==Current->HitAt;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const CCharString&n)const{assert(id==1&&a==14&&n.text=="SCRIPT_NAME_HERO");return Current->HitKind==3&&Current->Frame==Current->HitAt;}
bool CScriptThing::MsgIsTalkedToBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->TalkAt;}
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector&p,float r,int m,bool a,bool b){assert(id==1&&m==1&&!a&&((p.X==2&&r==2&&b)||(p.X!=2&&r==0&&!b)));Current->MePosition=p.X;Current->TaskUntil=Current->Frame+Current->TaskLength;++Current->MoveCalls;Current->Approach=false;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&h,const char*n,int g,bool a,bool b,bool c){assert(id==1&&h.id==2&&g==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+Current->TaskLength;}
struct CWideScreenMagicPauseEntities{GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};struct CIsThingConscious{bool b;CIsThingConscious(const CScriptThing&t):b(Current->Conscious){assert(t.id==1);}};struct TRetVoid{void Return(){}};
static bool IsDistanceFromThingToPositionOver(const CScriptThing&t,const C3DVector&p,float r){assert(t.id==1&&(r==2||r==4));return p.X==2?Current->Approach:Current->MePosition!=p.X;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float r){assert(a.id==1&&b.id==2);assert(r==10||r==15);return r==10?Current->Near10:Current->Near15;}
static int StubRand(){++Current->RandomCalls;return Current->RandomValue;}
#define rand StubRand
struct BarrelMan{enum EMyPhase{PHASE_WAITING=0,PHASE_WALKING_AWAY=1,PHASE_AWAY=2,PHASE_RETURNING=3,PHASE_RETURNED=4,PHASE_OVER=5};GameStub*Game;CScriptThing Me;Parent ParentClass;bool ComplainedAboutStock,HeroLetMeDown,OverheardYet;EMyPhase MyPhase;BarrelMan(GameStub&g,int p):Game(&g),Me(1),ComplainedAboutStock(false),HeroLetMeDown(false),OverheardYet(false),MyPhase((EMyPhase)p){}void SetPredicate(const CIsThingConscious&p){Game->PredicateOkay=p.b;}bool IsActiveThreadTerminating()const{return !Game->PredicateOkay||Game->Frame>=Game->TerminateAt;}void Main();};
void BarrelMan::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    static long need_wee_frequency = 100;
    static float barrelman_near_distance = 10.0f;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.SetCreatureBrain(Me, "BRAIN_PASSIVE_OVERRIDE");
    Game.SetWanderCentrePoint(Me, Me.GetHomePos());
    Game.SetWanderMinDistance(Me, 0.0f);
    Game.SetWanderMaxDistance(Me, 1.0f);
    Game.SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    CScriptThing guard_man_start_position = Game.GetThingWithScriptName("M_WHouse_ManStart");
    CScriptThing guard_hero_start_position = Game.GetThingWithScriptName("M_WHouse_GuardPoint");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (MyPhase != PHASE_WAITING) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            switch (MyPhase) {
            case PHASE_WALKING_AWAY: {
                ParentClass.WatchTimer = 45;
                Game.EntitySetTargetable(Me, false);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing walk_away_pos = Game.GetThingWithScriptName("M_BarrelManWalkOff");
                EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                float local_range = 2.0f;
                C3DVector local_position = walk_away_pos.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                    Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(local_position, 0.0f, local_movetype, false, false);
                    while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                MyPhase = PHASE_AWAY;
                break;
            }
            case PHASE_AWAY: {
                while (ParentClass.WatchTimer.GetTime() != 15) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing return_pos_1 = Game.GetThingWithScriptName("M_BarrelManWalkOff");
                CScriptThing return_pos_2 = Game.GetThingWithScriptName("M_BarrelManWalkOffAlt");
                if (!Game.IsCameraPosOnScreen(return_pos_1.GetPos())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntityTeleportToThing(Me, return_pos_1, false);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntityTeleportToThing(Me, return_pos_2, false);
                }
                MyPhase = PHASE_RETURNING;
                break;
            }
            case PHASE_RETURNING: {
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
                Game.EntitySetTargetable(Me, false);
                {
                    EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                    float local_range = 2.0f;
                    C3DVector local_position = guard_man_start_position.GetPos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                        Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(local_position, 0.0f, local_movetype, false, false);
                        while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                MyPhase = PHASE_RETURNED;
                break;
            }
            case PHASE_RETURNED:
                ParentClass.BarrelManSpokenToHeroOnReturn = true;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                Game.EntitySetTargetable(Me, true);
                if (!Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(), Me) && !IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 10.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE", false, Me, CScriptThing());
                    HeroLetMeDown = true;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.DisplayGameInfo("TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED");
                    while (!Game.MsgIsGameInfoClickedPast()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.AddBadDeed(BD_DERELICTION_OF_DUTY);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_THANKS", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.AddGoodDeed();
                }
                Game.SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
                MyPhase = PHASE_OVER;
                break;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
            ParentClass.AddBadDeed(BD_VIOLENCE);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_CAREFUL", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        } else if ((MyPhase == PHASE_WAITING && IsDistanceBetweenThingsUnder(Me, Game.GetHero(), barrelman_near_distance)) || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (MyPhase == PHASE_WAITING) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CTimer WaitTimer;
                while (IsDistanceFromThingToPositionOver(Me, Game.GetHero().GetPos(), 4.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(Game.GetHero().GetPos(), 2.0f, ENTITY_MOVE_RUN, false, true);
                    WaitTimer = 2;
                    while (seh_me.IsPerformingScriptTask() && WaitTimer.GetTime() > 0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_FAVOUR", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.FadeScreenOut(1.0f, 1.0f, CRGBColour(0,0,0,255));
                Game.Pause(2.0f);
                Game.EntityTeleportToThing(Game.GetHero(), Game.GetThingWithScriptName("M_WHouse_GuardPoint"), false);
                Game.EntityTeleportToThing(Me, Game.GetThingWithScriptName("M_BarrelManHiddenPos"), false);
                MyPhase = PHASE_AWAY;
                Game.ClearThingHasInformation(Me);
                Game.FadeScreenIn();
                ParentClass.WatchTimer = 45;
                ParentClass.BarrelManLeftHeroInCharge = true;
            } else if (MyPhase < PHASE_RETURNED) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_NOT_LARKING", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (MyPhase == PHASE_OVER) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (HeroLetMeDown) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.BarrelBrokenPersistent) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_NO_TIME", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (MyPhase == PHASE_WAITING && (!OverheardYet || rand() % need_wee_frequency == 0) && IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            OverheardYet = true;
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_048_BARRELMAN_OVERHEAR", false, Me, CScriptThing());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}

#undef rand
static bool Balanced(const GameStub&g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==1&&TimerList.size()==1&&g.PauseDepth==0;}
static int Count(const GameStub&g,const char*n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}
static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 for(int camera=0;camera<2;++camera){GameStub g;BarrelMan o(g,1);g.CameraOnScreen=camera!=0;o.Main();ok=Check(Balanced(g)&&o.MyPhase==BarrelMan::PHASE_OVER&&g.MoveCalls==2&&g.CameraCalls==1&&g.TeleportTargets.size()==1&&g.TeleportTargets[0]==(camera?6:5)&&o.ParentClass.GoodDeeds==1&&o.ParentClass.BadDeeds.empty()&&!o.HeroLetMeDown&&o.ParentClass.BarrelManSpokenToHeroOnReturn&&g.Targetable==1&&g.Cutscene==2&&Count(g,"TEXT_QST_048_BARRELMAN_THANKS")==1&&g.Brains.size()==2,++n)&&ok;}
 {GameStub g;BarrelMan o(g,4);g.Visible=false;o.Main();ok=Check(Balanced(g)&&o.MyPhase==BarrelMan::PHASE_OVER&&o.HeroLetMeDown&&o.ParentClass.BadDeeds.size()==1&&o.ParentClass.BadDeeds[0]==1&&o.ParentClass.GoodDeeds==0&&g.QuestInfo==1&&Count(g,"TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE")==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,4);g.Visible=false;g.Near10=true;o.Main();ok=Check(Balanced(g)&&o.ParentClass.GoodDeeds==1&&!o.HeroLetMeDown&&g.QuestInfo==0,++n)&&ok;}
 for(int kind=1;kind<=3;++kind){GameStub g;BarrelMan o(g,5);g.HitKind=kind;g.HitAt=2;g.TerminateAt=9;o.Main();ok=Check(Balanced(g)&&g.AllyCalls==(kind==3?0:2)&&o.ParentClass.BadDeeds.size()==(kind==3?0u:1u)&&Count(g,"TEXT_QST_048_BARRELMAN_CAREFUL")==int(kind!=3),++n)&&ok;}
 {GameStub g;BarrelMan o(g,0);g.Near10=true;g.TerminateAt=20;g.Approach=true;o.Main();ok=Check(Balanced(g)&&o.MyPhase==BarrelMan::PHASE_AWAY&&o.ParentClass.BarrelManLeftHeroInCharge&&o.ParentClass.WatchTimer.time>15&&g.ClearInfo==1&&g.HeroTeleport==1&&g.MyTeleport==1&&g.MoveCalls==1&&g.FadeOut==1&&g.FadeIn==1&&g.PauseCalls==1&&Count(g,"TEXT_QST_048_BARRELMAN_FAVOUR")==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,2);g.TalkAt=1;g.TerminateAt=5;o.ParentClass.WatchTimer=0;o.Main();ok=Check(Balanced(g)&&o.MyPhase==BarrelMan::PHASE_AWAY&&Count(g,"TEXT_QST_048_BARRELMAN_NOT_LARKING")==0&&g.Frame==5,++n)&&ok;}
 // Phase1 exits the walk into phase2, then the talk message runs before the timer wait starts next frame.
 {GameStub g;BarrelMan o(g,1);g.TalkAt=3;g.TerminateAt=8;o.Main();ok=Check(Balanced(g)&&Count(g,"TEXT_QST_048_BARRELMAN_NOT_LARKING")==1&&o.MyPhase==BarrelMan::PHASE_AWAY,++n)&&ok;}
 for(int reply=0;reply<3;++reply){GameStub g;BarrelMan o(g,5);o.HeroLetMeDown=reply!=0;o.ParentClass.BarrelBrokenPersistent=reply==2;g.TalkAt=2;g.TerminateAt=10;o.Main();const char*line=reply==0?"TEXT_QST_048_BARRELMAN_NO_TIME":reply==1?"TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE":"TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN";ok=Check(Balanced(g)&&Count(g,line)==1&&o.ParentClass.GoodDeeds==0&&o.ParentClass.BadDeeds.empty(),++n)&&ok;}
 {GameStub g;BarrelMan o(g,0);g.Near15=true;g.TerminateAt=8;o.Main();ok=Check(Balanced(g)&&o.OverheardYet&&g.RandomCalls==6&&Count(g,"TEXT_QST_048_BARRELMAN_OVERHEAR")==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,0);o.OverheardYet=true;g.Near15=true;g.RandomValue=100;g.TerminateAt=5;o.Main();ok=Check(Balanced(g)&&g.RandomCalls==4&&Count(g,"TEXT_QST_048_BARRELMAN_OVERHEAR")==4,++n)&&ok;}
 {GameStub g;BarrelMan o(g,0);g.Conscious=false;o.Main();ok=Check(Balanced(g)&&g.Frame==1&&g.Brains.empty(),++n)&&ok;}
 {GameStub g;BarrelMan o(g,1);g.StartAt=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.Frame==4&&g.MoveCalls==0&&o.MyPhase==BarrelMan::PHASE_WALKING_AWAY&&o.ParentClass.WatchTimer.time==42,++n)&&ok;}
 {GameStub g;BarrelMan o(g,1);g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.MoveCalls==1&&o.MyPhase==BarrelMan::PHASE_WALKING_AWAY,++n)&&ok;}
 {GameStub g;BarrelMan o(g,4);g.Visible=false;g.InfoDelay=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&o.HeroLetMeDown&&o.ParentClass.BadDeeds.empty()&&o.MyPhase==BarrelMan::PHASE_RETURNED&&g.Brains.size()==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,4);g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&o.ParentClass.GoodDeeds==0&&o.MyPhase==BarrelMan::PHASE_RETURNED&&Count(g,"TEXT_QST_048_BARRELMAN_THANKS")==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,5);g.TalkAt=2;g.Health=0;g.TerminateAt=6;o.Main();ok=Check(Balanced(g)&&g.Lines.empty()&&g.Starts==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,0);g.Near10=true;g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&o.MyPhase==BarrelMan::PHASE_WAITING&&g.FadeOut==0&&Count(g,"TEXT_QST_048_BARRELMAN_FAVOUR")==1&&Timers==1,++n)&&ok;}
 {GameStub g;BarrelMan o(g,0);g.Near10=true;g.Approach=true;g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.MoveCalls==1&&g.Lines.empty()&&o.MyPhase==BarrelMan::PHASE_WAITING,++n)&&ok;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty();std::printf(ok?"00db5330_TEST PASS\n":"00db5330_TEST FAIL\n");return ok?0:1;
}