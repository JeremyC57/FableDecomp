#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#include "engine/C3DVector.h"
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/GFAngles.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
const double TWO_PI=6.283185482025146484375;
struct TestThing {C3DVector position;const C3DVector& GetPos()const{return position;}};
struct Definition {float ZTargetStringLengthToObservedThing,ZTargetStringLengthPerpendicularToObservedThing;};
struct Target {bool present,dead;};
static bool IS_THING_ALIVE(const Target& p){return p.present&&!p.dead;}
namespace NCameraModes{
struct CZTargetNew{
 Target PTargetedThing; TestThing observed;Definition def;
 float Theta,DeltaTheta,BaseHeightOffset,DeltaBaseHeightOffset,BaseOffsetXY,DeltaBaseOffsetXY,CageRadius;
 int FramesForTransition;
 C3DVector XAxis,YAxis,current;
 TestThing* GetObservedThing(){return &observed;}
 const C3DVector& GetPosition(){return current;}
 Definition* GetCameraModeDef(){return &def;}
 C3DVector GetCameraPosition();
};}
C3DVector NCameraModes::CZTargetNew::GetCameraPosition()
{
    C3DVector base_pos=GetObservedThing()->GetPos();
    bool keep_to_anchor_pos=false;
    if(FramesForTransition>0) {
        --FramesForTransition;
        keep_to_anchor_pos=true;
        Theta+=DeltaTheta;
        BaseHeightOffset+=DeltaBaseHeightOffset;
        BaseOffsetXY+=DeltaBaseOffsetXY;
    }
    if(!IS_THING_ALIVE(PTargetedThing))keep_to_anchor_pos=true;
    C3DVector base_thing_anchor=base_pos+C3DVector(0,0,BaseHeightOffset);
    C3DVector anchor_pos=base_thing_anchor+(XAxis*(float)GFCos(Theta)+YAxis*(float)GFSin(Theta))*BaseOffsetXY;
    if(keep_to_anchor_pos)return anchor_pos;
    C3DVector cam_pos=GetPosition();
    C3DVector line_to_cam=cam_pos-anchor_pos;line_to_cam.Z=0;
    float dist_x=line_to_cam.Dot(XAxis);
    const float max_dist_x=GetCameraModeDef()->ZTargetStringLengthToObservedThing;
    float dist_y=line_to_cam.Dot(YAxis);
    const float max_dist_y=GetCameraModeDef()->ZTargetStringLengthPerpendicularToObservedThing;
    if(fabs(dist_x)>max_dist_x)dist_x=GFGetSign(dist_x)*max_dist_x;
    if(fabs(dist_y)>max_dist_y)dist_y=GFGetSign(dist_y)*max_dist_y;
    cam_pos=anchor_pos+XAxis*dist_x+YAxis*dist_y;
    C3DVector obsthing_to_camera=cam_pos-base_pos;obsthing_to_camera.Z=0;
    float push_sqr_mag=obsthing_to_camera.GetSquaredMagnitude();
    if(push_sqr_mag<GFSqr(CageRadius))
        cam_pos=base_thing_anchor+(obsthing_to_camera*CageRadius)/GFRoot(push_sqr_mag);
    return cam_pos;
}
static bool close_value(double x,double y){if(_isnan(x)||_isnan(y))return _isnan(x)&&_isnan(y);return fabs(x-y)<0.00002;}
int main(){
 for(int state=0;state<3;++state)for(int steps=0;steps<3;++steps)for(int rot=0;rot<4;++rot)
 for(int offset=0;offset<3;++offset)for(int radius=0;radius<4;++radius)for(int x=-2;x<=2;++x)for(int y=-1;y<=1;++y){
 NCameraModes::CZTargetNew c;c.PTargetedThing.present=state!=0;c.PTargetedThing.dead=state==1;c.FramesForTransition=steps;
 c.Theta=0.125f;c.DeltaTheta=0.125f;c.BaseHeightOffset=3;c.DeltaBaseHeightOffset=-1;c.BaseOffsetXY=(float)offset;c.DeltaBaseOffsetXY=1;
 c.CageRadius=(float)radius;c.def.ZTargetStringLengthToObservedThing=1.25f;c.def.ZTargetStringLengthPerpendicularToObservedThing=0.75f;
 int ax[4]={1,0,-1,0},ay[4]={0,1,0,-1};int xx=ax[rot],xy=ay[rot],yx=-xy,yy=xx;
 c.XAxis=C3DVector((float)xx,(float)xy,0);c.YAxis=C3DVector((float)yx,(float)yy,0);c.observed.position=C3DVector(10,-5,6);
 c.current=C3DVector(10+2.0f*x,-5+3.0f*y,27);
 C3DVector result=c.GetCameraPosition();
 double theta=steps?0.25:0.125,height=steps?2:3,distance=offset+(steps?1:0);
 double px=10+distance*(xx*cos(theta*TWO_PI)+yx*sin(theta*TWO_PI));
 double py=-5+distance*(xy*cos(theta*TWO_PI)+yy*sin(theta*TWO_PI));double pz=6+height;
 if(!steps&&state==2){
 double dx=c.current.X-px,dy=c.current.Y-py;
 double u=dx*xx+dy*xy,v=dx*yx+dy*yy;if(u>1.25)u=1.25;if(u< -1.25)u=-1.25;if(v>0.75)v=0.75;if(v< -0.75)v=-0.75;
 px+=xx*u+yx*v;py+=xy*u+yy*v;dx=px-10;dy=py+5;double len=sqrt(dx*dx+dy*dy);
 if(len<radius){px=10+dx*radius/len;py=-5+dy*radius/len;if(len==0)pz=0.0/len;}
 }
 if(!close_value(result.X,px)||!close_value(result.Y,py)||!close_value(result.Z,pz)||c.FramesForTransition!=(steps?steps-1:0)||c.Theta!=theta||c.BaseHeightOffset!=height||c.BaseOffsetXY!=distance){printf("mismatch %d %d %d %d %d %d %d\n",state,steps,rot,offset,radius,x,y);return 1;}
 }puts("00872af0_TEST PASS");return 0;}