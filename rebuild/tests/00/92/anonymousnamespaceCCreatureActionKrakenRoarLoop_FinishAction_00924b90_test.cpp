#include <stdio.h>
#include <string.h>
static int failed,line_failed,get_calls,lookup_calls,ctor_calls,set_calls,dtor_calls,base_calls,trace_n;
static char trace[32];static void note(char c){trace[trace_n++]=c;trace[trace_n]=0;}
#define CHECK(x) do{if(!(x)){failed=1;line_failed=__LINE__;return;}}while(0)
class CKrakenDef {public:long RoarLoops;};
class CTCKraken {public:CKrakenDef*PKrakenDef;const CKrakenDef&GetKrakenDef()const{return *PKrakenDef;}};
class CCreatureActionBase;
class CThingCreatureBase {public:CTCKraken*component;int identity;bool set_result;
 bool GetTC(CTCKraken**out){note('T');++lookup_calls;*out=component;return true;}
 bool SetCurrentAction(const CCreatureActionBase&);
};
template<class T>class CIntelligentPointer{public:T*item;T*Get()const{++get_calls;return item;}T*operator->()const{return Get();}T&operator*()const{return *Get();}};
class CCreatureActionBase {public:CIntelligentPointer<CThingCreatureBase>PCreature;int kind;long captured_count;CThingCreatureBase*captured_creature;virtual void FinishAction();};
class CCombatActionBase:public CCreatureActionBase {public:~CCombatActionBase(){++dtor_calls;note('D');}};
namespace {
class CCreatureAction_KrakenRoarLoop:public CCombatActionBase {public:
 long LoopNum;CCreatureAction_KrakenRoarLoop(CThingCreatureBase& c,long n){++ctor_calls;note('L');kind=1;captured_count=n;captured_creature=&c;}
 virtual void FinishAction();
};
class CCreatureAction_KrakenRoarOutOf:public CCombatActionBase {public:
 CCreatureAction_KrakenRoarOutOf(CThingCreatureBase&c){++ctor_calls;note('O');kind=2;captured_count=-1;captured_creature=&c;}
};
}
static bool change_loop,change_creature;
static CThingCreatureBase* alternate;
static CThingCreatureBase*selected;static long expected_next;static bool expected_repeat;
void CCreatureActionBase::FinishAction(){note('B');++base_calls;if(change_loop)static_cast<CCreatureAction_KrakenRoarLoop*>(this)->LoopNum=7;if(change_creature)PCreature.item=alternate;}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&action){++set_calls;note('S');if(this!=selected||action.captured_creature!=selected||action.kind!=(expected_repeat?1:2)||action.captured_count!=(expected_repeat?expected_next:-1))failed=1;return set_result;}
void CCreatureAction_KrakenRoarLoop::FinishAction()
{
 CCreatureActionBase::FinishAction();
 CTCKraken* pTCKraken;
 PCreature->GetTC(&pTCKraken);
 if(++LoopNum<pTCKraken->GetKrakenDef().RoarLoops)
  PCreature->SetCurrentAction(CCreatureAction_KrakenRoarLoop(*PCreature,LoopNum));
 else
  PCreature->SetCurrentAction(CCreatureAction_KrakenRoarOutOf(*PCreature));
}

static void one(long count,long limit,int modify,int replace,int returns,int seed)
{
 CKrakenDef def0,def1;def0.RoarLoops=limit;def1.RoarLoops=limit;
 CTCKraken kraken0,kraken1;kraken0.PKrakenDef=&def0;kraken1.PKrakenDef=&def1;
 CThingCreatureBase a,b;a.component=&kraken0;b.component=&kraken1;a.identity=seed;b.identity=seed+1;a.set_result=b.set_result=returns!=0;
 CCreatureAction_KrakenRoarLoop action(a,count);action.PCreature.item=&a;action.LoopNum=count;
 failed=0;trace_n=0;trace[0]=0;get_calls=lookup_calls=ctor_calls=set_calls=dtor_calls=base_calls=0;
 change_loop=modify!=0;change_creature=replace!=0;alternate=&b;selected=replace?&b:&a;expected_next=(modify?7:count)+1;expected_repeat=expected_next<limit;
 action.FinishAction();
 CHECK(!failed);CHECK(action.LoopNum==expected_next);CHECK(action.PCreature.item==selected);CHECK(base_calls==1&&lookup_calls==1&&ctor_calls==1&&set_calls==1&&dtor_calls==1&&get_calls==3);
 CHECK(strcmp(trace,expected_repeat?"BTLSD":"BTOSD")==0);CHECK(def0.RoarLoops==limit&&def1.RoarLoops==limit);CHECK(a.identity==seed&&b.identity==seed+1);CHECK(a.component==&kraken0&&b.component==&kraken1);CHECK(kraken0.PKrakenDef==&def0&&kraken1.PKrakenDef==&def1);
}
int main(){long counts[]={-2147483647L-1,-4,-1,0,1,7,16,2147483646L};long limits[]={-2147483647L-1,-5,-1,0,1,7,17,2147483647L};int cases=0;
 for(int c=0;c<8;++c)for(int l=0;l<8;++l)for(int m=0;m<2;++m)for(int p=0;p<2;++p)for(int r=0;r<2;++r)for(int s=0;s<3;++s){one(counts[c],limits[l],m,p,r,s);if(failed){printf("00924b90_TEST FAIL line%d case%d\n",line_failed,cases);return 1;}++cases;}
 if(cases!=1536)return 2;puts("00924b90_TEST PASS");return 0;}