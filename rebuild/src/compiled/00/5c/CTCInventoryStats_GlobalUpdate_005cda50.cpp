// CTCInventoryStats::GlobalUpdate -- update the mannequin default animation.
// Leaf: ?GlobalUpdate@CTCInventoryStats@@UAEX_N@Z
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_GETANIMATION_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_CTHING
// Inline-budget: retail expands HasTC while calling bitset::test at 005CDAED.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_GLOBALUPDATE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_MAINANIMINDEX_PASS26
#define FABLE_RETAIL_CANIMSET_FRAME
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "stlport/fable_containers.h"
// STLport test<274> at 00410DE0 retains its speed-mode exception path.
template<> inline bool std::bitset<274>::test(size_t n) const
{
    if (n >= 274)
        std::__stl_throw_out_of_range("bitset");
    return _Unchecked_test(n);
}
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThing.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCInventoryStats.h"
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s",on)
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDefClassBase_GetSubDef.h"
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