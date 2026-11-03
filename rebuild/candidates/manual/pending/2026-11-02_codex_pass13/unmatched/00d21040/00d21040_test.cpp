#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <limits>
static int live_things=0;
class C3DVector { public: float X,Y,Z; C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} };
class CCharString { public: std::string value; CCharString(const char* s=""):value(s){} CCharString(const CCharString& s):value(s.value){} };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1 };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
struct MockState {
 int frames,guards,terminate_at,release_frame,done_frame,starts_frame,wait_acquire,wait_conversation;
 int acquire_calls,conversations,lines,people,face_calls,moves_pace,moves_escape,clear_calls;
 int timer_reg,timer_dereg,timer_set,timer_value,resources,release_calls,sound,teleport,remove,mini_remove,mini_add,random_calls;
 bool talked,physical,special,heal,busy,screen,moved;
 float horn_distance,escape_distance;
 std::vector<std::string> line_text,factions;
 MockState():frames(0),guards(0),terminate_at(300),release_frame(1),done_frame(999),starts_frame(999),wait_acquire(0),wait_conversation(0),acquire_calls(0),conversations(0),lines(0),people(0),face_calls(0),moves_pace(0),moves_escape(0),clear_calls(0),timer_reg(0),timer_dereg(0),timer_set(0),timer_value(0),resources(0),release_calls(0),sound(0),teleport(0),remove(0),mini_remove(0),mini_add(0),random_calls(0),talked(false),physical(false),special(false),heal(false),busy(false),screen(false),moved(false),horn_distance(10.5f),escape_distance(2.5f){}
};
static MockState* state;
class CScriptThing { public: int id; CScriptThing(int n=0):id(n){++live_things;} CScriptThing(const CScriptThing& x):id(x.id){++live_things;} CScriptThing& operator=(const CScriptThing&x){id=x.id;return *this;} virtual ~CScriptThing(){--live_things;}
 bool MsgIsTalkedToBy(const CCharString&s) const {assert(s.value=="SCRIPT_NAME_HERO");return state->talked;}
 bool MsgIsHitBy(const CCharString&s)const {assert(s.value=="SCRIPT_NAME_HERO");return state->physical;}
 bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const {assert(s.value=="SCRIPT_NAME_HERO");return state->special;}
 bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const {assert(a==HERO_ABILITY_HEAL_LIFE_SPELL&&s.value=="SCRIPT_NAME_HERO");return state->heal;}
 const C3DVector& GetPos()const {static C3DVector p(1,2,4);assert(id==31);return p;}
 C3DVector GetHomePos()const {assert(id==1);return C3DVector(8,16,32);}
};
namespace NScript { class CQ_BreakSiegeScript; class CIsThingAlive { public: CScriptThing Thing; CIsThingAlive(const CScriptThing&t):Thing(t){} }; }
class CScriptGameResourceObjectScriptedThingBase { bool valid; public: CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;} ~CScriptGameResourceObjectScriptedThingBase(){--state->resources;} bool IsValid()const{return valid;} void Release(){assert(valid);valid=false;++state->release_calls;} void Acquire(){valid=true;} bool IsPerformingScriptTask()const{return state->busy;}
 void MoveToThing(const CScriptThing&t,float radius,EScriptEntityMoveType m,bool a,bool b,bool c,bool d){assert(valid&&!a&&b&&!c&&d);if(t.id>=20&&t.id<=22){assert(radius==1.0f&&m==ENTITY_MOVE_WALK);++state->moves_pace;}else {assert(t.id==30&&radius==0.0f&&m==ENTITY_MOVE_RUN);++state->moves_escape;state->moved=true;}}
 void ClearCommands(){assert(valid);++state->clear_calls;}
};
class CTimer {public: CTimer(){++state->timer_reg;} ~CTimer(){++state->timer_dereg;} CTimer& operator=(long n){assert(n==5||n==10);state->timer_value=(int)n;++state->timer_set;return *this;} bool operator==(long n)const{return state->timer_value==n;} };
class TRetVoid {public:void Return(){} };
class CGameScriptInterfaceBase {public: NScript::CQ_BreakSiegeScript* owner;CScriptThing hero;CGameScriptInterfaceBase():owner(0),hero(2){}
 void NewScriptFrame();
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(t.id==1&&p==SCRIPT_AI_PRIORITY_HIGHEST);++state->acquire_calls;if(state->wait_acquire>0){--state->wait_acquire;return false;}r.Acquire();return true;}
 void EntitySetAsOpinionSource(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.value=="OPINION_SOURCE_INANIMATE_EVIL_HIGH");}
 long GetAllThingsWithScriptName(const CCharString&s,std::vector<CScriptThing>&v){assert(s.value=="RestlessMarker");for(int n=20;n<23;++n)v.push_back(CScriptThing(n));return 3;}
 CScriptThing& GetHero(){return hero;}
 void EntitySetFacingAngleTowardsThing(const CScriptThing&a,const CScriptThing&b,bool c){assert(a.id==1&&b.id==2&&!c);++state->face_calls;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);return ++state->conversations;}
 CScriptThing GetNearestWithScriptName(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.value=="GuardHoldingBandit");return CScriptThing(10);}
 void AddPersonToConversation(long n,const CScriptThing&t){assert(n==state->conversations&&(t.id==10||t.id==2));++state->people;}
 void AddLineToConversation(long n,const CCharString&s,bool b,const CScriptThing&speaker,const CScriptThing&listener){assert(n==state->conversations&&!b&&listener.id==2);if(s.value=="TEXT_QST_018_GUARD_STOP_THAT")assert(speaker.id==10);else assert(speaker.id==1);++state->lines;state->line_text.push_back(s.value);}
 bool IsConversationActive(long n){assert(n==state->conversations);if(state->wait_conversation>0){--state->wait_conversation;return false;}return true;}
 void MiniMapRemoveMarker(const CScriptThing&t){assert(t.id==1);++state->mini_remove;}
 void MiniMapAddMarker(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.value=="HUD_ORB_QUEST_OPTIONAL");++state->mini_add;}
 void EntitySetInFaction(const CScriptThing&t,const CCharString&s){assert(t.id==1);state->factions.push_back(s.value);}
 CScriptThing GetThingWithScriptName(const CCharString&s){if(s.value=="MB_OutsideBanditCamp")return CScriptThing(30);assert(s.value=="HornBlowMarker");return CScriptThing(31);}
 bool IsCameraPosOnScreen(const C3DVector&p){assert(p.X==1&&p.Y==2&&p.Z==4);return state->screen;}
 unsigned long PlayCriteriaSoundOnThing(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.value=="KNG_HORN");++state->sound;return 1;}
 void EntityTeleportToPosition(const CScriptThing&t,const C3DVector&p,float angle,bool a,bool b){assert(t.id==1&&p.X==8&&p.Y==16&&p.Z==32&&angle==0&&!a&&!b);++state->teleport;state->moved=true;}
 void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);++state->remove;}
};
class CEntityScriptBase {public:CGameScriptInterfaceBase&Game;CScriptThing Me;CEntityScriptBase(CGameScriptInterfaceBase&g):Game(g),Me(1){}void SetPredicate(const NScript::CIsThingAlive&p){assert(p.Thing.id==1);}bool IsActiveThreadTerminating()const{return ++state->guards>=state->terminate_at;}};
namespace NScript {
class CQ_BreakSiegeScript {public:bool BanditLeaderFree,EndCutsceneDone,EndCutsceneStarts,MissionSucceeded,MissionFailed;long ReasonForFailure;CQ_BreakSiegeScript():BanditLeaderFree(false),EndCutsceneDone(false),EndCutsceneStarts(false),MissionSucceeded(false),MissionFailed(false),ReasonForFailure(-1){}
 class CImprisonedBanditLeader:public CEntityScriptBase {public:CQ_BreakSiegeScript&ParentClass;CImprisonedBanditLeader(CGameScriptInterfaceBase&g,CQ_BreakSiegeScript&p):CEntityScriptBase(g),ParentClass(p){}void Main();};
}; }
void CGameScriptInterfaceBase::NewScriptFrame(){++state->frames;assert(state->frames<1000);if(state->timer_value>0)--state->timer_value;if(state->frames>=state->release_frame)owner->BanditLeaderFree=true;if(state->frames>=state->done_frame)owner->EndCutsceneDone=true;if(state->frames>=state->starts_frame)owner->EndCutsceneStarts=true;}
static bool IsDistanceBetweenThingsOver(const CScriptThing&a,const CScriptThing&b,float limit){if(a.id==31){assert(b.id==2&&limit==10.0f);return state->horn_distance>limit;}assert(a.id==1&&b.id==30&&limit==2.0f);return !state->moved&&state->escape_distance>limit;}
static int ControlledRandom(){return state->random_calls++;}
#define rand ControlledRandom

void NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main()
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
    Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    std::vector<CScriptThing> RestlessMarkers;
    Game.GetAllThingsWithScriptName("RestlessMarker",RestlessMarkers);
    CScriptThing DestMarker=RestlessMarkers[rand()%RestlessMarkers.size()];
    CTimer RestlessFrequency;
    RestlessFrequency=5;
    while (!ParentClass.BanditLeaderFree && !ParentClass.EndCutsceneDone) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            CScriptThing Guard=Game.GetNearestWithScriptName(Me,"GuardHoldingBandit");
            Game.AddPersonToConversation(HLStyleConversationIndex,Guard);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_PRISONER_CURSES_AT_YOU",false,Me,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_GUARD_STOP_THAT",false,Guard,Game.GetHero());
            while (!Game.IsConversationActive(HLStyleConversationIndex)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            CScriptThing Guard=Game.GetNearestWithScriptName(Me,"GuardHoldingBandit");
            Game.AddPersonToConversation(HLStyleConversationIndex,Guard);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_PRISONER_CURSES_AT_YOU_ON_HIT",false,Me,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_GUARD_STOP_THAT",false,Guard,Game.GetHero());
            while (!Game.IsConversationActive(HLStyleConversationIndex)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!seh_me.IsPerformingScriptTask() && RestlessFrequency==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToThing(DestMarker,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            DestMarker=RestlessMarkers[rand()%RestlessMarkers.size()];
            RestlessFrequency=10;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.MiniMapRemoveMarker(Me);
    if (!ParentClass.EndCutsceneDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.MiniMapAddMarker(Me,"HUD_ORB_QUEST_OPTIONAL");
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetInFaction(Me,"FACTION_MONSTERS");
        CScriptThing MB_OutsideCamp=Game.GetThingWithScriptName("MB_OutsideBanditCamp");
        if (IsDistanceBetweenThingsOver(Game.GetThingWithScriptName("HornBlowMarker"),Game.GetHero(),10.0f) && !Game.IsCameraPosOnScreen(Game.GetThingWithScriptName("HornBlowMarker").GetPos())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.PlayCriteriaSoundOnThing(Me,"KNG_HORN");
        }
        while (IsDistanceBetweenThingsOver(Me,MB_OutsideCamp,2.0f)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.EndCutsceneStarts) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToThing(MB_OutsideCamp,0.0f,ENTITY_MOVE_RUN,false,true,false,true);
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                Game.EntitySetInFaction(Me,"FACTION_NEUTRALS");
                while (!ParentClass.EndCutsceneDone) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityTeleportToPosition(Me,Me.GetHomePos(),0.0f,false,false);
                break;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (!ParentClass.EndCutsceneStarts) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.ReasonForFailure=3;
        if (!ParentClass.MissionSucceeded) ParentClass.MissionFailed=true;
    } else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.MiniMapRemoveMarker(Me);
        Game.RemoveThing(Me,false,true);
    }
}

#undef rand
struct Outcome { bool failed,success;long reason; };
static Outcome Run(MockState& s,int initial_flags=0) {
 state=&s;int before=live_things;Outcome out;
 {CGameScriptInterfaceBase game;NScript::CQ_BreakSiegeScript parent;game.owner=&parent;
  parent.BanditLeaderFree=(initial_flags&1)!=0;parent.EndCutsceneDone=(initial_flags&2)!=0;parent.EndCutsceneStarts=(initial_flags&4)!=0;parent.MissionSucceeded=(initial_flags&8)!=0;
  NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader actor(game,parent);actor.Main();
  assert(s.resources==0&&s.timer_reg==s.timer_dereg&&s.timer_reg<=1);assert(s.guards<=s.terminate_at);assert(s.lines==2*s.conversations&&s.people==2*s.conversations&&s.face_calls==s.conversations);
  out.failed=parent.MissionFailed;out.success=parent.MissionSucceeded;out.reason=parent.ReasonForFailure;
 }assert(live_things==before);return out;
}
int main(){
 int cases=0;
 {MockState s;s.terminate_at=1;Run(s);assert(s.acquire_calls==0&&s.timer_reg==0&&s.random_calls==0);++cases;}
 float horn[]={9.5f,10.0f,10.5f,std::numeric_limits<float>::quiet_NaN()};float escape[]={1.5f,2.0f,2.5f,std::numeric_limits<float>::quiet_NaN()};
 for(int h=0;h<4;++h)for(int d=0;d<4;++d)for(int screen=0;screen<2;++screen)for(int success=0;success<2;++success){
  MockState s;s.horn_distance=horn[h];s.escape_distance=escape[d];s.screen=screen!=0;Outcome o=Run(s,success?8:0);
  assert(o.reason==3&&o.failed==!success&&o.success==(success!=0));assert(s.sound==(h==2&&!screen?1:0));assert(s.moves_escape==(d==2?1:0));assert(s.moves_pace==0&&s.random_calls==1&&s.timer_set==1);assert(s.mini_add==1&&s.mini_remove==1&&s.remove==0&&s.teleport==0);assert(s.factions.size()==1&&s.factions[0]=="FACTION_MONSTERS");++cases;
 }
 for(int starts=0;starts<2;++starts)for(int success=0;success<2;++success){MockState s;Outcome o=Run(s,2|(starts?4:0)|(success?8:0));assert(s.mini_add==0&&s.sound==0&&s.moves_escape==0);assert(s.remove==starts&&s.mini_remove==1+starts);assert(o.reason==(starts?-1:3));assert(o.failed==(!starts&&!success));++cases;}
 {MockState s;s.done_frame=4;Outcome o=Run(s,4);assert(!o.failed&&o.reason==-1);assert(s.clear_calls==1&&s.teleport==1&&s.moves_escape==0&&s.remove==1);assert(s.factions.size()==2&&s.factions[0]=="FACTION_MONSTERS"&&s.factions[1]=="FACTION_NEUTRALS");++cases;}
 for(int talk=0;talk<2;++talk)for(int physical=0;physical<2;++physical)for(int special=0;special<2;++special)for(int heal=0;heal<2;++heal){
  MockState s;s.release_frame=2;s.talked=talk!=0;s.physical=physical!=0;s.special=special!=0;s.heal=heal!=0;Run(s);int attacked=physical||(special&&!heal);assert(s.conversations==talk+attacked);assert(s.lines==2*(talk+attacked));int i=0;if(talk){assert(s.line_text[i++]=="TEXT_QST_018_PRISONER_CURSES_AT_YOU");assert(s.line_text[i++]=="TEXT_QST_018_GUARD_STOP_THAT");}if(attacked){assert(s.line_text[i++]=="TEXT_QST_018_PRISONER_CURSES_AT_YOU_ON_HIT");assert(s.line_text[i++]=="TEXT_QST_018_GUARD_STOP_THAT");}++cases;
 }
 {MockState s;s.release_frame=7;Run(s);assert(s.moves_pace==1&&s.random_calls==2&&s.timer_set==2);++cases;}
 for(int delay=0;delay<4;++delay){MockState s;s.wait_acquire=delay;Run(s);assert(s.acquire_calls==delay+2);++cases;}
 for(int delay=0;delay<4;++delay){MockState s;s.release_frame=2;s.talked=true;s.wait_conversation=delay;Run(s);assert(s.conversations==1&&s.frames>=2+delay);++cases;}
 for(int mode=0;mode<8;++mode)for(int stop=1;stop<=120;++stop){
  MockState s;s.terminate_at=stop;s.release_frame=7;s.talked=(mode&1)!=0;s.physical=(mode&2)!=0;s.special=(mode&4)!=0;s.wait_acquire=2;s.wait_conversation=2;if(mode==7){s.starts_frame=8;s.done_frame=11;}Run(s);++cases;
 }
 assert(cases==1055);std::puts("00d21040_TEST PASS");return 0;
}
