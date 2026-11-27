#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
namespace NGlobalConsole {bool UseBodyReorientationYZ;}
const float MAX_DOT_FOR_ANGLE_YZ_CHANGE=.45f,DELTA_NOTIONAL_ZERO=.0001f;
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector operator-(C3DVector v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}C3DVector operator*(float n)const{return C3DVector(X*n,Y*n,Z*n);}float Dot(C3DVector v)const{return X*v.X+Y*v.Y+Z*v.Z;}void Normalise(){float l=(float)sqrt(Dot(*this));X/=l;Y/=l;Z/=l;}};
struct CRightHandedSet{};struct CMeshIntersectionData{C3DVector IntersectionPosition;};
static int hits,lookups,selected;static float headZ,ground,height,angle,expectedZ,side,initial;static bool intersect;
const int HEAD_POS_HELPER_NAME=0,EYE_LEFT_DUMMY_OBJECT_NAME=1,MAIN_FOCAL_POSITION_NAME=2;
static bool lookup(int i,C3DVector& p){if(i!=lookups++)abort();p=C3DVector(1,0,headZ);return i==selected;}
struct Graphic{bool GetHelperPoint(int i,long,unsigned,C3DVector& p,float){return lookup(i,p);}bool GetDummyObject(int i,long,unsigned,C3DVector& p,CRightHandedSet&,float){return lookup(i,p);}};
struct CPhysicsMeshObject {bool GetPointIntersectionFromTo(C3DVector a,C3DVector b,CMeshIntersectionData& d){if(a.Z!=headZ+3||b.Z<ground)abort();++hits;d.IntersectionPosition.Z=height;return intersect;}};
struct Physics {float set;int changes;CPhysicsMeshObject GetPhysicsMeshObject(){return CPhysicsMeshObject();}C3DVector GetFacingVector(){return C3DVector(side,0,0);}void SetFacingAngleYZ(float a){++changes;set=a;}float GetFacingAngleYZ(){return initial;}};
struct CThing {Physics phys;Graphic graphic;int type;Physics* GetPhysicsTC(){return &phys;}Graphic* GetTCGraphicAppearance(){return &graphic;}C3DVector GetPos(){return C3DVector();}int GetTypeFlags(){return type;}};
struct CThingCreatureBase: CThing {bool flag;void SetHadFacingAngleYZSetByBodyOrientation(bool v){flag=v;}};
template<class T> struct CIntelligentPointer{T* p;T* Get(){return p;}};
struct CMainGameComponent{static CMainGameComponent* Get(){static CMainGameComponent g;return &g;}CMainGameComponent& GetWorld(){return *this;}CMainGameComponent& GetWorldMap(){return *this;}float GetGroundSizeZAt(C3DVector v){if(v.Z!=headZ+3)abort();return ground;}};
float GFGetAngleYZTo(C3DVector a,C3DVector b){if(a.X!=0||b.X!=1||fabs(b.Z-expectedZ)>.00001)abort();return angle;}
struct CThingBodyReorienter {std::vector<CIntelligentPointer<CThing> > PossibleIntersectingThings;CThing* Body;void ReorientBodyYZForSlopes();};
#define FABLE_RETAIL_ASSERT_ARM(condition) ((void)0)
template<class T,class U> U* GFDebugDynamicCast(T* p,U*) {return static_cast<U*>(p);}
void CThingBodyReorienter::ReorientBodyYZForSlopes() {
    if (!NGlobalConsole::UseBodyReorientationYZ) return;
    C3DVector head_pos;
    CRightHandedSet rhset;
    if (!Body->GetTCGraphicAppearance()->GetHelperPoint(HEAD_POS_HELPER_NAME,0,0,head_pos,1)) {
        if (!Body->GetTCGraphicAppearance()->GetDummyObject(EYE_LEFT_DUMMY_OBJECT_NAME,0,0,head_pos,rhset,1)) {
            if (!Body->GetTCGraphicAppearance()->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,head_pos,1)) {
                FABLE_RETAIL_ASSERT_ARM(!"Body reorienter: should not beable to get here");
                return;
            }
        }
    }
    C3DVector foot_pos=Body->GetPos();
    C3DVector head_to_foot=head_pos-foot_pos;
    head_to_foot.Normalise();
    C3DVector up(0,0,1);
    float dot=head_to_foot.Dot(up);
    if (dot<MAX_DOT_FOR_ANGLE_YZ_CHANGE && dot>-MAX_DOT_FOR_ANGLE_YZ_CHANGE) {
        C3DVector above_head_pos=head_pos;
        above_head_pos.Z+=3.0f;
        float ground_head_height=CMainGameComponent::Get()->GetWorld().GetWorldMap().GetGroundSizeZAt(above_head_pos);
        C3DVector best_head_pos=head_pos-up*3;
        if (best_head_pos.Z<ground_head_height) best_head_pos.Z=ground_head_height;
        bool using_ground=true;
        CMeshIntersectionData data;
        if (false) { } else for (std::vector<CIntelligentPointer<CThing> >::iterator it=PossibleIntersectingThings.begin();it!=PossibleIntersectingThings.end();++it) {
            CThing* thing=it->Get();
            if (thing) {
                CPhysicsMeshObject physics_object=thing->GetPhysicsTC()->GetPhysicsMeshObject();
                if (physics_object.GetPointIntersectionFromTo(above_head_pos,best_head_pos,data)) {
                    const float land_tolerance_gap=0.007f;
                    float new_pos_z=data.IntersectionPosition.Z+land_tolerance_gap;
                    if (new_pos_z>best_head_pos.Z) { using_ground=false; best_head_pos.Z=new_pos_z; }
                }
            }
        }
        best_head_pos.Z+=0.12f;
        float best_angle_yz=GFGetAngleYZTo(foot_pos,best_head_pos);
        if (best_angle_yz>0.5f) best_angle_yz-=1.0f;
        float limit_angle=using_ground?0.17f:0.17f;
        if ((best_angle_yz>0 && best_angle_yz<limit_angle)||(best_angle_yz<0 && best_angle_yz>-limit_angle)) {
            C3DVector facing_vector=Body->GetPhysicsTC()->GetFacingVector();
            C3DVector foot_to_head=best_head_pos-foot_pos;
            float dot2=facing_vector.Dot(foot_to_head);
            if (dot2>0) Body->GetPhysicsTC()->SetFacingAngleYZ(best_angle_yz);
            else Body->GetPhysicsTC()->SetFacingAngleYZ(-best_angle_yz);
            if (Body->GetTypeFlags()&8) {
                CThingCreatureBase* creature=GFDebugDynamicCast(Body,(CThingCreatureBase*)0);
                float yz=creature->GetPhysicsTC()->GetFacingAngleYZ();
                if (fabs(yz)>DELTA_NOTIONAL_ZERO) creature->SetHadFacingAngleYZSetByBodyOrientation(true);
            }
        }
    }
}

namespace AngleOwner {
float GFGetWorld2DDistance(const C3DVector& pos1,const C3DVector& pos2) {
 float dx=pos1.X-pos2.X,dy=pos1.Y-pos2.Y;return (float)sqrt(dx*dx+dy*dy);
}
float GFArcTan(double opposite,double adjacent){return (float)atan2(opposite,adjacent)/6.2831855f;}
float GET_ROUNDED_ANGLE(float a){if(a<0.0f || a>=1.0f){a=(float)fmod((double)a,1.0);if(a<0)a+=1.0f;}return a;}
float GFGetAngleYZTo(const C3DVector& pos1,const C3DVector& pos2)
{
    float adjacent = GFGetWorld2DDistance(pos1,pos2);
    float z1 = pos1.Z;
    float z2 = pos2.Z;
    return GET_ROUNDED_ANGLE(GFArcTan(z2-z1,adjacent));
}
}
static int verifyAngles(){
 for(int x=-3;x<=3;++x) for(int y=-3;y<=3;++y) for(int z=-3;z<=3;++z){
  if(x==0 && y==0 && z==0)continue;
  C3DVector start(5,-2,7),finish(float(5+x),float(-2+y),float(7+z));
  double expected=atan2((double)z,sqrt(double(x*x+y*y)))/(2.0*acos(-1.0));
  if(expected<0)expected+=1.0;
  if(fabs(double(AngleOwner::GFGetAngleYZTo(start,finish))-expected)>0.000001)return 1;
 }
 return 0;
}
int main(){if(verifyAngles())return 2;float angles[]={0,.125f,-.125f,.17f,-.17f,.875f,.5f,.75f};
for(int f=0;f<256;++f)for(selected=0;selected<4;++selected)for(int a=0;a<8;++a){CThingCreatureBase body;CThing thing;CThingBodyReorienter row;row.Body=&body;CIntelligentPointer<CThing> p;p.p=0;row.PossibleIntersectingThings.push_back(p);p.p=&thing;row.PossibleIntersectingThings.push_back(p);
NGlobalConsole::UseBodyReorientationYZ=(f&1)!=0;headZ=(f&2)?3.f:0.f;intersect=(f&4)!=0;height=(f&8)?2.f:-4.f;ground=(f&16)?1.f:-5.f;side=(f&32)?1.f:-1.f;body.type=(f&64)?8:0;initial=(f&128)?-.1f:0.f;body.flag=false;body.phys.changes=0;body.phys.set=0;angle=angles[a];hits=lookups=0;
expectedZ=headZ-3;if(expectedZ<ground)expectedZ=ground;if(intersect&&height+.007f>expectedZ)expectedZ=height+.007f;expectedZ+=.12f;
row.ReorientBodyYZForSlopes();bool active=(f&1)&&selected<3&&headZ==0;float wrapped=angle>.5f?angle-1:angle;bool rotates=active&&wrapped!=0&&fabs(wrapped)<.17f;
if(hits!=(active?1:0)||lookups!=((f&1)?(selected<3?selected+1:3):0)||body.phys.changes!=int(rotates)||(rotates&&body.phys.set!=(side>0?wrapped:-wrapped))||body.flag!=(rotates&&body.type&&initial!=0))return 1;}
puts("008633b0_TEST PASS");return 0;}