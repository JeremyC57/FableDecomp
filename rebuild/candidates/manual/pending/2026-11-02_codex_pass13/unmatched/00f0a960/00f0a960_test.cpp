#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
static int strings=0,things=0,handles=0,movies=0,pauses=0,predicates=0,owners[3]={0,0,0};
static int frame,checks,failures,tasks,startsMe,startsHero,talkCalls,unloadCalls;
static bool stopped=false;
static std::vector<std::string> events;
static void action(const char*s){assert(!stopped);events.push_back(s);}
class CCharString{public:std::string text;CCharString():text(){++strings;}CCharString(const char*s):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}~CCharString(){--strings;}bool operator==(const char*s)const{return text==s;}bool operator<(const CCharString&s)const{return text<s.text;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
struct Config{bool attack,complete,talk,unload,heroAcquire;int region,fail,wait,stop,limit;float health;Config():attack(false),complete(false),talk(false),unload(false),heroAcquire(true),region(0),fail(0),wait(0),stop(0),limit(8),health(100){}};
static Config config;
class CScriptThing{public:int id;C3DVector pos;CScriptThing(int i=0):id(i),pos((float)i,2,3){++things;}CScriptThing(const CScriptThing&t):id(t.id),pos(t.pos){++things;}~CScriptThing(){--things;}const C3DVector&GetPos()const{return pos;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(id==1&&s.text=="SCRIPT_NAME_HERO");++talkCalls;return config.talk;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EScriptingStateGroups{ESSG_WANDER_NEAR=4};enum ETextGroupSelectionMethod{GROUP_SELECT_RANDOM_NO_REPEAT=2};
class CScriptGameResourceObjectScriptedThingBase{int entity;void drop(){if(entity)--owners[entity];entity=0;}public:CScriptGameResourceObjectScriptedThingBase():entity(0){++handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&s):entity(s.entity){++handles;if(entity)++owners[entity];}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&s){if(this!=&s){drop();entity=s.entity;if(entity)++owners[entity];}return *this;}~CScriptGameResourceObjectScriptedThingBase(){drop();--handles;}void acquire(int i){drop();entity=i;++owners[i];}bool IsValid()const{return entity!=0;}void Release(){action("release");drop();}CScriptThing GetScriptThing()const{return CScriptThing(entity);}void Speak(const CScriptThing&t,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(entity==1&&t.id==2&&std::string(s)=="TEXT_QST_003_CONTACT_ATTACK"&&m==2&&!a&&b&&!c);action("speak");tasks=config.wait;}bool IsPerformingScriptTask()const{return tasks>0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;}};
class GameMock{CScriptThing hero;public:GameMock():hero(2){}void NewScriptFrame(){action("frame");++frame;if(tasks)--tasks;}void SetWanderCentrePoint(CScriptThing t,C3DVector p){assert(t.id==1&&p.X==1);action("centre");}void SetWanderMinDistance(CScriptThing t,float x){assert(t.id==1&&x==0);action("min");}void SetWanderMaxDistance(CScriptThing t,float x){assert(t.id==1&&x==12.5f);action("max");}void SetScriptingStateGroup(CScriptThing t,EScriptingStateGroups x){assert(t.id==1&&x==4);action("wander");}void EntityGetAppearanceSeed(const CScriptThing&t,unsigned long&x){assert(t.id==1);action("seed");x=2026;}void SetThingHasInformation(const CScriptThing&t,bool a,bool b,bool c){assert(t.id==1&&a&&!b&&!c);action("information");}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert(p==4&&(t.id==1||t.id==2));action(t.id==1?"startMe":"startHero");if(t.id==2){++startsHero;if(config.heroAcquire)s.acquire(2);return config.heroAcquire;}++startsMe;if(failures){--failures;return false;}s.acquire(1);return true;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){action("pause");++pauses;}else{assert(pauses==1&&movies==1);--pauses;events.push_back("unpause");}}float GetHealth(const CScriptThing&t){assert(t.id==1);return config.health;}CScriptThing&GetHero(){return hero;}bool MsgOnRegionUnloaded(CCharString&s){++unloadCalls;if(!config.unload)return false;s=CCharString(config.region==0?"BowerstonePosh":config.region==1?"OtherRegion":"");return true;}void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&b);action("remove");}void FixMovieSequenceCamera(bool b){action(b?"fixCamera":"freeCamera");}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="BowerstoneJailHSP");return CScriptThing(3);}void EntityTeleportToThing(const CScriptThing&t,const CScriptThing&to,bool b){assert(t.id==2&&to.id==3&&!b);action("teleport");}};
class CWideScreenMagicPauseEntities{GameMock&g;public:CWideScreenMagicPauseEntities(GameMock&x):g(x){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g.PauseAllNonScriptedEntities(false);}};
struct ScriptDef{float AmbushTradersContactWanderDistance;};static ScriptDef defs={12.5f};static ScriptDef*pGlobalScriptDef=&defs;
static void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*a,std::map<CCharString,CCharString>*b,bool c,bool d){assert(s.text=="CS_GIBBETHILL_AMBUSH_PREINTRO"&&!a&&!b&&!c&&d&&m.size()==2&&m["WOMAN"].IsValid());assert(m["HERO"].IsValid()==config.heroAcquire);action("cutscene");}
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.id==1);++predicates;}~CIsThingAlive(){--predicates;}};class CQ_AmbushTradersScript{public:bool ContactAttackedByHero,BanditContactIntroComplete;unsigned long ContactSeed;class CAT_BanditContact{GameMock&Game;CScriptThing Me;CQ_AmbushTradersScript&ParentClass;void Main();public:CAT_BanditContact(GameMock&g,CQ_AmbushTradersScript&p):Game(g),Me(1),ParentClass(p){}void Run(){Main();}void SetPredicate(const CIsThingAlive&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit){stopped=true;return true;}return false;}};};}
void NScript::CQ_AmbushTradersScript::CAT_BanditContact::Main()

{
    CCharString regionName;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CScriptThing Exit;
    Game.SetWanderCentrePoint(Me,Me.GetPos());
    Game.SetWanderMinDistance(Me,0.0f);
    Game.SetWanderMaxDistance(Me,pGlobalScriptDef->AmbushTradersContactWanderDistance);
    Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
    Game.EntityGetAppearanceSeed(Me,ParentClass.ContactSeed);
    Game.SetThingHasInformation(Me,true,false,false);
    if(IsActiveThreadTerminating()) return;
    do {
        if(ParentClass.ContactAttackedByHero) {
            if(IsActiveThreadTerminating()) return;
            if(sehMe.IsValid()) sehMe.Release();
            while(!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                sehMe.Speak(Game.GetHero(),"TEXT_QST_003_CONTACT_ATTACK",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while(sehMe.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return;
                }
                if(IsActiveThreadTerminating()) return;
            }
            if(sehMe.IsValid()) sehMe.Release();
            Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
            ParentClass.ContactAttackedByHero=false;
        } else if(ParentClass.BanditContactIntroComplete && Game.MsgOnRegionUnloaded(regionName)) {
            if(IsActiveThreadTerminating()) return;
            if(regionName=="BowerstonePosh") {
                if(IsActiveThreadTerminating()) return;
                Game.RemoveThing(Me,false,true);
            }
        } else if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            if(!ParentClass.BanditContactIntroComplete) {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase sehHero;
                Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
                if(sehMe.IsValid()) sehMe.Release();
                while(!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return;
                }
                if(IsActiveThreadTerminating()) return;
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csMap;
                csMap["HERO"]=sehHero;
                csMap["WOMAN"]=sehMe;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_GIBBETHILL_AMBUSH_PREINTRO",csMap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.BanditContactIntroComplete=true;
                CScriptThing HSP=Game.GetThingWithScriptName("BowerstoneJailHSP");
                Game.EntityTeleportToThing(Game.GetHero(),HSP,false);
                Game.RemoveThing(Me,false,true);
            }
        }
        Game.NewScriptFrame();
    } while(!IsActiveThreadTerminating());
}

struct Result{std::vector<std::string> events;bool attack,complete;unsigned long seed;int checks,me,hero,talk,unload;};
static Result run(const Config&c){config=c;frame=-1;checks=tasks=startsMe=startsHero=talkCalls=unloadCalls=0;failures=c.fail;events.clear();stopped=false;Result r;{GameMock g;NScript::CQ_AmbushTradersScript p;p.ContactAttackedByHero=c.attack;p.BanditContactIntroComplete=c.complete;p.ContactSeed=19;NScript::CQ_AmbushTradersScript::CAT_BanditContact t(g,p);t.Run();r.attack=p.ContactAttackedByHero;r.complete=p.BanditContactIntroComplete;r.seed=p.ContactSeed;}assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&predicates==0&&owners[1]==0&&owners[2]==0);r.events=events;r.checks=checks;r.me=startsMe;r.hero=startsHero;r.talk=talkCalls;r.unload=unloadCalls;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static void before(const Result&r,const char*a,const char*b){unsigned x=0,y=0;while(x<r.events.size()&&r.events[x]!=a)++x;while(y<r.events.size()&&r.events[y]!=b)++y;assert(x<y&&y<r.events.size());}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int i=1;i<=r.checks;++i){c.stop=i;Result q=run(c);++scenarios;assert(q.checks>=i||frame>=c.limit);assert(count(q,"cutscene")<=1);}}
int main(){Config c;Result r;
 c=Config();r=run(c);assert(r.seed==2026&&count(r,"speak")==0&&count(r,"cutscene")==0&&count(r,"remove")==0&&r.me==0&&r.hero==0);sweep(c);
 for(int fail=0;fail<=2;fail+=2)for(int wait=0;wait<=2;wait+=2)for(int health=0;health<3;++health){c=Config();c.attack=true;c.fail=fail;c.wait=wait;c.health=health==0?-1.0f:health==1?0.0f:1.0f;r=run(c);assert(!r.attack&&!r.complete&&r.me==fail+1&&r.hero==0);assert(count(r,"speak")==int(c.health>0));assert(count(r,"movie")==1&&count(r,"pause")==1&&count(r,"unpause")==1&&count(r,"wander")==2&&count(r,"release")==1);before(r,"pause","release");before(r,"release","unpause");sweep(c);}
 for(int fail=0;fail<=2;fail+=2)for(int hero=0;hero<2;++hero){c=Config();c.talk=true;c.fail=fail;c.heroAcquire=hero!=0;r=run(c);assert(r.complete&&r.hero==1&&r.me==fail+1);assert(count(r,"cutscene")==1&&count(r,"teleport")==1&&count(r,"remove")==1);before(r,"fixCamera","cutscene");before(r,"cutscene","freeCamera");before(r,"freeCamera","teleport");before(r,"teleport","remove");before(r,"remove","unpause");sweep(c);}
 for(int reg=0;reg<3;++reg)for(int talk=0;talk<2;++talk){c=Config();c.complete=true;c.unload=true;c.region=reg;c.talk=talk!=0;r=run(c);assert(count(r,"remove")==int(reg==0)*c.limit&&count(r,"cutscene")==0&&r.talk==0);sweep(c);}
 c=Config();c.complete=true;c.talk=true;r=run(c);assert(r.talk>0&&count(r,"cutscene")==0&&r.me==0);sweep(c);
 for(int flags=0;flags<16;++flags){c=Config();c.attack=(flags&1)!=0;c.complete=(flags&2)!=0;c.talk=(flags&4)!=0;c.unload=(flags&8)!=0;c.region=1;sweep(c);}
 assert(scenarios>300);std::puts("00f0a960_TEST PASS");}
