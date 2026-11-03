#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
static int strings=0,things=0,handles=0,movies=0,pauses=0,owned=0,frame,checks,tasks,created,distanceCalls,starts;
static bool stopped=false;
static std::vector<std::string> events,cutscenes;
static std::vector<int> spawnPoints,spawnFrames;
static void action(const char*s){assert(!stopped);events.push_back(s);}
class CCharString{public:std::string text;CCharString():text(){++strings;}CCharString(const char*s):text(s){++strings;}CCharString(const CCharString&s):text(s.text){++strings;}~CCharString(){--strings;}bool operator<(const CCharString&s)const{return text<s.text;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0):X(x),Y(0),Z(0){}};
struct Config{bool acquire,physics,nullMarker;int wait,stop,limit,nearMask;float initial;std::vector<float> health;Config():acquire(true),physics(true),nullMarker(false),wait(0),stop(0),limit(20),nearMask(0),initial(0){float a[]={188,187,126,125,64,63,0};for(int i=0;i<7;++i)health.push_back(a[i]);}};
static Config config;
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};
class CScriptThing{public:int id;C3DVector pos;CScriptThing(int i=0):id(i),pos((float)i){++things;}CScriptThing(const CScriptThing&t):id(t.id),pos(t.pos){++things;}CScriptThing&operator=(const CScriptThing&t){id=t.id;pos=t.pos;return *this;}~CScriptThing(){--things;}const C3DVector&GetPos()const{assert(id==0||id==3||id==4);return pos;}};
class CScriptGameResourceObjectScriptedThingBase{int entity;void drop(){if(entity)--owned;entity=0;}public:CScriptGameResourceObjectScriptedThingBase():entity(0){++handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&s):entity(s.entity){++handles;if(entity)++owned;}CScriptGameResourceObjectScriptedThingBase&operator=(const CScriptGameResourceObjectScriptedThingBase&s){if(this!=&s){drop();entity=s.entity;if(entity)++owned;}return *this;}~CScriptGameResourceObjectScriptedThingBase(){drop();--handles;}void acquire(int n){drop();entity=n;++owned;}bool IsValid()const{return entity!=0;}void Release(){action("release");drop();}void PlayAnimation(const CCharString&s,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(s.text=="HOWL"&&!a&&!b&&!c&&d&&e==config.physics&&!f&&!g);if(entity){assert(entity==1);action("animation");tasks=config.wait;}}bool IsPerformingScriptTask()const{return tasks>0;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++movies;}~CScriptGameResourceObjectMovieBase(){assert(pauses==0);--movies;}};
class GameMock{CScriptThing hero;float initialHealth;public:GameMock():hero(2),initialHealth(config.initial){}void NewScriptFrame(){action("frame");++frame;if(tasks)--tasks;}void ModifyThingHealth(const CScriptThing&t,float n,bool b){assert(t.id==1&&n==250.0f&&!b);action("health");initialHealth+=n;}float GetHealth(const CScriptThing&t){assert(t.id==1);if(frame<0)return initialHealth;unsigned n=(unsigned)frame;if(n>=config.health.size())n=config.health.size()-1;return config.health[n];}CScriptThing&GetHero(){return hero;}CScriptThing GetThingWithScriptName(const CCharString&s){assert(s.text=="MK_WBW_FIRSTSPAWN"||s.text=="MK_WBW_FIRSTSPAWNB");action(s.text=="MK_WBW_FIRSTSPAWN"?"primary":"backup");return CScriptThing(s.text=="MK_WBW_FIRSTSPAWNB"?4:config.nullMarker?0:3);}CScriptThing CreateCreature(const CCharString&s,const C3DVector&p,const CCharString&name,bool b){assert(s.text=="CREATURE_BALVERINE_01"&&name.text=="WBWW_SoldierBalverine"&&!b);action("create");spawnPoints.push_back((int)p.X);spawnFrames.push_back(frame);return CScriptThing(10+created++);}void GiveThingBestEnemyTarget(const CScriptThing&a,const CScriptThing&b){assert(a.id>=10&&b.id==2);action("enemy");}void Pause(float n){assert(n==0.2f||n==0.1f);action(n==0.2f?"pause2":"pause1");}void EntitySetAsDamageable(const CScriptThing&t,bool b){assert(t.id==1);action(b?"damageOn":"damageOff");}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&s,EScriptAIPriority p){assert(p==4&&(t.id==1||t.id>=10));action(t.id==1?"startWhite":"startBalv");++starts;if(config.acquire)s.acquire(t.id);return config.acquire;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&movies==1);action("movie");}void PauseAllNonScriptedEntities(bool b){if(b){action("pauseEntities");++pauses;}else{assert(pauses==1&&movies==1);--pauses;events.push_back("unpause");}}void EntitySetAttackThingImmediately(const CScriptThing&a,const CScriptThing&b,bool c,bool d){assert(a.id==1&&b.id==2&&c&&d);action("attack");}unsigned long PlaySoundOnThing(const CScriptThing&t,const CCharString&s){assert(t.id==1&&s.text=="SND_LONGWOLFHOWL_01");action("sound");return 77;}void CameraDefault(){action("camera");}CScriptThing GetRandomThingWithScriptName(const CCharString&s){assert(s.text=="WBWW_SoldierBalverine"&&created>0);action("random");return CScriptThing(9+created);}};
class CWideScreenMagicPauseEntities{GameMock&g;public:CWideScreenMagicPauseEntities(GameMock&x):g(x){g.PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g.PauseAllNonScriptedEntities(false);}};
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert((a.id==0||a.id==3)&&b.id==2&&d==10.0f);bool near=(config.nearMask&(1<<distanceCalls))!=0;++distanceCalls;return near;}
static void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*a,std::map<CCharString,CCharString>*b,bool c,bool d){assert((s.text=="CS_WBW_FIRSTSPAWN"||s.text=="CS_WBW_FIRSTSPAWNB")&&!a&&!b&&!c&&d&&m.size()==1);assert(m["BALV"].IsValid()==config.acquire);action("cutscene");cutscenes.push_back(s.text);}
namespace NScript{class CQ_WhiteBalverineWWScript{GameMock&Game;CScriptThing WhiteBalverine;public:CQ_WhiteBalverineWWScript(GameMock&g):Game(g),WhiteBalverine(1){}void SpawnBalverines();bool IsActiveThreadTerminating(){++checks;if((config.stop&&checks>=config.stop)||frame>=config.limit){stopped=true;return true;}return false;}};}
void NScript::CQ_WhiteBalverineWWScript::SpawnBalverines()

{
    bool UsedBackupSpawnPoint=false;
    bool SummonCutsceneShown=false;
    Game.ModifyThingHealth(WhiteBalverine,250.0f,false);
    long HealthLossToTriggerSummon=(long)(Game.GetHealth(WhiteBalverine)*0.25f);
    long NextHealthBarrier=(long)(Game.GetHealth(WhiteBalverine)-HealthLossToTriggerSummon);
    long RoundNumber=1;
    while(RoundNumber<=3) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
        if(Game.GetHealth(WhiteBalverine)<NextHealthBarrier) {
            if(IsActiveThreadTerminating()) return;
            CScriptThing Marker=Game.GetThingWithScriptName("MK_WBW_FIRSTSPAWN");
            if(IsDistanceBetweenThingsUnder(Marker,Game.GetHero(),10.0f)) {
                if(IsActiveThreadTerminating()) return;
                Marker=Game.GetThingWithScriptName("MK_WBW_FIRSTSPAWNB");
                UsedBackupSpawnPoint=true;
            }
            for(long i=0;i<RoundNumber;++i) {
                if(IsActiveThreadTerminating()) return;
                CScriptThing NewBalv=Game.CreateCreature("CREATURE_BALVERINE_01",Marker.GetPos(),"WBWW_SoldierBalverine",false);
                Game.GiveThingBestEnemyTarget(NewBalv,Game.GetHero());
                Game.Pause(0.2f);
            }
            if(IsActiveThreadTerminating()) return;
            Game.EntitySetAsDamageable(WhiteBalverine,false);
            {
                CScriptGameResourceObjectScriptedThingBase seh_WhiteBalverine;
                Game.StartScriptingEntity(WhiteBalverine,seh_WhiteBalverine,SCRIPT_AI_PRIORITY_HIGHEST);
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.Pause(0.2f);
                    Game.EntitySetAttackThingImmediately(WhiteBalverine,Game.GetHero(),true,true);
                    Game.Pause(0.2f);
                    Game.PlaySoundOnThing(WhiteBalverine,"SND_LONGWOLFHOWL_01");
                    seh_WhiteBalverine.PlayAnimation("HOWL",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    while(seh_WhiteBalverine.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                    Game.CameraDefault();
                }
                if(seh_WhiteBalverine.IsValid()) seh_WhiteBalverine.Release();
            }
            Game.EntitySetAsDamageable(WhiteBalverine,true);
            if(!SummonCutsceneShown) {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase seh_balv;
                CScriptThing CutsceneBalverine=Game.GetRandomThingWithScriptName("WBWW_SoldierBalverine");
                Game.StartScriptingEntity(CutsceneBalverine,seh_balv,SCRIPT_AI_PRIORITY_HIGHEST);
                if(UsedBackupSpawnPoint) {
                    if(IsActiveThreadTerminating()) return;
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["BALV"]=seh_balv;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_WBW_FIRSTSPAWNB",mymap,0,0,false,true);
                    Game.Pause(0.1f);
                    Game.CameraDefault();
                } else {
                    if(IsActiveThreadTerminating()) return;
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["BALV"]=seh_balv;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_WBW_FIRSTSPAWN",mymap,0,0,false,true);
                    Game.Pause(0.1f);
                    Game.CameraDefault();
                }
                SummonCutsceneShown=true;
            }
            NextHealthBarrier-=HealthLossToTriggerSummon;
            ++RoundNumber;
        }
    }
    if(IsActiveThreadTerminating()) return;
}

struct Result{std::vector<std::string> events,cutscenes;std::vector<int> points,frames;int created,checks,starts,distances;};
static Result run(const Config&c){config=c;NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations=c.physics;frame=-1;checks=tasks=created=distanceCalls=starts=0;events.clear();cutscenes.clear();spawnPoints.clear();spawnFrames.clear();stopped=false;Result r;{GameMock g;NScript::CQ_WhiteBalverineWWScript q(g);q.SpawnBalverines();}assert(strings==0&&things==0&&handles==0&&movies==0&&pauses==0&&owned==0);r.events=events;r.cutscenes=cutscenes;r.points=spawnPoints;r.frames=spawnFrames;r.created=created;r.checks=checks;r.starts=starts;r.distances=distanceCalls;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.events.size();++i)if(r.events[i]==s)++n;return n;}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int i=1;i<=r.checks;++i){c.stop=i;Result q=run(c);++scenarios;assert(q.checks>=i||frame>=c.limit);assert(q.cutscenes.size()<=1&&q.created<=6);}}
int main(){Config c;Result r;
 r=run(c);assert(r.created==6&&r.distances==3&&r.starts==4&&r.cutscenes.size()==1&&r.cutscenes[0]=="CS_WBW_FIRSTSPAWN");int expect[]={1,3,3,5,5,5};for(int i=0;i<6;++i)assert(r.frames[i]==expect[i]);assert(count(r,"damageOff")==3&&count(r,"damageOn")==3&&count(r,"enemy")==6&&count(r,"camera")==4&&count(r,"animation")==3&&count(r,"release")==3);sweep(c);
 for(int mask=0;mask<8;++mask)for(int physics=0;physics<2;++physics)for(int nullMarker=0;nullMarker<2;++nullMarker){c=Config();c.nearMask=mask;c.physics=physics!=0;c.nullMarker=nullMarker!=0;r=run(c);assert(r.created==6&&r.cutscenes.size()==1&&r.cutscenes[0]==(mask&1?"CS_WBW_FIRSTSPAWNB":"CS_WBW_FIRSTSPAWN"));int i=0;for(int round=0;round<3;++round)for(int n=0;n<=round;++n)assert(r.points[i++]==((mask&(1<<round))?4:nullMarker?0:3));sweep(c);}
 for(int wait=0;wait<3;++wait)for(int acquire=0;acquire<2;++acquire){c=Config();c.wait=wait;c.acquire=acquire!=0;r=run(c);assert(r.created==6&&r.starts==4&&r.cutscenes.size()==1&&count(r,"animation")==3*acquire&&count(r,"release")==3*acquire);sweep(c);}
 c=Config();c.health.assign(1,188.0f);c.limit=5;r=run(c);assert(r.created==0&&r.cutscenes.empty());sweep(c);
 c=Config();c.health.assign(1,0.0f);r=run(c);assert(r.created==6&&r.frames[0]==0&&r.frames[1]==1&&r.frames[3]==2);sweep(c);
 c=Config();c.initial=0.5f;c.health.assign(1,187.9f);c.limit=5;r=run(c);assert(r.created==1);sweep(c);
 c=Config();c.initial=-249.0f;c.health.assign(1,0.0f);r=run(c);assert(r.created==6);sweep(c);
 c=Config();union{unsigned int bits;float value;}nan;nan.bits=0x7fc00000;c.health.assign(1,nan.value);c.limit=4;r=run(c);assert(r.created==0);sweep(c);
 assert(scenarios>1000);std::puts("00e19560_TEST PASS");}
