#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
const float DELTA_NOTIONAL_ZERO=0.0001f;
enum EAnimUpdateMode{ANIM_UPDATE_NONE,ANIM_UPDATE_FROM_SPEED,ANIM_UPDATE_FROM_DIST,ANIM_UPDATE_PERCENTAGE_COMPLETE,ANIM_UPDATE_LOCKED_TO_MAIN};
class CAnimFrameInfo{public:float Current,Last;};
class CAnimInit{public:EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;};
class CAnimateThing;
class CTransitionBlendAlpha{public:float CurrentAlpha,LastAlpha;CTransitionBlendAlpha(float a,float b):CurrentAlpha(a),LastAlpha(b){}};
template<class T,class L>inline void __fastcall GFLimitLower(T& value,const L& lower){if(value<lower)value=lower;}
class CAnimSet{public:
 EAnimUpdateMode UpdateMode;std::vector<short> Anims;std::vector<CTransitionBlendAlpha> InternalAlphas;
 float CurrentPercentageComplete,LastPercentageComplete,Speed;unsigned long FrameInternalAlphasUpdated;
 float CurrentAlpha,LastAlpha,LastUpdateChange,ChangeAlphaBy;CAnimateThing* PAnimator;unsigned long Flags,TypeFlags;long AnimDelay,Priority;unsigned char Type;bool Looping:1,Finished:1,Forwards:1,HadUpdate:1,OwnedByMainAnim:1;
 bool IsOwnedByMainAnim()const{return OwnedByMainAnim;}
 void SetAsFinished(long frames){if(!Finished){Finished=true;GFLimitLower(frames,1);ChangeAlphaBy=CurrentAlpha/(float)frames;}}
 bool IsValid()const{unsigned int count=Anims.size();return (count==1||(count==2&&Anims[1]>0))&&Anims[0]>0&&count==InternalAlphas.size();}
 float GetChangeAlphaBy()const{return ChangeAlphaBy;}void SetChangeAlphaBy(float value){ChangeAlphaBy=value;}float GetAlpha()const{return CurrentAlpha;}
 void SetAlpha(float value){if(value<0)value=0;else if(value>1)value=1;LastAlpha=CurrentAlpha;CurrentAlpha=value;}
 void Init(CAnimateThing& animator,const CAnimInit& init,float alpha){PAnimator=&animator;Anims.clear();Anims.push_back((short)init.Anim);InternalAlphas.clear();InternalAlphas.push_back(CTransitionBlendAlpha(init.Alpha,init.Alpha));CurrentPercentageComplete=init.Frame.Current;LastPercentageComplete=init.Frame.Last;CurrentAlpha=LastAlpha=alpha;LastUpdateChange=ChangeAlphaBy=0;Type=init.Type;UpdateMode=init.UpdateMode;Looping=init.Looping;Speed=init.Speed;Finished=false;Forwards=true;Flags=init.Flags;TypeFlags=init.TypeFlags;HadUpdate=false;Priority=init.Priority;}
};
class CAnimFromFileInfo{public:bool Active;};
class CAnimateThing{public:class CCompleteAnimation{public:CAnimSet MainAnimSet;std::list<CAnimSet> TransitionAnimSets,CombinationAnims;bool MainAnimCanTransitionOut;};class CDelayedAnimation{public:std::list<CAnimSet> CombinationAnims;};CCompleteAnimation CurrentAnim;CDelayedAnimation DelayedAnim;CAnimFromFileInfo AnimFromFileInfo;float ChangeAlphaBy;long AnimDelay;unsigned long LastAnimRefreshStamp;void ClearUseAnimFromFile(){AnimFromFileInfo.Active=false;}void GetAllCombinationAnims(std::vector<CAnimSet*>&);void SetMainAnim(const CAnimInit&,long,long);};
namespace {class CMultiplyAnimSetAlphaBy:public std::unary_function<CAnimSet,void>{float MultiplyBy;public:CMultiplyAnimSetAlphaBy(float multiply_by):MultiplyBy(multiply_by){}void operator()(CAnimSet&)const;};}
void CAnimateThing::GetAllCombinationAnims(std::vector<CAnimSet*>& anims){
 for(std::list<CAnimSet>::iterator it=CurrentAnim.CombinationAnims.begin();it!=CurrentAnim.CombinationAnims.end();++it)anims.push_back(&*it);
 for(std::list<CAnimSet>::iterator it=DelayedAnim.CombinationAnims.begin();it!=DelayedAnim.CombinationAnims.end();++it)anims.push_back(&*it);
}
void CAnimateThing::SetMainAnim(const CAnimInit& init,long world_ticks_transition_time,long anim_delay){
 LastAnimRefreshStamp=0;
 CCompleteAnimation& anim=CurrentAnim;
 ClearUseAnimFromFile();
 std::vector<CAnimSet*> comb_anims;
 GetAllCombinationAnims(comb_anims);
 for(std::vector<CAnimSet*>::iterator it=comb_anims.begin();it!=comb_anims.end();++it){
  CAnimSet* panim_set=*it;
  if(panim_set->IsOwnedByMainAnim())panim_set->SetAsFinished(world_ticks_transition_time);
 }
 if(init.Anim!=0){
  if(init.Flags&1){anim.CombinationAnims.clear();DelayedAnim.CombinationAnims.clear();}
  AnimDelay=anim_delay;
  float main_alpha=0.0f;
  if(world_ticks_transition_time>0&&anim.MainAnimCanTransitionOut&&anim.MainAnimSet.IsValid()){
   ChangeAlphaBy=1.0f/world_ticks_transition_time;
   anim.TransitionAnimSets.push_back(anim.MainAnimSet);
   for(std::list<CAnimSet>::iterator trans_it=anim.TransitionAnimSets.begin();trans_it!=anim.TransitionAnimSets.end();++trans_it){CAnimSet& trans_anim=*trans_it;trans_anim.SetChangeAlphaBy(trans_anim.GetChangeAlphaBy()+ChangeAlphaBy);}
   float alpha_total=0.0f;
   for(std::list<CAnimSet>::const_iterator anim_it=anim.TransitionAnimSets.begin();anim_it!=anim.TransitionAnimSets.end();++anim_it){const CAnimSet& trans_anim=*anim_it;alpha_total+=trans_anim.GetAlpha();}
   if(alpha_total>DELTA_NOTIONAL_ZERO&&alpha_total<1.0f-DELTA_NOTIONAL_ZERO){float one_over_total=1.0f/alpha_total;std::for_each(anim.TransitionAnimSets.begin(),anim.TransitionAnimSets.end(),CMultiplyAnimSetAlphaBy(one_over_total));}
  }else{anim.TransitionAnimSets.clear();ChangeAlphaBy=0.0f;main_alpha=1.0f;}
  anim.MainAnimSet.Init(*this,init,main_alpha);
  anim.MainAnimCanTransitionOut=!(init.Flags&2);
  anim.MainAnimSet.SetChangeAlphaBy(-ChangeAlphaBy);
 }
}

void CMultiplyAnimSetAlphaBy::operator()(CAnimSet& set)const{set.SetAlpha(set.GetAlpha()*MultiplyBy);}
static CAnimSet make(float alpha,bool owned,bool finished){CAnimSet a;a.UpdateMode=ANIM_UPDATE_NONE;a.Anims.push_back(2);a.InternalAlphas.push_back(CTransitionBlendAlpha(alpha,alpha));a.CurrentPercentageComplete=a.LastPercentageComplete=a.Speed=0;a.FrameInternalAlphasUpdated=0;a.LastUpdateChange=0;a.PAnimator=0;a.Flags=a.TypeFlags=0;a.AnimDelay=a.Priority=0;a.Type=0;a.Looping=a.Forwards=a.HadUpdate=false;a.CurrentAlpha=a.LastAlpha=alpha;a.ChangeAlphaBy=.125f;a.OwnedByMainAnim=owned;a.Finished=finished;return a;}
int main(){for(int transition=-1;transition<=3;++transition)for(int can=0;can<2;++can)for(int valid=0;valid<4;++valid)for(int flags=0;flags<4;++flags)for(int exists=0;exists<2;++exists){CAnimateThing a;CAnimInit init;init.Anim=exists?7:0;init.Flags=flags;init.Alpha=.75f;init.LastAlpha=.5f;init.Frame.Current=.25f;init.Frame.Last=.125f;init.Type=1;init.TypeFlags=9;init.Speed=1;init.Looping=true;init.UpdateMode=ANIM_UPDATE_NONE;init.Priority=3;
a.CurrentAnim.MainAnimSet=make(.25f,false,false);if(valid==0)a.CurrentAnim.MainAnimSet.Anims.clear();if(valid==2)a.CurrentAnim.MainAnimSet.Anims[0]=-1;if(valid==3)a.CurrentAnim.MainAnimSet.InternalAlphas.clear();bool blend=transition>0&&can&&valid==1;
a.CurrentAnim.MainAnimCanTransitionOut=can!=0;a.CurrentAnim.TransitionAnimSets.push_back(make(.25f,false,false));a.CurrentAnim.CombinationAnims.push_back(make(.5f,true,false));a.CurrentAnim.CombinationAnims.push_back(make(.5f,false,false));a.DelayedAnim.CombinationAnims.push_back(make(.5f,true,true));a.AnimFromFileInfo.Active=true;a.LastAnimRefreshStamp=19;a.AnimDelay=99;a.ChangeAlphaBy=.375f;
a.SetMainAnim(init,transition,11);assert(!a.AnimFromFileInfo.Active&&a.LastAnimRefreshStamp==0);
if(exists){assert(a.AnimDelay==11&&a.CurrentAnim.MainAnimCanTransitionOut==!(flags&2));assert(a.CurrentAnim.MainAnimSet.Anims.size()==1&&a.CurrentAnim.MainAnimSet.Anims[0]==7);assert(fabs(a.CurrentAnim.MainAnimSet.CurrentAlpha-(blend?0:1))<.00001);assert(fabs(a.ChangeAlphaBy-(blend?1.0f/transition:0))<.00001);assert(a.CurrentAnim.TransitionAnimSets.size()==(blend?2:0));if(blend){float total=0;for(std::list<CAnimSet>::iterator it=a.CurrentAnim.TransitionAnimSets.begin();it!=a.CurrentAnim.TransitionAnimSets.end();++it)total+=it->CurrentAlpha;assert(fabs(total-1)<.00001);}}
if(exists&&(flags&1)){assert(a.CurrentAnim.CombinationAnims.empty()&&a.DelayedAnim.CombinationAnims.empty());}else{std::list<CAnimSet>::iterator it=a.CurrentAnim.CombinationAnims.begin();assert(it->Finished&&fabs(it->ChangeAlphaBy-.5f/(transition<1?1:transition))<.00001);++it;assert(!it->Finished&&it->ChangeAlphaBy==.125f);assert(a.DelayedAnim.CombinationAnims.front().ChangeAlphaBy==.125f);}
}puts("0070d580_TEST PASS");return 0;}