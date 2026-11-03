#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
struct Event {bool talk,hit,magic,heal; Event():talk(false),hit(false),magic(false),heal(false){} };
struct State {
    Event e[256];int frame,checks,stopCheck,limit,startFailures,starts,moves,moveFailures,taskFrames,taskUntil,resources,movies,pauses,speaks,convs,faces,timers,randomValue,scytheAt;
    float health; bool nearHero; bool* mission;bool* scythe;std::vector<std::string> text;std::vector<long> durations;
    State():frame(0),checks(0),stopCheck(0),limit(12),startFailures(0),starts(0),moves(0),moveFailures(0),taskFrames(0),taskUntil(0),resources(0),movies(0),pauses(0),speaks(0),convs(0),faces(0),timers(0),randomValue(0),scytheAt(0),health(1.0f),nearHero(true),mission(0),scythe(0){}
};
State* state;
extern "C" int rand(){return state->randomValue;}
class CCharString {public:std::string text;CCharString(){} CCharString(const char*s):text(s){} CCharString(const std::string&s):text(s){} operator const char*()const{return text.c_str();}};
CCharString operator+(const char*a,const CCharString&b){return CCharString(std::string(a)+b.text);}
CCharString GFIntToCharString(long v){char b[32];std::sprintf(b,"%ld",v);return CCharString(b);}
enum EScriptAIPriority {SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod {GROUP_SELECT_NONE=0};enum EHeroAbility {HERO_ABILITY_HEAL_LIFE_SPELL=14};enum EScriptEntityMoveType {ENTITY_MOVE_WALK=0};
class TRetVoid {public:void Return(){}};
class C3DVector {public:float X,Y,Z;C3DVector():X(1),Y(2),Z(3){}};
class CScriptThing {public:int kind;CScriptThing(int k=0):kind(k){} C3DVector GetPos()const{assert(kind==3);return C3DVector();}
 bool MsgIsTalkedToBy(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].talk;}
 bool MsgIsHitBy(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].hit;}
 bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].magic;}
 bool MsgIsHitBySpecialAbilityFrom(EHeroAbility e,const CCharString&s)const{assert(e==HERO_ABILITY_HEAL_LIFE_SPELL&&s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].heal;}
};
class CScriptGameResourceObjectScriptedThingBase {public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid;}void Release(){valid=false;}CScriptThing GetScriptThing()const{return CScriptThing(2);}
 void MoveToPosition(const C3DVector&p,float r,EScriptEntityMoveType t,bool a,bool b){assert(p.X==1&&p.Y==2&&p.Z==3&&r==0&&t==ENTITY_MOVE_WALK&&!a&&b);++state->moves;state->taskUntil=state->frame+state->taskFrames;}
 bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}
 void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.kind==1&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauses==1);++state->speaks;state->text.push_back(s);state->taskUntil=state->frame+state->taskFrames;}
};
bool IsDistanceFromThingToPositionOver(const CScriptThing&t,const C3DVector&p,float r){assert(t.kind==2&&p.X==1&&r==2.0f);return state->moves<state->moveFailures;}
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float r){assert(a.kind==0&&b.kind==1&&r==7.0f);return state->nearHero;}
class CScriptGameResourceObjectMovieBase {public:CScriptGameResourceObjectMovieBase(){++state->movies;}~CScriptGameResourceObjectMovieBase(){--state->movies;}};
class CTimer {long end;public:CTimer():end(0){++state->timers;}~CTimer(){--state->timers;}CTimer& operator=(long n){assert(n>=0);state->durations.push_back(n);end=state->frame+n;return *this;}bool operator<=(long n)const{return end-state->frame<=n;}};
class CGameScriptInterfaceBase {public:CScriptThing hero;CGameScriptInterfaceBase():hero(1){}
 void NewScriptFrame(){assert(state->frame<254);++state->frame;if(state->frame>=state->limit)*state->mission=true;if(state->frame>=state->scytheAt)*state->scythe=true;}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(t.kind==0&&p==SCRIPT_AI_PRIORITY_HIGHEST);++state->starts;if(state->starts<=state->startFailures)return false;r.valid=true;return true;}
 CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="FH_GuildMasterMarker"||s.text=="FH_GuildMasterLookAt");return CScriptThing(s.text=="FH_GuildMasterMarker"?3:4);}
 void EntitySetFacingAngleTowardsThing(const CScriptThing&a,const CScriptThing&b,bool f){assert(a.kind==0&&(b.kind==4||b.kind==1)&&!f);++state->faces;}
 void SetThingHasInformation(const CScriptThing&t,bool a,bool b,bool c){assert(t.kind==0&&!a&&b&&!c);}
 void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty());}
 void PauseAllNonScriptedEntities(bool v){state->pauses+=v?1:-1;assert(state->pauses>=0&&state->pauses<=1);}
 float GetHealth(const CScriptThing&t){assert(t.kind==2);return state->health;}
 CScriptThing& GetHero(){return hero;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.kind==0&&!a&&!b);return ++state->convs;}
 void AddPersonToConversation(long c,const CScriptThing&h){assert(c==state->convs&&h.kind==1);}
 void AddLineToConversation(long c,const CCharString&s,bool b,const CScriptThing&a,const CScriptThing&h){assert(c==state->convs&&!b&&a.kind==0&&h.kind==1);state->text.push_back(s.text);}
};
class CWideScreenMagicPauseEntities {CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript {class CQ_FireHeartScript {public:bool HasScytheCutscenePlayed,MissionSucceeded;CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;
 CQ_FireHeartScript():HasScytheCutscenePlayed(false),MissionSucceeded(false){}
 class CFH_GuildMaster {public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_FireHeartScript& ParentClass;long NextLine;CFH_GuildMaster(CQ_FireHeartScript&p,CGameScriptInterfaceBase&g):Game(g),ParentClass(p),NextLine(1){}
 bool IsActiveThreadTerminating()const{++state->checks;return (state->stopCheck&&state->checks>=state->stopCheck)||state->frame>=240;}void Main();};};}

void NScript::CQ_FireHeartScript::CFH_GuildMaster::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ParentClass.seh_GuildMaster=seh_me;
    while (!ParentClass.HasScytheCutscenePlayed) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing marker=Game.GetThingWithScriptName("FH_GuildMasterMarker");
    {
        EScriptEntityMoveType local_movetype=ENTITY_MOVE_WALK;
        float local_range=0.0f;
        C3DVector local_position=marker.GetPos();
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),local_position,2.0f)) {
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
    CScriptThing lookAt=Game.GetThingWithScriptName("FH_GuildMasterLookAt");
    Game.EntitySetFacingAngleTowardsThing(Me,lookAt,false);
    Game.SetThingHasInformation(Me,false,true,false);
    CTimer point_timer;
    point_timer=5;
    while (!ParentClass.MissionSucceeded) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B01_GM_SPEECH",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
             !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B01_GM_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        } else if (point_timer<=0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),7.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            CCharString message="TEXT_QST_B01_GM_ENTER_DOOR"+GFIntToCharString(NextLine);
            ++NextLine;
            if (NextLine>3) NextLine=1;
            long conv=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(conv,Game.GetHero());
            Game.AddLineToConversation(conv,message,false,Me,Game.GetHero());
            point_timer=(long)(15.0f+((float)rand()*15.0f)*(1.0f/32767.0f));
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

long run(State&s,long initial=1){state=&s;long result;{CGameScriptInterfaceBase g;NScript::CQ_FireHeartScript p;p.HasScytheCutscenePlayed=s.scytheAt==0;s.mission=&p.MissionSucceeded;s.scythe=&p.HasScytheCutscenePlayed;NScript::CQ_FireHeartScript::CFH_GuildMaster x(p,g);x.NextLine=initial;x.Main();result=x.NextLine;assert(!s.movies&&!s.pauses&&!s.timers&&s.resources==1);}assert(s.resources==0);return result;}
int main(){int cases=0;
 {State s;run(s);assert(s.moves==0&&s.starts==1&&s.convs==1&&s.faces==2&&s.durations[0]==5&&s.durations[1]==15&&s.text[0]=="TEXT_QST_B01_GM_ENTER_DOOR1");++cases;}
 for(int r=0;r<3;++r){State s;s.limit=100;s.randomValue=r==0?0:r==1?16383:32767;run(s);long expected=r==0?15:r==1?22:30;
#ifdef _MSC_VER
 if(r==2)expected=29;
#endif
assert(s.convs>=4);for(unsigned k=1;k<s.durations.size();++k)assert(s.durations[k]==expected);for(unsigned k=0;k<s.text.size();++k){char b[64];std::sprintf(b,"TEXT_QST_B01_GM_ENTER_DOOR%u",k%3+1);assert(s.text[k]==b);}++cases;}
 for(int kind=0;kind<4;++kind)for(int alive=0;alive<2;++alive){State s;s.nearHero=false;s.health=alive?1.0f:0.0f;s.e[1].talk=kind==0;s.e[1].hit=kind==1;s.e[1].magic=kind>=2;s.e[1].heal=kind==3;run(s);assert(s.speaks==((kind<3&&alive)?1:0));if(s.speaks)assert(s.text[0]==(kind==0?"TEXT_QST_B01_GM_SPEECH":"TEXT_QST_B01_GM_ATTACKED"));++cases;}
 {State s;s.nearHero=false;s.startFailures=2;s.scytheAt=4;s.moveFailures=2;s.taskFrames=2;run(s);assert(s.starts==3&&s.moves==2&&s.faces==1);++cases;}
 {State s;s.e[1].talk=true;s.e[1].hit=true;s.nearHero=false;run(s);assert(s.speaks==1&&s.text[0]=="TEXT_QST_B01_GM_SPEECH");++cases;}
 for(int stop=1;stop<=100;++stop)for(int mode=0;mode<3;++mode){State s;s.stopCheck=stop;s.startFailures=mode;s.scytheAt=mode+1;s.moveFailures=mode;s.taskFrames=mode;for(int f=1;f<12;++f){s.e[f].talk=mode==1;s.e[f].hit=mode==2;}run(s);++cases;}
 assert(cases==314);std::puts("00d368d0_TEST PASS");}
