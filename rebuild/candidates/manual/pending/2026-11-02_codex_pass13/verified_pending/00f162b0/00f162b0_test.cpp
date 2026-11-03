// Selected source SHA256 aec9e9c783eb6a1a002ba2da00aa926c81c384d0f3016cadc1bce80ecbc962bd
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
struct State{int arenaState,introUntil,hint;bool early,cham;int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,paused,unpaused,startAttempts,startFailures,taskDelay,taskUntil,cleared,released,modified,allies,healthChecks,physicalChecks,abilityChecks,healChecks,talkChecks,randomValue,randomCalls,round;bool talk,physical,ability,heal,inHit,initialValid,busyOnClaim;float health;std::vector<std::string>speech,setup;State():arenaState(3),introUntil(0),hint(0),early(false),cham(false),frame(0),limit(1),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),unpaused(0),startAttempts(0),startFailures(0),taskDelay(0),taskUntil(0),cleared(0),released(0),modified(0),allies(0),healthChecks(0),physicalChecks(0),abilityChecks(0),healChecks(0),talkChecks(0),randomValue(0),randomCalls(0),round(7),talk(false),physical(false),ability(false),heal(false),inHit(false),initialValid(false),busyOnClaim(false),health(100.0f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
extern "C" int rand(void) throw(){assert(state->pauseDepth==1);++state->randomCalls;return state->randomValue;}
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=1.5f,float y=-2.0f,float z=3.25f):X(x),Y(y),Z(z){}};
class CScriptThing{public:int id;bool fromHandle;CScriptThing(int n=-1,bool h=false):id(n),fromHandle(h){++state->things;}CScriptThing(const CScriptThing&o):id(o.id),fromHandle(o.fromHandle){++state->things;}~CScriptThing(){--state->things;}const C3DVector&GetPos()const{assert(id==1);static C3DVector p;return p;}void target(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s)const{target(s);++state->talkChecks;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{target(s);++state->physicalChecks;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{target(s);++state->abilityChecks;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s)const{target(s);assert(a==14);++state->healChecks;return state->heal;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum EScriptingStateGroups{ESSG_WANDER_NEAR=4};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(state->initialValid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->movies==0&&state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->released;}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1,true);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.id==-2&&valid&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1);state->speech.push_back(s);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}void ClearCommands(){assert(valid&&IsPerformingScriptTask());++state->cleared;state->taskUntil=state->frame;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;assert(state->movies==1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m){assert(m.id==-1&&!m.fromHandle);}void EntityUnsetAsOpinionSource(const CScriptThing&m,bool b){self(m);assert(!b);state->setup.push_back("opinion-source");}void EntitySetInFaction(const CScriptThing&m,const CCharString&s){self(m);assert(s.text=="FACTION_HERO");state->setup.push_back("faction");}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);state->setup.push_back("killable");}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&m,bool b){self(m);assert(!b);state->setup.push_back("combo");}void EntitySetOpinionReactionsEnabled(const CScriptThing&m,bool b){self(m);assert(!b);state->setup.push_back("reactions");}void SetCreatureBrain(const CScriptThing&m,const CCharString&s){self(m);assert(s.text=="BRAIN_ARENA_CELLS");state->setup.push_back("brain");}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="ShadowPoint");return CScriptThing(1);}void SetWanderCentrePoint(CScriptThing m,C3DVector p){self(m);assert(p.X==1.5f&&p.Y==-2.0f&&p.Z==3.25f);state->setup.push_back("centre");}void SetWanderMinDistance(CScriptThing m,float f){self(m);assert(f==0.0f);state->setup.push_back("minimum");}void SetWanderMaxDistance(CScriptThing m,float f){self(m);assert(f==5.0f);state->setup.push_back("maximum");}void SetScriptingStateGroup(CScriptThing m,EScriptingStateGroups g){self(m);assert(g==ESSG_WANDER_NEAR);state->setup.push_back("wander");}void NewScriptFrame(){assert(state->frame<255);++state->frame;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST);r.valid=++state->startAttempts>state->startFailures;if(r.valid&&state->busyOnClaim)state->taskUntil=state->frame+20;return r.valid;}CScriptThing&GetHero(){return hero;}float GetHealth(const CScriptThing&m){assert(m.id==-1&&m.fromHandle&&state->pauseDepth==1);++state->healthChecks;return state->health;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;++state->unpaused;}}void ModifyThingHealth(const CScriptThing&m,float f,bool b){self(m);assert(f==10000.0f&&!b&&state->pauseDepth==1);++state->modified;}void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert(state->pauseDepth==1);if(state->allies%2==0)assert(a.id==-1&&b.id==-2);else assert(a.id==-2&&b.id==-1);++state->allies;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CQ_ArenaScript{public:enum {MAS_CELLS_PRE_ARENA_INTRO=2,MAS_CELLS_PRE_ARENA_NORMAL=3,MAS_CELLS_PRE_ARENA_NORMAL_GO=5};long ArenaRound,ArenaState;bool InHitCutsceneAlready;CQ_ArenaScript():ArenaRound(state->round),ArenaState(state->introUntil?2:state->arenaState),InHitCutsceneAlready(state->inHit){}class CShadow{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_ArenaScript&ParentClass;bool EarlyTalk,ChamTalk;long HintNumber;CShadow(CGameScriptInterfaceBase&g,CQ_ArenaScript&p):Game(g),Me(-1),ParentClass(p),EarlyTalk(state->early),ChamTalk(state->cham),HintNumber(state->hint){}bool IsActiveThreadTerminating()const{if(state->frame>=state->introUntil)ParentClass.ArenaState=state->arenaState;return state->term();}void Main();};};}

void NScript::CQ_ArenaScript::CShadow::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.EntityUnsetAsOpinionSource(Me,false);
    Game.EntitySetInFaction(Me,"FACTION_HERO");
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    while (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_INTRO) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetCreatureBrain(Me,"BRAIN_ARENA_CELLS");
    Game.SetWanderCentrePoint(Me,Game.GetThingWithScriptName("ShadowPoint").GetPos());
    Game.SetWanderMinDistance(Me,0.0f);
    Game.SetWanderMaxDistance(Me,5.0f);
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
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL && !EarlyTalk) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_FIRST_CHAT_PRE_CHAM",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    EarlyTalk=true;
                } else if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL_GO && !ChamTalk) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_FIRST_CHAT_POST_CHAM",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ChamTalk=true;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    switch (HintNumber) {
                    case 0:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_FIRST_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ++HintNumber;
                        break;
                    case 1:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_SECOND_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ++HintNumber;
                        break;
                    case 2:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_THIRD_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ++HintNumber;
                        break;
                    case 3:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_FOURTH_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ++HintNumber;
                        break;
                    case 4:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_FIFTH_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ++HintNumber;
                        break;
                    case 5:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_SIXTH_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ++HintNumber;
                        break;
                    case 6:
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_SEVENTH_HINT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        HintNumber=5;
                        break;
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready=true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_005_V2_ARENA_SHADOW_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.ModifyThingHealth(Me,10000.0f,false);
                Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
                Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
                ParentClass.InHitCutsceneAlready=false;
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

void run(State&s){state=&s;{CGameScriptInterfaceBase g;NScript::CQ_ArenaScript p;NScript::CQ_ArenaScript::CShadow t(g,p);t.Main();s.inHit=p.InHitCutsceneAlready;s.early=t.EarlyTalk;s.cham=t.ChamTalk;s.hint=int(t.HintNumber);assert(s.things==2&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.paused==s.unpaused);const char*names[]={"opinion-source","faction","killable","combo","brain","centre","minimum","maximum"};assert(s.setup.size()==4||s.setup.size()==8);for(unsigned i=0;i<s.setup.size();++i)assert(s.setup[i]==names[i]);}assert(s.things==0);}
int main(){int cases=0;const char*lines[]={"TEXT_QST_005_V2_ARENA_SHADOW_FIRST_CHAT_PRE_CHAM","TEXT_QST_005_V2_ARENA_SHADOW_FIRST_CHAT_POST_CHAM","TEXT_QST_005_V2_ARENA_SHADOW_FIRST_HINT","TEXT_QST_005_V2_ARENA_SHADOW_SECOND_HINT","TEXT_QST_005_V2_ARENA_SHADOW_THIRD_HINT","TEXT_QST_005_V2_ARENA_SHADOW_FOURTH_HINT","TEXT_QST_005_V2_ARENA_SHADOW_FIFTH_HINT","TEXT_QST_005_V2_ARENA_SHADOW_SIXTH_HINT","TEXT_QST_005_V2_ARENA_SHADOW_SEVENTH_HINT"};const char*hitLine="TEXT_QST_005_V2_ARENA_SHADOW_ATTACKED";float health[]={0.5f,0.0f,-1.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int stage=-2;stage<=10;++stage)if(stage!=2)for(int flags=0;flags<4;++flags)for(int hint=-1;hint<=7;++hint)for(int h=0;h<4;++h){State s;s.talk=true;s.arenaState=stage;s.early=(flags&1)!=0;s.cham=(flags&2)!=0;s.hint=hint;s.health=health[h];bool early=s.early,cham=s.cham;int next=hint,n=-1;if(stage==3&&!early){early=true;n=0;}else if(stage==5&&!cham){cham=true;n=1;}else if(hint>=0&&hint<=6){n=hint+2;next=hint==6?5:hint+1;}run(s);assert(s.early==early&&s.cham==cham&&s.hint==next&&s.paused==1&&s.released==1&&s.physicalChecks==0);assert(s.healthChecks==int(n>=0));if(n>=0&&h==0)assert(s.speech.size()==1&&s.speech[0]==lines[n]);else assert(s.speech.empty());++cases;}
 for(int flags=0;flags<16;++flags){State s;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;s.inHit=(flags&8)!=0;bool was=s.inHit,hit=s.physical||(s.ability&&!s.heal);run(s);assert(s.physicalChecks==1&&s.abilityChecks==int(!s.physical)&&s.healChecks==int(!s.physical&&s.ability));assert(s.startAttempts==int(hit)&&s.released==int(hit)&&s.paused==int(hit&&!was)&&s.modified==int(hit&&!was)&&s.allies==2*s.modified&&s.inHit==was);assert(s.speech.size()==unsigned(hit&&!was));if(!s.speech.empty())assert(s.speech[0]==hitLine);++cases;}
 for(int flags=0;flags<8;++flags){State s;s.talk=true;s.physical=(flags&1)!=0;s.ability=(flags&2)!=0;s.heal=(flags&4)!=0;run(s);assert(s.physicalChecks==0&&s.abilityChecks==0&&s.healChecks==0&&s.speech.size()==1&&s.speech[0]==lines[0]&&s.modified==0);++cases;}
 for(int h=0;h<4;++h){State s;s.physical=true;s.health=health[h];run(s);assert(s.healthChecks==1&&s.speech.size()==unsigned(h==0)&&s.modified==1&&s.allies==2&&!s.inHit);++cases;}
 for(int talk=0;talk<2;++talk){State s;s.talk=talk!=0;s.physical=true;s.busyOnClaim=true;s.initialValid=true;run(s);assert(s.cleared==1&&s.released==2&&s.speech.size()==1);++cases;}
 for(int delay=0;delay<5;++delay){State s;s.introUntil=delay;s.talk=true;s.limit=delay+1;run(s);assert(s.setup.size()==8&&s.speech.size()==1&&s.early&&s.frame==delay+1);++cases;}
 {State s;s.arenaState=2;s.limit=5;s.talk=true;run(s);assert(s.setup.size()==4&&s.talkChecks==0&&s.frame==5&&s.speech.empty());++cases;}
 {State s;s.limit=0;run(s);assert(s.setup.size()==4&&s.talkChecks==0&&s.frame==0);++cases;}
 {State s;s.talk=true;s.startFailures=3;s.limit=4;run(s);assert(s.startAttempts==4&&s.frame==4&&s.speech.size()==1);++cases;}
 {State s;s.talk=true;s.startFailures=50;s.limit=3;run(s);assert(s.startAttempts==3&&s.speech.empty()&&s.released==0);++cases;}
 {State s;s.talk=true;s.taskDelay=50;s.limit=3;run(s);assert(s.speech.size()==1&&s.frame==3&&s.released==0&&s.paused==1&&!s.early);++cases;}
 {State s;s.physical=true;s.taskDelay=50;s.limit=3;run(s);assert(s.speech.size()==1&&s.modified==0&&s.inHit&&s.released==0&&s.paused==1);++cases;}
 {State s;s.physical=true;s.taskDelay=2;s.limit=3;run(s);assert(s.speech.size()==1&&s.modified==1&&!s.inHit&&s.released==1&&s.frame==3);++cases;}
 {State s;s.talk=true;s.limit=12;run(s);int expected[]={0,2,3,4,5,6,7,8,7,8,7,8};assert(s.speech.size()==12&&s.released==12&&s.hint==5&&s.early);for(int i=0;i<12;++i)assert(s.speech[i]==lines[expected[i]]);++cases;}
 for(int mode=0;mode<16;++mode)for(int stop=1;stop<=120;++stop){State s;s.limit=25;s.stopCheck=stop;s.talk=mode<10;s.physical=mode>=10;s.arenaState=mode%3==0?5:3;s.early=mode%4==0;s.cham=mode%5==0;s.hint=mode%8-1;s.introUntil=mode==8?3:0;s.taskDelay=mode==6||mode==12?3:0;s.startFailures=mode==7||mode==11?5:0;s.inHit=mode==13;s.initialValid=mode==14;s.busyOnClaim=mode==15;s.health=mode==5?0.0f:100.0f;run(s);assert(s.checks<=stop&&s.allies==2*s.modified);++cases;}
 assert(cases==3691);std::puts("00f162b0_TEST PASS");}
