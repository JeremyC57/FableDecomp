#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,paused,unpaused,predicates,startAttempts,startFailures,taskDelay,taskUntil,aliveCount,firstCount,laterCount,collections,nearFrame,regionExit,moveRemaining,moves,healthChecks,sheathed,markers,releases,ended;bool intro,ending,talk;float nearDistance,moveDistance,health;std::vector<std::string>speech;std::vector<int>modes;std::vector<float>pauses;State():frame(0),limit(8),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),predicates(0),startAttempts(0),startFailures(0),taskDelay(0),taskUntil(0),aliveCount(2),firstCount(2),laterCount(2),collections(0),nearFrame(0),regionExit(100),moveRemaining(0),moves(0),healthChecks(0),sheathed(0),markers(0),releases(0),ended(0),intro(true),ending(false),talk(false),nearDistance(2.5f),moveDistance(5.5f),health(100.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CScriptThing{public:int id;bool fromHandle;CScriptThing(int n=-1,bool h=false):id(n),fromHandle(h){++state->things;}CScriptThing(const CScriptThing&o):id(o.id),fromHandle(o.fromHandle){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");return state->talk;}const C3DVector&GetPos()const{assert(id==2);static C3DVector p(1,2,3);return p;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM=1};enum EScriptEntityMoveType{ENTITY_MOVE_WALK=0};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->movies==0&&state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1,true);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.id==-2&&valid&&!a&&b&&!c&&state->pauseDepth==1);state->speech.push_back(s);state->modes.push_back(m);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}void MoveToPosition(const C3DVector&p,float range,EScriptEntityMoveType mt,bool a,bool b){assert(valid&&p.X==1&&p.Y==2&&p.Z==3&&range==3.0f&&mt==ENTITY_MOVE_WALK&&!a&&b);++state->moves;if(state->moveRemaining>0)--state->moveRemaining;state->taskUntil=state->frame+state->taskDelay;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(m.id==-1&&p==SCRIPT_AI_PRIORITY_HIGHEST&&!r.valid);r.valid=++state->startAttempts>state->startFailures;return r.valid;}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){if(s.text=="M_EndTheQuestHere")return CScriptThing(1);assert(s.text=="TraderEndPos");return CScriptThing(2);}long GetAllThingsWithScriptName(const CCharString&s,std::vector<CScriptThing>&v){assert(s.text=="DarkwoodTrader"&&v.empty());int n=state->collections++?state->laterCount:state->firstCount;for(int i=0;i<n;++i)v.push_back(CScriptThing(3+i));return n;}bool IsRegionLoaded(const CCharString&s){assert(s.text=="BarrowFields");return state->frame<state->regionExit;}float GetHealth(const CScriptThing&m){assert(m.id==-1&&m.fromHandle&&state->pauseDepth==1);++state->healthChecks;return state->health;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;++state->unpaused;}}void Pause(float f){assert(state->pauseDepth==1);state->pauses.push_back(f);}void SheatheHeroWeapons(){assert(state->pauseDepth==1);++state->sheathed;}void MiniMapRemoveMarker(const CScriptThing&m){assert(m.id==-1&&state->pauseDepth==0);++state->markers;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float f){assert(a.id==1&&b.id==-2&&f==3.0f);return state->frame>=state->nearFrame&&state->nearDistance<f;}
bool IsDistanceFromThingToPositionOver(const CScriptThing&a,const C3DVector&p,float f){assert(a.id==-1&&a.fromHandle&&p.X==1&&f==5.0f);return state->moveRemaining>0&&state->moveDistance>f;}
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class CQ_TraderEscortScript{public:bool IntroFinished,EndStarted;long TradersStillAliveCounter;CQ_TraderEscortScript():IntroFinished(state->intro),EndStarted(false),TradersStillAliveCounter(state->aliveCount){}class CEndTrader{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_TraderEscortScript&ParentClass;bool EndingCanStart;CEndTrader(CGameScriptInterfaceBase&g,CQ_TraderEscortScript&p):Game(g),Me(-1),ParentClass(p),EndingCanStart(state->ending){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_TraderEscortScript::CEndTrader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing StartTheEnd=Game.GetThingWithScriptName("M_EndTheQuestHere");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.IntroFinished) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        while (!IsDistanceBetweenThingsUnder(StartTheEnd,Game.GetHero(),3.0f)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        std::vector<CScriptThing> DarkwoodTraderList;
        Game.GetAllThingsWithScriptName("DarkwoodTrader",DarkwoodTraderList);
        long TradersInBarrowFields=DarkwoodTraderList.size();
        if (TradersInBarrowFields==ParentClass.TradersStillAliveCounter && ParentClass.TradersStillAliveCounter>0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            EndingCanStart=true;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (EndingCanStart) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.SheatheHeroWeapons();
                Game.Pause(0.2f);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_067_ENDTRADER_GREETINGS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            ParentClass.EndStarted=true;
            break;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.Pause(0.5f);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            while (Game.IsRegionLoaded("BarrowFields")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing EndPos=Game.GetThingWithScriptName("TraderEndPos");
    {
        float local_range=3.0f;
        C3DVector local_position=EndPos.GetPos();
        EScriptEntityMoveType local_movetype=ENTITY_MOVE_WALK;
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),local_position,local_range+2.0)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(local_position,local_range,local_movetype,false,true);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    Game.MiniMapRemoveMarker(Me);
    seh_me.Release();
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_067_ENDTRADER_THANKS",GROUP_SELECT_RANDOM,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            seh_me.Release();
        }
    }
}

namespace AccessorFixture {
template<class T> class CCountedPointer { public:T* Data;CCountedPointer():Data(0){}bool IsNull() const{return Data==0;}T* operator->() const{return Data;} };
class CScriptThing {public:CCountedPointer<CScriptThing> PImp;static C3DVector NullCoord;virtual const C3DVector& GetPos() const;};
C3DVector CScriptThing::NullCoord;
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
class PositionThing:public CScriptThing {public:C3DVector Pos;PositionThing():Pos(1,2,3){}virtual const C3DVector&GetPos()const{return Pos;}};
int check(){CScriptThing t;assert(&t.GetPos()==&CScriptThing::NullCoord);PositionThing p;t.PImp.Data=&p;assert(&t.GetPos()==&p.Pos&&t.GetPos().X==1);p.Pos.X=4;assert(t.GetPos().X==4);return 3;}
}
bool run(State&s){state=&s;bool result;{CGameScriptInterfaceBase g;NScript::CQ_TraderEscortScript p;NScript::CQ_TraderEscortScript::CEndTrader t(g,p);t.Main();result=t.EndingCanStart;s.ended=p.EndStarted;assert(s.things==2&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.paused==s.unpaused&&s.predicates==1);}assert(s.things==0);return result;}
int main(){int cases=0;const char*greeting="TEXT_QST_067_ENDTRADER_GREETINGS";const char*missing="TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT";const char*thanks="TEXT_QST_067_ENDTRADER_THANKS";
 {State s;assert(run(s)&&s.ended&&s.markers==1&&s.sheathed==1&&s.collections==1&&s.speech.size()==1&&s.speech[0]==greeting&&s.modes[0]==0&&s.releases==1&&s.paused==1);++cases;}
 {State s;s.intro=false;assert(!run(s)&&!s.ended&&s.releases==1&&s.collections==0&&s.speech.empty());++cases;}
 {State s;s.firstCount=1;assert(!run(s)&&!s.ended&&s.speech.size()==1&&s.speech[0]==missing&&s.pauses.size()==1&&s.pauses[0]==0.5f);++cases;}
 {State s;s.firstCount=1;s.talk=true;s.limit=4;assert(!run(s)&&s.speech.size()==3);for(unsigned i=0;i<s.speech.size();++i)assert(s.speech[i]==missing&&s.modes[i]==0);++cases;}
 {State s;s.firstCount=1;s.laterCount=2;s.regionExit=3;assert(run(s)&&s.ended&&s.collections==2&&s.speech.size()==2&&s.speech[0]==missing&&s.speech[1]==greeting&&s.markers==1);++cases;}
 {State s;s.talk=true;s.limit=4;assert(run(s)&&s.ended&&s.speech.size()==4&&s.startAttempts==4&&s.releases==4);for(unsigned i=1;i<s.speech.size();++i)assert(s.speech[i]==thanks&&s.modes[i]==1);++cases;}
 for(int alive=-1;alive<=3;++alive)for(int count=0;count<=3;++count){State s;s.aliveCount=alive;s.firstCount=count;s.laterCount=count;bool yes=alive>0&&alive==count;assert(run(s)==yes&&s.ended==int(yes)&&s.speech.size()==1&&s.speech[0]==(yes?greeting:missing));++cases;}
 {State s;s.ending=true;s.aliveCount=0;s.firstCount=0;assert(run(s)&&s.ended&&s.speech[0]==greeting);++cases;}
 for(int h=0;h<4;++h){State s;s.health=h==0?1.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();assert(run(s)&&s.ended&&s.markers==1&&s.healthChecks==1&&s.speech.size()==unsigned(h==0));++cases;}
 for(int v=0;v<4;++v){State s;s.nearDistance=v==0?2.5f:v==1?3.0f:v==2?3.5f:std::numeric_limits<float>::quiet_NaN();bool yes=v==0;assert(run(s)==yes&&s.ended==int(yes)&&s.collections==int(yes));++cases;}
 for(int v=0;v<4;++v){State s;s.moveRemaining=2;s.moveDistance=v==0?4.5f:v==1?5.0f:v==2?5.5f:std::numeric_limits<float>::quiet_NaN();assert(run(s)&&s.ended&&s.markers==1&&s.moves==(v==2?2:0));++cases;}
 {State s;s.nearFrame=4;assert(run(s)&&s.ended&&s.frame==8&&s.collections==1);++cases;}
 {State s;s.startFailures=3;assert(run(s)&&s.startAttempts==4&&s.ended);++cases;}
 {State s;s.startFailures=100;s.limit=5;assert(!run(s)&&s.startAttempts==4&&s.speech.empty());++cases;}
 {State s;s.taskDelay=100;s.limit=5;assert(run(s)&&!s.ended&&s.speech.size()==1&&s.markers==0&&s.paused==1);++cases;}
 {State s;s.taskDelay=2;s.moveRemaining=2;s.limit=15;assert(run(s)&&s.ended&&s.moves==2&&s.markers==1);++cases;}
 for(int mode=0;mode<12;++mode)for(int stop=1;stop<=110;++stop){State s;s.limit=24;s.stopCheck=stop;s.intro=mode!=0;s.firstCount=mode==1||mode==2||mode==3?1:2;s.regionExit=mode==3?4:100;s.talk=mode==2||mode==4||mode==5;s.taskDelay=mode==5||mode==6?3:0;s.moveRemaining=mode==6||mode==7?3:0;s.nearFrame=mode==8?5:0;s.startFailures=mode==9?5:0;s.health=mode==10?0.0f:mode==11?std::numeric_limits<float>::quiet_NaN():100.0f;run(s);assert(s.checks<=stop);++cases;}
 cases+=AccessorFixture::check();assert(cases==1367);std::puts("00e055b0_TEST PASS");}
