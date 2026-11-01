// Behavior-only fixture: exact selected target body, constructed objects.
// Unused interfaces/tails are reduced. Native slot indices remain independent
// evidence, and no byte parity is claimed by this behavioral test.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int cases,stage,validCalls,releaseCalls,copyCalls,copyDtors,mutate;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void*Data;};
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer&o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}};
class CBaseClass{public:virtual~CBaseClass(){}};
class CScriptThing:public CBaseClass{public:CCountedPointer<CScriptThing>PImp;CScriptThing(){}CScriptThing(const CScriptThing&o):CBaseClass(),PImp(o.PImp){++copyCalls;}virtual~CScriptThing(){if(PImp.Info){CHECK(PImp.Info->RefCount>0);--PImp.Info->RefCount;++copyDtors;}}};
class CScriptGameResourceObjectScriptedThingBase{public:CCountedPointer<CScriptGameResourceObjectScriptedThingBase>PImp;bool IsValid()const{++validCalls;return PImp.Data!=0;}void Release();};
class CGameScriptInterfaceBase{public:virtual~CGameScriptInterfaceBase(){}virtual CScriptThing&GetHero()=0;virtual void GiveThingBestEnemyTarget(const CScriptThing&,const CScriptThing&)const=0;virtual void EntitySetAsKillable(const CScriptThing&,bool,bool)const=0;virtual void EntitySetAsDamageable(const CScriptThing&,bool)const=0;virtual void SetIsPushableByHero(CScriptThing,bool)=0;virtual void EntitySetAsUseMovementInActions(const CScriptThing&,bool)const=0;};
class CEntityScriptBase{public:CGameScriptInterfaceBase*Game;CScriptThing Me;virtual~CEntityScriptBase(){}};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{public:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*begin(){return this->_M_start;}T&operator[](unsigned int n){return *(begin()+n);}};
}
namespace NScript{
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript{public:std::vector<CScriptGameResourceObjectScriptedThingBase>seh_Guards;class CMurderTwistSheriffGuard;};
class CV_MurderTwistFinalScript::CMurderTwistSheriffGuard:public CEntityScriptBase{public:CV_MurderTwistFinalScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;CScriptGameResourceObjectScriptedThingBase MyEntityHandle;unsigned long NextLine,ID;CMurderTwistSheriffGuard(CV_MurderTwistFinalScript&p):ParentClass(p),PMasterData(0),NextLine(0),ID(0){}void AttackHero();};
void CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero(){
 Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
 Game->EntitySetAsKillable(Me,true,true);
 Game->EntitySetAsDamageable(Me,true);
 Game->SetIsPushableByHero(Me,true);
 Game->EntitySetAsUseMovementInActions(Me,true);
 if(MyEntityHandle.IsValid())MyEntityHandle.Release();
 CScriptGameResourceObjectScriptedThingBase*handle=&ParentClass.seh_Guards[ID];
 if(handle->IsValid())handle->Release();
}
}
static NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard*guard;
static CScriptThing*expectedHero;static CScriptGameResourceObjectScriptedThingBase*released[2];static unsigned long newID;
void CScriptGameResourceObjectScriptedThingBase::Release(){CHECK(releaseCalls<2);released[releaseCalls++]=this;CHECK(PImp.Data);if(PImp.Info){CHECK(PImp.Info->RefCount>0);--PImp.Info->RefCount;}PImp.Data=0;PImp.Info=0;if(this==&guard->MyEntityHandle&&mutate)guard->ID=newID;}
class CTestGame:public CGameScriptInterfaceBase{public:
 CScriptThing&GetHero(){CHECK(stage++==0);return *expectedHero;}
 void GiveThingBestEnemyTarget(const CScriptThing&a,const CScriptThing&b)const{CHECK(stage++==1&&&a==&guard->Me&&&b==expectedHero);}
 void EntitySetAsKillable(const CScriptThing&a,bool b,bool c)const{CHECK(stage++==2&&&a==&guard->Me&&b&&c);}
 void EntitySetAsDamageable(const CScriptThing&a,bool b)const{CHECK(stage++==3&&&a==&guard->Me&&b);}
 void SetIsPushableByHero(CScriptThing a,bool b){CHECK(stage++==4&&b&&a.PImp.Data==guard->Me.PImp.Data&&a.PImp.Info==guard->Me.PImp.Info);if(a.PImp.Info)CHECK(a.PImp.Info->RefCount==8);}
 void EntitySetAsUseMovementInActions(const CScriptThing&a,bool b)const{CHECK(stage++==5&&&a==&guard->Me&&b);}
};
int main(){
 for(int n=1;n<=8;++n)for(int at=0;at<n;++at)for(int own=0;own<2;++own)for(int target=0;target<2;++target)for(int infoPresent=0;infoPresent<2;++infoPresent)for(int change=0;change<2;++change){
  NScript::CV_MurderTwistFinalScript parent;NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard g(parent);guard=&g;
  CScriptGameResourceObjectScriptedThingBase nodes[8],impl;CCPPointerInfo nodeInfos[8],ownInfo,thingInfo;
  CScriptThing hero,thing;CTestGame game;g.Game=&game;g.ID=(unsigned long)at;newID=(at+1)%n;mutate=change;
  g.Me.PImp.Data=&thing;g.Me.PImp.Info=infoPresent?&thingInfo:0;thingInfo.RefCount=7;
  ownInfo.RefCount=3;g.MyEntityHandle.PImp.Data=own?&impl:0;g.MyEntityHandle.PImp.Info=own?&ownInfo:0;
  for(int i=0;i<n;++i){nodeInfos[i].RefCount=3;nodes[i].PImp.Data=target?&impl:0;nodes[i].PImp.Info=target?&nodeInfos[i]:0;}
  parent.seh_Guards._M_start=nodes;parent.seh_Guards._M_finish=nodes+n;parent.seh_Guards._M_end_of_storage._M_data=nodes+n;
  expectedHero=&hero;stage=validCalls=releaseCalls=copyCalls=copyDtors=0;released[0]=released[1]=0;
  g.AttackHero();int actual=(own&&change)?(int)newID:at;
  CHECK(stage==6&&validCalls==2&&releaseCalls==own+target);CHECK(copyCalls==1&&copyDtors==infoPresent);CHECK(thingInfo.RefCount==7);
  if(own)CHECK(released[0]==&g.MyEntityHandle&&ownInfo.RefCount==2&&!g.MyEntityHandle.PImp.Data&&!g.MyEntityHandle.PImp.Info);else CHECK(ownInfo.RefCount==3);
  if(target)CHECK(released[own]==&nodes[actual]);
  for(int j=0;j<n;++j){CHECK(nodeInfos[j].RefCount==(target&&j==actual?2UL:3UL));CHECK(nodes[j].PImp.Data==(target&&j!=actual?&impl:0));}
  CHECK(g.ID==(unsigned long)actual);++cases;
 }
 CHECK(cases==576);puts("00eb3ba0_TEST PASS");return 0;
}
