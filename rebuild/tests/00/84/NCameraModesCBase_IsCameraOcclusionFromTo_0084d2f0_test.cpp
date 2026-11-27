static const float DELTA_NOTIONAL_ZERO=0.0001f;
#include <stdio.h>
#include <math.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}
 C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 C3DVector operator*(float a)const{return C3DVector(X*a,Y*a,Z*a);}
 float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}
 C3DVector Cross(const C3DVector& o)const{return C3DVector(Y*o.Z-Z*o.Y,Z*o.X-X*o.Z,X*o.Y-Y*o.X);}
 void AccurateNormalise(){float n=(float)sqrt(GetSquaredMagnitude());if(n){X/=n;Y/=n;Z/=n;}}
};
struct CThing {};
struct CCameraModeDef {bool PerformExtendedOcclusionTests;};
namespace NCameraModes {
struct CTools {
 enum ELOSTest {TEST_THINGS=1,TEST_CREATURES=8,TEST_ALL=15};
 mutable int count;
 int blocked,seed;
 mutable C3DVector paths[5][2];
 mutable int flags[5];
 mutable bool ok;
 bool IsLineOfSight(const C3DVector& from,const C3DVector& to,C3DVector& pos,C3DVector& normal,float& radius,const CThing** thing,ELOSTest flag,bool b)const {
  if(count>=5){ok=false;return false;}
  const int i=count++;paths[i][0]=from;paths[i][1]=to;flags[i]=flag;
  static CThing t;
  if(!b||!thing||*thing!=(i?&t:0)||fabs(radius-(i?float(i+seed)*.25f:.01f))>.000001)ok=false;
  *thing=&t;radius=(i+1+seed)*.25f;pos=C3DVector(i+seed,3-i,i*2);normal=C3DVector(1,-2,3);return i!=blocked;
 }
};
struct CGameCameraManager { CTools tools;const CTools& GetCameraTools()const{return tools;} };
struct CBase { CCameraModeDef mode; CGameCameraManager manager;
 const CCameraModeDef* GetCameraModeDef()const{return &mode;}
 const CGameCameraManager& GetCameraManager()const{return manager;}
 const CTools& GetCamTools()const;
 bool IsCameraOcclusionFromTo(const C3DVector&,const C3DVector&,C3DVector&)const;
};
}
namespace {
C3DVector __fastcall GetNewCameraPosFromOcclusionParams(const C3DVector& p,const C3DVector& n,float r,const C3DVector&)
{
    C3DVector result=p+n*r; return result;
}
}
inline const NCameraModes::CTools& NCameraModes::CBase::GetCamTools() const { return GetCameraManager().GetCameraTools(); }
bool NCameraModes::CBase::IsCameraOcclusionFromTo(const C3DVector& from_pos,const C3DVector& to_pos,C3DVector& occlusion_pos)const
{
    C3DVector line=to_pos-from_pos;
    if (line.GetSquaredMagnitude()>10000.0f) return false;
    const float DEFAULT_CAMERA_RADIUS=0.01f;
    float camera_radius=DEFAULT_CAMERA_RADIUS;
    float other_camera_radius=0.225f;
    C3DVector look_dir=from_pos-to_pos;
    C3DVector shooter_perp,shooter_up;
    shooter_perp.Z=0;
    bool perform_extended_checks=GetCameraModeDef()->PerformExtendedOcclusionTests;
    if (fabs(look_dir.Y)>DELTA_NOTIONAL_ZERO) {
        shooter_perp.X=1;
        shooter_perp.Y=-shooter_perp.X*look_dir.X/look_dir.Y;
        shooter_perp.AccurateNormalise();
        shooter_up=look_dir.Cross(shooter_perp);
        shooter_up.AccurateNormalise();
    } else if (fabs(look_dir.X)>DELTA_NOTIONAL_ZERO) {
        shooter_perp.Y=1;
        shooter_perp.X=-shooter_perp.Y*look_dir.Y/look_dir.X;
        shooter_perp.AccurateNormalise();
        shooter_up=look_dir.Cross(shooter_perp);
        shooter_up.AccurateNormalise();
    } else perform_extended_checks=false;
    const CTools::ELOSTest std_los_check=CTools::TEST_ALL;
    const CTools::ELOSTest extended_los_check=static_cast<CTools::ELOSTest>(CTools::TEST_THINGS|CTools::TEST_CREATURES);
    bool occlusion=false;
    C3DVector intersect_pos,normal_vector;
    const CThing* phit_thing=0;
    if (!GetCamTools().IsLineOfSight(from_pos,to_pos,intersect_pos,normal_vector,camera_radius,&phit_thing,std_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!perform_extended_checks) {}
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos+shooter_perp*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos-shooter_perp*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos+shooter_up*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos-shooter_up*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    return occlusion;
}
static bool eq(const C3DVector& a,const C3DVector& b){return fabs(a.X-b.X)<.00004 && fabs(a.Y-b.Y)<.00004 && fabs(a.Z-b.Z)<.00004;}
int main() {
 float axis[]={-101,-100,-3,-.0001f,0,.0001f,.75f,4,100,101};
 for(int ix=0;ix<10;++ix)for(int iy=0;iy<10;++iy)for(int iz=-1;iz<=1;++iz)
 for(int ex=0;ex<2;++ex)for(int block=-1;block<5;++block)for(int seed=0;seed<3;++seed){
  NCameraModes::CBase base; NCameraModes::CTools& t=base.manager.tools;
  base.mode.PerformExtendedOcclusionTests=ex!=0;t.count=0;t.ok=true;t.blocked=block;t.seed=seed;
  C3DVector to(0,0,seed),from(axis[ix],axis[iy],seed+iz),out(12,17,31),expected=out;
  double x=from.X-to.X,y=from.Y-to.Y,z=from.Z-to.Z;
  bool run=(float(x*x)+float(y*y)+float(z*z))<=10000;
  bool side=ex&&(fabs(y)>.0001f||fabs(x)>.0001f);
  int calls=run?(side?5:1):0;
  bool blocked=block>=0&&block<calls;
  if(blocked){calls=block+1;float r=(block+seed+1)*.25f;expected=C3DVector(block+seed+r,3-block-2*r,2*block+3*r);}
  if(base.IsCameraOcclusionFromTo(from,to,out)!=blocked||!t.ok||t.count!=calls||!eq(out,expected))return 1;
  C3DVector axis1,axis2;
  if(side){
   double k=sqrt(x*x+y*y),sign=fabs(y)>.0001f?(y<0?-1.:1.):(x<0?1.:-1.);
   axis1=C3DVector(float(sign*y/k),float(-sign*x/k),0);
   double length=sqrt(x*x+y*y+z*z);
   axis2=C3DVector(float(-z*axis1.Y/length),float(z*axis1.X/length),float((x*axis1.Y-y*axis1.X)/length));
  }
  for(int i=0;i<calls;++i){C3DVector end=to;if(i){const C3DVector& dir=i<=2?axis1:axis2;end=to+dir*(i%2?.225f:-.225f);}if(t.flags[i]!=(i?9:15)||!eq(t.paths[i][0],i?to:from)||!eq(t.paths[i][1],end))return 2;}
 }
 puts("0084d2f0_TEST PASS");return 0;
}