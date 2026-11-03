#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
static int strings=0,things=0,handles=0,movies=0,pauses=0,predicates=0,owners[3]={0,0,0};
static int frame,checks,failures,tasks,startsMe,startsHero,anyCalls,healCalls,distanceCalls,posCalls;
static bool stopped=false,removed=false;
static std::vector<std::string> events;
static void action(const char*s){assert(!stopped);events.push_back(s);}
class CCharString{public:std::string text;CCharString():text(){++strings;}CCharString(const char*s):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}~CCharString(){--strings;}bool operator<(const CCharString&s)const{return text<s.text;}};
class C3DVector{public:float X,Y,Z;C3DVector():X(1),Y(2),Z(3){}};
struct Config{bool complete,waved,talk,hit,special,heal,heroAcquire,physics;bool questActive;int state;int initialFail,retryFail,wait,stop,limit;long trigger;float health,distance;Config():complete(false),waved(false),talk(false),hit(false),special(false),heal(false),heroAcquire(true),physics(true),questActive(false),state(2),initialFail(0),retryFail(0),wait(0),stop(0),limit(4),trigger(10),health(100),distance(5){}};
static Config config;
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGH=3,SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
class CScriptThing{public:int id;C3DVector pos;CScriptThing(int i=0):id(i){++things;}CScriptThing(const CScriptThing&t):id(t.id),pos(t.pos){++things;}~CScriptThing(){--things;}const C3DVector&GetPos()const{assert(id==3);++posCalls;return pos;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");action("talkQuery");return config.talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");action("hitQuery");return config.hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++anyCalls;return config.special;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(id==1&&a==14&&s.text=="SCRIPT_NAME_HERO");++healCalls;return config.heal;}void SetFriendsWithEverythingFlag(bool b){assert(id==1&&b);action("friends");}};
class CScriptGameResourceObjectScriptedThingBase{int entity;void drop(){if(entity)--owners[entity];entity=0;}public:CScriptGameResourceObjectScriptedThingBase():entity(0){++handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&s):entity(s.entity){++handles;if(entity)++owners[entity];}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&s){if(this!=&s){drop();entity=s.entity;if(entity)++owners[entity];}return *this;}~CScriptGameResourceObjectScriptedThingBase(){drop();--handles;}void acquire(int i){drop();entity=i;++owners[i];}bool IsValid()const{return entity!=0;}void Release(){action("release");drop();failures=config.retryFail;}CScriptThing GetScriptThing()const{return CScriptThing(entity);}void Speak(const CScriptThing&t,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(entity==1&&t.id==2&&std::string(s).find("TEXT_QST_074_CHIEF_")==0&&m==0&&!a&&b&&!c);action(s);tasks=config.wait;}bool IsPerformingScriptTask()const{return tasks>0;}void PlayAnimation(const CCharString&s,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(entity==1&&s.text=="ST_HELLO"&&!a&&!b&&!c&&d&&e==config.physics&&!f&&!g);action("wave");}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;}};
class GameMock{CScriptThing hero;public:GameMock():hero(2){}void NewScriptFrame(){action("frame");++frame;if(tasks)--tasks;}void EntitySetAsOpinionSource(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.text=="OPINION_SOURCE_MAZE_BS_PUB");action("opinion");}void MiniMapAddMarker(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.text=="HUD_ORB_QUEST_CORE");action("marker");}void SetThingHasInformation(const CScriptThing&t,bool a,bool b,bool c){assert(t.id==1&&!a&&!b&&!c);action("information");}void SetIsPushableByHero(CScriptThing t,bool b){assert(t.id==1&&!b);action("pushable");}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert((p==3||p==4)&&(t.id==1||t.id==2));action(t.id==1?"startMe":"startHero");if(t.id==2){assert(p==4);++startsHero;if(config.heroAcquire)s.acquire(2);return config.heroAcquire;}++startsMe;if(failures){--failures;return false;}s.acquire(1);return true;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){action("pause");++pauses;}else{assert(pauses==1&&movies==1);--pauses;events.push_back("unpause");}}float GetHealth(const CScriptThing&t){assert(t.id==1);return config.health;}CScriptThing&GetHero(){return hero;}void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);action("killable");}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&t,bool b){assert(t.id==1&&!b);action("combo");}void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&t,bool b){assert(t.id==1&&!b);action("stats");}bool IsQuestActive(const CCharString&s){assert(s.text=="Q_WhiteBalverineWW");action("questActive");return config.questActive;}void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);action("remove");removed=true;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="M_MazeExit");action("lookup");return CScriptThing(3);}long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);action("conversation");return 2026;}void AddPersonToConversation(long n,const CScriptThing&t){assert(n==2026&&t.id==2);action("person");}void AddLineToConversation(long n,const CCharString&s,bool b,const CScriptThing&a,const CScriptThing&t){assert(n==2026&&s.text=="TEXT_QST_027_MAZE_CALL_HERO_OVER_10"&&!b&&a.id==1&&t.id==2);action("line");}void EntitySetFacingAngleTowardsThing(const CScriptThing&a,const CScriptThing&b,bool c){assert(a.id==1&&b.id==2&&!c);action("face");}void ClearThingBestEnemyTarget(const CScriptThing&t){assert(t.id==1);action("clearEnemy");}void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);action("clearInfo");}CCharString GetActiveQuestName(){action("questName");return CCharString("quest");}void SetQuestAsCompleted(const CCharString&s,bool a,bool b,bool c){assert(s.text=="quest"&&!a&&!b&&!c);action("complete");}void DeactivateQuestLater(const CCharString&s,unsigned long n){assert(s.text=="quest"&&!n);action("deactivate");}};
class CWideScreenMagicPauseEntities{GameMock&g;public:CWideScreenMagicPauseEntities(GameMock&x):g(x){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g.PauseAllNonScriptedEntities(false);}};
struct ScriptDef{long GSI_MazeCallsOverDistance;};static ScriptDef defs;static ScriptDef*pGlobalScriptDef=&defs;
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==2&&b.id==1&&d==(float)config.trigger);++distanceCalls;return config.distance<d;}
static void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*a,std::map<CCharString,CCharString>*b,bool c,bool d){assert(s.text=="CS_GUARDIANSISTER_BOWERSTONE"&&!a&&!b&&!c&&d&&m.size()==2&&m["ME"].IsValid());assert(m["HERO"].IsValid()==config.heroAcquire);action("cutscene");}
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.id==1);++predicates;}~CIsThingAlive(){--predicates;}};class CQ_WhiteBalverineKnotholeGladeScript{public:enum MAIN_BALVERINE_STATES { MBS_NONE, MBS_WAITING_FOR_GATES, MBS_FIRST_BALVERINE_ATTACK, MBS_AFTER_FIRST_BALVERINE_ATTACK, MBS_SECOND_BALVERINE_ATTACK, MBS_AFTER_SECOND_BALVERINE_ATTACK, MBS_THIRD_BALVERINE_ATTACK, MBS_AFTER_THIRD_BALVERINE_ATTACK };long BalverineState;bool GuardianSpokeToHero;class CKG_Chief{GameMock&Game;CScriptThing Me;CQ_WhiteBalverineKnotholeGladeScript&ParentClass;void Main();public:bool WavedOver;long HitCount;CKG_Chief(GameMock&g,CQ_WhiteBalverineKnotholeGladeScript&p):Game(g),Me(1),ParentClass(p),WavedOver(config.waved),HitCount(0){}void Run(){Main();}void SetPredicate(const CIsThingAlive&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit||removed){stopped=true;return true;}return false;}};};}
void NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main()

{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.EntitySetAsToAddToStatChangesWhenHit(Me,false);
    Game.SetIsPushableByHero(Me,false);
    Game.EntitySetAsKillable(Me,false,true);
    Game.SetThingHasInformation(Me,false,false,false);
    while(!IsActiveThreadTerminating()) {
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(ParentClass.BalverineState==MBS_FIRST_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_AFTER_FIRST_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_SECOND_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_THIRD_BALVERINE_ATTACK) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
                else if(ParentClass.BalverineState==MBS_AFTER_THIRD_BALVERINE_ATTACK && !Game.IsQuestActive("Q_WhiteBalverineWW")) {
                    if(IsActiveThreadTerminating()) return;
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) return;
                        }
                        if(IsActiveThreadTerminating()) return;
                    }
                }
            }
            if(seh_me.IsValid()) seh_me.Release();
        }
        if((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && ParentClass.BalverineState!=MBS_AFTER_THIRD_BALVERINE_ATTACK) {
            if(IsActiveThreadTerminating()) return;
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_074_CHIEF_BEEN_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                }
            }
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
            Me.SetFriendsWithEverythingFlag(true);
            if(seh_me.IsValid()) seh_me.Release();
        }
        Game.NewScriptFrame();
    }
}

struct Result{std::vector<std::string> events;int checks,me,any,heal;};
static Result run(const Config&c){config=c;frame=-1;checks=tasks=startsMe=startsHero=anyCalls=healCalls=distanceCalls=posCalls=0;failures=c.initialFail;events.clear();stopped=removed=false;Result r;{GameMock g;NScript::CQ_WhiteBalverineKnotholeGladeScript p;p.BalverineState=c.state;NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief t(g,p);t.Run();assert(p.BalverineState==c.state);}assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&predicates==0&&owners[1]==0&&owners[2]==0);r.events=events;r.checks=checks;r.me=startsMe;r.any=anyCalls;r.heal=healCalls;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static void before(const Result&r,const char*a,const char*b){unsigned x=0,y=0;while(x<r.events.size()&&r.events[x]!=a)++x;while(y<r.events.size()&&r.events[y]!=b)++y;assert(x<y&&y<r.events.size());}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int i=1;i<=r.checks;++i){c.stop=i;run(c);++scenarios;}}
int main(){Config c;Result r;union{unsigned int bits;float value;}nan;nan.bits=0x7fc00000;
 const char*lines[8]={"","","TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK","TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK","TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK","","TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK","TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK"};
 for(int state=-1;state<=8;++state)for(int active=0;active<2;++active)for(int talk=0;talk<2;++talk)for(int health=0;health<4;++health){c=Config();c.state=state;c.questActive=active!=0;c.talk=talk!=0;c.health=health==0?-1:health==1?0:health==2?100:nan.value;r=run(c);bool speaks=state>=0&&state<8&&lines[state][0]&&health==2&&!(state==7&&active)&&talk;int total=0;for(int i=2;i<8;++i)if(lines[i][0])total+=count(r,lines[i]);assert(total==4*int(speaks));if(speaks)assert(count(r,lines[state])==4);assert(count(r,"questActive")==4*int(talk&&state==7));assert(r.me==4*talk&&count(r,"release")==4*talk&&count(r,"movie")==4*talk);assert(count(r,"combo")==1&&count(r,"stats")==1&&count(r,"pushable")==1&&count(r,"killable")==1&&count(r,"information")==1);before(r,"combo","stats");before(r,"stats","pushable");before(r,"pushable","killable");before(r,"killable","information");sweep(c);}
 for(int state=6;state<=7;++state)for(int flags=0;flags<8;++flags)for(int health=0;health<4;++health){c=Config();c.state=state;c.hit=(flags&1)!=0;c.special=(flags&2)!=0;c.heal=(flags&4)!=0;c.health=health==0?-1:health==1?0:health==2?100:nan.value;c.limit=9;r=run(c);bool raw=c.hit||(c.special&&!c.heal),attack=raw&&state!=7;int iterations=attack?3:9;assert(count(r,"hitQuery")==iterations&&r.any==(c.hit?0:iterations)&&r.heal==(!c.hit&&c.special?iterations:0));assert(r.me==3*int(attack)&&count(r,"friends")==3*int(attack)&&count(r,"release")==3*int(attack));assert(count(r,"TEXT_QST_074_CHIEF_BEEN_ATTACKED")==3*int(attack&&health==2));sweep(c);}
 for(int state=2;state<=7;++state)for(int init=0;init<3;++init)for(int retry=0;retry<3;++retry)for(int wait=0;wait<3;++wait){c=Config();c.state=state;c.initialFail=init;c.retryFail=retry;c.wait=wait;c.limit=25;c.talk=c.hit=true;sweep(c);}
 c=Config();c.talk=c.hit=true;c.limit=9;r=run(c);before(r,"TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK","TEXT_QST_074_CHIEF_BEEN_ATTACKED");assert(r.me==6&&count(r,"release")==6);assert(scenarios>1000);std::puts("00e17840_TEST PASS");}
