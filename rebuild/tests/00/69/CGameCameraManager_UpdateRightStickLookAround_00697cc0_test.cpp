#include <cstdio>
#include <cstdlib>
static void check(bool b){if(!b)std::abort();}
class C2DVector{ public:
 float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}
 const C2DVector operator*(float v)const{return C2DVector(X*v,Y*v);}
 C2DVector operator+(const C2DVector& v)const{return C2DVector(X+v.X,Y+v.Y);}
 const C2DVector operator/(float v)const{float inverse=1.0f/v;return C2DVector(X*inverse,Y*inverse);}
};
class C3DVector{ public:
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 const C3DVector Cross(const C3DVector& v)const{return C3DVector((Y*v.Z-Z*v.Y),Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
 void Rotate(const C3DVector&,float);C3DVector& AccurateNormalise();
};
static bool equal(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static C3DVector receivers[4],axes[4],returns[4],normalReturns[2];static float angles[4];static int rotations,normals,sets;
// Dependency doubles verify each call's incoming state and provide distinguishable results.
void C3DVector::Rotate(const C3DVector& axis,float angle){check(rotations<4);check(equal(*this,receivers[rotations])&&equal(axis,axes[rotations])&&angle==angles[rotations]);*this=returns[rotations++];}
C3DVector& C3DVector::AccurateNormalise(){check(rotations==4&&normals<2);check(equal(*this,returns[2+normals]));*this=normalReturns[normals++];return *this;}
class CCamera{ public:
 C3DVector forward,up;
 const C3DVector& PeekViewDirection()const{return forward;}const C3DVector& PeekUpDirection()const{return up;}
 void SetView(const C3DVector& f,const C3DVector& u){check(normals==2&&equal(f,normalReturns[0])&&equal(u,normalReturns[1]));forward=f;up=u;++sets;}
};
namespace NCameraModes{
class CBase{ public:bool active;mutable int queried;virtual bool IsUseRightStickLookAround()const{++queried;return active;}};
class CTools{ public:template<class T>T GetRollingAverage(const T& a,const T& b,float w)const{return (a*w+b)/(w+1.0f);}};
}
class CGameCameraManager{ public:
 CCamera Camera;NCameraModes::CTools tools;NCameraModes::CBase* mode;C2DVector RightStickLookAroundAngles,RightStickTargetLookAroundAngles;
 NCameraModes::CTools& GetCameraTools(){return tools;}NCameraModes::CBase* GetCameraMode(){return mode;}
 void UpdateRightStickLookAround();
};
void CGameCameraManager::UpdateRightStickLookAround()
{
 RightStickLookAroundAngles=GetCameraTools().GetRollingAverage(RightStickLookAroundAngles,RightStickTargetLookAroundAngles,3.0f);
 if(GetCameraMode()->IsUseRightStickLookAround()){
  C3DVector forward=Camera.PeekViewDirection();
  C3DVector up=Camera.PeekUpDirection();
  C3DVector right=forward.Cross(up);
  float rotation_angle_x=RightStickLookAroundAngles.X;
  forward.Rotate(up,rotation_angle_x);
  right.Rotate(up,rotation_angle_x);
  float rotation_angle_y=RightStickLookAroundAngles.Y;
  forward.Rotate(right,rotation_angle_y);
  up.Rotate(right,rotation_angle_y);
  forward.AccurateNormalise();
  up.AccurateNormalise();
  Camera.SetView(forward,up);
 }
}

int main(){
 CGameCameraManager manager;NCameraModes::CBase mode;manager.mode=&mode;int cases=0;
 const float values[5]={-2.0f,-0.5f,0.0f,0.25f,4.0f};
 for(int active=0;active<2;++active)for(int x=0;x<5;++x)for(int y=0;y<5;++y)for(int tx=0;tx<5;++tx)for(int ty=0;ty<5;++ty){
  manager.RightStickLookAroundAngles=C2DVector(values[x],values[y]);manager.RightStickTargetLookAroundAngles=C2DVector(values[tx],values[ty]);
  float desiredX=(3*values[x]+values[tx])/4;float desiredY=(3*values[y]+values[ty])/4;
  manager.Camera.forward=C3DVector(1,2,4);manager.Camera.up=C3DVector(-2,1,0);
  receivers[0]=manager.Camera.forward;axes[0]=manager.Camera.up;
  receivers[1]=C3DVector(-4,-8,5);axes[1]=manager.Camera.up;
  returns[0]=C3DVector(8,16,32);returns[1]=C3DVector(-8,4,2);returns[2]=C3DVector(3,4,0);returns[3]=C3DVector(0,0,8);
  receivers[2]=returns[0];axes[2]=returns[1];receivers[3]=manager.Camera.up;axes[3]=returns[1];
  normalReturns[0]=C3DVector(0.5f,0.25f,0);normalReturns[1]=C3DVector(0,0,1);
  angles[0]=angles[1]=desiredX;angles[2]=angles[3]=desiredY;
  mode.active=active!=0;mode.queried=0;rotations=normals=sets=0;
  manager.UpdateRightStickLookAround();
  check(mode.queried==1&&manager.RightStickLookAroundAngles.X==desiredX&&manager.RightStickLookAroundAngles.Y==desiredY);
  check(manager.RightStickTargetLookAroundAngles.X==values[tx]&&manager.RightStickTargetLookAroundAngles.Y==values[ty]);
  check(rotations==(active?4:0)&&normals==(active?2:0)&&sets==(active?1:0));
  if(!active)check(equal(manager.Camera.forward,receivers[0])&&equal(manager.Camera.up,axes[0]));
  ++cases;
 }
 check(cases==1250);std::puts("00697cc0_TEST PASS");return 0;
}