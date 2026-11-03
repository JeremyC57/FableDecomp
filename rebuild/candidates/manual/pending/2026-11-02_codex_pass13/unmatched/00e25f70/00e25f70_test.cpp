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
struct Config{bool complete,waved,talk,hit,special,heal,heroAcquire,physics;int initialFail,retryFail,wait,stop,limit;long trigger;float health,distance;Config():complete(false),waved(false),talk(false),hit(false),special(false),heal(false),heroAcquire(true),physics(true),initialFail(0),retryFail(0),wait(0),stop(0),limit(4),trigger(10),health(100),distance(5){}};
static Config config;
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGH=3,SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};
class CScriptThing{public:int id;C3DVector pos;CScriptThing(int i=0):id(i){++things;}CScriptThing(const CScriptThing&t):id(t.id),pos(t.pos){++things;}~CScriptThing(){--things;}const C3DVector&GetPos()const{assert(id==3);++posCalls;return pos;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");return config.talk;}bool MsgIsHitBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");return config.hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++anyCalls;return config.special;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{assert(id==1&&a==14&&s.text=="SCRIPT_NAME_HERO");++healCalls;return config.heal;}void SetFriendsWithEverythingFlag(bool b){assert(id==1&&b);action("friends");}};
class CScriptGameResourceObjectScriptedThingBase{int entity;void drop(){if(entity)--owners[entity];entity=0;}public:CScriptGameResourceObjectScriptedThingBase():entity(0){++handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&s):entity(s.entity){++handles;if(entity)++owners[entity];}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&s){if(this!=&s){drop();entity=s.entity;if(entity)++owners[entity];}return *this;}~CScriptGameResourceObjectScriptedThingBase(){drop();--handles;}void acquire(int i){drop();entity=i;++owners[i];}bool IsValid()const{return entity!=0;}void Release(){action("release");drop();failures=config.retryFail;}CScriptThing GetScriptThing()const{return CScriptThing(entity);}void Speak(const CScriptThing&t,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(entity==1&&t.id==2&&std::string(s)=="TEXT_QST_027_MAZE_ON_HIT_10"&&m==0&&!a&&b&&!c);action("speak");tasks=config.wait;}bool IsPerformingScriptTask()const{return tasks>0;}void PlayAnimation(const CCharString&s,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(entity==1&&s.text=="ST_HELLO"&&!a&&!b&&!c&&d&&e==config.physics&&!f&&!g);action("wave");}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;}};
class GameMock{CScriptThing hero;public:GameMock():hero(2){}void NewScriptFrame(){action("frame");++frame;if(tasks)--tasks;}void EntitySetAsOpinionSource(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.text=="OPINION_SOURCE_MAZE_BS_PUB");action("opinion");}void MiniMapAddMarker(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.text=="HUD_ORB_QUEST_CORE");action("marker");}void SetThingHasInformation(const CScriptThing&t,bool a,bool b,bool c){assert(t.id==1&&!a&&b&&!c);action("information");}void SetIsPushableByHero(CScriptThing t,bool b){assert(t.id==1&&!b);action("pushable");}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert((p==3||p==4)&&(t.id==1||t.id==2));action(t.id==1?"startMe":"startHero");if(t.id==2){assert(p==4);++startsHero;if(config.heroAcquire)s.acquire(2);return config.heroAcquire;}++startsMe;if(failures){--failures;return false;}s.acquire(1);return true;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){action("pause");++pauses;}else{assert(pauses==1&&movies==1);--pauses;events.push_back("unpause");}}float GetHealth(const CScriptThing&t){assert(t.id==1);return config.health;}CScriptThing&GetHero(){return hero;}void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);action("remove");removed=true;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="M_MazeExit");action("lookup");return CScriptThing(3);}long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);action("conversation");return 2026;}void AddPersonToConversation(long n,const CScriptThing&t){assert(n==2026&&t.id==2);action("person");}void AddLineToConversation(long n,const CCharString&s,bool b,const CScriptThing&a,const CScriptThing&t){assert(n==2026&&s.text=="TEXT_QST_027_MAZE_CALL_HERO_OVER_10"&&!b&&a.id==1&&t.id==2);action("line");}void EntitySetFacingAngleTowardsThing(const CScriptThing&a,const CScriptThing&b,bool c){assert(a.id==1&&b.id==2&&!c);action("face");}void ClearThingBestEnemyTarget(const CScriptThing&t){assert(t.id==1);action("clearEnemy");}void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);action("clearInfo");}CCharString GetActiveQuestName(){action("questName");return CCharString("quest");}void SetQuestAsCompleted(const CCharString&s,bool a,bool b,bool c){assert(s.text=="quest"&&!a&&!b&&!c);action("complete");}void DeactivateQuestLater(const CCharString&s,unsigned long n){assert(s.text=="quest"&&!n);action("deactivate");}};
class CWideScreenMagicPauseEntities{GameMock&g;public:CWideScreenMagicPauseEntities(GameMock&x):g(x){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g.PauseAllNonScriptedEntities(false);}};
struct ScriptDef{long GSI_MazeCallsOverDistance;};static ScriptDef defs;static ScriptDef*pGlobalScriptDef=&defs;
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==2&&b.id==1&&d==(float)config.trigger);++distanceCalls;return config.distance<d;}
static void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*a,std::map<CCharString,CCharString>*b,bool c,bool d){assert(s.text=="CS_GUARDIANSISTER_BOWERSTONE"&&!a&&!b&&!c&&d&&m.size()==2&&m["ME"].IsValid());assert(m["HERO"].IsValid()==config.heroAcquire);action("cutscene");}
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.id==1);++predicates;}~CIsThingAlive(){--predicates;}};class CQS_GuardianSisterInfoScript{public:bool GuardianSpokeToHero;class CMazeAtTavern{GameMock&Game;CScriptThing Me;CQS_GuardianSisterInfoScript&ParentClass;void Main();public:bool WavedOver;long HitCount;CMazeAtTavern(GameMock&g,CQS_GuardianSisterInfoScript&p):Game(g),Me(1),ParentClass(p),WavedOver(config.waved),HitCount(0){}void Run(){Main();}void SetPredicate(const CIsThingAlive&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit||removed){stopped=true;return true;}return false;}};};}
void NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main()

{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_MAZE_BS_PUB");
    C3DVector ExitPos=Game.GetThingWithScriptName("M_MazeExit").GetPos();
    Game.MiniMapAddMarker(Me,"HUD_ORB_QUEST_CORE");
    Game.SetThingHasInformation(Me,false,true,false);
    Game.SetIsPushableByHero(Me,false);
    long TriggerDistance=pGlobalScriptDef->GSI_MazeCallsOverDistance;
    while(!IsActiveThreadTerminating()) {
        if(!ParentClass.GuardianSpokeToHero && !WavedOver && IsDistanceBetweenThingsUnder(Game.GetHero(),Me,(float)TriggerDistance)) {
            if(IsActiveThreadTerminating()) return;
            seh_me.PlayAnimation("ST_HELLO",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_027_MAZE_CALL_HERO_OVER_10",false,Me,Game.GetHero());
            WavedOver=true;
        }
        if(!ParentClass.GuardianSpokeToHero) {
            if(IsActiveThreadTerminating()) return;
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
        }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_027_MAZE_ON_HIT_10",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return;
                }
                if(IsActiveThreadTerminating()) return;
            }
            ++HitCount;
            Me.SetFriendsWithEverythingFlag(true);
            Game.ClearThingBestEnemyTarget(Me);
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ME"]=seh_me;
                mymap["HERO"]=seh_hero;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                RunCutsceneMacro("CS_GUARDIANSISTER_BOWERSTONE",mymap,0,0,false,true);
            }
            ParentClass.GuardianSpokeToHero=true;
            Game.ClearThingHasInformation(Me);
            Game.SetQuestAsCompleted(Game.GetActiveQuestName(),false,false,false);
            Game.DeactivateQuestLater(Game.GetActiveQuestName(),0);
            Game.RemoveThing(Me,false,true);
        }
        Game.NewScriptFrame();
    }
}

struct Result{std::vector<std::string> events;bool complete,waved;long hits;int checks,me,hero,any,heal,distance;};
static Result run(const Config&c){config=c;defs.GSI_MazeCallsOverDistance=c.trigger;NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations=c.physics;frame=-1;checks=tasks=startsMe=startsHero=anyCalls=healCalls=distanceCalls=posCalls=0;failures=c.initialFail;events.clear();stopped=removed=false;Result r;{GameMock g;NScript::CQS_GuardianSisterInfoScript p;p.GuardianSpokeToHero=c.complete;NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern t(g,p);t.Run();r.complete=p.GuardianSpokeToHero;r.waved=t.WavedOver;r.hits=t.HitCount;}assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&predicates==0&&owners[1]==0&&owners[2]==0);r.events=events;r.checks=checks;r.me=startsMe;r.hero=startsHero;r.any=anyCalls;r.heal=healCalls;r.distance=distanceCalls;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static void before(const Result&r,const char*a,const char*b){unsigned x=0,y=0;while(x<r.events.size()&&r.events[x]!=a)++x;while(y<r.events.size()&&r.events[y]!=b)++y;assert(x<y&&y<r.events.size());}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int i=1;i<=r.checks;++i){c.stop=i;Result q=run(c);++scenarios;assert(q.checks>=i||frame>=c.limit);}}
int main(){Config c;Result r;
 for(int done=0;done<2;++done)for(int waved=0;waved<2;++waved)for(int dist=9;dist<=11;++dist)for(int physics=0;physics<2;++physics){c=Config();c.complete=done!=0;c.waved=waved!=0;c.distance=(float)dist;c.physics=physics!=0;r=run(c);assert(count(r,"wave")==int(!done&&!waved&&dist<10));assert(count(r,"face")==4*int(!done));if(count(r,"wave")){before(r,"wave","conversation");before(r,"conversation","person");before(r,"person","line");assert(r.distance==1);}sweep(c);}
 for(int flags=0;flags<8;++flags)for(int h=0;h<4;++h){c=Config();c.hit=(flags&1)!=0;c.special=(flags&2)!=0;c.heal=(flags&4)!=0;union{unsigned int bits;float value;}n;n.bits=0x7fc00000;c.health=h==0?-1.0f:h==1?0.0f:h==2?1.0f:n.value;r=run(c);bool attack=c.hit||(c.special&&!c.heal);assert(r.hits==4*int(attack)&&count(r,"friends")==r.hits&&count(r,"clearEnemy")==r.hits);assert(count(r,"speak")==int(attack&&c.health>0)*4);assert(r.any==(c.hit?0:4)&&r.heal==(!c.hit&&c.special?4:0));sweep(c);}
 for(int hero=0;hero<2;++hero)for(int hit=0;hit<2;++hit){c=Config();c.talk=true;c.hit=hit!=0;c.heroAcquire=hero!=0;r=run(c);assert(r.complete&&count(r,"cutscene")==1&&count(r,"complete")==1&&count(r,"deactivate")==1&&count(r,"remove")==1&&r.hero==1);before(r,"cutscene","clearInfo");before(r,"clearInfo","complete");before(r,"complete","deactivate");before(r,"deactivate","remove");if(hit)before(r,"clearEnemy","cutscene");sweep(c);}
 for(int initial=0;initial<3;initial+=2)for(int retry=0;retry<3;retry+=2)for(int wait=0;wait<3;wait+=2){c=Config();c.initialFail=initial;c.retryFail=retry;c.wait=wait;c.limit=15;c.hit=c.talk=true;sweep(c);}
 for(int t=-1;t<=1;++t){c=Config();c.trigger=t;c.distance=0;sweep(c);}
 assert(scenarios>1000);std::puts("00e25f70_TEST PASS");}
