// Selected source 69ae33c952f9b2667de41fde4a011d67c2e8c686187543cc4920ac3447a02154

#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,paused,starts,startFailures,taskDelay,taskUntil,setup,faces,cameras,modified,questCalls,healthCalls,talkReads,hitReads,anyReads,healReads,completeAt;bool talk,hit,ability,heal,completed,inHit,pattern;float health;std::vector<int>spoken;State():frame(0),limit(5),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),starts(0),startFailures(0),taskDelay(0),taskUntil(0),setup(0),faces(0),cameras(0),modified(0),questCalls(0),healthCalls(0),talkReads(0),hitReads(0),anyReads(0),healReads(0),completeAt(-1),talk(false),hit(false),ability(false),heal(false),completed(false),inHit(false),pattern(false),health(10.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string s;CCharString(const char*p):s(p){++state->strings;}CCharString(const CCharString&o):s(o.s){++state->strings;}~CCharString(){--state->strings;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};class TRetVoid{public:void Return(){}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}void chk(const CCharString&s)const{assert(id==-1&&s.s=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s)const{chk(s);++state->talkReads;return state->pattern?state->frame%2==0:state->talk;}bool MsgIsHitBy(const CCharString&s)const{chk(s);++state->hitReads;return state->hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{chk(s);++state->anyReads;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{chk(s);assert(a==HERO_ABILITY_HEAL_LIFE_SPELL);++state->healReads;return state->heal;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&o):valid(o.valid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod mode,bool a,bool b,bool c){assert(valid&&h.id==-2&&mode==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1&&state->health>0);std::string t(s);int code=-1;if(t=="TEXT_QST_005_V2_ENTRANCE_GUARD2_RETURN_WELL_DONE")code=0;if(t=="TEXT_QST_005_V2_ENTRANCE_GUARD2_GREETING")code=1;if(t=="TEXT_QST_005_V2_ENTRANCE_GUARD2_GREETING_AGAIN")code=2;if(t=="TEXT_QST_005_V2_ENTRANCE_GUARD2_ATTACKED")code=3;assert(code>=0);if(code==3)assert(state->inHit);state->spoken.push_back(code);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{assert(valid);return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(state->movies==0);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST);return r.valid=++state->starts>state->startFailures;}void EntitySetInFaction(const CScriptThing&m,const CCharString&s){self(m);assert(s.s=="FACTION_HERO");++state->setup;}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);++state->setup;}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&m,bool a){self(m);assert(!a);++state->setup;}void SetIsPushableByHero(CScriptThing m,bool a){self(m);assert(!a);++state->setup;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.s=="GuardStareMarker");return CScriptThing(1);}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&t,bool a){self(m);assert(t.id==1&&!a);++state->faces;}bool IsQuestCompleted(const CCharString&s){assert(s.s=="Q_Arena");++state->questCalls;return state->completed||(state->completeAt>=0&&state->frame>=state->completeAt);}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.s.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;}}CScriptThing&GetHero(){return hero;}float GetHealth(const CScriptThing&m){self(m);++state->healthCalls;return state->health;}void CameraDefault(){assert(state->inHit&&state->pauseDepth==1);++state->cameras;}void ModifyThingHealth(const CScriptThing&m,float n,bool b){self(m);assert(n==100.0f&&!b&&state->inHit&&state->cameras==state->modified+1);++state->modified;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CQ_ArenaHoldingScriptScript{public:bool&InHitCutsceneAlready;CQ_ArenaHoldingScriptScript():InHitCutsceneAlready(state->inHit){}class CEntranceGuard2{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_ArenaHoldingScriptScript&ParentClass;CEntranceGuard2(CGameScriptInterfaceBase&g,CQ_ArenaHoldingScriptScript&p):Game(g),Me(-1),ParentClass(p){}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}
void NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard2::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetInFaction(Me,"FACTION_HERO");
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.SetIsPushableByHero(Me,false);
    bool HaveChatted=false;
    Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("GuardStareMarker"),false);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.IsQuestCompleted("Q_Arena")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ENTRANCE_GUARD2_RETURN_WELL_DONE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("GuardStareMarker"),false);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (HaveChatted) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ENTRANCE_GUARD2_GREETING_AGAIN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("GuardStareMarker"),false);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ENTRANCE_GUARD2_GREETING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("GuardStareMarker"),false);
                    HaveChatted=true;
                }
            }
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready=true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ENTRANCE_GUARD2_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("GuardStareMarker"),false);
                Game.CameraDefault();
                Game.ModifyThingHealth(Me,100.0f,false);
                ParentClass.InHitCutsceneAlready=false;
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}


int scenarios=0;
void run(State&s){++scenarios;state=&s;{CGameScriptInterfaceBase game;NScript::CQ_ArenaHoldingScriptScript parent;NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard2 guard(game,parent);guard.Main();}assert(s.things==0&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0);assert(s.cameras==s.modified&&s.healthCalls==s.paused&&s.spoken.size()<=static_cast<unsigned>(s.paused));}
int main(){
 {State s;run(s);assert(s.setup==4&&s.faces==1&&s.spoken.empty()&&s.paused==0);}
 {State s;s.talk=true;run(s);assert(s.spoken.size()==5&&s.spoken[0]==1);for(int i=1;i<5;++i)assert(s.spoken[i]==2);assert(s.faces==6&&s.hitReads==0&&s.questCalls==5);}
 {State s;s.talk=true;s.completed=true;run(s);assert(s.spoken.size()==5);for(int i=0;i<5;++i)assert(s.spoken[i]==0);}
 {State s;s.talk=true;s.completeAt=2;run(s);assert(s.spoken.size()==5&&s.spoken[0]==1&&s.spoken[1]==2&&s.spoken[2]==0);}
 float values[5]={-2.0f,0.0f,0.5f,10.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int flags=0;flags<64;++flags)for(int h=0;h<5;++h){State s;s.talk=(flags&1)!=0;s.hit=(flags&2)!=0;s.ability=(flags&4)!=0;s.heal=(flags&8)!=0;s.inHit=(flags&16)!=0;s.completed=(flags&32)!=0;s.health=values[h];run(s);bool attack=!s.talk&&(s.hit||(s.ability&&!s.heal))&&!(flags&16);int events=(s.talk||attack)?5:0;assert(s.paused==events&&s.faces==events+1&&s.modified==(attack?5:0));assert(s.spoken.size()==static_cast<unsigned>(h==2||h==3?events:0));assert(s.inHit==((flags&16)!=0));if(s.talk)assert(s.hitReads==0);if(s.hit&&!s.talk)assert(s.anyReads==0&&s.healReads==0);}
 for(int delay=0;delay<6;++delay)for(int fail=0;fail<6;++fail){State s;s.limit=20;s.startFailures=fail;s.taskDelay=delay;s.talk=true;run(s);assert(s.starts==fail+1&&s.spoken.size()>0&&s.spoken[0]==1&&s.setup==4);}
 {State s;s.pattern=true;s.hit=true;run(s);assert(s.spoken.size()==5&&s.spoken[0]==1&&s.spoken[1]==3&&s.spoken[2]==2&&s.spoken[3]==3&&s.spoken[4]==2&&s.modified==2);}
 {State s;s.limit=0;run(s);assert(s.setup==0&&s.paused==0&&s.starts==1);}
 {State s;s.startFailures=20;run(s);assert(s.setup==0&&s.paused==0&&s.starts==5);}
 for(int stop=1;stop<=160;++stop)for(int mode=0;mode<16;++mode){State s;s.stopCheck=stop;s.limit=12;switch(mode){case 0:break;case 1:s.talk=true;break;case 2:s.talk=true;s.completed=true;break;case 3:s.talk=true;s.health=0;break;case 4:s.talk=true;s.taskDelay=3;break;case 5:s.hit=true;break;case 6:s.ability=true;break;case 7:s.ability=true;s.heal=true;break;case 8:s.hit=true;s.taskDelay=3;break;case 9:s.hit=true;s.inHit=true;break;case 10:s.hit=true;s.health=0;break;case 11:s.pattern=true;s.hit=true;break;case 12:s.startFailures=30;break;case 13:s.talk=true;s.completeAt=3;break;case 14:s.talk=true;s.startFailures=3;break;case 15:s.hit=true;s.health=std::numeric_limits<float>::quiet_NaN();break;}run(s);}
 assert(scenarios==2923);std::puts("00cf0d20_TEST PASS");return 0;
}
