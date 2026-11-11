// Entire Main body with deterministic world/resource/ownership callees.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Bindings,Threads,ReturnCalls;
struct CCharString{std::string text;CCharString():text(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}};
struct C3DVector{float X,Y,Z;C3DVector(float x=0):X(x),Y(0),Z(0){}};
struct GameStub;static GameStub*Current;
struct CScriptThing{int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}CScriptThing&operator=(const CScriptThing&t){id=t.id;return *this;}bool IsAlive()const;bool IsDead()const{return !IsAlive();}C3DVector GetPos()const{assert(id==3);return C3DVector(3);}};
struct BindingBase{CCharString Name;void*Owner;void(*Alloc)();BindingBase(const CCharString&n,void*p,void(*f)()):Name(n),Owner(p),Alloc(f){++Bindings;}virtual~BindingBase(){--Bindings;}};
template<class T>struct CEntityScriptBinding:BindingBase{CEntityScriptBinding(const CCharString&n,T*p,void(*f)(),unsigned long flags):BindingBase(n,p,f){assert(flags==0);}};
struct ThreadBase{CCharString Name;ThreadBase(const CCharString&n):Name(n){++Threads;}virtual~ThreadBase(){--Threads;}virtual bool Valid(void*)const=0;};
template<class T>struct CSpawnedFunc:ThreadBase{T*Owner;void(T::*Func)();unsigned long Id;CSpawnedFunc(const CCharString&n,unsigned long i,T*p,void(T::*f)()):ThreadBase(n),Owner(p),Func(f),Id(i){}bool Valid(void*p)const{return Owner==p&&Func==&T::CheckRegionLeaving&&Id==0&&Name.text=="CheckRegionLeaving";}};
struct Haunted;typedef Haunted CV_HauntedHouseScript;
struct GameStub{Haunted*H;int Frame,TerminateAt,QuestAt,RegionAt,RegionOffAt,ReloadAt,EnterAt,DoorAt,KillBothAt,KillOneAt,Created,Removed,Fills,NumCandles,LightOff,LightOn,LightCalls,Unusable,Hidden,LocalPost,GamePost,StopAtFill,StopAtLight;bool MarkerAlive,Stopped;std::vector<bool>EffectAlive;std::vector<std::string>Events;
 GameStub():H(0),Frame(0),TerminateAt(40),QuestAt(6),RegionAt(2),RegionOffAt(-1),ReloadAt(-1),EnterAt(10),DoorAt(14),KillBothAt(-1),KillOneAt(-1),Created(0),Removed(0),Fills(0),NumCandles(3),LightOff(0),LightOn(0),LightCalls(0),Unusable(0),Hidden(0),LocalPost(0),GamePost(0),StopAtFill(-1),StopAtLight(-1),MarkerAlive(true),Stopped(false){Current=this;ReturnCalls=0;}
 bool Region()const{return Frame>=RegionAt&&(RegionOffAt<0||Frame<RegionOffAt||(ReloadAt>=0&&Frame>=ReloadAt));}void NewScriptFrame();bool IsQuestCompleted(const CCharString&n){assert(n.text=="Q_TraderEscort");return QuestAt>=0&&Frame>=QuestAt;}bool IsRegionLoaded(const CCharString&n){assert(n.text=="HauntedHouse");return Region();}
 CScriptThing GetThingWithScriptName(const CCharString&n){int id=n.text=="PreBarrierHere"?3:n.text=="HH_CellarDoor"?8:n.text=="DoorToLock"?9:0;assert(id);return CScriptThing(id);}CScriptThing CreateEffect(const CCharString&n,const C3DVector&p,const CCharString&e,float f,bool a,bool b){assert((n.text=="NEW_RED_FORCEFIELD_IDLE_SMALL_01"||n.text=="NEW_RED_FORCEFIELD_IDLE_SMALL_02")&&p.X==3&&e.text.empty()&&f==0&&!a&&!b);int id=100+Created++;EffectAlive.push_back(true);return CScriptThing(id);}void SetThingAsUsable(const CScriptThing&t,bool b){assert(t.id==8&&!b);++Unusable;}void EntitySetAsDrawable(const CScriptThing&t,bool b){assert(t.id==9&&!b);++Hidden;}
 void GetAllThingsWithScriptName(const CCharString&n,std::vector<CScriptThing>&v){assert(n.text=="CandleToLight");v.clear();for(int i=0;i<NumCandles;++i)v.push_back(CScriptThing(10+i));++Fills;if(Fills==StopAtFill)Stopped=true;}void EntitySetLightAsOn(const CScriptThing&t,bool b){assert(t.id>=10&&t.id<10+NumCandles);b?++LightOn:++LightOff;++LightCalls;if(LightCalls==StopAtLight)Stopped=true;}void RemoveThing(const CScriptThing&t,bool a,bool b){assert(t.id>=100&&t.id<100+Created&&!a&&b&&EffectAlive[t.id-100]);EffectAlive[t.id-100]=false;++Removed;}void PostAddScriptedEntities(){++GamePost;Events.push_back("game_post");}
};
bool CScriptThing::IsAlive()const{if(id==3)return Current->MarkerAlive&&Current->Region();if(id>=100)return Current->Region()&&Current->EffectAlive[id-100];return false;}
struct TRetVoid{void Return(){++ReturnCalls;}};
struct Haunted{GameStub&Game;bool InHouse,CellarDoorOpen;std::vector<BindingBase*>OwnedBindings;std::vector<ThreadBase*>OwnedThreads;struct CM_UndeadHere{static void Alloc(){}};struct CHHUndead{static void Alloc(){}};struct CM_QuestControl{static void Alloc(){}};struct CM_HeroInsideHouse{static void Alloc(){}};
 Haunted(GameStub&g):Game(g),InHouse(false),CellarDoorOpen(false){g.H=this;}~Haunted(){for(unsigned i=0;i<OwnedBindings.size();++i)delete OwnedBindings[i];for(unsigned i=0;i<OwnedThreads.size();++i)delete OwnedThreads[i];}bool IsActiveThreadTerminating()const{return Game.Stopped||Game.Frame>=Game.TerminateAt;}void AddEntityScriptBinding(BindingBase*b){assert(b->Owner==this);assert((b->Name.text=="M_UndeadHere"&&b->Alloc==&CM_UndeadHere::Alloc)||(b->Name.text=="HHUndead"&&b->Alloc==&CHHUndead::Alloc)||(b->Name.text=="M_QuestControl"&&b->Alloc==&CM_QuestControl::Alloc)||(b->Name.text=="M_HeroInsideHouse"&&b->Alloc==&CM_HeroInsideHouse::Alloc));OwnedBindings.push_back(b);Game.Events.push_back(b->Name.text);}void PostAddScriptedEntities(){++Game.LocalPost;Game.Events.push_back("local_post");}void AddSpawnedFunction(ThreadBase*t,const CCharString&n){assert(t->Valid(this)&&n.text.empty());OwnedThreads.push_back(t);Game.Events.push_back("spawn");}void CheckRegionLeaving(){}void Main();};
void GameStub::NewScriptFrame(){++Frame;assert(Frame<200);if(RegionOffAt>=0&&Frame==RegionOffAt)for(unsigned i=0;i<EffectAlive.size();++i)EffectAlive[i]=false;if(KillBothAt>=0&&Frame==KillBothAt)for(unsigned i=0;i<EffectAlive.size();++i)EffectAlive[i]=false;if(KillOneAt>=0&&Frame==KillOneAt&&!EffectAlive.empty())EffectAlive[EffectAlive.size()-2]=false;if(H){if(EnterAt>=0&&Frame>=EnterAt)H->InHouse=true;if(DoorAt>=0&&Frame>=DoorAt)H->CellarDoorOpen=true;}}
void Haunted::Main()
{
    TRetVoid _Fail_Val;
    bool DoneThisBefore = false;
    CScriptThing FXBarrier;
    CScriptThing Forcefield1;
    CScriptThing Forcefield2;
    std::vector<CScriptThing> Candles;
    while (!Game.IsQuestCompleted("Q_TraderEscort")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!DoneThisBefore && Game.IsRegionLoaded("HauntedHouse")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            FXBarrier = Game.GetThingWithScriptName("PreBarrierHere");
            Forcefield1 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_01", FXBarrier.GetPos(), "", 0.0f, false, false);
            Forcefield2 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_02", FXBarrier.GetPos(), "", 0.0f, false, false);
            CScriptThing CellarDoor = Game.GetThingWithScriptName("HH_CellarDoor");
            Game.SetThingAsUsable(CellarDoor, false);
            CScriptThing LockableDoor = Game.GetThingWithScriptName("DoorToLock");
            Game.EntitySetAsDrawable(LockableDoor, false);
            Game.GetAllThingsWithScriptName("CandleToLight", Candles);
            if (!false) {
                for (long i = 0; i < Candles.size(); ++i) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntitySetLightAsOn(Candles[i], false);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoneThisBefore = true;
        }
        if (FXBarrier.IsAlive() && Forcefield1.IsDead() && Forcefield2.IsDead()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Forcefield1 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_01", FXBarrier.GetPos(), "", 0.0f, false, false);
            Forcefield2 = Game.CreateEffect("NEW_RED_FORCEFIELD_IDLE_SMALL_02", FXBarrier.GetPos(), "", 0.0f, false, false);
        }
        if (DoneThisBefore && !Game.IsRegionLoaded("HauntedHouse")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoneThisBefore = false;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (Forcefield1.IsAlive() && Forcefield2.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Forcefield1, false, true);
        Game.RemoveThing(Forcefield2, false, true);
    }
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_UndeadHere", this, &CM_UndeadHere::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("HHUndead", this, &CHHUndead::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_QuestControl", this, &CM_QuestControl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HauntedHouseScript>("M_HeroInsideHouse", this, &CM_HeroInsideHouse::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_HauntedHouseScript>("CheckRegionLeaving", 0, this, &CV_HauntedHouseScript::CheckRegionLeaving), CCharString(""));
    while (!InHouse) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game.IsRegionLoaded("HauntedHouse")) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.GetAllThingsWithScriptName("CandleToLight", Candles);
        if (!false) {
            for (long i = 0; i < Candles.size(); ++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetLightAsOn(Candles[i], false);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (Game.IsRegionLoaded("HauntedHouse") && !InHouse) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!false) {
        for (long i = 0; i < Candles.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetLightAsOn(Candles[i], true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!CellarDoorOpen) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    if (!false) {
        for (long i = 0; i < Candles.size(); ++i) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetLightAsOn(Candles[i], false);
        }
    }
    if (IsActiveThreadTerminating()) {
        void (TRetVoid::*ReturnFunction)()=&TRetVoid::Return;
        (_Fail_Val.*ReturnFunction)();
        return;
    }
}

static bool Balanced(const GameStub&g,const Haunted&o){return Things==0&&Strings==(int)(o.OwnedBindings.size()+o.OwnedThreads.size())&&Bindings==(int)o.OwnedBindings.size()&&Threads==(int)o.OwnedThreads.size();}
static bool Posted(const GameStub&g,const Haunted&o){return o.OwnedBindings.size()==4&&o.OwnedThreads.size()==1&&g.LocalPost==1&&g.GamePost==1&&g.Events.size()==7&&g.Events[0]=="M_UndeadHere"&&g.Events[1]=="HHUndead"&&g.Events[2]=="M_QuestControl"&&g.Events[3]=="M_HeroInsideHouse"&&g.Events[4]=="local_post"&&g.Events[5]=="game_post"&&g.Events[6]=="spawn";}
static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 {GameStub g;Haunted o(g);o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Frame==14&&o.InHouse&&o.CellarDoorOpen&&g.Created==2&&g.Removed==2&&g.Unusable==1&&g.Hidden==1&&g.Fills==2&&g.LightOn==3&&g.LightOff==9&&ReturnCalls==0,++n)&&ok;}
 {GameStub g;Haunted o(g);g.KillBothAt=4;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Created==4&&g.Removed==2&&g.LightOn==3&&g.Unusable==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.KillOneAt=6;o.Main();ok=Check(Balanced(g,o)&&g.Created==2&&g.Removed==0&&g.EffectAlive[1]&&Posted(g,o),++n)&&ok;}
 {GameStub g;Haunted o(g);g.RegionAt=1;g.RegionOffAt=3;g.ReloadAt=5;g.QuestAt=8;g.EnterAt=12;g.DoorAt=16;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Created==4&&g.Removed==2&&g.Unusable==2&&g.Hidden==2&&g.Fills==3&&g.LightOff==12&&g.LightOn==3,++n)&&ok;}
 {GameStub g;Haunted o(g);g.QuestAt=0;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Created==0&&g.Removed==0&&g.Unusable==0&&g.Hidden==0&&g.Fills==1&&g.LightOff==6&&g.LightOn==3&&g.Frame==14,++n)&&ok;}
 {GameStub g;Haunted o(g);g.QuestAt=0;o.InHouse=true;o.CellarDoorOpen=true;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Frame==0&&g.Fills==0&&g.LightCalls==0&&ReturnCalls==0,++n)&&ok;}
 {GameStub g;Haunted o(g);o.InHouse=true;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Fills==1&&g.LightOn==3&&g.LightOff==6&&g.Frame==14,++n)&&ok;}
 {GameStub g;Haunted o(g);g.QuestAt=-1;g.RegionAt=1;g.TerminateAt=4;o.Main();ok=Check(Balanced(g,o)&&o.OwnedBindings.empty()&&o.OwnedThreads.empty()&&g.Created==2&&g.Removed==0&&g.LightOff==3&&g.LightOn==0&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.RegionAt=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g,o)&&g.Created==0&&o.OwnedBindings.empty()&&g.Fills==0&&ReturnCalls==1,++n)&&ok;}
 for(int fill=1;fill<3;++fill){GameStub g;Haunted o(g);g.StopAtFill=fill;o.Main();ok=Check(Balanced(g,o)&&g.Stopped&&g.Fills==fill&&g.LightOff==(fill==1?0:3)&&g.LightOn==0&&o.OwnedBindings.size()==(fill==1?0u:4u)&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.StopAtLight=1;o.Main();ok=Check(Balanced(g,o)&&g.LightOff==1&&g.LightOn==0&&o.OwnedBindings.empty()&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.QuestAt=0;g.RegionAt=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Fills==0&&g.LightCalls==0&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.EnterAt=100;g.TerminateAt=8;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&!o.InHouse&&g.Fills==2&&g.LightOn==0&&g.Removed==2&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.StopAtLight=7;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.LightOff==6&&g.LightOn==1&&o.InHouse&&!o.CellarDoorOpen&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.DoorAt=100;g.TerminateAt=12;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.LightOn==3&&g.LightOff==6&&!o.CellarDoorOpen&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.StopAtLight=10;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.LightOn==3&&g.LightOff==7&&o.CellarDoorOpen&&ReturnCalls==1,++n)&&ok;}
 {GameStub g;Haunted o(g);g.StopAtLight=12;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.LightOn==3&&g.LightOff==9&&o.CellarDoorOpen&&ReturnCalls==1&&g.Stopped,++n)&&ok;}
 for(int candles=0;candles<5;++candles){GameStub g;Haunted o(g);g.NumCandles=candles;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.LightOn==candles&&g.LightOff==3*candles&&g.Frame==14&&ReturnCalls==0,++n)&&ok;}
 {GameStub g;Haunted o(g);g.QuestAt=0;g.RegionAt=1;g.EnterAt=1;g.DoorAt=1;o.Main();ok=Check(Balanced(g,o)&&Posted(g,o)&&g.Frame==1&&g.Fills==1&&g.LightOn==3&&g.LightOff==6,++n)&&ok;}
 ok=ok&&Strings==0&&Things==0&&Bindings==0&&Threads==0;std::printf(ok?"00e93eb0_TEST PASS\n":"00e93eb0_TEST FAIL\n");return ok?0:1;
}
