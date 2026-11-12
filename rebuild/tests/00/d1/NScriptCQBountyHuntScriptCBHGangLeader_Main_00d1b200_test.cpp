// Complete GangLeader Main body with deterministic callees.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Strings,Things,Handles,Movies,Spawns;
class CCharString { public:
 std::string text;CCharString():text(){++Strings;}CCharString(const char* t):text(t){++Strings;}CCharString(const CCharString& t):text(t.text){++Strings;}~CCharString(){--Strings;}
 CCharString& operator=(const CCharString& t){text=t.text;return *this;}CCharString& operator=(const char* t){text=t;return *this;}bool operator<(const CCharString& t)const{return text<t.text;}operator const char*()const{return text.c_str();}
};
static CCharString operator+(const CCharString& a,const CCharString& b){return CCharString((a.text+b.text).c_str());}
class C3DVector{ public:int id;C3DVector(int n=0):id(n){}};
struct GameStub;static GameStub* Current;
class CScriptThing { public:
 int id;CScriptThing(int n=0):id(n){++Things;}CScriptThing(const CScriptThing& t):id(t.id){++Things;}~CScriptThing(){--Things;}
 C3DVector GetPos()const{return C3DVector(id);}CCharString GetDefName()const{return CCharString("CREATURE_BANDIT");}bool IsAlive()const;
 bool MsgIsHitBy(const char*)const;bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;
};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CScriptGameResourceObjectScriptedThingBase { public:
 int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& h):id(h.id){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
 CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& h){id=h.id;return *this;}
 bool IsValid()const{return id!=0;}void Release(){id=0;}CScriptThing GetScriptThing()const{return CScriptThing(id);}
 void MoveToPosition(const C3DVector&,float,int,bool,bool);void Speak(const CScriptThing&,const char*,int,bool,bool,bool);bool IsPerformingScriptTask()const;
};
struct TeleportData{long appearanceSeed;CCharString defName,scriptName,teleportTo,teleportMkr;float health;};
class CSpawnedFuncBase{ public:std::string name;CSpawnedFuncBase(const CCharString& n):name(n.text){++Spawns;}virtual ~CSpawnedFuncBase(){--Spawns;}};
template<class T>struct CSpawnedFunc: CSpawnedFuncBase{CSpawnedFunc(const CCharString& n,long z,T* p,void(T::*f)()):CSpawnedFuncBase(n){assert(z==0&&p&&f);}};
struct CQ_BountyHuntScript {
 bool BountyHuntIntroComplete,BanditCutsceneDone,FishComplete,LeaderCutsceneComplete,SpawnLeaderGrunts,LeaderAttackingHero,Hostage2Killed,LeaderAttackingHostage,LeaderChargeComplete,LeaderIntro1Complete,LeaderIntro2Complete,LeaderEscaped;
 std::vector<TeleportData> TeleportInfo;std::vector<std::string> SpawnPoints,Watchers;
 CQ_BountyHuntScript():BountyHuntIntroComplete(true),BanditCutsceneDone(true),FishComplete(true),LeaderCutsceneComplete(true),SpawnLeaderGrunts(true),LeaderAttackingHero(false),Hostage2Killed(false),LeaderAttackingHostage(false),LeaderChargeComplete(false),LeaderIntro1Complete(false),LeaderIntro2Complete(false),LeaderEscaped(false){}
 void SpawnBanditGangGrunts(const char* p){SpawnPoints.push_back(p);}void WatchForLeaderGruntsKilled(){}void WatchForLeaderChargeGruntsKilled(){}
 void AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& n){assert(n.text.empty());Watchers.push_back(p->name);delete p;}
};
struct GameStub {
 int Frame,TerminateAt,BanditAt,LakeAt,TaskFrames,TaskUntil,HitAt,HitAt2,AbilityAt,HealAt,Near1At,Near2At,ExitArrivalAt,StartAt,PauseDepth,Removed,ItemAdds,Gold,HeroQueries,Target,FadeCalls,Cutscenes,MoveCalls;
 bool Entrance,HeroHasReward,HostageAlive,ExitAlive,CameraFixed;float LeaderHealth,HostageHealth,FadeSeconds;CQ_BountyHuntScript* Parent;
 std::vector<std::string> Lines,Scenes;
 GameStub():Frame(0),TerminateAt(15),BanditAt(0),LakeAt(0),TaskFrames(1),TaskUntil(0),HitAt(-1),HitAt2(-1),AbilityAt(-1),HealAt(-1),Near1At(1000),Near2At(1000),ExitArrivalAt(4),StartAt(0),PauseDepth(0),Removed(0),ItemAdds(0),Gold(0),HeroQueries(0),Target(0),FadeCalls(0),Cutscenes(0),MoveCalls(0),Entrance(false),HeroHasReward(false),HostageAlive(true),ExitAlive(true),CameraFixed(false),LeaderHealth(100),HostageHealth(50),FadeSeconds(0),Parent(0){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<1000);if(Parent&&Frame>=BanditAt)Parent->BanditCutsceneDone=true;}
 bool IsRegionLoaded(const char* n){if(!std::strcmp(n,"GreatwoodEntrance"))return Entrance;if(!std::strcmp(n,"GreatwoodLake"))return Frame>=LakeAt;assert(false);return false;}
 void RemoveThing(const CScriptThing& t,bool a,bool b){assert(t.id==1&&!a&&!b);++Removed;}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int n){assert(n==4);if(Frame<StartAt)return false;h.id=t.id;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetThingWithScriptName(const char* n){int i=0;if(!std::strcmp(n,"M_BH_FadeOut"))i=4;else if(!std::strcmp(n,"BH_Hostage2"))i=3;else if(!std::strcmp(n,"M_BH_LeaderApproach1"))i=5;else if(!std::strcmp(n,"M_BH_LeaderApproach2"))i=6;else if(!std::strcmp(n,"M_BH_MoneyBag"))i=7;else if(!std::strcmp(n,"M_BH_Greatwood3Exit"))i=8;assert(i);return CScriptThing(i);}
 void EntityGetAppearanceSeed(const CScriptThing& t,long& s){assert(t.id==1);s=7;}
 float GetHealth(const CScriptThing& t){return t.id==3?HostageHealth:LeaderHealth;}
 void FadeScreenOutUntilNextCallToFadeScreenIn(float a,float b){assert(a==0.5f&&b==0);}
 void StartMovieSequence(const char* n,CScriptGameResourceObjectMovieBase&){assert(!*n);}
 void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
 void SetThingHasInformation(const CScriptThing&,bool,bool,bool){}void EntitySetFacingAngleTowardsThing(const CScriptThing&,const CScriptThing&,bool){}
 void GiveThingBestEnemyTarget(const CScriptThing& a,const CScriptThing& b){assert(a.id==1);Target=b.id;}
 void ModifyThingHealth(const CScriptThing& t,float delta,bool b){assert(t.id==3&&!b);HostageHealth+=delta;}
 bool IsObjectInThingsPossession(const char* n,const CScriptThing& t){assert(!std::strcmp(n,"OBJECT_REWARD")&&t.id==2);++HeroQueries;return HeroHasReward;}
 void AddItemToContainer(const CScriptThing& t,const char* n){assert(t.id==3&&!std::strcmp(n,"OBJECT_REWARD"));++ItemAdds;}
 void EntityGiveGold(const CScriptThing& t,long n){assert(t.id==3);Gold+=(int)n;}
 long AddNewConversation(const CScriptThing&,bool,bool){return 1;}void AddPersonToConversation(long,const CScriptThing& t){assert(t.id==2);}void AddLineToConversation(long,const char* n,bool,const CScriptThing&,const CScriptThing&){Lines.push_back(n);}
 void SetScriptingStateGroup(const CScriptThing& t,int n){assert(t.id==1&&n==0);}
};
bool CScriptThing::IsAlive()const{return id==3?Current->HostageAlive:id==8?Current->ExitAlive:true;}
bool CScriptThing::MsgIsHitBy(const char*)const{return Current->Frame==Current->HitAt||Current->Frame==Current->HitAt2;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*)const{return Current->Frame==Current->AbilityAt;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int,const char*)const{return Current->Frame==Current->HealAt;}
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector& p,float f,int mt,bool a,bool b){assert(id==1&&mt==1&&!a&&((p.id==4&&f==4&&!b)||(p.id==8&&f==1&&b)));++Current->MoveCalls;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* n,int,bool,bool,bool){assert(id==1&&h.id==2);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+Current->TaskFrames;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
struct CWideScreenMagicPauseEntities{GameStub* g;CWideScreenMagicPauseEntities(GameStub& p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
struct CIsThingAlive{CIsThingAlive(const CScriptThing&){}};struct TRetVoid{void Return()const{}};
class CDefString{ public:const char* GetString()const{return "OBJECT_REWARD";}};struct GlobalDef{CDefString BountyHuntRewardObject;long BountyHuntGoldReward;GlobalDef():BountyHuntGoldReward(10){}};
static GlobalDef Def;static GlobalDef* pGlobalScriptDef=&Def;static const float DELTA_NOTIONAL_ZERO=0.0001f;
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_RUN=1,HERO_ABILITY_HEAL_LIFE_SPELL=14};typedef int EScriptingStateGroups;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a,const CScriptThing& b,float d){if(b.id==4){assert(a.id==1&&d==5);return true;}if(a.id==5){assert(b.id==2&&d==3);return Current->Frame>=Current->Near1At;}if(a.id==6){assert(b.id==2&&d==3);return Current->Frame>=Current->Near2At;}assert(a.id==1&&b.id==8&&d==2);return Current->Frame>=Current->ExitArrivalAt;}
static void FadeOutAndKillEntity(const CScriptThing& t,float seconds,bool a,bool b){assert(t.id==1&&a&&b);++Current->FadeCalls;Current->FadeSeconds=seconds;}
static void RunCutsceneMacro(const char* n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& m,int a,int b,bool c,bool d){assert(m["Hero"].id==2&&m["Victim"].id==3&&m["Leader"].id==1&&a==0&&b==0&&!c&&d);++Current->Cutscenes;Current->Scenes.push_back(n);}
struct Leader {
 GameStub* Game;CScriptThing Me;CQ_BountyHuntScript ParentClass;float HealthCurrent,HealthInitial;
 Leader(GameStub& g):Game(&g),Me(1),HealthCurrent(0),HealthInitial(0){g.Parent=&ParentClass;}
 void SetPredicate(const CIsThingAlive&){}bool IsActiveThreadTerminating()const{return Game->Frame>=Game->TerminateAt;}void Main();
};
void Leader::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    float HealthDiff;
    if (!ParentClass.BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,false);
        return _Fail_Val.Return();
    }
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.BanditCutsceneDone) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (Game.IsRegionLoaded("GreatwoodEntrance")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing fadeOutMarker=Game.GetThingWithScriptName("M_BH_FadeOut");
        sehMe.MoveToPosition(fadeOutMarker.GetPos(),4.0f,ENTITY_MOVE_RUN,false,false);
        while (!IsDistanceBetweenThingsUnder(Me,fadeOutMarker,5.0f)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TeleportData tData;
        Game.EntityGetAppearanceSeed(Me,tData.appearanceSeed);
        tData.defName=Me.GetDefName();
        tData.health=Game.GetHealth(Me);
        tData.scriptName="BH_GangLeader";
        tData.teleportTo="GreatwoodLake";
        tData.teleportMkr="M_BH_LeaderPos";
        ParentClass.TeleportInfo.push_back(tData);
        FadeOutAndKillEntity(Me,1.0f,true,true);
        while (!(Game.IsRegionLoaded("GreatwoodLake") && ParentClass.FishComplete)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    else if (Game.IsRegionLoaded("GreatwoodLake")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (sehMe.IsValid()) sehMe.Release();
        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.LeaderCutsceneComplete) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.0f);
            CScriptGameResourceObjectScriptedThingBase sehHero;
            CScriptGameResourceObjectScriptedThingBase sehVictim;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> myMap;
            Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(Game.GetThingWithScriptName("BH_Hostage2"),sehVictim,SCRIPT_AI_PRIORITY_HIGHEST);
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                myMap["Hero"]=sehHero;
                myMap["Victim"]=sehVictim;
                myMap["Leader"]=sehMe;
                RunCutsceneMacro("CS_BOUNTYHUNT_GREATLAKEVICTIM",myMap,0,0,false,true);
            }
            if (sehHero.IsValid()) sehHero.Release();
            if (sehVictim.IsValid()) sehVictim.Release();
            ParentClass.LeaderCutsceneComplete=true;
        }
        if (!ParentClass.SpawnLeaderGrunts) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLake");
            ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderGruntsKilled), CCharString(""));
            ParentClass.SpawnLeaderGrunts=true;
        }
        Game.SetThingHasInformation(Me,false,true,false);
        HealthCurrent=HealthInitial=Game.GetHealth(Me);
        CScriptThing Hostage2=Game.GetThingWithScriptName("BH_Hostage2");
        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
        CScriptThing approachMkr1=Game.GetThingWithScriptName("M_BH_LeaderApproach1");
        CScriptThing approachMkr2=Game.GetThingWithScriptName("M_BH_LeaderApproach2");
        CScriptThing moneyMkr=Game.GetThingWithScriptName("M_BH_MoneyBag");
        for (; !false; Game.NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))
                && !ParentClass.LeaderAttackingHero && !ParentClass.Hostage2Killed && !ParentClass.LeaderAttackingHostage) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ParentClass.LeaderChargeComplete) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (sehMe.IsValid()) sehMe.Release();
                        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                            sehMe.Speak(Game.GetHero(),"TEXT_QST_017_LEADER6",GROUP_SELECT_NONE,false,true,false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (sehMe.IsValid()) sehMe.Release();
                        Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
                        ParentClass.LeaderChargeComplete=true;
                    }
                }
                ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLeaderCharge");
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderChargeGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderChargeGruntsKilled), CCharString(""));
                ParentClass.LeaderAttackingHero=true;
            }
            if (!ParentClass.LeaderAttackingHero && !ParentClass.LeaderIntro1Complete && IsDistanceBetweenThingsUnder(approachMkr1,Game.GetHero(),3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ParentClass.Hostage2Killed) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (sehMe.IsValid()) sehMe.Release();
                        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                            sehMe.Speak(Game.GetHero(),"TEXT_QST_017_LEADER_DEMAND_2",GROUP_SELECT_NONE,false,true,false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.LeaderIntro1Complete=true;
                }
            }
            else if (!ParentClass.LeaderAttackingHero && !ParentClass.LeaderIntro2Complete && IsDistanceBetweenThingsUnder(approachMkr2,Game.GetHero(),3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ParentClass.Hostage2Killed) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (sehMe.IsValid()) sehMe.Release();
                        while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                            sehMe.Speak(Game.GetHero(),"TEXT_QST_017_LEADER_WARNING_3",GROUP_SELECT_NONE,false,true,false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.LeaderIntro2Complete=true;
                }
            }
            if (ParentClass.LeaderIntro2Complete && !ParentClass.Hostage2Killed && !ParentClass.LeaderAttackingHostage && !ParentClass.LeaderAttackingHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Hostage2,false);
                Game.ModifyThingHealth(Hostage2,1.0f-Game.GetHealth(Hostage2),false);
                if (!Game.IsObjectInThingsPossession(pGlobalScriptDef->BountyHuntRewardObject.GetString(),Game.GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddItemToContainer(Hostage2,pGlobalScriptDef->BountyHuntRewardObject.GetString());
                }
                Game.EntityGiveGold(Hostage2,pGlobalScriptDef->BountyHuntGoldReward);
                Game.GiveThingBestEnemyTarget(Me,Hostage2);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_017_LEADER7",false,Me,Game.GetHero());
                ParentClass.SpawnBanditGangGrunts("M_BH_GruntSpawnLeaderCharge");
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("ParentClass.") + CCharString("WatchForLeaderChargeGruntsKilled"), 0, &ParentClass, &CQ_BountyHuntScript::WatchForLeaderChargeGruntsKilled), CCharString(""));
                Game.SetScriptingStateGroup(Me,(EScriptingStateGroups)0);
                ParentClass.LeaderAttackingHostage=true;
                if (sehMe.IsValid()) sehMe.Release();
            }
            if (ParentClass.Hostage2Killed || !Hostage2.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing Exit=Game.GetThingWithScriptName("M_BH_Greatwood3Exit");
                if (sehMe.IsValid() && Exit.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                    while (!IsDistanceBetweenThingsUnder(Me,Exit,2.0f)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.LeaderEscaped=true;
                    FadeOutAndKillEntity(Me,2.0f,true,true);
                    for (;;) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
}

static bool Balanced(const GameStub& g,const Leader& l){return Strings==(int)l.ParentClass.TeleportInfo.size()*4&&Things==1&&Handles==0&&Movies==0&&Spawns==0&&g.PauseDepth==0;}
static bool Has(const GameStub& g,const char* n){for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)return true;return false;}
int main(){bool ok=true;
 {GameStub g;Leader l(g);l.ParentClass.BountyHuntIntroComplete=false;l.Main();ok=ok&&Balanced(g,l)&&g.Removed==1&&g.Frame==0;}
 {GameStub g;Leader l(g);g.Entrance=true;g.LakeAt=3;l.Main();ok=ok&&Balanced(g,l)&&g.MoveCalls==1&&g.FadeCalls==1&&g.FadeSeconds==1&&l.ParentClass.TeleportInfo.size()==1;const TeleportData& t=l.ParentClass.TeleportInfo[0];ok=ok&&t.appearanceSeed==7&&t.health==100&&t.defName.text=="CREATURE_BANDIT"&&t.scriptName.text=="BH_GangLeader"&&t.teleportTo.text=="GreatwoodLake"&&t.teleportMkr.text=="M_BH_LeaderPos";}
 {GameStub g;Leader l(g);g.Entrance=true;g.LakeAt=5;g.TerminateAt=2;l.Main();ok=ok&&Balanced(g,l)&&g.Frame==2&&g.FadeCalls==1&&l.ParentClass.TeleportInfo.size()==1;}
 {GameStub g;Leader l(g);l.ParentClass.LeaderCutsceneComplete=false;l.ParentClass.SpawnLeaderGrunts=false;l.Main();ok=ok&&Balanced(g,l)&&g.Cutscenes==1&&l.ParentClass.LeaderCutsceneComplete&&l.ParentClass.SpawnLeaderGrunts&&l.ParentClass.SpawnPoints.size()==1&&l.ParentClass.Watchers[0]=="ParentClass.WatchForLeaderGruntsKilled"&&l.HealthCurrent==100&&l.HealthInitial==100;}
 {GameStub g;Leader l(g);g.HitAt=2;g.HitAt2=6;l.Main();ok=ok&&Balanced(g,l)&&l.ParentClass.LeaderChargeComplete&&l.ParentClass.LeaderAttackingHero&&g.Target==2&&l.ParentClass.SpawnPoints.size()==1&&Has(g,"TEXT_QST_017_LEADER6");}
 {GameStub g;Leader l(g);g.AbilityAt=g.HealAt=2;l.Main();ok=ok&&Balanced(g,l)&&!l.ParentClass.LeaderAttackingHero&&l.ParentClass.SpawnPoints.empty();}
 {GameStub g;Leader l(g);g.HitAt=g.AbilityAt=g.HealAt=2;l.Main();ok=ok&&Balanced(g,l)&&l.ParentClass.LeaderAttackingHero;}
 {GameStub g;Leader l(g);g.Near1At=2;g.Near2At=6;l.Main();ok=ok&&Balanced(g,l)&&l.ParentClass.LeaderIntro1Complete&&l.ParentClass.LeaderIntro2Complete&&l.ParentClass.LeaderAttackingHostage&&g.Target==3&&g.HostageHealth==1&&g.HeroQueries==1&&g.ItemAdds==1&&g.Gold==10&&Has(g,"TEXT_QST_017_LEADER_DEMAND_2")&&Has(g,"TEXT_QST_017_LEADER_WARNING_3")&&Has(g,"TEXT_QST_017_LEADER7");}
 {GameStub g;Leader l(g);l.ParentClass.LeaderIntro1Complete=l.ParentClass.LeaderIntro2Complete=true;g.HeroHasReward=true;l.Main();ok=ok&&Balanced(g,l)&&g.HeroQueries==1&&g.ItemAdds==0&&g.Gold==10&&l.ParentClass.LeaderAttackingHostage;}
 {GameStub g;Leader l(g);l.ParentClass.Hostage2Killed=true;g.TerminateAt=9;l.Main();ok=ok&&Balanced(g,l)&&l.ParentClass.LeaderEscaped&&g.MoveCalls==1&&g.FadeCalls==1&&g.FadeSeconds==2&&g.Frame==9;}
 {GameStub g;Leader l(g);g.HostageAlive=false;g.ExitAlive=false;l.Main();ok=ok&&Balanced(g,l)&&!l.ParentClass.LeaderEscaped&&g.MoveCalls==0&&g.FadeCalls==0;}
 {GameStub g;Leader l(g);g.HitAt=2;g.TaskFrames=3;g.TerminateAt=4;l.Main();ok=ok&&Balanced(g,l)&&!l.ParentClass.LeaderChargeComplete&&!l.ParentClass.LeaderAttackingHero&&Has(g,"TEXT_QST_017_LEADER6");}
 {GameStub g;Leader l(g);l.ParentClass.BanditCutsceneDone=false;g.BanditAt=3;g.TerminateAt=2;l.Main();ok=ok&&Balanced(g,l)&&g.Lines.empty()&&g.Frame==2;}
 {GameStub g;Leader l(g);g.StartAt=8;g.TerminateAt=3;l.Main();ok=ok&&Balanced(g,l)&&g.Lines.empty()&&g.Frame==3;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Spawns==0;std::printf(ok?"00d1b200_TEST PASS\n":"00d1b200_TEST FAIL\n");return ok?0:1;
}