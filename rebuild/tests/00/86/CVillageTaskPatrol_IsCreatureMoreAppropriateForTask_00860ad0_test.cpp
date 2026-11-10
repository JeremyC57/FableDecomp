// 00860ad0 behavioural fixture; selected function and genuine traversal copied below.
// Dependency fixtures exercise behaviour, not the production object layout.
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
class C3DVector { public:
 float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector& x):X(x.X),Y(x.Y),Z(x.Z){}
 const C3DVector operator-(const C3DVector& x)const{return C3DVector(X-x.X,Y-x.Y,Z-x.Z);}
 float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
};
inline float GFSqr(float n){return n*n;}
class CThing { public:
 C3DVector Pos; mutable int Reads;
 CThing():Pos(0,0,0),Reads(0){}
 const C3DVector& GetPos()const{++Reads;return Pos;}
};
class CThingTrackNode:public CThing{ public:
 CThingTrackNode* NextNode;bool IsEnd;
 CThingTrackNode():NextNode(0),IsEnd(false){}
 const CThingTrackNode* PeekNextNode()const{return NextNode;}
 bool IsTrackEnd()const{return IsEnd;}
};
class CVillageTaskPatrol{ public:
 const CThing* CreatureAssignedToTask;const CThingTrackNode* PatrolStart;bool TaskLocked;
 const CThingTrackNode* GetPatrolStart()const{return PatrolStart;}
 const CThingTrackNode* GetNearestNode(const C3DVector&)const;
 bool IsCreatureMoreAppropriateForTask(const CThing&)const;
};
// Original body: FableWin029bece0 and native00860a50; read-only position input.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
const CThingTrackNode* CVillageTaskPatrol::GetNearestNode(const C3DVector& to_pos)const
{
 const CThingTrackNode* ret=0;
 const CThingTrackNode* cur_node=GetPatrolStart();
 if(cur_node){
  float best_distance_sq=3.402823466e+38f;
  do{
   float cur_distance_sq=(cur_node->GetPos()-to_pos).GetSquaredMagnitude();
   if(cur_distance_sq<best_distance_sq){best_distance_sq=cur_distance_sq;ret=cur_node;}
   cur_node=cur_node->PeekNextNode();
  }while(cur_node&&!cur_node->IsTrackEnd());
 }
 return ret;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
bool CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing& creature)const
{
 if(TaskLocked)return false;
 const float FACTOR_HAVE_TO_BE_NEARER_BY=1.25f;
 C3DVector current_creature_pos=CreatureAssignedToTask->GetPos();
 C3DVector new_creature_pos=creature.GetPos();
 const CThingTrackNode* pnode=GetNearestNode(current_creature_pos);
 C3DVector current_creature_nearest_node_pos=pnode->GetPos();
 pnode=GetNearestNode(new_creature_pos);
 C3DVector new_creature_nearest_node_pos=pnode->GetPos();
 float dist_from_current_creature_to_patrol_start_sq=(current_creature_nearest_node_pos-current_creature_pos).GetSquaredMagnitude();
 float dist_from_new_creature_to_patrol_start_sq=(new_creature_nearest_node_pos-new_creature_pos).GetSquaredMagnitude();
 return (dist_from_new_creature_to_patrol_start_sq*GFSqr(FACTOR_HAVE_TO_BE_NEARER_BY))<dist_from_current_creature_to_patrol_start_sq;
}

static int squared(const C3DVector& a,const C3DVector& b){
 int x=int(a.X)-int(b.X),y=int(a.Y)-int(b.Y),z=int(a.Z)-int(b.Z);return x*x+y*y+z*z;
}
static int nearest(const CThingTrackNode* nodes,int count,const C3DVector& p){
 int value=1000000;for(int k=0;k<count;++k){int d=squared(nodes[k].Pos,p);if(d<value)value=d;}return value;
}
int main(){
 unsigned count=0;
 for(int mode=0;mode<3;++mode)for(int locked=0;locked<2;++locked)
 for(int x=-6;x<=6;++x)for(int y=-6;y<=6;++y){
  CThing assigned,candidate;CThingTrackNode nodes[3];
  assigned.Pos=C3DVector(float(x),float((x+6)%3),float((x+6)%2));
  candidate.Pos=C3DVector(float(y),float((y+6)%3),float((y+6)%2));
  nodes[0].Pos=C3DVector(0,0,0);nodes[1].Pos=C3DVector(4,2,0);nodes[2].Pos=C3DVector(-4,0,2);
  nodes[0].NextNode=mode?&nodes[1]:0;nodes[1].NextNode=&nodes[2];nodes[2].IsEnd=mode==1;
  int n=mode==0?1:(mode==1?2:3);
  CVillageTaskPatrol task;task.CreatureAssignedToTask=&assigned;task.PatrolStart=nodes;task.TaskLocked=locked!=0;
  bool want=!locked && nearest(nodes,n,candidate.Pos)*25<nearest(nodes,n,assigned.Pos)*16;
  assert(task.IsCreatureMoreAppropriateForTask(candidate)==want);
  assert(assigned.Reads==(locked?0:1));assert(candidate.Reads==(locked?0:1));
  if(locked)for(int k=0;k<3;++k)assert(nodes[k].Reads==0);
  if(mode==1)assert(nodes[2].Reads==0);
  ++count;
 }
 // Exact boundary: 4*1.25 == 5. Equality must reject; a strictly nearer point passes.
 CThing a,b;CThingTrackNode node;CVillageTaskPatrol t;
 t.CreatureAssignedToTask=&a;t.PatrolStart=&node;t.TaskLocked=false;
 a.Pos=C3DVector(5,0,0);b.Pos=C3DVector(4,0,0);assert(!t.IsCreatureMoreAppropriateForTask(b));
 b.Pos=C3DVector(3,0,0);assert(t.IsCreatureMoreAppropriateForTask(b));
 assert(count==1014);puts("00860ad0_TEST PASS");return 0;
}