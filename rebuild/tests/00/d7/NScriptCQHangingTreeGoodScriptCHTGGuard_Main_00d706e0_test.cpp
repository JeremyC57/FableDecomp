// Behavior fixture: exact selected CHTG_Guard::Main, deterministic game calls.
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
struct World {
 int frame,stopAt,partyAt,talkAt,hitAt,spellAt,healAt,exitAt,nearAt,startDelay,taskDelay;
 int states[40],moves,follows,targets,releases,movies,pauses,live,leaderDiesAt;
 bool waypointAlive,prisonerAlive,freedomAlive;float health;std::vector<std::string> lines;
 World():frame(0),stopAt(12),partyAt(1),talkAt(-1),hitAt(-1),spellAt(-1),healAt(-1),exitAt(5),nearAt(6),startDelay(0),taskDelay(0),moves(0),follows(0),targets(0),releases(0),movies(0),pauses(0),live(0),leaderDiesAt(100),waypointAlive(true),prisonerAlive(true),freedomAlive(true),health(1.0f){for(int i=0;i<40;++i)states[i]=-1;}
};
static World* g;
class C3DVector { public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}};
class CScriptThing { public:
 int id;C3DVector pos;CScriptThing(int x=0):id(x){++g->live;}CScriptThing(const CScriptThing& b):id(b.id),pos(b.pos){++g->live;}~CScriptThing(){--g->live;}
 bool IsAlive()const{if(id==3)return g->frame<g->leaderDiesAt;if(id==4)return g->prisonerAlive;if(id==5)return g->waypointAlive;if(id==6)return g->freedomAlive;return id!=0;}
 bool IsDead()const{return !IsAlive();}bool operator==(const CScriptThing& x)const{return id==x.id;}
 const C3DVector& GetPos()const{return pos;}
 bool MsgIsTalkedToBy(const char*)const{return g->frame==g->talkAt;}
 bool MsgIsHitBy(const char*)const{return g->frame==g->hitAt;}
 bool MsgIsHitByAnySpecialAbilityFrom(const char*)const{return g->frame==g->spellAt||g->frame==g->healAt;}
 bool MsgIsHitBySpecialAbilityFrom(int,const char*)const{return g->frame==g->healAt;}
};
class CScriptGameResourceObjectScriptedThingBase { public:
 bool valid;CScriptThing thing;CScriptGameResourceObjectScriptedThingBase():valid(false),thing(2){}
 bool IsValid()const{return valid;}void Release(){valid=false;++g->releases;}void ClearCommands(){}
 CScriptThing GetScriptThing()const{return thing;}
 bool IsPerformingScriptTask()const{return g->taskDelay>0;}
 void Speak(CScriptThing,const char* line,int,bool,bool,bool){g->lines.push_back(line);}
 void MoveToPosition(const C3DVector&,float,int,bool,bool){++g->moves;}
 void FollowThing(CScriptThing p,float d,bool){assert(p.id==4&&d==1.0f);++g->follows;}
};
class CScriptGameResourceObjectMovieBase { public:CScriptGameResourceObjectMovieBase(){++g->movies;}~CScriptGameResourceObjectMovieBase(){--g->movies;}};
class CGameScriptInterfaceBase;
struct CWideScreenMagicPauseEntities {CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&){++g->pauses;}~CWideScreenMagicPauseEntities(){--g->pauses;}};
struct TRetVoid{void Return(){}};
enum {SCRIPT_AI_PRIORITY_HIGH=3,GROUP_SELECT_NONE=0,ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,HERO_ABILITY_HEAL_LIFE_SPELL=14};
namespace NScript {
class CIsThingAlive{ public:CScriptThing Thing;CIsThingAlive(const CScriptThing& t):Thing(t){}};
class CQ_HangingTreeGoodScript { public:
 bool PartySuccessfullySetUp,LeaderSpokenTo,BanditYelledOut,AtWayPoint,ExitCutsceneStart,ReassignLeaderNow;
 long HTG_State,CurrentWayPoint;CScriptThing Leader,Prisoner,ActiveWayPoint;
 CQ_HangingTreeGoodScript():PartySuccessfullySetUp(false),LeaderSpokenTo(false),BanditYelledOut(false),AtWayPoint(false),ExitCutsceneStart(false),ReassignLeaderNow(false),HTG_State(1),CurrentWayPoint(5),Leader(2),Prisoner(4),ActiveWayPoint(5){}
 struct CHTG_Guard {
  CGameScriptInterfaceBase* Game;CScriptThing Me;CQ_HangingTreeGoodScript& ParentClass;
  CHTG_Guard(CGameScriptInterfaceBase* game,CQ_HangingTreeGoodScript& p):Game(game),Me(2),ParentClass(p){}
  bool IsActiveThreadTerminating()const{return g->frame>=g->stopAt;}
  void SetPredicate(const CIsThingAlive& p){assert(p.Thing.id==2);}
  void Main();
 };
};
}
class CGameScriptInterfaceBase { public:
 NScript::CQ_HangingTreeGoodScript& p;CGameScriptInterfaceBase(NScript::CQ_HangingTreeGoodScript& x):p(x){}
 void NewScriptFrame(){++g->frame;assert(g->frame<40);if(g->frame>=g->partyAt)p.PartySuccessfullySetUp=true;if(g->states[g->frame]>=0)p.HTG_State=g->states[g->frame];if(g->frame>=g->exitAt)p.ExitCutsceneStart=true;if(g->taskDelay>0)--g->taskDelay;}
 bool StartScriptingEntity(CScriptThing,CScriptGameResourceObjectScriptedThingBase& h,int priority){assert(priority==3);if(g->startDelay){--g->startDelay;return false;}h.valid=true;return true;}
 void StartMovieSequence(const char*,CScriptGameResourceObjectMovieBase&){}
 float GetHealth(CScriptThing){return g->health;}
 CScriptThing GetHero(){return CScriptThing(1);}
 long AddNewConversation(CScriptThing,bool,bool){return 1;}
 void AddLineToConversation(long,const char* s,bool,CScriptThing,CScriptThing){g->lines.push_back(s);}
 void AddPersonToConversation(long,CScriptThing){}
 CScriptThing GetThingWithScriptName(const char*){return CScriptThing(7);}
 CScriptThing GetNearestWithScriptName(CScriptThing,const char*){return CScriptThing(6);}
 void GiveThingBestEnemyTarget(CScriptThing,CScriptThing enemy){assert(enemy.id==6);++g->targets;}
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float){return g->frame>=g->nearAt;}
static bool IsDistanceBetweenThingsOver(const CScriptThing&,const CScriptThing&,float){return g->frame<g->nearAt;}
static bool IsDistanceFromThingToPositionOver(const CScriptThing&,const C3DVector&,float){return g->frame<g->nearAt;}

void NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::CGameScriptInterfaceBase& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.PartySuccessfullySetUp) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass.HTG_State!=7) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.ClearCommands();
        if (ParentClass.Leader.IsAlive() && ParentClass.Leader==Me) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==1) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_030_GUARD_INTRODUCTION",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.LeaderSpokenTo=true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==2) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.BanditYelledOut=false;
                if (ParentClass.LeaderSpokenTo) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.ActiveWayPoint.IsAlive()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(ParentClass.ActiveWayPoint.GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
                        while (ParentClass.HTG_State==2 && !IsDistanceBetweenThingsUnder(Me,ParentClass.ActiveWayPoint,5.0f)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_HIT_BY_HERO",false,Me,Game.GetHero());
                            }
                            if (!seh_me.IsPerformingScriptTask()) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.MoveToPosition(ParentClass.ActiveWayPoint.GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
                            }
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ParentClass.HTG_State==2) ParentClass.AtWayPoint=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ParentClass.CurrentWayPoint<=14) {
                            ParentClass.HTG_State=3;
                            CScriptThing ExitPoint=Game.GetThingWithScriptName("HTG_ExitPoint");
                            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_WAITING",false,Me,Game.GetHero());
                            while (!ParentClass.ExitCutsceneStart) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.MoveToPosition(ExitPoint.GetPos(),2.0f,ENTITY_MOVE_WALK,true,false);
                            if (seh_me.IsValid()) seh_me.Release();
                            while (!false) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==5) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (!ParentClass.BanditYelledOut) {
                    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_ATTACKED",false,Me,Game.GetHero());
                    ParentClass.BanditYelledOut=true;
                }
                C3DVector PrisonerPos=ParentClass.Prisoner.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),PrisonerPos,2.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(PrisonerPos,0.0f,ENTITY_MOVE_RUN,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (ParentClass.HTG_State!=2) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_LETS_MOVE",false,Me,Game.GetHero());
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==1) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_030_GUARD_SUPPORT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==2) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.Prisoner.IsAlive()) seh_me.FollowThing(ParentClass.Prisoner,1.0f,true);
                while (ParentClass.HTG_State==2 && !(ParentClass.Leader==Me) && !ParentClass.Leader.IsDead()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_HIT_BY_HERO",false,Me,Game.GetHero());
                    }
                    if (!seh_me.IsPerformingScriptTask()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.FollowThing(ParentClass.Prisoner,1.0f,true);
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (ParentClass.Leader.IsDead()) ParentClass.ReassignLeaderNow=true;
                else if (ParentClass.HTG_State!=7 && ParentClass.HTG_State!=3 && ParentClass.HTG_State!=4) ParentClass.HTG_State=5;
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==5) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                CScriptThing FreedomFighter=Game.GetNearestWithScriptName(Me,"HTG_FreedomFighter");
                if (FreedomFighter.IsAlive()) Game.GiveThingBestEnemyTarget(Me,FreedomFighter);
                while (ParentClass.HTG_State==5) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HTG_State==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (!false) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing AudienceWatchPoint=Game.GetThingWithScriptName("HTG_AudienceWatchPoint");
    while (IsDistanceBetweenThingsOver(Me,AudienceWatchPoint,2.1f)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!seh_me.IsPerformingScriptTask()) seh_me.MoveToPosition(AudienceWatchPoint.GetPos(),2.0f,ENTITY_MOVE_WALK,false,false);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}

static bool Has(const World& w,const char* s){for(unsigned i=0;i<w.lines.size();++i)if(w.lines[i].find(s)!=std::string::npos)return true;return false;}
static bool Run(World& w,int state,bool leader,bool spoken,bool* reassigned=0){g=&w;bool ok=true;{NScript::CQ_HangingTreeGoodScript p;p.HTG_State=state;p.Leader.id=leader?2:3;p.LeaderSpokenTo=spoken;CGameScriptInterfaceBase game(p);NScript::CQ_HangingTreeGoodScript::CHTG_Guard c(&game,p);c.Main();ok=w.frame==w.stopAt&&w.movies==0&&w.pauses==0;if(reassigned)*reassigned=p.ReassignLeaderNow;}return ok&&w.live==0;}
int main(){bool ok=true;
 World a;a.talkAt=3;a.states[4]=2;a.states[8]=7;ok=Run(a,1,true,false)&&Has(a,"INTRODUCTION")&&a.moves>0&&ok;
 World b;b.waypointAlive=false;ok=Run(b,2,true,true)&&Has(b,"GUARD_WAITING")&&b.moves==1&&b.releases==1&&ok;
 World c;c.nearAt=5;c.states[6]=2;c.states[8]=7;ok=Run(c,5,true,true)&&Has(c,"GUARD_ATTACKED")&&Has(c,"LETS_MOVE")&&ok;
 World d;d.talkAt=3;d.states[4]=2;d.states[8]=7;d.hitAt=6;ok=Run(d,1,false,false)&&Has(d,"GUARD_SUPPORT")&&Has(d,"HIT_BY_HERO")&&d.follows>0&&ok;
 World e;e.states[8]=7;e.healAt=5;ok=Run(e,2,false,false)&&!Has(e,"HIT_BY_HERO")&&ok;
 World f;f.states[8]=7;f.healAt=5;f.hitAt=5;ok=Run(f,2,false,false)&&Has(f,"HIT_BY_HERO")&&ok;
 World h;h.states[6]=7;ok=Run(h,5,false,false)&&h.targets==1&&ok;
 World i;i.states[6]=7;i.freedomAlive=false;ok=Run(i,5,false,false)&&i.targets==0&&ok;
 World j;j.leaderDiesAt=5;j.states[8]=7;bool reassigned=false;ok=Run(j,2,false,false,&reassigned)&&reassigned&&ok;
 World k;ok=Run(k,3,false,false)&&k.targets==0&&ok;
 World l;l.talkAt=3;l.health=0.0f;l.states[5]=7;ok=Run(l,1,true,false)&&!Has(l,"INTRODUCTION")&&ok;
 for(int n=1;n<8;++n){World m;m.stopAt=n;m.talkAt=3;m.taskDelay=5;m.states[7]=7;ok=Run(m,1,true,false)&&ok;}
 World delay;delay.startDelay=2;delay.partyAt=5;delay.states[6]=7;ok=Run(delay,1,true,false)&&ok;
 std::printf(ok?"00d706e0_TEST PASS\n":"00d706e0_TEST FAIL\n");return ok?0:1;}