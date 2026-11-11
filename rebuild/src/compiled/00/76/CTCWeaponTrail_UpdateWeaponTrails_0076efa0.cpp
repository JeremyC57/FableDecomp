// CTCWeaponTrail::UpdateWeaponTrails -- extend active trails and discard expired points.
// Retail:0076efa0,1312 bytes,thiscall; assigned1888 includes following helpers.
// Leaf: ?UpdateWeaponTrails@CTCWeaponTrail@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CTCTrapBase.h"
#include "engine/CTCCarryable.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ThingAlive.h"
namespace {
extern float TRAIL_EXISTENCE_TIME_SECONDS;
extern long NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;
extern long MAX_PROJECTILE_TRAIL_SIZE;
std::pair<C3DVector,C3DVector> FABLE_FASTCALL GetProjectileTrailElementFromPoint(const C3DVector&);
}
void CTCWeaponTrail::UpdateWeaponTrails()
{
 if(NoTrails==-1){InitialiseTrails();return;}
 float time=((float)PeekWorld().GetFrame())/CMainGameComponent::GetConstantFPS();
 const NThingShot::CTCShotDefinitionBase* ptc_shot_defn;
 if(GetParentThing().GetTC(&ptc_shot_defn)) {
  NGraphicAppearance::CTCGraphicAppearance* ptc_graph=GetParentThing().GetTCGraphicAppearance();
  CDefPointer<const CWeaponDef> pweapon_def;
  if(GetParentThing().PeekBaseDef().GetSubDef(&pweapon_def)) {
   bool extended_trails=false;
   if(pweapon_def->UseExtendedProjectileTrails) {
    float base_interp=1.0f;
    C3DVector pos_a;
    bool got_a=ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameTip,0,0,pos_a,base_interp);
    C3DVector pos_b;
    bool got_b=ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameBase,0,0,pos_b,base_interp);
    if(got_a&&got_b)ProjectileWeaponTrail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(pos_a,pos_b,time));
   } else {
    if(ProjectileWeaponTrail.empty()) {
     float base_interp=0.0f;C3DVector pos_b;
     if(ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameBase,0,0,pos_b,base_interp)) {
      std::pair<C3DVector,C3DVector> element=GetProjectileTrailElementFromPoint(pos_b);
      ProjectileWeaponTrail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(element.first,element.second,time));
     }
    }
    float tip_interp=1.0f;C3DVector pos_a;
    if(ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameTip,0,0,pos_a,tip_interp)) {
     std::pair<C3DVector,C3DVector> element=GetProjectileTrailElementFromPoint(pos_a);
     float time_fraction=1.0f/CMainGameComponent::GetConstantFPS();
     ProjectileWeaponTrail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(element.first,element.second,time+time_fraction));
    }
   }
  }
  if(ProjectileWeaponTrail.size()>MAX_PROJECTILE_TRAIL_SIZE)ProjectileWeaponTrail.pop_front();
  return;
 }
 if(!ProjectileWeaponTrail.empty()) {
  if(MinProjectileTrailTimer==-1)MinProjectileTrailTimer=15;
  if(MinProjectileTrailTimer--==0)ProjectileWeaponTrail.clear();
 }
 const CThingCreatureBase* pcreature_actioneer=0;
 const CThing* ptrap=0;
 bool quit_out=true;
 const CTCTrapBase* ptc_trap;
 const CTCCarryable* ptc_carryable;
 if(GetParentThing().PeekTC(&ptc_trap)&&ptc_trap->IsOpen())ptrap=&GetParentThing();
 else if(GetParentThing().GetTypeFlags()&8)pcreature_actioneer=GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
 else if(GetParentThing().PeekTC(&ptc_carryable)) {
  if(IS_THING_ALIVE(ptc_carryable->GetPCarriedByThing())&&(ptc_carryable->GetPCarriedByThing()->GetTypeFlags()&8))
   pcreature_actioneer=GFDebugPDynamicCast(ptc_carryable->GetPCarriedByThing(),(const CThingCreatureBase*)0);
 }
 if(IS_THING_ALIVE(pcreature_actioneer)&&pcreature_actioneer->IsPerformingAnyAction()) {
  const CCreatureActionBase* paction=pcreature_actioneer->PeekPCurrentAction();
  if(!paction->IsStrikeEventActive())pcreature_actioneer=0;
 }else pcreature_actioneer=0;
 if(ptrap||pcreature_actioneer)WeaponTrailActive=true;
 if(!WeaponTrailActive)return;
 bool active_elements=false;
 C3DVector top_pos,bottom_pos;
 long c1=0;
 if(c1<NoTrails) {
 for(;c1<NoTrails;++c1) {
  bool active=IsTrailActiveForThing(c1,ptrap,pcreature_actioneer);
  std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>& current_trail=Trail[c1];
  const CGlobalDef& glob_def=GetDefinitionManager().PeekGlobalDef();
  if(!active)ActiveTrail[c1] ? InactiveTicks[c1]=0 : InactiveTicks[c1]++;
  if(active||InactiveTicks[c1]<glob_def.WeaponTrialNumFramesToFadeOut) {
   if(!ActiveTrail[c1]&&active)current_trail.clear();
   float fraction=0.0f;
   float time_fraction=1.0f/CMainGameComponent::GetConstantFPS();
   for(long c2=0;c2<NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;++c2) {
    fraction+=1.0f/NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;
    CalculateInterpolatedWeaponHelpers(c1,fraction,top_pos,bottom_pos);
    current_trail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(top_pos,bottom_pos,time+time_fraction*fraction));
   }
  }
  ActiveTrail[c1]=active;
  while(!current_trail.empty()&&time-current_trail.front().CreationTime>TRAIL_EXISTENCE_TIME_SECONDS)current_trail.pop_front();
  if(!current_trail.empty())active_elements=true;
 }
 }
 if(!active_elements)WeaponTrailActive=false;
}