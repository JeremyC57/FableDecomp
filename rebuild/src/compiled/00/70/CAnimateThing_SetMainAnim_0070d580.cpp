// CAnimateThing::SetMainAnim -- replace the main animation with transition blending.
// Inline-budget: Plain and S50 expand genuine list::push_back; native calls it. The scoped noinline on its complete STLport body preserves that call.
// Flags:
// Retail: 0070d580,504 bytes,thiscall with animation initializer and two frame counts.
// Leaf: ?SetMainAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ@Z
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CAnimateThing.h"
#include "stlport/fable_algorithm.h"
extern const float DELTA_NOTIONAL_ZERO;
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