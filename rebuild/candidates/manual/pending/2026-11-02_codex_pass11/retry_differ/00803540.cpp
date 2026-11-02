// CTCTargetingPlayer::UpdateTargetedPositionRandomWalk(void) -- update the player targeting position with its random walk.
// Leaf: ?UpdateTargetedPositionRandomWalk@CTCTargetingPlayer@@IAEXXZ
// Grade: DIFFER(1108v1108) -- adjacent 'lea eax,[esi+9Ch]' and 'fadd st(1)' in the opposite order at +0x350/+0x356 (scheduler tie); 15 shapes (member reference/pointer placement, grouped scalars, named sum/second-order locals, statement fusion, per-component walk, scale local, __forceinline C3DVector per owner rule, /EHsc) leave or worsen it. Graded in Codex's header context with C3DVector __forceinline (FABLE_RETAIL_TYPED_GETTC / FABLE_RETAIL_TARGET_RANDOM_WALK per-feature macros = header debt)
// Flags: /Gr
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_TARGET_RANDOM_WALK
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CHEROCOMBATDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_FORCE_GETTC
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAMERA_TRANSITIONS
#define FABLE_RETAIL_VECTOR_SPLINE
#define FABLE_RETAIL_C2DVECTOR_MASS
#include <math.h>
#include "engine/CThingCreatureBase.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CCamera.h"
#include "engine/CGameCameraManager.h"
#include "engine/CWorld.h"
#pragma intrinsic(sqrt,sin,cos,fabs)
extern const float DELTA_NOTIONAL_ZERO;
extern const double TWO_PI;
float __fastcall GFFloatRandom(float,unsigned long*);
void CTCTargetingPlayer::UpdateTargetedPositionRandomWalk()
{
 CTCHeroRangedAttacks* ptc_ranged;
 if(ParentThing.GetTC(&ptc_ranged)){
 float build_up=ptc_ranged->GetPullBackAmount();
 float accuracy=ptc_ranged->GetShotAccuracyFraction();
 float offset_scale=build_up;
 const CGameCameraManager* pcam_man;
 GetWorld().PeekGameCamera()->GetCameraManager(&pcam_man);
 C3DVector line_to_target_pos=TargetedPos-pcam_man->GetCamera().PeekWorldPos();
 float dist_to_target=line_to_target_pos.GetMagnitude();
 if(dist_to_target<DELTA_NOTIONAL_ZERO)return;
 line_to_target_pos/=dist_to_target;
 dist_to_target*=1.0f/7.0f;
 float offset_scale_second_order=dist_to_target*0.1f;
 const CTCHeroExperience* ptc_exp;
 if(ParentThing.GetTypeFlags()&8){
  CThingCreatureBase& creature=static_cast<CThingCreatureBase&>(ParentThing);
  if(creature.PeekTC(&ptc_exp)){
   if(!creature.GetPHeroCombatDef().IsNull()){
    const CHeroCombatDef& hero_combat_def=*creature.GetPHeroCombatDef();
    float weight=hero_combat_def.RangedAccuracyShakeyCursorWeighting.GetParameter(*ptc_exp);
    offset_scale=((hero_combat_def.RangedAccuracyShakeyCursor.HighValue-hero_combat_def.RangedAccuracyShakeyCursor.LowValue)*weight+hero_combat_def.RangedAccuracyShakeyCursor.LowValue)*build_up;
    offset_scale_second_order=dist_to_target*hero_combat_def.RangedAccuracyShakeFractionOfRadius;
   }
  }
 }
 C3DVector x_axis(0.0f,0.0f,0.0f);
 if(fabs(line_to_target_pos.X)>fabs(line_to_target_pos.Y)){x_axis.X=-1.0f*(line_to_target_pos.Y/line_to_target_pos.X);x_axis.Y=1.0f;}
 else{x_axis.X=1.0f;x_axis.Y=-1.0f*(line_to_target_pos.X/line_to_target_pos.Y);}
 x_axis.AccurateNormalise();
 C3DVector y_axis=x_axis.Cross(line_to_target_pos);y_axis.AccurateNormalise();
 float angle=GFFloatRandom(1.0f,GetWorld().GetWorldSeed());
 float random_walk_mag=GFFloatRandom(offset_scale_second_order,GetWorld().GetWorldSeed());
 angle=(float)(angle*TWO_PI);
 TargetedPosOffsetSecondOrder=C2DVector((float)sin(angle),(float)cos(angle));
 TargetedPosOffset+=DeltaTargetedPosOffset;
 TargetedPosOffset3D=(x_axis*TargetedPosOffset.X+y_axis*TargetedPosOffset.Y)*dist_to_target*build_up*0.0f;
 TargetedPosOffset3D+=y_axis*TargetedPosOffsetSecondOrder.Y*random_walk_mag*offset_scale;
 }
}
