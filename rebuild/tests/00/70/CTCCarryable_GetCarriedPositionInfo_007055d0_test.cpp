#include <stdio.h>
#include <stdlib.h>
struct C3DVector {float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector operator+(C3DVector b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}C3DVector operator-(C3DVector b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}C3DVector operator*(float n)const{return C3DVector(X*n,Y*n,Z*n);}bool operator==(C3DVector b)const{return X==b.X&&Y==b.Y&&Z==b.Z;}};
struct CRightHandedSet{C3DVector up,forward;C3DVector GetForward()const{return forward;}C3DVector GetUp()const{return up;}C3DVector GetRight()const{return C3DVector(forward.Y*up.Z-forward.Z*up.Y,forward.Z*up.X-forward.X*up.Z,forward.X*up.Y-forward.Y*up.X);}bool operator==(CRightHandedSet b)const{return up==b.up&&forward==b.forward;}};
static CRightHandedSet cd,od,obj;static C3DVector cp,delta;static bool accurate,available;static float time_arg;static int stage,flip;
static C3DVector transform(C3DVector v){return C3DVector((flip?-1.f:1.f)*v.Y,v.Z,v.X);}
struct CMatrix3x4 {int value;void PostMultiply(CMatrix3x4 m){if(stage++!=4||value!=2||m.value!=3)abort();value=4;}void TransformRightHandedSet(CRightHandedSet& r){if(stage++!=5||value!=4||!(r==obj))abort();r.up=transform(r.up);r.forward=transform(r.forward);}};
struct C3DMeshObject {static CMatrix3x4 GetRotationMatrixForTransformFromTo(CRightHandedSet a,CRightHandedSet b){CMatrix3x4 m;m.value=stage++;if(m.value==2){if(!(a==od&&b==obj))abort();}else if(m.value==3){if(!(a==obj&&b==cd))abort();}else abort();return m;}};
struct Thing {CRightHandedSet GetRHSet(float t)const{if(t!=time_arg)abort();return obj;}const Thing* GetPhysicsTC()const{return this;}C3DVector GetPos()const{return C3DVector(7,6,5);}};
struct CTCCarrying {bool GetCarriedThingInterpolatedPosAndOrientation(const Thing&,C3DVector& p,CRightHandedSet& r,bool f,float t){if(stage++!=0||f!=accurate||t!=time_arg)abort();p=cp;r=cd;return available;}};
struct CTCCarryable {CTCCarrying* PCarryingTC;C3DVector OffsetCoordRelativeToAttachToDummy;float OffsetAmount;Thing parent;const Thing& GetParentThing()const{return parent;}bool IsCarried()const{return PCarryingTC!=0;}
void GetInterpolatedPickupOffsetAndOrientation(C3DVector& p,CRightHandedSet& r,bool f,float t)const{if(stage++!=1||f!=accurate||t!=time_arg)abort();p=C3DVector(12,21,31);r=od;}
void GetInterpolatedPickupOffsetAndOrientationAt(C3DVector p,CRightHandedSet r,C3DVector& out,CRightHandedSet& dummy,bool f)const{if(stage++!=6||f!=accurate||!(p==parent.GetPos())||!(r.up==transform(obj.up)&&r.forward==transform(obj.forward))||!(out==C3DVector(12,21,31)&&dummy==od))abort();out=delta;}
bool GetCarriedPositionInfo(C3DVector&,CRightHandedSet&,bool,float)const;};
bool CTCCarryable::GetCarriedPositionInfo(C3DVector& position,CRightHandedSet& rhset,bool force_accurate,float interpolate) const
{
    if (!PCarryingTC || !IsCarried()) return false;
    CRightHandedSet carrying_dummy_rhs;
    C3DVector carrying_dummy_pos;
    if (!PCarryingTC->GetCarriedThingInterpolatedPosAndOrientation(GetParentThing(),carrying_dummy_pos,carrying_dummy_rhs,force_accurate,interpolate)) {
        return false;
    } else {
        C3DVector offset_x=carrying_dummy_rhs.GetForward();
        C3DVector offset_y=carrying_dummy_rhs.GetRight();
        C3DVector offset_z=carrying_dummy_rhs.GetUp();
        C3DVector offset_world_coord=(offset_x*OffsetCoordRelativeToAttachToDummy.X+offset_y*OffsetCoordRelativeToAttachToDummy.Y+offset_z*OffsetCoordRelativeToAttachToDummy.Z)*OffsetAmount;
        CRightHandedSet object_dummy_rhs;
        C3DVector object_dummy_offset;
        GetInterpolatedPickupOffsetAndOrientation(object_dummy_offset,object_dummy_rhs,force_accurate,interpolate);
        CRightHandedSet object_rhs=GetParentThing().GetPhysicsTC()->GetRHSet(interpolate);
        CMatrix3x4 obj_dummy_2_obj=C3DMeshObject::GetRotationMatrixForTransformFromTo(object_dummy_rhs,object_rhs);
        CMatrix3x4 obj_2_carry_dummy=C3DMeshObject::GetRotationMatrixForTransformFromTo(object_rhs,carrying_dummy_rhs);
        CMatrix3x4 mat=obj_dummy_2_obj;
        mat.PostMultiply(obj_2_carry_dummy);
        mat.TransformRightHandedSet(object_rhs);
        GetInterpolatedPickupOffsetAndOrientationAt(GetParentThing().GetPos(),object_rhs,object_dummy_offset,object_dummy_rhs,force_accurate);
        rhset=object_rhs;
        position=carrying_dummy_pos-object_dummy_offset+offset_world_coord;
        return true;
    }
}

int main(){for(int f=0;f<128;++f)for(int n=-2;n<=2;++n)for(int j=0;j<3;++j){CTCCarryable x;CTCCarrying c;x.PCarryingTC=(f&1)?&c:0;available=(f&2)!=0;accurate=(f&4)!=0;flip=(f&8)!=0;time_arg=.25f*j;
float dir=(f&16)?-1.f:1.f;cd.up=C3DVector(0,0,1);cd.forward=C3DVector(0,dir,0);obj.up=C3DVector(0,1,0);obj.forward=C3DVector(0,0,1);od.up=C3DVector(1,0,0);od.forward=C3DVector(0,0,1);cp=C3DVector(n,3,-1);delta=C3DVector(j,-n,5);x.OffsetCoordRelativeToAttachToDummy=C3DVector(3*n,j-1,-n);x.OffsetAmount=(f&32)?-2.f:((f&64)?0.f:2.f);
stage=0;C3DVector p(11,12,13);CRightHandedSet r=od;bool ok=x.GetCarriedPositionInfo(p,r,accurate,time_arg);bool expect=(f&1)&&available;if(ok!=expect||stage!=(expect?7:(f&1)?1:0))return 1;
if(!expect){if(!(p==C3DVector(11,12,13)&&r==od))return 2;}else{float scale=x.OffsetAmount;C3DVector ep(cp.X-delta.X+dir*(j-1)*scale,cp.Y-delta.Y+dir*3*n*scale,cp.Z-delta.Z-n*scale);if(!(p==ep&&r.up==transform(obj.up)&&r.forward==transform(obj.forward)))return 3;}}puts("007055d0_TEST PASS");return 0;}