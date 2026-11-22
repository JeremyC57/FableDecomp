// CNavigatorAStar::MoveTowardNextWayPoint -- steer and optionally improve the first route leg.
// Leaf: ?MoveTowardNextWayPoint@CNavigatorAStar@@AAEXABVC3DVector@@MMAAV2@_N222@Z
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DECL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_MAGNITUDE_GFFASTSQRT_PASS26
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_DIVASSIGN_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C3DVECTOR_ADD2D_DIRECT
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVIGATORBASE_HEIGHTTHEMES
#define FABLE_RETAIL_CNAVIGATORBASE_FINDPOSZ_PASS26
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS
#define FABLE_RETAIL_CSTEERINGAGENT_MOVETOWARD_PASS26
#define FABLE_RETAIL_CNAVIGATORASTAR_MOVETOWARD_PASS26
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CNavigatorAStar.h"
// Original local-data record at FableWin section4+17f8c4; mutable data in the anonymous namespace.
namespace {extern C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM;}
void CNavigatorAStar::MoveTowardNextWayPoint(const C3DVector& start_pos,float speed,float proximity,C3DVector& direction,bool stop_within_nav_line_of_sight,bool avoid_dynamic_obstacles,bool avoid_static_obstacles,bool reduce_speed_when_near)
{
 SteeringAgent->SetCurrentMaxSpeed(!stop_within_nav_line_of_sight&&reduce_speed_when_near?GetSpeedReducedForStartAndEnd(speed,proximity,C2DVector(start_pos)):speed);
 SteeringAgent->SetAvoidDynamicObstacles(avoid_dynamic_obstacles);
 SteeringAgent->Process();
 C2DVector dir=SteeringAgent->GetSteer();
 direction.X=dir.X;direction.Y=dir.Y;
 direction.Z=FindPosZAt(start_pos+direction+ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM,true)-start_pos.Z;
 if(SteeringAgent->GetRecalcWidth())CalculateNavigableWidthOfLine(start_pos);
 if(SteeringAgent->GetAddWaypoint()) {
  CNavWayPoint current_front=Route.front();
  C2DVector to_next=current_front.Position-C2DVector(start_pos);
  if(to_next.GetMagnitude()>SteeringAgent->GetRadius(false)*8.0f) {
   to_next/=2.0f;
   Route.push_front(CNavWayPoint(C2DVector(start_pos+to_next),current_front.NavLayer,true));
   float new_best_left,new_best_right;
   CalculateNavigableWidthOfLine(CNavWayPoint(start_pos,current_front.NavLayer,true),Route.front(),new_best_left,new_best_right,false);
   if(new_best_left>SteeringAgent->GetBestDistLeft()&&new_best_right>SteeringAgent->GetBestDistRight()) {
    CalculateNavigableWidthOfLine(start_pos);
    SteeringAgent->SetGoal(Route.front().Position,Route.size()>1?(++Route.begin())->Position:Route.front().Position);
   } else Route.pop_front();
  }
 }
}