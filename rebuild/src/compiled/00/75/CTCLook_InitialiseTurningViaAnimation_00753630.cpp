#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
// Original named locals and actual GetResult on for_each temporary, no fitted snapshots.
// Leaf: ?InitialiseTurningViaAnimation@CTCLook@@AAEXABVC3DVector@@J@Z
// Flags: (none)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
// The original debug assertion appears at tc_look.cpp:569 in FableWin and Ego_d.
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_THING_TYPE_IDS
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY
#include "engine/CAnimInit.h"
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES
#include "engine/CTCLook.h"
#include "engine/CGetBestTurningAnim.h"
#include "engine/CAnimationEntry.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimateThing.h"
#include "engine/CThingManager.h"
#include "fable_assert.h"
#include "engine/GFCos.h"
#include "stlport/fable_algorithm.h"
void CTCLook::InitialiseTurningViaAnimation(const C3DVector& look_at,long look_flags)
{
 FABLE_RETAIL_ASSERT(GetParentThing().GetType() != THING_TYPE_PLAYER_CREATURE);
 CTCPhysicsBase* ptc_phys=GetParentThing().GetPhysicsTC();
 C3DVector facing_dir=ptc_phys->GetFacingVector();
 C3DVector look_vector=look_at-ptc_phys->GetPosition();
 facing_dir.Z=look_vector.Z=0;
 if(look_vector.IsNonZero()) {
  look_vector.AccurateNormalise();facing_dir.AccurateNormalise();
  float fraction_of_movement_to_accept_each_frame=1;
  bool old_style_movement=false;
  if(look_vector.Dot(facing_dir)>GFCos(TurningViaAnimMinAngle/2.0f))old_style_movement=true;
  else {
   const CAnimationEntry* panim_entry;
   std::for_each(TurningAnims.begin(),TurningAnims.end(),CGetBestTurningAnim(facing_dir,look_vector)).GetResult(&panim_entry,fraction_of_movement_to_accept_each_frame);
   if(fraction_of_movement_to_accept_each_frame>1){if(!PDef->AllowOversteer)fraction_of_movement_to_accept_each_frame=1;}
   else if(fraction_of_movement_to_accept_each_frame<1){if(!PDef->AllowUndersteer)fraction_of_movement_to_accept_each_frame=1;}
   CTCAnimationComplex* ptc_anim_complex;
   GetParentThing().GetTC(&ptc_anim_complex);
   CAnimateThing& anim=ptc_anim_complex->GetAnimation();
   anim.SetMainAnim(CAnimInit(7,0,panim_entry->BankIndex,(EAnimUpdateMode)1,false,panim_entry->GetAnimateThingFlags(),100),panim_entry->GetTimeToBlendIn(GetParentThing()),panim_entry->GetDelayTime());
   PCurrentTurningAnim=panim_entry;
  }
  FractionOfTurningToAcceptEachFrame=fraction_of_movement_to_accept_each_frame;
  UsingOldStyleTurning=old_style_movement;
 }
}