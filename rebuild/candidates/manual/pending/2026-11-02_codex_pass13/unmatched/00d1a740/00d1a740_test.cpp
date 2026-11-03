// Selected source 629bcecc0b1b39083f662b589f49ebe00c22408b33d3d21ae28edfb12b4f54ab
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,paused,predicates,removed,region,banditAt,lakeAt,fishAt,readyAt,startAttempts,startFailures,released,moveFade,moveExit,fadeAt,exitAt,faded,cutscenes,gold,renown,given,objectRemoved,face,seedCalls,defCalls,setupCalls,heroClaims,conversionCalls;bool intro,bandit,fish,killed,escaped,grunts,safe,lakeComplete,leaderAlive,exitAlive,heroHas,meHas,heroClaimOK,killAfterFade,invalidateExit,exitStage;float fadeDistance,exitDistance,health;State():frame(0),limit(6),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),paused(0),predicates(0),removed(0),region(2),banditAt(0),lakeAt(0),fishAt(0),readyAt(0),startAttempts(0),startFailures(0),released(0),moveFade(0),moveExit(0),fadeAt(0),exitAt(0),faded(0),cutscenes(0),gold(0),renown(0),given(0),objectRemoved(0),face(0),seedCalls(0),defCalls(0),setupCalls(0),heroClaims(0),conversionCalls(0),intro(true),bandit(true),fish(true),killed(true),escaped(false),grunts(true),safe(false),lakeComplete(false),leaderAlive(true),exitAlive(true),heroHas(false),meHas(true),heroClaimOK(true),killAfterFade(true),invalidateExit(false),exitStage(false),fadeDistance(4.5f),exitDistance(1.5f),health(37.5f){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck)||(killAfterFade&&faded>0);}};State*state;
class CCharString{public:std::string text;CCharString():text(){++state->strings;}CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&o):text(o.text){++state->strings;}~CCharString(){--state->strings;}CCharString&operator=(const CCharString&o){text=o.text;return*this;}CCharString&operator=(const char*s){text=s;return*this;}bool operator<(const CCharString&o)const{return text<o.text;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}const C3DVector&GetPos()const{assert(id==1||id==2||id==4);static C3DVector a(1,2,3),b(4,5,6),c(7,8,9);return id==1?a:id==2?b:c;}CCharString GetDefName()const{assert(id==-1);++state->defCalls;return CCharString("CREATURE_HOSTAGE");}bool IsAlive()const{if(id==3)return state->leaderAlive;assert(id==4);state->exitStage=true;return state->exitAlive;}};
class TRetVoid{public:void Return(){}};enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EScriptingStateGroups{ESSG_WANDER_NEAR=4};enum EScriptEntityMoveType{ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,ENTITY_MOVE_SNEAK=2};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;int who;CScriptGameResourceObjectScriptedThingBase():valid(false),who(0){++state->resources;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&o):valid(o.valid),who(o.who){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->pauseDepth==0||who==0);--state->resources;}bool IsValid()const{return valid&&!(state->invalidateExit&&state->exitStage);}void Release(){assert(valid);valid=false;++state->released;}void MoveToPosition(const C3DVector&p,float range,EScriptEntityMoveType type,bool a,bool b){assert(valid&&who==-1&&type==ENTITY_MOVE_RUN&&!a);if(p.X==1){assert(range==4.0f&&!b);++state->moveFade;}else{assert(p.X==7&&range==1.0f&&b);++state->moveExit;}}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(state->movies==0);++state->movies;}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;}};
class CDefStringTable{public:CCharString GetString(long pos)const{assert(pos==9);++state->conversionCalls;return CCharString("OBJECT_REWARD");}};CDefStringTable GDefStringTable;
class CDefString{long TablePos;public:CDefString():TablePos(9){}operator CCharString()const{return GetString();}CCharString GetString()const{return GDefStringTable.GetString(TablePos);}};
struct CScriptDef{float BountyHuntHostage2WanderDistance;long BountyHuntGoldReward,BountyHuntRenownHostage2;CDefString BountyHuntRewardObject;CScriptDef():BountyHuntHostage2WanderDistance(6.5f),BountyHuntGoldReward(125),BountyHuntRenownHostage2(35){}};CScriptDef scriptDef;const CScriptDef*pGlobalScriptDef=&scriptDef;
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->frame>=state->banditAt)state->bandit=true;if(state->frame>=state->fishAt)state->fish=true;if(state->frame>=state->readyAt)state->grunts=true;}void RemoveThing(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b);++state->removed;}bool IsRegionLoaded(const CCharString&s)const{if(s.text=="GreatwoodEntrance")return state->region==1;assert(s.text=="GreatwoodLake");return state->region==2||(state->region==1&&state->frame>=state->lakeAt);}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){self(m);assert(b&&a==(state->region==2));++state->setupCalls;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(p==SCRIPT_AI_PRIORITY_HIGHEST);r.who=m.id;if(m.id==-2){++state->heroClaims;return r.valid=state->heroClaimOK;}self(m);return r.valid=++state->startAttempts>state->startFailures;}CScriptThing GetThingWithScriptName(const CCharString&s)const{if(s.text=="M_BH_FadeOut")return CScriptThing(1);if(s.text=="M_BH_Hostage2Pos")return CScriptThing(2);if(s.text=="BH_GangLeader")return CScriptThing(3);assert(s.text=="M_BH_Greatwood3Exit");return CScriptThing(4);}void EntityGetAppearanceSeed(const CScriptThing&m,unsigned long&n){self(m);n=123456;++state->seedCalls;}float GetHealth(const CScriptThing&m){self(m);return state->health;}void SetIsThingLightningable(const CScriptThing&m,bool b){self(m);assert(!b);++state->setupCalls;}void SetWanderCentrePoint(CScriptThing m,C3DVector p){self(m);assert(p.X==4&&p.Y==5&&p.Z==6);++state->setupCalls;}void SetWanderMinDistance(CScriptThing m,float f){self(m);assert(f==0);++state->setupCalls;}void SetWanderMaxDistance(CScriptThing m,float f){self(m);assert(f==6.5f);++state->setupCalls;}void SetScriptingStateGroup(CScriptThing m,EScriptingStateGroups g){self(m);assert(g==ESSG_WANDER_NEAR);++state->setupCalls;}void SetThingHasInformation(const CScriptThing&m,bool a,bool b,bool c){self(m);assert(!a&&b&&!c);++state->setupCalls;}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&t,bool b){self(m);assert(t.id==3&&!b&&state->leaderAlive&&!state->killed);++state->face;}void EntitySetAsScared(const CScriptThing&m,bool b){self(m);assert(!b);}CScriptThing&GetHero(){return hero;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->paused;}else{assert(state->pauseDepth==1);--state->pauseDepth;}}void GiveHeroGold(long n){assert(n==125&&state->pauseDepth==0&&state->safe);state->gold+=n;}bool IsObjectInThingsPossession(const CCharString&s,const CScriptThing&m){assert(s.text=="OBJECT_REWARD");if(m.id==-2)return state->heroHas;self(m);return state->meHas;}void RemoveItemFromContainer(const CScriptThing&m,const CCharString&s){self(m);assert(s.text=="OBJECT_REWARD"&&state->meHas&&!state->heroHas);state->meHas=false;++state->objectRemoved;}void GiveHeroObject(const CCharString&s,long n,bool b){assert(s.text=="OBJECT_REWARD"&&n==-1&&!b&&!state->heroHas);state->heroHas=true;++state->given;}void GiveHeroRenownPoints(long n){assert(n==35);state->renown+=n;}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float f){assert(a.id==-1);if(b.id==1){assert(f==5.0f);return state->frame>=state->fadeAt&&state->fadeDistance<f;}assert(b.id==4&&f==2.0f);return state->frame>=state->exitAt&&state->exitDistance<f;}
void FadeOutAndKillEntity(const CScriptThing&m,float f,bool a,bool b){assert(m.id==-1&&f==1.0f&&a&&b);++state->faded;}
void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,std::map<CCharString,bool>*f,std::map<CCharString,CCharString>*a,bool b,bool c){assert(s.text=="CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO"&&!f&&!a&&!b&&c&&state->pauseDepth==1&&m.size()==2);assert(m[CCharString("Hero")].who==-2&&m[CCharString("Hero")].valid==state->heroClaimOK&&m[CCharString("Victim")].who==-1&&m[CCharString("Victim")].valid);++state->cutscenes;}
namespace NScript{class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class CQ_BountyHuntScript{public:struct TeleportData{CCharString defName;unsigned long appearanceSeed;float health;CCharString scriptName,teleportTo,teleportMkr;};bool&BountyHuntIntroComplete;bool&BanditCutsceneDone;bool&FishComplete;bool&LeaderKilled;bool&LeaderEscaped;bool&AllLeaderChargeGruntsKilled;bool&Hostage2Safe;bool&LakeComplete;std::vector<TeleportData>TeleportInfo;CQ_BountyHuntScript():BountyHuntIntroComplete(state->intro),BanditCutsceneDone(state->bandit),FishComplete(state->fish),LeaderKilled(state->killed),LeaderEscaped(state->escaped),AllLeaderChargeGruntsKilled(state->grunts),Hostage2Safe(state->safe),LakeComplete(state->lakeComplete){}class CBH_Hostage2{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CQ_BountyHuntScript&ParentClass;CBH_Hostage2(CGameScriptInterfaceBase&g,CQ_BountyHuntScript&p):Game(g),Me(-1),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CQ_BountyHuntScript::CBH_Hostage2::Main()
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
        tData.scriptName="BH_Hostage2";
        tData.teleportTo="GreatwoodLake";
        tData.teleportMkr="M_BH_Hostage2Pos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game.IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    } else if (Game.IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetAsKillable(Me,true,true);
        Game.SetIsThingLightningable(Me,false);
        CScriptThing teleportMkr=Game.GetThingWithScriptName("M_BH_Hostage2Pos");
        Game.SetWanderCentrePoint(Me,teleportMkr.GetPos());
        Game.SetWanderMinDistance(Me,0.0f);
        Game.SetWanderMaxDistance(Me,pGlobalScriptDef->BountyHuntHostage2WanderDistance);
        Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        Game.SetThingHasInformation(Me,false,true,false);
        CScriptThing Leader=Game.GetThingWithScriptName("BH_GangLeader");
        for (; !false; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (sehMe.IsValid()) sehMe.Release();
            while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Leader.IsAlive() && !ParentClass.LeaderKilled) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Leader,false);
            }
            if (ParentClass.AllLeaderChargeGruntsKilled && (ParentClass.LeaderKilled || ParentClass.LeaderEscaped)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsScared(Me,false);
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase sehHero;
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> myMap;
                    Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    myMap["Hero"]=sehHero;
                    myMap["Victim"]=sehMe;
                    RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO",myMap,0,0,false,true);
                }
                ParentClass.Hostage2Safe=true;
                Game.GiveHeroGold(pGlobalScriptDef->BountyHuntGoldReward);
                if (!Game.IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject,Game.GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject,Me)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.RemoveItemFromContainer(Me,pGlobalScriptDef->BountyHuntRewardObject);
                    }
                    Game.GiveHeroObject(pGlobalScriptDef->BountyHuntRewardObject,-1,false);
                }
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Exit=Game.GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (Exit.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (sehMe.IsValid()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    }
                    while (!(IsDistanceBetweenThingsUnder(Me,Exit,2.0f))) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroRenownPoints(pGlobalScriptDef->BountyHuntRenownHostage2);
                    FadeOutAndKillEntity(Me,1.0f,true,true);
                }
                ParentClass.LakeComplete=true;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}

int run(State&s){state=&s;int records;{CGameScriptInterfaceBase g;NScript::CQ_BountyHuntScript p;{NScript::CQ_BountyHuntScript::CBH_Hostage2 t(g,p);t.Main();assert(s.things==2&&s.resources==0&&s.movies==0&&s.pauseDepth==0);records=int(p.TeleportInfo.size());if(records){assert(records==1);const NScript::CQ_BountyHuntScript::TeleportData&v=p.TeleportInfo[0];assert(v.defName.text=="CREATURE_HOSTAGE"&&v.appearanceSeed==123456&&v.health==s.health&&v.scriptName.text=="BH_Hostage2"&&v.teleportTo.text=="GreatwoodLake"&&v.teleportMkr.text=="M_BH_Hostage2Pos");}assert(s.strings==4*records);}assert(s.things==1);}assert(s.things==0&&s.strings==0);return records;}
int main(){int cases=0;
 {State s;s.intro=false;assert(run(s)==0&&s.removed==1&&s.predicates==0&&s.frame==0);++cases;}
 {State s;s.intro=false;s.stopCheck=1;run(s);assert(s.removed==0&&s.predicates==0);++cases;}
 {State s;s.region=0;run(s);assert(s.predicates==1&&s.frame==1&&s.startAttempts==0&&s.setupCalls==0);++cases;}
 {State s;run(s);assert(s.safe&&s.lakeComplete&&s.cutscenes==1&&s.paused==1&&s.heroClaims==1&&s.gold==125&&s.renown==35&&s.given==1&&s.objectRemoved==1&&s.faded==1&&s.moveExit==1&&s.setupCalls==7);++cases;}
 for(int flags=0;flags<8;++flags){State s;s.heroHas=(flags&1)!=0;s.meHas=(flags&2)!=0;s.heroClaimOK=(flags&4)!=0;run(s);assert(s.cutscenes==1&&s.given==int(!(flags&1))&&s.objectRemoved==int(!(flags&1)&&(flags&2))&&s.conversionCalls==((flags&1)?1:((flags&2)?4:3)));++cases;}
 for(int flags=0;flags<16;++flags){State s;s.limit=3;s.leaderAlive=(flags&1)!=0;s.killed=(flags&2)!=0;s.escaped=(flags&4)!=0;s.grunts=(flags&8)!=0;s.readyAt=s.grunts?0:100;run(s);bool ready=(flags&8)&&((flags&2)||(flags&4));assert(s.cutscenes==int(ready)&&s.safe==ready&&s.face==(s.leaderAlive&&!s.killed?(ready?1:2):0));++cases;}
 {State s;s.exitAlive=false;s.limit=3;run(s);assert(s.safe&&s.lakeComplete&&s.renown==0&&s.faded==0&&s.moveExit==0&&s.cutscenes==2&&s.gold==250&&s.given==1);++cases;}
 {State s;s.invalidateExit=true;run(s);assert(s.moveExit==0&&s.renown==35&&s.faded==1&&s.lakeComplete);++cases;}
 {State s;s.bandit=false;s.banditAt=3;run(s);assert(s.frame==4&&s.cutscenes==1);++cases;}
 {State s;s.bandit=false;s.banditAt=50;s.limit=3;run(s);assert(s.frame==3&&s.setupCalls==0&&s.startAttempts==0);++cases;}
 {State s;s.startFailures=3;s.limit=8;run(s);assert(s.startAttempts==6&&s.cutscenes==1);++cases;}
 {State s;s.startFailures=50;s.limit=3;run(s);assert(s.startAttempts==2&&s.cutscenes==0);++cases;}
 for(int h=0;h<3;++h){State s;s.region=1;s.health=h==0?0.0f:h==1?-1.0f:37.5f;assert(run(s)==1&&s.seedCalls==1&&s.defCalls==1&&s.moveFade==1&&s.faded==1&&s.cutscenes==0);++cases;}
 {State s;s.region=1;s.killAfterFade=false;s.lakeAt=3;s.fish=false;s.fishAt=4;assert(run(s)==1&&s.frame==4&&s.faded==1);++cases;}
 {State s;s.region=1;s.killAfterFade=false;s.lakeAt=50;assert(run(s)==1&&s.frame==6&&s.faded==1);++cases;}
 for(int d=0;d<4;++d){State s;s.region=1;s.fadeDistance=d==0?4.5f:d==1?5.0f:d==2?5.5f:std::numeric_limits<float>::quiet_NaN();int n=run(s);assert(n==int(d==0)&&s.faded==int(d==0));++cases;}
 for(int d=0;d<4;++d){State s;s.exitDistance=d==0?1.5f:d==1?2.0f:d==2?2.5f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.safe&&s.lakeComplete==(d==0)&&s.faded==int(d==0)&&s.renown==(d==0?35:0));++cases;}
 {State s;s.region=1;s.fadeAt=3;assert(run(s)==1&&s.frame==3&&s.faded==1);++cases;}
 {State s;s.exitAt=3;run(s);assert(s.frame==4&&s.faded==1&&s.lakeComplete);++cases;}
 {State s;s.grunts=false;s.readyAt=3;run(s);assert(s.cutscenes==1&&s.frame==4);++cases;}
 for(int mode=0;mode<14;++mode)for(int stop=1;stop<=150;++stop){State s;s.limit=20;s.stopCheck=stop;s.region=mode<5?1:2;s.killAfterFade=mode!=4;s.fadeAt=mode==1?4:0;s.lakeAt=mode==2?8:0;s.fish=mode!=3;s.fishAt=mode==3?6:0;s.startFailures=mode==6?5:0;s.grunts=mode!=7;s.readyAt=mode==7?7:0;s.exitAt=mode==8?8:0;s.heroHas=mode==9;s.meHas=mode!=10;s.exitAlive=mode!=11;s.heroClaimOK=mode!=12;s.invalidateExit=mode==13;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==2150);std::puts("00d1a740_TEST PASS");}
