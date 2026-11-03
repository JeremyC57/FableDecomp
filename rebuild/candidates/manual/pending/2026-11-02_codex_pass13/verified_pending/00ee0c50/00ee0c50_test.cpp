#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
static int strings=0,things=0,predicates=0;
static bool stopped=false;
static std::vector<std::string> events;
static void action(const std::string& s){assert(!stopped);events.push_back(s);}
static std::string number(int n){std::ostringstream s;s<<n;return s.str();}
class CCharString{public:std::string s;CCharString(const char* p):s(p){++strings;}CCharString(const CCharString& p):s(p.s){++strings;}~CCharString(){--strings;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
static bool alive[8];
class CScriptThing{public:int id;C3DVector pos;CScriptThing(int i=0):id(i),pos((float)i,2,3){++things;}CScriptThing(const CScriptThing& t):id(t.id),pos(t.pos){++things;}CScriptThing& operator=(const CScriptThing& t){id=t.id;pos=t.pos;return *this;}~CScriptThing(){--things;}bool IsAlive()const{return alive[id];}const C3DVector& GetPos()const{return pos;}};
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&){++predicates;}~CIsThingAlive(){--predicates;}};}
enum EGameflowPosition{EGP_ORCHARD_FARM=450,EGP_ARENA=900};
struct Data{EGameflowPosition PostSavePosition;};
struct Config{int position,frames,stop;bool started,initialAlive,initialPlay,night[6],screen[6],far[6];Config():position(500),frames(3),stop(0),started(false),initialAlive(false),initialPlay(false){for(int i=0;i<6;++i){night[i]=false;screen[i]=false;far[i]=true;}}};
static Config config;static int frame,checks,nightCalls,screenCalls,distanceCalls;
namespace NScript{class CV_TravellingHeroesScript;}
class GameMock{public:NScript::CV_TravellingHeroesScript* parent;CScriptThing hero;GameMock():parent(0),hero(2){}void NewScriptFrame();void SetThingPersistent(const CScriptThing& t,bool b){assert(t.id==1&&b);action("persistent");}void RemoveRumourCategory(const CCharString& s){assert(s.s=="TravHero Thunder");action("rumour");}void RemoveThing(const CScriptThing& t,bool a,bool b){assert(!a&&b);action("remove"+number(t.id));alive[t.id]=false;}CScriptThing GetThingWithScriptName(const CCharString& s){int id=s.s=="TH_Thunder"?3:s.s=="TH_ThunderBoy"?4:s.s=="TH_ThunderGirl"?5:s.s=="TH_ThunderBoyMarker2"?6:s.s=="TH_ThunderGirlMarker2"?7:0;assert(id);return CScriptThing(id);}CScriptThing CreateCreature(const CCharString& def,const C3DVector& pos,const CCharString& name,bool b){assert(!b);int id=name.s=="TH_Thunder"?3:name.s=="TH_ThunderBoy"?4:name.s=="TH_ThunderGirl"?5:0;assert(id);assert(def.s==(id==3?"CREATURE_RIVAL_HERO_THUNDER":id==4?"CREATURE_BS_VILLAGER_BOY":"CREATURE_BS_VILLAGER_GIRL"));assert(pos.X==(id==3?1:id==4?6:7));action("create"+number(id));alive[id]=true;return CScriptThing(id);}bool IsCameraPosOnScreen(const C3DVector& p){assert(p.X==1||p.X==3);++screenCalls;return config.screen[frame];}CScriptThing& GetHero(){return hero;}};
static bool IsDistanceBetweenThingsOver(const CScriptThing& a,const CScriptThing& b,float d){assert((a.id==1||a.id==3)&&b.id==2&&d==25);++distanceCalls;return config.far[frame];}
namespace NScript{class CV_TravellingHeroesScript{public:bool ThunderSceneStarted;void GossipSetup(CCharString a,CCharString b,CCharString c,CCharString d){assert(a.s=="TravHero Thunder"&&b.s=="TEXT_AI_GOSSIP_THUNDER_BOWERSTONE"&&c.s=="VILLAGE_BOWERSTONE_SLUMS"&&d.s=="FACTION_VILLAGERS");action("gossip");}class CTH_Trigger_Bowerstone{GameMock& Game;CScriptThing Me;CV_TravellingHeroesScript& ParentClass;Data* PMasterData;bool ThunderInPlay;void Main();public:CTH_Trigger_Bowerstone(GameMock& g,CV_TravellingHeroesScript& p,Data& d):Game(g),Me(1),ParentClass(p),PMasterData(&d),ThunderInPlay(config.initialPlay){}void Run(){Main();}bool InPlay()const{return ThunderInPlay;}void SetPredicate(const CIsThingAlive&){action("predicate");}bool IsActiveThreadTerminating()const{++checks;if(config.stop&&checks>=config.stop){stopped=true;return true;}return false;}bool IsNightTime(){++nightCalls;bool n=config.night[frame];if(IsActiveThreadTerminating())return false;return n;}};};}
void GameMock::NewScriptFrame(){events.push_back("frame");++frame;assert(frame<=config.frames);if(frame==config.frames)parent->ThunderSceneStarted=true;}
void NScript::CV_TravellingHeroesScript::CTH_Trigger_Bowerstone::Main()

{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    Game.SetThingPersistent(Me,true);
    EGameflowPosition GameflowPosition = PMasterData->PostSavePosition;
    if(GameflowPosition > EGP_ARENA || ParentClass.ThunderSceneStarted) {
        if(IsActiveThreadTerminating()) return;
        Game.RemoveRumourCategory("TravHero Thunder");
        Game.RemoveThing(Me,false,true);
    } else if(GameflowPosition > EGP_ORCHARD_FARM) {
        if(IsActiveThreadTerminating()) return;
        ParentClass.GossipSetup("TravHero Thunder","TEXT_AI_GOSSIP_THUNDER_BOWERSTONE","VILLAGE_BOWERSTONE_SLUMS","FACTION_VILLAGERS");
        C3DVector MyPos = Me.GetPos();
        CScriptThing Thunder = Game.GetThingWithScriptName("TH_Thunder");
        CScriptThing Boy = Game.GetThingWithScriptName("TH_ThunderBoy");
        CScriptThing Girl = Game.GetThingWithScriptName("TH_ThunderGirl");
        if(!Thunder.IsAlive() && !IsNightTime()) {
            if(IsActiveThreadTerminating()) return;
            ThunderInPlay = true;
            Thunder = Game.CreateCreature("CREATURE_RIVAL_HERO_THUNDER",MyPos,"TH_Thunder",false);
            Boy = Game.CreateCreature("CREATURE_BS_VILLAGER_BOY",Game.GetThingWithScriptName("TH_ThunderBoyMarker2").GetPos(),"TH_ThunderBoy",false);
            Girl = Game.CreateCreature("CREATURE_BS_VILLAGER_GIRL",Game.GetThingWithScriptName("TH_ThunderGirlMarker2").GetPos(),"TH_ThunderGirl",false);
        } else if(Thunder.IsAlive()) {
            if(IsActiveThreadTerminating()) return;
            ThunderInPlay = true;
        }
        while(!ParentClass.ThunderSceneStarted) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
            if(IsNightTime()) {
                if(IsActiveThreadTerminating()) return;
                if(ThunderInPlay && !Game.IsCameraPosOnScreen(Thunder.GetPos()) && IsDistanceBetweenThingsOver(Thunder,Game.GetHero(),25.0f)) {
                    if(IsActiveThreadTerminating()) return;
                    Game.RemoveThing(Boy,false,true);
                    Game.RemoveThing(Girl,false,true);
                    Game.RemoveThing(Thunder,false,true);
                    ThunderInPlay = false;
                }
            } else {
                if(IsActiveThreadTerminating()) return;
                if(!ThunderInPlay && !IsNightTime() && !Game.IsCameraPosOnScreen(MyPos) && IsDistanceBetweenThingsOver(Me,Game.GetHero(),25.0f)) {
                    if(IsActiveThreadTerminating()) return;
                    Thunder = Game.CreateCreature("CREATURE_RIVAL_HERO_THUNDER",MyPos,"TH_Thunder",false);
                    Boy = Game.CreateCreature("CREATURE_BS_VILLAGER_BOY",Game.GetThingWithScriptName("TH_ThunderBoyMarker2").GetPos(),"TH_ThunderBoy",false);
                    Girl = Game.CreateCreature("CREATURE_BS_VILLAGER_GIRL",Game.GetThingWithScriptName("TH_ThunderGirlMarker2").GetPos(),"TH_ThunderGirl",false);
                    ThunderInPlay = true;
                }
            }
        }
        if(IsActiveThreadTerminating()) return;
        Game.RemoveRumourCategory("TravHero Thunder");
        Game.RemoveThing(Me,false,true);
    }
}

struct Result{std::vector<std::string> log;int checks,nights,screens,distances;bool play,stopped;};
static Result run(const Config& c){config=c;frame=-1;checks=nightCalls=screenCalls=distanceCalls=0;stopped=false;events.clear();for(int i=0;i<8;++i)alive[i]=false;alive[3]=config.initialAlive;alive[4]=alive[5]=config.initialAlive;Result r;{GameMock g;NScript::CV_TravellingHeroesScript p;p.ThunderSceneStarted=c.started;g.parent=&p;Data d;d.PostSavePosition=(EGameflowPosition)c.position;NScript::CV_TravellingHeroesScript::CTH_Trigger_Bowerstone t(g,p,d);t.Run();r.play=t.InPlay();}assert(strings==0&&things==0&&predicates==0);r.log=events;r.checks=checks;r.nights=nightCalls;r.screens=screenCalls;r.distances=distanceCalls;r.stopped=stopped;return r;}
static int count(const Result&r,const char*s){int n=0;for(unsigned i=0;i<r.log.size();++i)if(r.log[i]==s)++n;return n;}
static void expected(const Result&r,const char* words){std::istringstream in(words);std::string s;std::vector<std::string> v;while(in>>s)v.push_back(s);assert(v==r.log);}
static int scenarios=0;
static void sweep(Config c){Result r=run(c);++scenarios;for(int n=1;n<=r.checks;++n){c.stop=n;Result t=run(c);++scenarios;assert(t.stopped);assert(t.checks>=n);assert(t.log.size()<=r.log.size()+1);assert(count(t,"rumour")<=1);}}
int main(){
 Config c;int positions[]={-1,0,450,451,899,900,901,2000};
 for(unsigned p=0;p<sizeof(positions)/sizeof(positions[0]);++p)for(int started=0;started<2;++started){c=Config();c.position=positions[p];c.started=started!=0;Result r=run(c);if(c.position>900||c.started)expected(r,"predicate frame persistent rumour remove1");else if(c.position<=450)expected(r,"predicate frame persistent");else{assert(count(r,"gossip")==1&&count(r,"create3")==1&&count(r,"rumour")==1&&count(r,"remove1")==1);}sweep(c);}
 c=Config();c.night[1]=true;Result r=run(c);expected(r,"predicate frame persistent gossip create3 create4 create5 frame remove4 remove5 remove3 frame create3 create4 create5 frame rumour remove1");assert(r.play);sweep(c);
 c=Config();c.initialAlive=true;c.night[0]=true;c.night[1]=true;c.night[2]=true;c.night[3]=true;r=run(c);expected(r,"predicate frame persistent gossip frame remove4 remove5 remove3 frame frame rumour remove1");assert(!r.play);sweep(c);
 c=Config();c.night[0]=c.night[1]=c.night[2]=c.night[3]=true;r=run(c);expected(r,"predicate frame persistent gossip frame frame frame rumour remove1");assert(!r.play&&r.screens==0&&r.distances==0);sweep(c);
 for(int initial=0;initial<2;++initial)for(int play=0;play<2;++play)for(int nightMask=0;nightMask<16;++nightMask)for(int screen=0;screen<2;++screen)for(int far=0;far<2;++far){c=Config();c.initialAlive=initial!=0;c.initialPlay=play!=0;for(int i=0;i<4;++i){c.night[i]=(nightMask&(1<<i))!=0;c.screen[i]=screen!=0;c.far[i]=far!=0;}r=run(c);assert(count(r,"create3")==count(r,"create4")&&count(r,"create4")==count(r,"create5"));assert(count(r,"remove3")==count(r,"remove4")&&count(r,"remove4")==count(r,"remove5"));assert(count(r,"rumour")==1&&count(r,"remove1")==1);if(screen)assert(r.distances==0);if(screen||!far){assert(count(r,"remove3")==0);assert(count(r,"create3")<=1);}sweep(c);}
 std::printf("00ee0c50_TEST PASS\n");
}
