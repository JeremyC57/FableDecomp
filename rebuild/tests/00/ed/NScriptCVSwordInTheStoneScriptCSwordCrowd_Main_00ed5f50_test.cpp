// Behaviour test: selected CSwordCrowd::Main definition copied below, with deterministic game interfaces.
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <map>
struct World {
 int frame,stopAt,talkAt,hitAt,spellAt,releaseCutAt,startDelay,taskDelay,coin;
 int cards,cutscenes,spawns,kills,resets,pulls,fades,clears,info;
 int moves,movies,pauses,thingLive,randomIndex;
 bool exitAlive,overDistance; float health;
 std::vector<std::string> lines;std::vector<long> loopCounts;
 World():frame(0),stopAt(8),talkAt(-1),hitAt(-1),spellAt(-1),releaseCutAt(-1),startDelay(0),taskDelay(0),coin(0),cards(0),cutscenes(0),spawns(0),kills(0),resets(0),pulls(0),fades(0),clears(0),info(0),moves(0),movies(0),pauses(0),thingLive(0),randomIndex(0),exitAlive(true),overDistance(false),health(1.0f){}
};
static World* g;
static const float DELTA_NOTIONAL_ZERO=0.0001f;
class CCharString { public:
 std::string text; CCharString(const char* p=""):text(p){} CCharString(const std::string& p):text(p){}
 bool operator<(const CCharString& b)const{return text<b.text;}
};
static CCharString operator+(const CCharString& a,const CCharString& b){return CCharString(a.text+b.text);}
class C3DVector { public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}};
class CScriptThing { public:
 int id;C3DVector pos;
 CScriptThing(int x=0):id(x){++g->thingLive;}
 CScriptThing(const CScriptThing& b):id(b.id),pos(b.pos){++g->thingLive;}
 ~CScriptThing(){--g->thingLive;}
 bool MsgIsHitBy(const char*)const{return g->frame==g->hitAt;}
 bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const char*)const{return g->frame==g->spellAt;}
 bool MsgIsTalkedToBy(const char*)const{return g->frame==g->talkAt;}
 bool IsAlive()const{return id!=4||g->exitAlive;}
 const C3DVector& GetPos()const{return pos;}
 C3DVector GetHomePos()const{return pos;}
};
class CTimer { public:int expires;CTimer():expires(1000){}CTimer& operator=(long t){expires=g->frame+(int)t;return *this;}long GetTime()const{return expires>g->frame?expires-g->frame:0;}};
class CScriptGameResourceObjectScriptedThingBase { public:
 bool valid;CScriptThing thing;
 CScriptGameResourceObjectScriptedThingBase():valid(false),thing(2){}
 bool IsValid()const{return valid;}void Release(){valid=false;}
 CScriptThing GetScriptThing()const{return thing;}
 void Speak(CScriptThing,const char* s,int,bool,bool,bool){g->lines.push_back(s);}
 bool IsPerformingScriptTask()const{return g->taskDelay>0;}
 void MoveToPosition(const C3DVector&,float,int,bool,bool){++g->moves;g->overDistance=false;}
 void ClearCommands(){++g->clears;}
 void PlayAnimation(const char* s,bool,bool,bool,bool,bool,bool,bool){g->lines.push_back(s);}
 void PlayLoopingAnimation(const char*,long count,bool,bool,bool,bool,bool,bool,bool){g->loopCounts.push_back(count);}
};
class CScriptGameResourceObjectMovieBase { public:CScriptGameResourceObjectMovieBase(){++g->movies;}~CScriptGameResourceObjectMovieBase(){--g->movies;}};
class CGameScriptInterfaceBase;
struct CWideScreenMagicPauseEntities {CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&){++g->pauses;}~CWideScreenMagicPauseEntities(){--g->pauses;}};
struct TRetVoid {void Return(){}};
class CSpawnedFuncBase { public:virtual ~CSpawnedFuncBase(){}};
template<class T,class P>struct CSpawnedFunc1: CSpawnedFuncBase {
 CSpawnedFunc1(const CCharString& name,unsigned long id,T*,void(T::*)(P),P par){assert(name.text=="ParentClass.WatchForDeathDuringSwordPull"&&id==62&&par.id==2);++g->spawns;}
};
namespace NGlobalConsole {static bool GDefaultToUsingPhysicsInCutsceneAnimations=false;}
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_RANDOM_NO_REPEAT=2,ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1};
namespace NScript {
class CIsThingAlive{ public:CScriptThing Thing;CIsThingAlive(const CScriptThing& t):Thing(t){}};
class CV_SwordInTheStoneScript { public:
 bool AttackedByHero,SwordCutscene,HeroHasBeenToldSwordLegend,HeroHasTakenSword,HeroCongratulated,CrowdDisperse,CrowdSwordPullActive,CrowdAttemptingToPullSword;
 CTimer CrowdSwordTimer;CScriptThing CrowdMemberPullingCurrent,CrowdMemberPullingNull;
 CV_SwordInTheStoneScript():AttackedByHero(false),SwordCutscene(false),HeroHasBeenToldSwordLegend(false),HeroHasTakenSword(false),HeroCongratulated(false),CrowdDisperse(false),CrowdSwordPullActive(false),CrowdAttemptingToPullSword(false){}
 void WatchForDeathDuringSwordPull(CScriptThing){}
 void AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& region){assert(region.text.empty());delete p;}
 void KillSpawnedFunction(const char* s,int id){assert(std::string(s)=="ParentClass.WatchForDeathDuringSwordPull"&&id==62);++g->kills;}
 void ResetCrowdSwordTimer(){CrowdSwordTimer=100; ++g->resets;}
 struct CSwordCrowd {
  CGameScriptInterfaceBase* Game;CScriptThing Me;CV_SwordInTheStoneScript& ParentClass;
  CSwordCrowd(CGameScriptInterfaceBase* game,CV_SwordInTheStoneScript& p):Game(game),Me(2),ParentClass(p){}
  void SetPredicate(const CIsThingAlive& p){assert(p.Thing.id==2);}
  bool IsActiveThreadTerminating()const{return g->frame>=g->stopAt;}
  void Main();
 };
};
}
class CGameScriptInterfaceBase { public:
 NScript::CV_SwordInTheStoneScript& p;
 CGameScriptInterfaceBase(NScript::CV_SwordInTheStoneScript& x):p(x){}
 void NewScriptFrame(){++g->frame;assert(g->frame<200);if(g->frame==g->releaseCutAt)p.SwordCutscene=false;if(g->taskDelay>0)--g->taskDelay;}
 void SetThingHasInformation(CScriptThing,bool,bool,bool){++g->info;}
 CScriptThing GetThingWithScriptName(const char* s){return CScriptThing(std::string(s)=="M_SIS_ExitPoint"?4:3);}
 void EntitySetFacingAngleTowardsThing(CScriptThing,CScriptThing,bool){}
 float GetHealth(CScriptThing){return g->health;}
 long AddNewConversation(CScriptThing,bool,bool){return 1;}
 void AddPersonToConversation(long,CScriptThing){}
 CScriptThing GetHero(){return CScriptThing(1);}
 void AddLineToConversation(long,const char* s,bool,CScriptThing,CScriptThing){g->lines.push_back(s);}
 bool IsConversationActive(long){return false;}
 bool StartScriptingEntity(CScriptThing who,CScriptGameResourceObjectScriptedThingBase& h,int){if(g->startDelay){--g->startDelay;return false;}h.valid=true;h.thing=who;return true;}
 void StartMovieSequence(const char*,CScriptGameResourceObjectMovieBase&){}
 void GiveHeroQuestCardDirectly(const char* s,const CCharString&,bool){assert(std::string(s)=="OBJECT_QUEST_CARD_SWORD_IN_THE_STONE");++g->cards;}
 CCharString GetActiveQuestName(){return CCharString("QUEST");}
 void ClearThingHasInformation(CScriptThing){}
 void SetThingAsUsable(CScriptThing,bool usable){if(!usable)++g->pulls;}
 void EntitySetAsOpinionSource(CScriptThing,const char*){}
 void SetIsPushableByHero(CScriptThing,bool){}
 void EntityUnsetAsOpinionSource(CScriptThing,bool){}
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float){return !g->overDistance;}
static bool IsDistanceFromThingToPositionOver(const CScriptThing&,const C3DVector&,float){return g->overDistance;}
static void FadeOutAndKillEntity(const CScriptThing&,float f,bool a,bool b){assert(f==1.0f&&a&&b);++g->fades;}
static void RunCutsceneMacro(const char* s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& cast,int,int,bool,bool){assert(std::string(s)=="CS_SWORDINSTONE_INTRO"&&cast.size()==2);++g->cutscenes;}
static int FixtureRand(){return g->coin==0?0:(g->randomIndex++==0?g->coin:3);}
#define rand FixtureRand

void NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::CGameScriptInterfaceBase& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.SetThingHasInformation(Me,true,false,false);
    CScriptThing Sword=Game.GetThingWithScriptName("SwordOfStrength");
    Game.EntitySetFacingAngleTowardsThing(Me,Sword,true);
    CTimer AttackedTimer;
    AttackedTimer=0;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || Me.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) && Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (AttackedTimer.GetTime()==0 && !ParentClass.AttackedByHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.AttackedByHero=true;
                long conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(conversation,Game.GetHero());
                Game.AddLineToConversation(conversation,"TEXT_QST_062_ATTACK",false,Me,Game.GetHero());
                AttackedTimer=2;
                while (Game.IsConversationActive(conversation)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.AttackedByHero=false;
            }
        }
        else if (ParentClass.SwordCutscene) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.Release();
            while (ParentClass.SwordCutscene) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.HeroHasBeenToldSwordLegend) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["ME"]=seh_me;
                RunCutsceneMacro("CS_SWORDINSTONE_INTRO",mymap,0,0,false,true);
                Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SWORD_IN_THE_STONE",Game.GetActiveQuestName(),true);
                ParentClass.HeroHasBeenToldSwordLegend=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_062_TRYING",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.HeroHasTakenSword && !ParentClass.HeroCongratulated) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.HeroCongratulated=true;
            ParentClass.CrowdDisperse=true;
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.CrowdDisperse) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.ClearThingHasInformation(Me);
            CScriptThing ExitPoint=Game.GetThingWithScriptName("M_SIS_ExitPoint");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ExitPoint.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(ExitPoint.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,ExitPoint,2.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.0f,true,true);
            }
        }
        if (ParentClass.CrowdSwordTimer.GetTime()==0 && !ParentClass.CrowdSwordPullActive) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing SwordThing=Game.GetThingWithScriptName("SwordOfStrength");
            if (!ParentClass.CrowdAttemptingToPullSword && rand()%2==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CrowdSwordPullActive=true;
                ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_SwordInTheStoneScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForDeathDuringSwordPull"),62,&ParentClass,&CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull,Me),CCharString(""));
                ParentClass.CrowdAttemptingToPullSword=true;
                Game.SetThingAsUsable(SwordThing,false);
                ParentClass.CrowdMemberPullingCurrent=Me;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector PullPos=Game.GetThingWithScriptName("MK_SIS_HERO").GetPos();
                while (ParentClass.CrowdAttemptingToPullSword && IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),PullPos,0.1f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(PullPos,0.0f,ENTITY_MOVE_WALK,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.CrowdAttemptingToPullSword) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntitySetFacingAngleTowardsThing(Me,SwordThing,true);
                    Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_SWORD_IN_THE_STONE_VILLAGER");
                    Game.SetIsPushableByHero(Me,false);
                    seh_me.ClearCommands();
                    seh_me.PlayAnimation("ST_LIFT_SWORD_TRY_INTO",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count1=rand()%2+1;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_LOOP",count1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count2=rand()%5+2;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_REST1",count2,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count3=rand()%2+1;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_LOOP",count3,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    seh_me.PlayAnimation("ST_LIFT_SWORD_TRY_REST_INTO",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count4=rand()%3+3;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_REST2",count4,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    seh_me.PlayAnimation("ST_LIFT_SWORD_TRY_REST_INTO_GIVEUP",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    C3DVector HomePos=Me.GetHomePos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),HomePos,2.1f)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(HomePos,0.1f,ENTITY_MOVE_WALK,false,true);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.ResetCrowdSwordTimer();
                if (seh_me.IsValid()) seh_me.Release();
                Game.SetIsPushableByHero(Me,true);
                Game.EntityUnsetAsOpinionSource(Me,true);
                ParentClass.KillSpawnedFunction("ParentClass.WatchForDeathDuringSwordPull",62);
            }
            ParentClass.CrowdAttemptingToPullSword=false;
            Game.SetThingAsUsable(SwordThing,true);
            ParentClass.CrowdSwordPullActive=false;
            ParentClass.CrowdMemberPullingCurrent=ParentClass.CrowdMemberPullingNull;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}

#undef rand
static bool Has(const World& w,const char* s){for(unsigned i=0;i<w.lines.size();++i)if(w.lines[i].find(s)!=std::string::npos)return true;return false;}
static bool Run(World& w,int mode){
 g=&w;bool ok=true;
 {NScript::CV_SwordInTheStoneScript p;CGameScriptInterfaceBase game(p);NScript::CV_SwordInTheStoneScript::CSwordCrowd c(&game,p);
  if(mode==1)p.HeroHasBeenToldSwordLegend=true;
  if(mode==2){p.HeroHasTakenSword=true;}
  if(mode==3){p.CrowdSwordTimer=0;}
  if(mode==4){p.CrowdSwordTimer=0;p.CrowdSwordPullActive=true;}
  if(mode==5){p.CrowdSwordTimer=0;p.CrowdAttemptingToPullSword=true;}
  if(mode==6){p.SwordCutscene=true;}
  c.Main();ok=w.frame==w.stopAt&&w.movies==0&&w.pauses==0;
  if(mode==2&&w.stopAt>1)ok=ok&&p.HeroCongratulated&&p.CrowdDisperse;
  if(mode==3&&w.coin==1&&w.stopAt>1)ok=ok&&!p.CrowdSwordPullActive&&!p.CrowdAttemptingToPullSword&&p.CrowdMemberPullingCurrent.id==p.CrowdMemberPullingNull.id;
 }
 return ok&&w.thingLive==0;
}
int main(){bool ok=true;
 World a;a.talkAt=2;ok=Run(a,0)&&a.cards==1&&a.cutscenes==1&&ok;
 World b;b.talkAt=2;ok=Run(b,1)&&b.cards==0&&Has(b,"TEXT_QST_062_TRYING")&&ok;
 World c;c.hitAt=2;ok=Run(c,0)&&Has(c,"TEXT_QST_062_ATTACK")&&ok;
 World d;d.spellAt=2;d.health=0.0f;ok=Run(d,0)&&!Has(d,"TEXT_QST_062_ATTACK")&&ok;
 World e;ok=Run(e,2)&&e.fades>0&&e.moves>0&&ok;
 World f;f.exitAlive=false;ok=Run(f,2)&&f.fades==0&&ok;
 World h;h.coin=1;ok=Run(h,3)&&h.spawns==1&&h.kills==1&&h.resets==1&&h.pulls==1&&h.loopCounts.size()==4&&h.loopCounts[0]==2&&h.loopCounts[1]==5&&h.loopCounts[2]==2&&h.loopCounts[3]==3&&ok;
 World i;ok=Run(i,4)&&i.spawns==0&&ok;
 World j;j.stopAt=2;ok=Run(j,5)&&j.spawns==0&&ok;
 World k;k.releaseCutAt=3;ok=Run(k,6)&&ok;
 World l;l.coin=0;ok=Run(l,3)&&l.spawns==0&&ok;
 for(int n=1;n<6;++n){World m;m.stopAt=n;m.talkAt=2;m.taskDelay=4;m.startDelay=1;ok=Run(m,1)&&ok;}
 std::printf(ok?"00ed5f50_TEST PASS\n":"00ed5f50_TEST FAIL\n");return ok?0:1;}