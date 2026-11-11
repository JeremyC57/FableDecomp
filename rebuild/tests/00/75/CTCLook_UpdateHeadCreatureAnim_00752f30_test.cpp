#include <stdio.h>
#include <string.h>
#include <limits.h>
class CCharString{};
enum ECreatureAnim { CREATURE_ANIM_TEST_ZERO=0 };
enum EAnimUpdateMode { ANIM_UPDATE_TEST_ZERO=0 };
static int failed,line_failed;
#define CHECK(x) do{if(!(x)){failed=1;line_failed=__LINE__;return;}}while(0)
static char trace[32];static int trace_n,init_calls,query_calls,add_calls,compute_calls,set_calls;
static void note(char c){trace[trace_n++]=c;trace[trace_n]=0;}
class CAnimFrameInfo {public:float Current,Last;CAnimFrameInfo():Current(0),Last(0){} CAnimFrameInfo(float f):Current(f),Last(f){} CAnimFrameInfo(float c,float l):Current(c),Last(l){} };
class CAnimInit {public:
 EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;
 CAnimInit(unsigned char type,unsigned long type_flags,long anim,EAnimUpdateMode mode,bool looping,unsigned long flags,long priority){Init(type,type_flags,anim,mode,looping,flags,priority);}
 void Init(unsigned char type,unsigned long type_flags,long anim,EAnimUpdateMode mode,bool looping,unsigned long flags,long priority){
 ++init_calls;note('I');if(Frame.Current!=0||Frame.Last!=0)failed=1;
 Type=type;TypeFlags=type_flags;Anim=anim;UpdateMode=mode;Looping=looping;Flags=flags;Priority=priority;Speed=2;Alpha=1;LastAlpha=0;
 }
};
class CAnimSet {public:float current,last;
 CAnimFrameInfo GetFrame()const{return CAnimFrameInfo(current,last);}
 void SetFrame(const CAnimFrameInfo& frame){++set_calls;note('S');current=frame.Current;last=frame.Last;}
};
namespace NAnimationEntryBodge {class CAnimEntryLite {public:long BankIndex,DelayTime,TransitionTime,AnimFlags;};}
template<class T>class CCountedPointer{public:T*Data;T*Get()const{return Data;}T*operator->()const{return Data;}};
class CAnimateThing {public:
 CAnimSet state;bool first,second;long expected_type,expected_bank;
 CAnimSet*GetFirstCombinationAnimFromType(long type){note('Q');if(type!=expected_type)failed=1;bool present=query_calls++==0?first:second;return present?&state:0;}
 void AddCombinationAnim(const CAnimInit& init,long a,long b,bool c,bool d){
 ++add_calls;note('A');if(a||b||c||d||init.Type!=expected_type||init.Anim!=expected_bank||init.UpdateMode!=0||init.Looping||init.Flags||init.TypeFlags||init.Priority!=100||init.Frame.Current!=0.5f||init.Frame.Last!=0.5f||init.Speed!=2||init.Alpha!=1||init.LastAlpha!=0)failed=1;
 }
};
class CTCLook {public:
 CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>PHeadTurnAnimH,PHeadTurnAnimV;
 float HeadAngleRelToBodyXY,HeadAngleRelToBodyYZ,HeadVelocityXY,HeadVelocityYZ;
 CAnimateThing* animator;long expected_type;float result;bool mutate;
 float GetHeadCreatureAnimFrame(ECreatureAnim type)const{++compute_calls;note('F');if(type!=expected_type)failed=1;if(mutate){animator->state.current=-64;animator->state.last=-128;}return result;}
 void UpdateHeadCreatureAnim(const CCharString&,ECreatureAnim,CAnimateThing*);
};
void CTCLook::UpdateHeadCreatureAnim(const CCharString& anim_name,ECreatureAnim creature_anim,CAnimateThing* p_anims)
{
 long bank_index=-1;
 if(creature_anim==11 && PHeadTurnAnimH.Get())bank_index=PHeadTurnAnimH->BankIndex;
 else if(creature_anim==12 && PHeadTurnAnimV.Get())bank_index=PHeadTurnAnimV->BankIndex;
 if(bank_index!=-1) {
  CAnimSet* p_anim_state=p_anims->GetFirstCombinationAnimFromType(creature_anim);
  if(!p_anim_state) {
   CAnimInit anim_init((unsigned char)creature_anim,0,bank_index,(EAnimUpdateMode)0,false,0,100);
   anim_init.Frame=CAnimFrameInfo(0.5f);
   p_anims->AddCombinationAnim(anim_init,0,0,false,false);
  }
  p_anim_state=p_anims->GetFirstCombinationAnimFromType(creature_anim);
  if(p_anim_state) {
   CAnimFrameInfo frame;
   frame.Last=p_anim_state->GetFrame().Current;
   frame.Current=GetHeadCreatureAnimFrame(creature_anim);
   p_anim_state->SetFrame(frame);
  } else {
   HeadAngleRelToBodyXY=0.0f;HeadAngleRelToBodyYZ=0.0f;
   HeadVelocityXY=0.0f;HeadVelocityYZ=0.0f;
  }
 }
}

static void one(int type,int ph,int pv,long bank,int q,float old,float next,int mutation)
{
 failed=0;trace_n=0;trace[0]=0;init_calls=query_calls=add_calls=compute_calls=set_calls=0;
 NAnimationEntryBodge::CAnimEntryLite h,v;h.BankIndex=bank;v.BankIndex=bank;h.DelayTime=v.DelayTime=3;h.TransitionTime=v.TransitionTime=4;h.AnimFlags=v.AnimFlags=5;
 CAnimateThing anim;anim.first=(q&1)!=0;anim.second=(q&2)!=0;anim.expected_type=type;anim.expected_bank=bank;anim.state.current=old;anim.state.last=-16;
 CTCLook look;look.PHeadTurnAnimH.Data=ph?&h:0;look.PHeadTurnAnimV.Data=pv?&v:0;look.HeadAngleRelToBodyXY=1;look.HeadAngleRelToBodyYZ=2;look.HeadVelocityXY=4;look.HeadVelocityYZ=8;look.animator=&anim;look.expected_type=type;look.result=next;look.mutate=mutation!=0;
 CCharString name;look.UpdateHeadCreatureAnim(name,(ECreatureAnim)type,&anim);
 CHECK(!failed);
 bool active=bank!=-1&&((type==11&&ph)||(type==12&&pv));
 bool present=active&&anim.second;
 CHECK(query_calls==(active?2:0));CHECK(add_calls==(active&&!anim.first?1:0));CHECK(init_calls==add_calls);CHECK(compute_calls==(present?1:0));CHECK(set_calls==compute_calls);
 CHECK(anim.state.current==(present?next:old));CHECK(anim.state.last==(present?old:-16));
 CHECK(look.HeadAngleRelToBodyXY==(active&&!present?0:1));CHECK(look.HeadAngleRelToBodyYZ==(active&&!present?0:2));CHECK(look.HeadVelocityXY==(active&&!present?0:4));CHECK(look.HeadVelocityYZ==(active&&!present?0:8));
 const char* expected=!active?"":anim.first?(present?"QQFS":"QQ"):(present?"QIAQFS":"QIAQ");CHECK(strcmp(trace,expected)==0);
 CHECK(look.PHeadTurnAnimH.Data==(ph?&h:0));CHECK(look.PHeadTurnAnimV.Data==(pv?&v:0));CHECK(h.BankIndex==bank&&v.BankIndex==bank);CHECK(h.DelayTime==3&&v.DelayTime==3&&h.TransitionTime==4&&v.TransitionTime==4&&h.AnimFlags==5&&v.AnimFlags==5);
}
int main(){int count=0;int types[]={-1,0,10,11,12,13};long banks[]={-1,0,7,2147483647L};float values[]={0,0.25f,-2,128};
 for(int t=0;t<6;++t)for(int ph=0;ph<2;++ph)for(int pv=0;pv<2;++pv)for(int b=0;b<4;++b)for(int q=0;q<4;++q)for(int o=0;o<4;++o)for(int n=0;n<4;++n)for(int m=0;m<2;++m){one(types[t],ph,pv,banks[b],q,values[o],values[n],m);if(failed){printf("00752f30_TEST FAIL line%d case%d\n",line_failed,count);return 1;}++count;}
 if(count!=12288)return 2;
 puts("00752f30_TEST PASS");return 0;}