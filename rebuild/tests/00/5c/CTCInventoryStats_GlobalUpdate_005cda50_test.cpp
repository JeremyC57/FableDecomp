#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_CANIMINIT_CTORS
#include "rebuild_abi.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThingUpdate.h"
static int base_calls,subdef_calls,lookup_calls,tc_calls,init_calls,set_calls,update_calls,ref_count,release_calls;
static bool last_paused;
void CAnimInit::Init(unsigned char type,unsigned long typeFlags,long anim,EAnimUpdateMode mode,bool looping,unsigned long flags,long priority) {
 assert(Frame.Current==0&&Frame.Last==0);++init_calls;
 Type=type;TypeFlags=typeFlags;Anim=anim;UpdateMode=mode;Looping=looping;Flags=flags;Priority=priority;
}
struct CCharString {const char* text;CCharString(const char*s):text(s){}};
struct CAnimFilterNoGroup {};
struct CAnimationEntry {long BankIndex;unsigned long flags;long transition;unsigned long GetAnimateThingFlags()const{return flags;}long GetTransitionTime()const{return transition;}};
static CAnimationEntry entry;
struct CAnimationSet {const CAnimationEntry&GetAnim(const CCharString&name,long n,const CAnimFilterNoGroup&)const {assert(!strcmp(name.text,"DEFAULT")&&n==0);++lookup_calls;return entry;}};
struct CAppearanceDef {CAnimationSet Animation;};
static CAppearanceDef appearance;
template<class T> struct CDefPointer {
 T*Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer&b):Object(b.Object){if(Object)++ref_count;}
 ~CDefPointer(){if(Object){++release_calls;--ref_count;}}
 T*operator->()const{return Object;}
};
struct CThingBaseDef {template<class T>bool GetSubDef(CDefPointer<const T>*p)const{++subdef_calls;p->Object=&appearance;++ref_count;return true;}};
static CThingBaseDef thing_def;
struct CAnimSet {float CurrentPercentageComplete,LastPercentageComplete;CAnimFrameInfo GetFrame()const{return CAnimFrameInfo(CurrentPercentageComplete,LastPercentageComplete);}};
struct CAnimateThing {
 long index;CAnimSet main_anim;
 long GetMainAnimIndex()const{return index;}CAnimSet&GetMainAnim(){return main_anim;}
 void SetMainAnim(const CAnimInit&i,long transition,long flags){
  ++set_calls;assert(i.Type==6&&i.TypeFlags==0&&i.Anim==entry.BankIndex&&i.UpdateMode==ANIM_UPDATE_FROM_SPEED&&i.Looping&&i.Flags==entry.flags&&i.Priority==100);
  assert(i.Frame.Current==main_anim.CurrentPercentageComplete&&i.Frame.Last==main_anim.LastPercentageComplete);
  assert(transition==entry.transition&&flags==0);index=i.Anim;
 }
 void Update(const CAnimateThingUpdate&u);
};
static CAnimateThing animation;
struct CTCAnimationComplex {CAnimateThing&GetAnimation(){return animation;}};
static CTCAnimationComplex animation_tc;
struct CThing {CDefPointer<const CThingBaseDef>GetPBaseDef()const{CDefPointer<const CThingBaseDef>p;p.Object=&thing_def;++ref_count;return p;}bool GetTC(CTCAnimationComplex**p){++tc_calls;*p=&animation_tc;return true;}};
struct CMainGameComponent {static long fps;static long GetConstantFPS(){return fps;}};long CMainGameComponent::fps=30;
void CAnimateThing::Update(const CAnimateThingUpdate&u){++update_calls;assert(fabs(u.TimePassed-1.0f/CMainGameComponent::fps)<1e-7&&u.DistanceMoved==0&&u.PercentageComplete==0);}
namespace NInventory {struct CTCInventoryBase {bool open;void GlobalUpdate(bool paused){++base_calls;last_paused=paused;}bool IsInventoryOpen()const{return open;}};}
struct CTCInventoryStats:NInventory::CTCInventoryBase {CThing*pMannequin;void GlobalUpdate(bool paused);};

void CTCInventoryStats::GlobalUpdate(bool paused)
{
 NInventory::CTCInventoryBase::GlobalUpdate(paused);
 if(IsInventoryOpen()) {
  CDefPointer<const CAppearanceDef> pAppearanceDef;
  if(pMannequin) {
   pMannequin->GetPBaseDef()->GetSubDef(&pAppearanceDef);
   const CAnimationEntry& default_anim_entry=pAppearanceDef->Animation.GetAnim("DEFAULT",0,CAnimFilterNoGroup());
   const CAnimationEntry* pneeded_anim=&default_anim_entry;
   CTCAnimationComplex* ptc_animation;
   pMannequin->GetTC(&ptc_animation);
   CAnimateThing& animate_thing=ptc_animation->GetAnimation();
   if(animate_thing.GetMainAnimIndex()!=pneeded_anim->BankIndex) {
    CAnimInit new_anim_init(6,0,pneeded_anim->BankIndex,ANIM_UPDATE_FROM_SPEED,true,pneeded_anim->GetAnimateThingFlags(),100);
    new_anim_init.Frame=animate_thing.GetMainAnim().GetFrame();
    animate_thing.SetMainAnim(new_anim_init,pneeded_anim->GetTransitionTime(),0);
   }
   float update_time=1.0f/CMainGameComponent::GetConstantFPS();
   CAnimateThingUpdate anim_update(update_time,0.0f);
   animate_thing.Update(anim_update);
  }
 }
}
int main(){
 CThing mannequin;
 for(int open=0;open<2;++open)for(int have=0;have<2;++have)for(int same=0;same<2;++same)for(int paused=0;paused<2;++paused){
  for(int rate=0;rate<3;++rate){
   base_calls=subdef_calls=lookup_calls=tc_calls=init_calls=set_calls=update_calls=ref_count=release_calls=0;
   entry.BankIndex=17;entry.flags=0x815;entry.transition=-3;animation.index=same?17:9;
   animation.main_anim.CurrentPercentageComplete=0.75f;animation.main_anim.LastPercentageComplete=0.375f;
   CMainGameComponent::fps=rate==0?1:(rate==1?30:120);
   CTCInventoryStats stats;stats.open=open!=0;stats.pMannequin=have?&mannequin:0;stats.GlobalUpdate(paused!=0);
   int active=open&&have;
   assert(base_calls==1&&last_paused==(paused!=0)&&stats.pMannequin==(have?&mannequin:0));
   assert(subdef_calls==active&&lookup_calls==active&&tc_calls==active&&update_calls==active);
   assert(init_calls==(active&&!same)&&set_calls==(active&&!same));
   assert(ref_count==0&&release_calls>=2*active);
   assert(animation.index==(active?17:(same?17:9)));
   assert(animation.main_anim.CurrentPercentageComplete==0.75f&&animation.main_anim.LastPercentageComplete==0.375f);
  }
 }
 puts("005cda50_TEST PASS");return 0;
}