#include <cstdio>
#include <cmath>
#include <list>
#include <cstdlib>
class C3DVector { public:
 float X,Y,Z;
 C3DVector():X(0),Y(0),Z(0){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
 float GetMagnitude()const{return float(std::sqrt(Z*Z+Y*Y+X*X));}
};
const C3DVector UnitSet[4]={C3DVector(0,0,0),C3DVector(1,0,0),C3DVector(0,1,0),C3DVector(0,0,1)};
static void check(bool b){if(!b)std::abort();}
class CWorldMap{ public:};
class CWorld { public:
 CWorldMap map;
 mutable int map_calls;
 CWorld():map_calls(0){}
 const CWorldMap& PeekWorldMap()const{++map_calls;return map;}
};
class CGameCameraManager { public:
 const CWorld& world;
 CGameCameraManager(const CWorld& w):world(w){}
 const CWorld& GetWorld()const{return world;}
};
struct CPhysicsCollisionPointsObject{std::list<C3DVector> Points;};
struct CPhysicsForceModelBase {
 C3DVector position;
 const CGameCameraManager& manager;
 CPhysicsCollisionPointsObject Themes;
 std::list<C3DVector> fixtures;
 int scans;
 CPhysicsForceModelBase(const CGameCameraManager& m):manager(m),scans(0){}
 C3DVector GetPosition(){return position;}
 const CGameCameraManager& GetCameraManager()const{return manager;}
 template<class T> void GetCollisionPoints(CPhysicsCollisionPointsObject& p,const T& map,const C3DVector& pos,long type,float radius){
  check(&p==&Themes&&&map==&manager.world.map&&type==2&&radius==50.0f);
  check(pos.X==position.X&&pos.Y==position.Y&&pos.Z==position.Z);
  ++scans;p.Points=fixtures;
 }
 float PotentialField(const C3DVector& pos,const C3DVector& source,float scale,float r_prime)const {
  return (scale*0.1f)/((pos-source).GetMagnitude()-r_prime);
 }
 C3DVector PotentialFieldBufferPoints(const C3DVector& field_is_at,const std::list<C3DVector>& points_list,float override_scale_factor,float r_prime)const {
  float field_at_pos[4]={0,0,0,0};
  for(std::list<C3DVector>::const_iterator it=points_list.begin();it!=points_list.end();++it)
   for(long counter=0;counter<4;++counter){
    C3DVector pos_to_check=field_is_at+UnitSet[counter]*0.1f;
    field_at_pos[counter]+=PotentialField(pos_to_check,*it,override_scale_factor,r_prime);
   }
  return C3DVector((field_at_pos[1]-field_at_pos[0])/0.1f,(field_at_pos[2]-field_at_pos[0])/0.1f,(field_at_pos[3]-field_at_pos[0])/0.1f);
 }
 C3DVector DoThemeEvasion(){
  std::list<C3DVector> theme_points;
  C3DVector cam_pos=GetPosition();
  const CWorld& world=GetCameraManager().GetWorld();
  GetCollisionPoints<const CWorldMap>(Themes,world.PeekWorldMap(),cam_pos,2,50.0f);
  C3DVector return_value=PotentialFieldBufferPoints(cam_pos,Themes.Points,50.0f,0.0f);
  return return_value;
 }
};
static double reference(const C3DVector& p,const std::list<C3DVector>& points,int axis){
 double samples[2]={0,0};
 for(int sample=0;sample<2;++sample)
  for(std::list<C3DVector>::const_iterator i=points.begin();i!=points.end();++i){
   double x=p.X-i->X+(sample&&axis==0?0.1:0.0);
   double y=p.Y-i->Y+(sample&&axis==1?0.1:0.0);
   double z=p.Z-i->Z+(sample&&axis==2?0.1:0.0);
   samples[sample]+=5.0/std::sqrt(x*x+y*y+z*z);
  }
 return (samples[1]-samples[0])*10.0;
}
int main(){
 CWorld world;CGameCameraManager manager(world);CPhysicsForceModelBase m(manager);
 for(int c=0;c<128;++c){
  m.position=C3DVector(float(c%7-3)*1.25f,float(c%11-5)*0.75f,float(c%5-2)*2.0f);
  m.fixtures.clear();
  int count=c%9;
  for(int j=0;j<count;++j)m.fixtures.push_back(C3DVector(float(j*3-12),float((j*j+c)%17-8),float(14+j)));
  C3DVector before=m.position;
  C3DVector got=m.DoThemeEvasion();
  const float values[3]={got.X,got.Y,got.Z};
  for(int axis=0;axis<3;++axis){double expected=reference(before,m.fixtures,axis);check(std::fabs(values[axis]-expected)<0.00004+std::fabs(expected)*0.001);}
  check(m.scans==c+1&&world.map_calls==c+1&&m.Themes.Points.size()==m.fixtures.size());
  check(m.position.X==before.X&&m.position.Y==before.Y&&m.position.Z==before.Z);
 }
 std::puts("008fac30_TEST PASS");return 0;
}