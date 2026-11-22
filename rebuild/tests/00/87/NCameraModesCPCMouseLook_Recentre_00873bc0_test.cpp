// Execute the production camera member with observable game and physics dependencies.
#include <stdio.h>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#define FABLE_RETAIL_PCMOUSELOOK_PASS24
#include "engine/C3DVector.h"
struct CameraDef {float SecondsToReset,DollyLength;};
struct Body {C3DVector direction;C3DVector GetFacingVector() const {return direction;}};
struct Thing {Body body;const Body* GetPhysicsTC()const{return &body;}};
struct Snapshot {
    float AngleXY,AngleUpDown,DollyLength,DeltaAngleXY;long RecentreTimer;
    C3DVector ViewPosOffset;
};
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#include "engine/GFAngles.h"
class CMainGameComponent {public:static long GetConstantFPS(){return 16;}};
// The external rotation dependency in these cases uses quarter turns about positive Z.
void C3DVector::Rotate(const C3DVector& axis,float turns) {
    if(axis.Z==1 && turns==0.25f){float x=X;X=-Y;Y=x;}
}
class CPCMouseLook {
public:
    C3DVector AxisX,AxisY,AxisZ; Snapshot StateCurrent;
    Thing thing; CameraDef def;
    const Thing* GetObservedThing()const{return &thing;}
    const CameraDef* GetCameraModeDef()const{return &def;}
    void Recentre();
};
void CPCMouseLook::Recentre()
{
    C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
    C3DVector old_axis_x(AxisX);
    old_axis_x.Rotate(AxisZ, StateCurrent.AngleXY);
    AxisZ = C3DVector(0,0,1);
    AxisX = facing * -1.0f;
    AxisY = AxisX.Cross(AxisZ);
    float cos_thet = old_axis_x.Dot(AxisX);
    if (cos_thet < -1.0f)
        cos_thet = -1.0f;
    else if (cos_thet > 1.0f)
        cos_thet = 1.0f;
    StateCurrent.AngleXY = GFArcCos(cos_thet);
    if (old_axis_x.Cross(AxisX).Z < 0.0f)
        StateCurrent.AngleXY *= -1.0f;
    const float RECENTRE_SECONDS = GetCameraModeDef()->SecondsToReset;
    StateCurrent.RecentreTimer = (long)(CMainGameComponent::GetConstantFPS() * RECENTRE_SECONDS);
    StateCurrent.DeltaAngleXY = StateCurrent.AngleXY * -1.0f / StateCurrent.RecentreTimer;
    --StateCurrent.RecentreTimer;
}

int main(){
    const float dir[][2]={{-1,0},{1,0},{0,1},{0,-1},{-2,0},{2,0},{-1,0}};
    const float angle[]={0,0.5f,-0.25f,0.25f,0,0.5f,-0.25f};
    for(int k=0;k<7;++k){
        CPCMouseLook m; m.AxisX=C3DVector(1,0,0); m.AxisZ=C3DVector(0,0,1);
        m.def.SecondsToReset=k==0?0.25f:0.28125f;
        m.StateCurrent.AngleXY=k==6?0.25f:0;
        m.thing.body.direction=C3DVector(dir[k][0],dir[k][1],0);m.Recentre();
        if(m.AxisX.X!=-dir[k][0]||m.AxisX.Y!=-dir[k][1]||m.AxisX.Z!=0 ||
           m.AxisZ.X!=0||m.AxisZ.Y!=0||m.AxisZ.Z!=1 ||
           m.AxisY.X!=-dir[k][1]||m.AxisY.Y!=dir[k][0]||m.AxisY.Z!=0)return 1;
        if(fabs(m.StateCurrent.AngleXY-angle[k])>0.00001 ||
           fabs(m.StateCurrent.DeltaAngleXY+angle[k]/4)>0.00001 ||
           m.StateCurrent.RecentreTimer!=3)return 2;
    }
    puts("00873bc0_TEST PASS");return 0;
}