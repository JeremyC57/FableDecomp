// CAIStateGroup_KidReactToBlockage::IsRunnable
// Flags:
// Leaf: ?IsRunnable@CAIStateGroup_KidReactToBlockage@@UAE_NXZ
// Retail: 0x00941fc0, 578 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_KidReactToBlockage.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CIsVeryCloseMotionlessCreature.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
CIsVeryCloseMotionlessCreature::CIsVeryCloseMotionlessCreature(CThingAICreature& creature):MyPos(creature.GetPos()),MyRadius(creature.GetPhysicsTC()->GetRadius()){}
bool CAIStateGroup_KidReactToBlockage::IsRunnable(){
 if(PeekWorld().GetFrame()-LastEndState<CurrentActivationMinInterval)return false;
 CTCCreatureModeManager* tc_mode_manager;
 if(GetCreature().GetTC(&tc_mode_manager)){
  if(tc_mode_manager->IsModeActive(0x28)||tc_mode_manager->IsModeActive(8)||tc_mode_manager->IsModeActive(9)||tc_mode_manager->IsModeActive(10)||tc_mode_manager->IsModeActive(11)||tc_mode_manager->IsModeActive(0x2d)||tc_mode_manager->IsModeActive(0x27))return false;
 }
 const CTCPhysicsBase* ptc_physics;
 GetCreature().PeekTC(&ptc_physics);
 if(!(ptc_physics->GetPosition()-ptc_physics->GetLastPosition()).IsNonZero()){
  const C3DVector& creature_pos=GetCreature().PeekPos();
  our_radius=GetCreature().GetPhysicsTC()->GetRadius();
  C2DBoxI area(creature_pos.ToC2DCoordI(),(long)(our_radius*10.0f));
  CThingCreatureBase* pcreature;
  if(PeekThingSearchTools().GetPBestThingInRoughArea(area,GFFilter_Bind2(CThingFilter_IsNot(GetCreature()),CIsVeryCloseMotionlessCreature(GetCreature())),CThingCompare_Nearest(GetCreature().PeekPos()),&pcreature)){
   PThingCreature=pcreature;
   their_radius=PThingCreature->GetPhysicsTC()->GetRadius();
   return true;
  }
 }
 return false;
}