#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
struct C3DVector;struct C2DVector{float X,Y;C2DVector():X(0),Y(0){}C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C3DVector&);C2DVector operator-(const C2DVector&o)const{return C2DVector(X-o.X,Y-o.Y);}float GetMagnitude()const{return(float)sqrt(X*X+Y*Y);}const C2DVector&operator/=(float scale){const float oo_scale=1.0f/scale;X*=oo_scale;Y*=oo_scale;return *this;}};
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}const C3DVector operator+(const C3DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}C3DVector operator+(const C2DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z);}};C2DVector::C2DVector(const C3DVector&v):X(v.X),Y(v.Y){}
struct CNavWayPoint{C2DVector Position;unsigned char NavLayer;bool Removable;CNavWayPoint(const C2DVector&p,unsigned char layer,bool removable):Position(p),NavLayer(layer),Removable(removable){}};
struct CSteeringAgent{float speed,left,right,radius;bool avoid,recalc,add;int processes,goals;C2DVector steer,goal,next;CSteeringAgent():speed(0),left(1),right(1),radius(1),avoid(false),recalc(false),add(false),processes(0),goals(0),steer(0.25f,0.5f){}void SetCurrentMaxSpeed(float s){speed=s;}void SetAvoidDynamicObstacles(bool b){avoid=b;}void Process(){++processes;}const C2DVector&GetSteer()const{return steer;}bool GetRecalcWidth(){return recalc;}bool GetAddWaypoint(){return add;}float&GetBestDistLeft(){return left;}float&GetBestDistRight(){return right;}float GetRadius(bool physical)const{if(physical)exit(1);return radius;}void SetGoal(const C2DVector&a,const C2DVector&b){goal=a;next=b;++goals;}};
namespace{C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM(0,0,5);}
struct CNavigatorAStar{CSteeringAgent agent;CSteeringAgent*SteeringAgent;std::list<CNavWayPoint>Route;int reduced,wholeWidth,localWidth;bool collapseRoute;float proposedLeft,proposedRight;C3DVector requested;C2DVector widthStart,widthEnd;CNavigatorAStar():SteeringAgent(&agent),reduced(0),wholeWidth(0),localWidth(0),proposedLeft(2),proposedRight(2),collapseRoute(false){}float GetSpeedReducedForStartAndEnd(float speed,float proximity,const C2DVector&){if(proximity!=1)exit(1);++reduced;return speed*0.5f;}float FindPosZAt(const C3DVector&p,bool exact){if(!exact)exit(1);requested=p;return p.Z+1;}void CalculateNavigableWidthOfLine(const C3DVector&){++wholeWidth;if(collapseRoute&&Route.size()>1){std::list<CNavWayPoint>::iterator it=Route.begin();++it;Route.erase(it,Route.end());}}void CalculateNavigableWidthOfLine(const CNavWayPoint&a,const CNavWayPoint&b,float&left,float&right,bool flag){if(flag||a.NavLayer!=b.NavLayer)exit(1);++localWidth;widthStart=a.Position;widthEnd=b.Position;left=proposedLeft;right=proposedRight;}void MoveTowardNextWayPoint(const C3DVector&,float,float,C3DVector&,bool,bool,bool,bool);};

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
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
static bool close(float a,float b){return fabs(a-b)<0.00003;}
int main(){C3DVector start(0,0,2);
 {CNavigatorAStar n;C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,false,true,true,true);check(n.reduced==1&&n.agent.speed==2&&n.agent.avoid&&n.agent.processes==1,"speed reduction, avoidance, process");check(close(out.X,0.25f)&&close(out.Y,0.5f)&&close(out.Z,6)&&close(n.requested.Z,7),"steer and requested height offset");}
 {CNavigatorAStar n;n.agent.recalc=true;C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,true,false,false,true);check(n.reduced==0&&n.agent.speed==4&&n.wholeWidth==1,"stop-within-sight disables speed reduction and width flag recalculates");}
 {CNavigatorAStar n;n.agent.add=true;n.Route.push_back(CNavWayPoint(C2DVector(8,0),3,false));C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,false,false,false,false);check(n.Route.size()==1&&n.localWidth==0&&n.agent.goals==0,"radius boundary strict");}
 {CNavigatorAStar n;n.agent.add=true;n.Route.push_back(CNavWayPoint(C2DVector(10,0),3,false));C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,false,false,false,false);check(n.Route.size()==2&&close(n.Route.front().Position.X,5)&&n.Route.front().NavLayer==3&&n.Route.front().Removable,"midpoint inserted with original layer");check(n.localWidth==1&&n.wholeWidth==1&&n.agent.goals==1&&close(n.agent.goal.X,5)&&close(n.agent.next.X,10),"better corridor retained and steering goal pair");}
 {CNavigatorAStar n;n.agent.add=true;n.proposedLeft=1;n.Route.push_back(CNavWayPoint(C2DVector(10,0),3,false));C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,false,false,false,false);check(n.Route.size()==1&&close(n.Route.front().Position.X,10)&&n.agent.goals==0,"left boundary rejects midpoint");}
 {CNavigatorAStar n;n.agent.add=true;n.proposedRight=0;n.Route.push_back(CNavWayPoint(C2DVector(10,0),3,false));C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,false,false,false,false);check(n.Route.size()==1&&n.agent.goals==0,"right corridor rejects midpoint");}
 {CNavigatorAStar n;n.agent.add=true;n.collapseRoute=true;n.Route.push_back(CNavWayPoint(C2DVector(10,0),3,false));C3DVector out;n.MoveTowardNextWayPoint(start,4,1,out,false,false,false,false);check(n.Route.size()==1&&n.agent.goals==1&&close(n.agent.goal.X,5)&&close(n.agent.next.X,5),"single remaining waypoint uses same goal twice");}
 {
  CNavigatorAStar n;
  n.agent.add=true;
  C3DVector translatedStart(3.0f,-4.0f,2.0f);
  n.Route.push_back(CNavWayPoint(C2DVector(13.0f,-4.0f),4,false));
  C3DVector out;
  n.MoveTowardNextWayPoint(translatedStart,4,1,out,false,false,false,false);
  check(n.localWidth==1 && close(n.widthStart.X,3) && close(n.widthStart.Y,-4),
        "width query projects the translated start onto the original XY plane");
  check(close(n.widthEnd.X,8) && close(n.widthEnd.Y,-4), "width query uses the inserted midpoint");
 }
 puts("00a1e110_TEST PASS");return 0;}