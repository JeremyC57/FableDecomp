#include <cstdio>
#include <cmath>
#include <list>
#include <utility>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}C3DVector operator/(float f)const{float inverse=1.0f/f;return C3DVector(X*inverse,Y*inverse,Z*inverse);}float GetSquaredMagnitude()const{return (X*X+Z*Z+Y*Y);}};
static float GFSqr(float n){return n*n;}static double GFTanh(double n){return tanh(n);}
static int gAverages;static C3DVector gCurrent,gNew;static float gWeight;
namespace NCameraModes {class CTools{public:template<class T>T GetRollingAverage(const T& current_val,const T& new_val,float weight)const{++gAverages;gCurrent=current_val;gNew=new_val;gWeight=weight;return (current_val*weight+new_val)/(weight+1.0);}};class CPhysicsState{public:C3DVector Position,Velocity,Acceleration;std::list<std::pair<C3DVector,float> > Attractors,IncidentalAttractors;bool CullPhysics;};class CBase{};}
class CGameCameraManager{public:NCameraModes::CTools CamTools;const NCameraModes::CTools& GetCameraTools()const{return CamTools;}};
namespace NCameraModes{class CPhysicsCoordModelBase{public:CPhysicsState& LinkedState;CBase& Owner;const CGameCameraManager& CameraManager;CPhysicsCoordModelBase(CPhysicsState& s,CBase& o,const CGameCameraManager& m):LinkedState(s),Owner(o),CameraManager(m){}const C3DVector& GetAcceleration()const{return LinkedState.Acceleration;}const C3DVector& GetPosition()const{return LinkedState.Position;}void SetPosition(const C3DVector& v){LinkedState.Position=v;}void SetVelocity(const C3DVector& v){LinkedState.Velocity=v;}const CGameCameraManager& GetCameraManager(){return CameraManager;}};class CPhysicsCoordRigid:public CPhysicsCoordModelBase{public:CPhysicsCoordRigid(CPhysicsState& s,CBase& o,const CGameCameraManager& m):CPhysicsCoordModelBase(s,o,m){}void FrameUpdate();};}
void NCameraModes::CPhysicsCoordRigid::FrameUpdate()
{
 C3DVector velocity;
 C3DVector wanted_pos=GetAcceleration();
 if(!LinkedState.CullPhysics){
  float arg=(wanted_pos-GetPosition()).GetSquaredMagnitude();
  float arg_scale=0.075f;
  float weight=GFSqr((float)(GFTanh(arg_scale*arg)))*5.0f;
  wanted_pos=GetCameraManager().GetCameraTools().GetRollingAverage(GetPosition(),wanted_pos,weight);
 }
 velocity=wanted_pos-GetPosition();
 LinkedState.CullPhysics=false;
 SetPosition(wanted_pos);
 SetVelocity(velocity);
}

static bool Near(double a,double b){return std::fabs(a-b)<1.0/16384.0;}
static bool Equal(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){int ok=1,cases=0;CGameCameraManager manager;NCameraModes::CBase owner;for(int x=-2;x<=2;++x)for(int y=-2;y<=2;++y)for(int z=-1;z<=1;++z)for(int dx=-2;dx<=2;++dx)for(int dy=-1;dy<=1;++dy)for(int dz=-1;dz<=1;++dz)for(int cull=0;cull<2;++cull){NCameraModes::CPhysicsState state;C3DVector before((float)x,(float)y,(float)z),target(float(x+dx),float(y+dy),float(z+dz));state.Position=before;state.Acceleration=target;state.Velocity=C3DVector(16,32,64);state.CullPhysics=cull!=0;state.Attractors.push_back(std::make_pair(C3DVector(1,2,4),8.0f));state.IncidentalAttractors.push_back(std::make_pair(C3DVector(2,4,8),16.0f));NCameraModes::CPhysicsCoordRigid coord(state,owner,manager);double distance=dx*dx+dy*dy+dz*dz;float t=(float)tanh(distance*(double)0.075f);float weight=t*t*5.0f;double px=cull?target.X:(before.X*(double)weight+target.X)/(weight+1.0),py=cull?target.Y:(before.Y*(double)weight+target.Y)/(weight+1.0),pz=cull?target.Z:(before.Z*(double)weight+target.Z)/(weight+1.0);gAverages=0;coord.FrameUpdate();ok &= !state.CullPhysics&&Near(state.Position.X,px)&&Near(state.Position.Y,py)&&Near(state.Position.Z,pz)&&Near(state.Velocity.X,px-before.X)&&Near(state.Velocity.Y,py-before.Y)&&Near(state.Velocity.Z,pz-before.Z)&&Equal(state.Acceleration,target)&&gAverages==(cull?0:1);if(!cull)ok &= Equal(gCurrent,before)&&Equal(gNew,target)&&Near(gWeight,weight);ok &= state.Attractors.size()==1&&state.IncidentalAttractors.size()==1&&state.Attractors.front().second==8&&state.IncidentalAttractors.front().second==16;++cases;}ok &= cases==6750;std::printf(ok?"008fcc60_TEST PASS\n":"008fcc60_TEST FAIL\n");return ok?0:1;}