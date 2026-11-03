// Selected source 978fc03997c79eff04c426afc4f7ca67942f634c02d8d346c0efd422b1a29f3f

#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <limits>
struct State {
 int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,pauseCalls,predicates,removed,region,banditAt,fishAt,killAt,gruntsAt,startFailures,startAttempts,heroClaims,banditClaims,released,moveFade,moveExit,fadeAt,exitAt,faded,intros,outros,objectives,renown,face,seedCalls,defCalls,setupCalls;
 bool intro,bandit,killed,grunts,fishComplete,safe,deputyAlive,exitAlive,heroClaimOK,banditClaimOK,killAfterFade,invalidateExit,exitStage;
 float fadeDistance,exitDistance,health;std::vector<int> events;
 State():frame(0),limit(6),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),pauseCalls(0),predicates(0),removed(0),region(2),banditAt(0),fishAt(0),killAt(-1),gruntsAt(-1),startFailures(0),startAttempts(0),heroClaims(0),banditClaims(0),released(0),moveFade(0),moveExit(0),fadeAt(0),exitAt(0),faded(0),intros(0),outros(0),objectives(0),renown(0),face(0),seedCalls(0),defCalls(0),setupCalls(0),intro(true),bandit(true),killed(true),grunts(true),fishComplete(false),safe(false),deputyAlive(true),exitAlive(true),heroClaimOK(true),banditClaimOK(true),killAfterFade(true),invalidateExit(false),exitStage(false),fadeDistance(4.5f),exitDistance(1.5f),health(37.5f){}
 bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck)||(killAfterFade&&faded>0);}
};State*state;
class CCharString{public:std::string text;CCharString():text(){++state->strings;}CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}CCharString&operator=(const CCharString&o){text=o.text;return*this;}CCharString&operator=(const char*s){text=s;return*this;}bool operator<(const CCharString&o)const{return text<o.text;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}const C3DVector&GetPos()const{assert(id==1||id==4);static C3DVector a(1,2,3),b(7,8,9);return id==1?a:b;}CCharString GetDefName()const{assert(id==-1);++state->defCalls;return CCharString("CREATURE_HOSTAGE");}bool IsAlive()const{if(id==3)return state->deputyAlive;assert(id==4);state->exitStage=true;return state->exitAlive;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EScriptEntityMoveType{ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,ENTITY_MOVE_SNEAK=2};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;int who;CScriptGameResourceObjectScriptedThingBase():valid(false),who(0){++state->resources;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&o):valid(o.valid),who(o.who){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid&&!(state->invalidateExit&&state->exitStage);}void Release(){assert(valid);valid=false;++state->released;}void MoveToPosition(const C3DVector&p,float range,EScriptEntityMoveType type,bool a,bool b){assert(valid&&who==-1&&type==ENTITY_MOVE_RUN&&!a);if(p.X==1){assert(p.Y==2&&p.Z==3&&range==4.0f&&!b);++state->moveFade;}else{assert(p.X==7&&p.Y==8&&p.Z==9&&range==1.0f&&b);++state->moveExit;}}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(state->movies==0);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
struct CScriptDef{long BountyHuntRenownHostage1;CScriptDef():BountyHuntRenownHostage1(25){}};CScriptDef scriptDef;const CScriptDef*pGlobalScriptDef=&scriptDef;
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->frame>=state->banditAt)state->bandit=true;if(state->killAt>=0&&state->frame>=state->killAt)state->killed=true;if(state->gruntsAt>=0&&state->frame>=state->gruntsAt)state->grunts=true;}void RemoveThing(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);++state->removed;}bool IsRegionLoaded(const CCharString&s)const{if(s.text=="GreatwoodEntrance")return state->region==1;assert(s.text=="Fisherman");return state->region==2||(state->region==1&&state->frame>=state->fishAt);}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){self(m);assert(b&&a==(state->region==2));++state->setupCalls;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(p==SCRIPT_AI_PRIORITY_HIGHEST);r.who=m.id;if(m.id==-2){++state->heroClaims;return r.valid=state->heroClaimOK;}if(m.id==3){++state->banditClaims;return r.valid=state->banditClaimOK;}self(m);return r.valid=++state->startAttempts>state->startFailures;}CScriptThing GetThingWithScriptName(const CCharString&s)const{if(s.text=="M_BH_FadeOut")return CScriptThing(1);if(s.text=="BH_GangDeputy1")return CScriptThing(3);assert(s.text=="M_BH_FishermanExit");return CScriptThing(4);}void EntityGetAppearanceSeed(const CScriptThing&m,unsigned long&n){self(m);n=123456;++state->seedCalls;}float GetHealth(const CScriptThing&m){self(m);return state->health;}void SetIsThingLightningable(const CScriptThing&m,bool b){self(m);assert(!b);++state->setupCalls;}void SetThingHasInformation(const CScriptThing&m,bool a,bool b,bool c){self(m);assert(!a&&b&&!c);++state->setupCalls;}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&t,bool b){self(m);assert(t.id==3&&!b&&state->deputyAlive&&!state->killed);++state->face;}void EntitySetAsScared(const CScriptThing&m,bool b){self(m);assert(!b);}CScriptThing&GetHero(){return hero;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->pauseCalls;}else{assert(state->pauseDepth==1);--state->pauseDepth;}}void FadeScreenOutUntilNextCallToFadeScreenIn(float a,float b){assert(a==0.5f&&b==0);++state->setupCalls;}CCharString GetActiveQuestName(){return CCharString("BOUNTY_HUNT");}void SetQuestCardObjective(const CCharString&q,const CCharString&t,const CCharString&r,const CCharString&a){assert(q.text=="BOUNTY_HUNT"&&t.text=="TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_03"&&r.text=="GreatwoodLake"&&a.text=="Greatwood"&&state->pauseDepth==0&&state->outros>state->objectives);++state->objectives;state->events.push_back(3);}void GiveHeroRenownPoints(long n){assert(n==25&&state->safe&&state->faded>0&&state->events.back()==4);state->renown+=n;state->events.push_back(5);}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float f){assert(a.id==-1);if(b.id==1){assert(f==5.0f);return state->frame>=state->fadeAt&&state->fadeDistance<f;}assert(b.id==4&&f==2.0f);return state->frame>=state->exitAt&&state->exitDistance<f;}
void FadeOutAndKillEntity(const CScriptThing&m,float f,bool a,bool b){assert(m.id==-1&&f==1.0f&&a&&b&&state->pauseDepth==0);++state->faded;state->events.push_back(4);}
void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*f,std::map<CCharString,CCharString>*a,bool b,bool c){assert(!f&&!a&&!b&&c&&state->pauseDepth==1);assert(m[CCharString("Hero")].who==-2&&m[CCharString("Hero")].valid==state->heroClaimOK&&m[CCharString("Victim")].who==-1&&m[CCharString("Victim")].valid);if(s.text=="BOUNTY_HUNT_FISHERMAN_VICTIM"){assert(m.size()==3&&m[CCharString("Bandit")].who==3&&m[CCharString("Bandit")].valid==state->banditClaimOK&&state->intros==0);++state->intros;state->events.push_back(1);}else{assert(s.text=="CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO"&&m.size()==2&&state->intros==1&&state->fishComplete);++state->outros;state->events.push_back(2);}}
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&m):Thing(m){assert(m.id==-1);}};class CQ_BountyHuntScript{public:struct TeleportData{CCharString defName;unsigned long appearanceSeed;float health;CCharString scriptName,teleportTo,teleportMkr;};bool&BountyHuntIntroComplete;bool&BanditCutsceneDone;bool&Deputy1Killed;bool&AllDeputy1GruntsKilled;bool&Hostage1Safe;bool&FishComplete;std::vector<TeleportData>TeleportInfo;CQ_BountyHuntScript():BountyHuntIntroComplete(state->intro),BanditCutsceneDone(state->bandit),Deputy1Killed(state->killed),AllDeputy1GruntsKilled(state->grunts),Hostage1Safe(state->safe),FishComplete(state->fishComplete){}class CBH_Hostage1{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_BountyHuntScript&ParentClass;CBH_Hostage1(CGameScriptInterfaceBase&g,CQ_BountyHuntScript&p):Game(g),Me(-1),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}
void NScript::CQ_BountyHuntScript::CBH_Hostage1::Main()
{
    TRetVoid _Fail_Val;
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,false);
        return;
    }
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!(ParentClass.BanditCutsceneDone)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (Game.IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetAsKillable(Me,false,true);
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing fadeOutMarker=Game.GetThingWithScriptName("M_BH_FadeOut");
        sehMe.MoveToPosition(fadeOutMarker.GetPos(),4.0f,ENTITY_MOVE_RUN,false,false);
        while (!(IsDistanceBetweenThingsUnder(Me,fadeOutMarker,5.0f))) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TeleportData tData;
        Game.EntityGetAppearanceSeed(Me,tData.appearanceSeed);
        tData.defName=Me.GetDefName();
        tData.health=Game.GetHealth(Me);
        tData.scriptName="BH_Hostage1";
        tData.teleportTo="Fisherman";
        tData.teleportMkr="M_BH_Hostage1Pos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game.IsRegionLoaded("Fisherman"))) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    } else if (Game.IsRegionLoaded("Fisherman")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
        Game.EntitySetAsKillable(Me,true,true);
        Game.SetIsThingLightningable(Me,false);
        CScriptThing Deputy1=Game.GetThingWithScriptName("BH_GangDeputy1");
        Game.SetThingHasInformation(Me,false,true,false);
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csMap;
            CScriptGameResourceObjectScriptedThingBase sehHero;
            CScriptGameResourceObjectScriptedThingBase sehBandit;
            Game.StartScriptingEntity(Deputy1,sehBandit,SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
            csMap["Hero"]=sehHero;
            csMap["Victim"]=sehMe;
            csMap["Bandit"]=sehBandit;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                RunCutsceneMacro("BOUNTY_HUNT_FISHERMAN_VICTIM",csMap,0,0,false,true);
            }
            if (sehHero.IsValid()) sehHero.Release();
            if (sehMe.IsValid()) sehMe.Release();
            if (sehBandit.IsValid()) sehBandit.Release();
        }
        for (; !false; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Deputy1.IsAlive() && !ParentClass.Deputy1Killed) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Deputy1,false);
            }
            if (ParentClass.Deputy1Killed && ParentClass.AllDeputy1GruntsKilled) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsScared(Me,false);
                ParentClass.FishComplete=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase sehHero;
                    Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csMap;
                    csMap["Hero"]=sehHero;
                    csMap["Victim"]=sehMe;
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        RunCutsceneMacro("CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO",csMap,0,0,false,true);
                    }
                    if (sehMe.IsValid()) sehMe.Release();
                    if (sehHero.IsValid()) sehHero.Release();
                }
                Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_03","GreatwoodLake","Greatwood");
                ParentClass.Hostage1Safe=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Exit=Game.GetThingWithScriptName("M_BH_FishermanExit");
                if (Exit.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (sehMe.IsValid()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me,Exit,2.0f)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    FadeOutAndKillEntity(Me,1.0f,true,true);
                    Game.GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage1);
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}


int scenarios=0;
void run(State&s){++scenarios;state=&s;{CGameScriptInterfaceBase game;NScript::CQ_BountyHuntScript parent;{NScript::CQ_BountyHuntScript::CBH_Hostage1 script(game,parent);script.Main();assert(s.resources==0&&s.movies==0&&s.pauseDepth==0);assert(parent.TeleportInfo.size()<=1);if(!parent.TeleportInfo.empty()){const NScript::CQ_BountyHuntScript::TeleportData&t=parent.TeleportInfo[0];assert(s.region==1&&t.defName.text=="CREATURE_HOSTAGE"&&t.appearanceSeed==123456&&t.health==s.health&&t.scriptName.text=="BH_Hostage1"&&t.teleportTo.text=="Fisherman"&&t.teleportMkr.text=="M_BH_Hostage1Pos");assert(s.seedCalls==1&&s.defCalls==1&&s.faded==1);}}}assert(s.things==0&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0);assert(s.pauseCalls==s.intros+s.outros&&s.objectives<=s.outros);if(s.safe)assert(s.objectives>0);if(s.renown)assert(s.safe&&s.fishComplete&&s.renown==s.faded*25);}
int main(){
 {State s;s.intro=false;run(s);assert(s.removed==1&&s.predicates==0&&s.frame==0);}
 {State s;s.intro=false;s.stopCheck=1;run(s);assert(s.removed==0&&s.predicates==0);}
 {State s;s.region=0;run(s);assert(s.predicates==1&&s.frame==1&&s.intros==0&&s.setupCalls==0);}
 {State s;run(s);assert(s.intros==1&&s.outros==1&&s.objectives==1&&s.renown==25&&s.moveExit==1&&s.fishComplete&&s.safe&&s.setupCalls==4&&s.events.size()==5);for(int i=0;i<5;++i)assert(s.events[i]==i+1);}
 for(int flags=0;flags<32;++flags){State s;s.killed=(flags&1)!=0;s.grunts=(flags&2)!=0;s.deputyAlive=(flags&4)!=0;s.heroClaimOK=(flags&8)!=0;s.banditClaimOK=(flags&16)!=0;run(s);assert(s.intros==1&&s.banditClaims==1);assert(s.outros==((flags&3)==3?1:0));assert((s.face>0)==(!s.killed&&s.deputyAlive));assert(s.renown==((flags&3)==3?25:0));}
 for(int band=0;band<5;++band)for(int claim=0;claim<5;++claim){State s;s.limit=16;s.bandit=false;s.banditAt=band;s.startFailures=claim;run(s);assert(s.intros==1&&s.outros==1&&s.renown==25&&s.startAttempts==claim+3);}
 for(int killed=1;killed<6;++killed)for(int grunts=1;grunts<6;++grunts){State s;s.limit=10;s.killed=false;s.grunts=false;s.killAt=killed;s.gruntsAt=grunts;run(s);assert(s.intros==1&&s.outros==1&&s.renown==25);}
 float distances[5]={-1.5f,1.5f,2.0f,2.5f,std::numeric_limits<float>::quiet_NaN()};
 for(int i=0;i<5;++i){State s;s.exitDistance=distances[i];run(s);assert(s.outros==1&&s.objectives==1&&s.moveExit==1);assert(s.renown==(i<2?25:0));}
 {State s;s.exitAlive=false;run(s);assert(s.intros==1&&s.outros==5&&s.objectives==5&&s.renown==0&&s.moveExit==0);}
 {State s;s.invalidateExit=true;run(s);assert(s.moveExit==0&&s.faded==1&&s.renown==25);}
 {State s;s.killAfterFade=false;run(s);assert(s.outros==5&&s.renown==125&&s.faded==5);}
 for(int wait=1;wait<6;++wait){State s;s.exitAt=wait;run(s);assert(s.renown==25&&s.outros==1);}
 float health[4]={-2.0f,0.0f,0.5f,37.5f};float fadeDist[5]={-1.0f,4.5f,5.0f,5.5f,std::numeric_limits<float>::quiet_NaN()};
 for(int h=0;h<4;++h)for(int d=0;d<5;++d){State s;s.region=1;s.health=health[h];s.fadeDistance=fadeDist[d];run(s);assert(s.intros==0&&s.moveFade==1&&s.renown==0);assert(s.seedCalls==(d<2?1:0)&&s.faded==(d<2?1:0));}
 for(int fade=0;fade<5;++fade)for(int fish=0;fish<5;++fish){State s;s.region=1;s.limit=12;s.fadeAt=fade;s.fishAt=fish;s.killAfterFade=false;run(s);assert(s.faded==1&&s.seedCalls==1&&s.frame>=fish&&s.frame>=fade&&s.intros==0);}
 for(int stop=1;stop<=180;++stop)for(int mode=0;mode<16;++mode){State s;s.stopCheck=stop;s.limit=12;switch(mode){case 0:break;case 1:s.region=0;break;case 2:s.region=1;break;case 3:s.region=1;s.fadeDistance=5;break;case 4:s.region=1;s.killAfterFade=false;s.fishAt=20;break;case 5:s.bandit=false;s.banditAt=20;break;case 6:s.startFailures=20;break;case 7:s.killed=false;break;case 8:s.grunts=false;break;case 9:s.exitAlive=false;break;case 10:s.exitDistance=2;break;case 11:s.heroClaimOK=false;s.banditClaimOK=false;break;case 12:s.invalidateExit=true;break;case 13:s.killAfterFade=false;break;case 14:s.killed=false;s.grunts=false;s.killAt=3;s.gruntsAt=5;break;case 15:s.region=1;s.fadeAt=4;s.killAfterFade=false;s.fishAt=8;break;}run(s);}
 assert(scenarios==3024);std::puts("00d19bd0_TEST PASS");return 0;
}
