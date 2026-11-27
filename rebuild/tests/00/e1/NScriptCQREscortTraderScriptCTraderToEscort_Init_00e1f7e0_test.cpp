
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine/EScriptingStateGroups.h"
static void ck(bool x){if(!x)exit(1);}
struct CCharString {
 char name[64];
 CCharString(const char* s=""){strcpy(name,s);}
};
static CCharString operator+(const CCharString& a,const CCharString& b){CCharString out=a;strcat(out.name,b.name);return out;}
struct C3DVector {float x,y,z;};
struct CScriptThing { unsigned int id; C3DVector pos; C3DVector GetPos() const{return pos;} };
static CScriptThing expected;
struct GlobalDef {float EscortTraderWanderDistance;};
static GlobalDef def;
static GlobalDef* pGlobalScriptDef=&def;
static int sequence,spawned,callbacks;
static void same(const CScriptThing& m){ck(m.id==expected.id);}
struct CSpawnedFuncBase { virtual ~CSpawnedFuncBase(){} virtual void Run()=0;char title[64]; };
template<class T,class P>struct CSpawnedFunc1: CSpawnedFuncBase {
 T* context; void(T::*callback)(P); P arg;
 CSpawnedFunc1(const CCharString& n,unsigned long id,T* c,void(T::*f)(P),P p):context(c),callback(f),arg(p){ck(id==0);strcpy(title,n.name);}
 void Run(){(context->*callback)(arg);}
};
struct GameInterface {
 void SetThingHasInformation(CScriptThing m,bool x,bool y,bool z){same(m);ck(sequence++==0&&x&&!y&&!z);}
 void SetThingPersistent(CScriptThing m,bool x){same(m);ck(sequence++==1&&x);}
 void SetWanderCentrePoint(CScriptThing m,C3DVector p){same(m);ck(sequence++==6&&p.x==expected.pos.x&&p.y==expected.pos.y&&p.z==expected.pos.z);}
 void SetWanderMinDistance(CScriptThing m,float d){same(m);ck(sequence++==7&&d==0);}
 void SetWanderMaxDistance(CScriptThing m,float d){same(m);ck(sequence++==8&&d==def.EscortTraderWanderDistance);}
 void SetScriptingStateGroup(CScriptThing m,EScriptingStateGroups group){same(m);ck(sequence++==9&&group==ESSG_WANDER_NEAR);}
 void EntitySetAsAllowedToFollowHero(CScriptThing m,bool flag){same(m);ck(sequence++==10&&!flag);}
};
namespace NScript {
class CQR_EscortTraderScript {
public:
 class CTraderToEscort;
 CSpawnedFuncBase* tasks[4];
 void WatchForTraderDead(CScriptThing m){ck(callbacks++==0);same(m);}
 void WatchForTraderHit(CScriptThing m){ck(callbacks++==1);same(m);}
 void WatchForExpressionFollow(CScriptThing m){ck(callbacks++==2);same(m);}
 void WatchForExpressionWait(CScriptThing m){ck(callbacks++==3);same(m);}
 void AddSpawnedFunction(CSpawnedFuncBase* p,const CCharString& empty){
  const char* names[]={"ParentClass.WatchForTraderDead","ParentClass.WatchForTraderHit","ParentClass.WatchForExpressionFollow","ParentClass.WatchForExpressionWait"};
  ck(sequence++==2+spawned && !*empty.name && strcmp(names[spawned],p->title)==0);tasks[spawned++]=p;
 }
};
class CQR_EscortTraderScript::CTraderToEscort {
public:
 CQR_EscortTraderScript& ParentClass;GameInterface& Game;CScriptThing Me;
 long lastRandomSpeechFollowIdx1,lastRandomSpeechFollowIdx2,lastRandomSpeechWaitIdx1,lastRandomSpeechWaitIdx2;
 CTraderToEscort(CQR_EscortTraderScript& p,GameInterface& g):ParentClass(p),Game(g),Me(expected){}
 void Init();
};
}
void NScript::CQR_EscortTraderScript::CTraderToEscort::Init()
{
    Game.SetThingHasInformation(Me, true, false, false);
    Game.SetThingPersistent(Me, true);
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForTraderDead"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForTraderDead, Me), CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForTraderHit"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForTraderHit, Me), CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForExpressionFollow"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForExpressionFollow, Me), CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForExpressionWait"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForExpressionWait, Me), CCharString(""));
    lastRandomSpeechFollowIdx1 = 10;
    lastRandomSpeechFollowIdx2 = 10;
    lastRandomSpeechWaitIdx1 = 5;
    lastRandomSpeechWaitIdx2 = 5;
    Game.SetWanderCentrePoint(Me, Me.GetPos());
    Game.SetWanderMinDistance(Me, 0.0f);
    Game.SetWanderMaxDistance(Me, pGlobalScriptDef->EscortTraderWanderDistance);
    Game.SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    Game.EntitySetAsAllowedToFollowHero(Me, false);
}

int main() {
 for(int j=-16;j<=16;++j)for(int i=0;i<8;++i){
  sequence=spawned=callbacks=0;expected.id=17*i+j;expected.pos.x=float(j);expected.pos.y=float(i);expected.pos.z=float(j*i);
  def.EscortTraderWanderDistance=float(i-j)*0.5f;
  NScript::CQR_EscortTraderScript parent; GameInterface game;
  NScript::CQR_EscortTraderScript::CTraderToEscort actor(parent,game);actor.Init();
  ck(spawned==4&&sequence==11&&actor.lastRandomSpeechFollowIdx1==10&&actor.lastRandomSpeechFollowIdx2==10
        &&actor.lastRandomSpeechWaitIdx1==5&&actor.lastRandomSpeechWaitIdx2==5);
  actor.Me.id++;
  for(int n=0;n<4;++n){parent.tasks[n]->Run();delete parent.tasks[n];}
  ck(callbacks==4);
 }
 puts("00e1f7e0_TEST PASS");return 0;
}