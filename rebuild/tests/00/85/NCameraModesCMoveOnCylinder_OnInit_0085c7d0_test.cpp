#include <cstdio>
#include <cstdlib>
static void check(bool value){if(!value)std::abort();}
static int phase=0,constructed=0,destroyed=0;
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}const C3DVector Cross(const C3DVector& o)const{return C3DVector(Y*o.Z-Z*o.Y,(Z*o.X-X*o.Z),X*o.Y-Y*o.X);}};
class CCamera{ public:float data[14];};
class CGameCameraManager{ public:CCamera Camera,OldCamera;bool SingleCut;int cuts;void SetSingleCut(){check(phase==4);phase=5;OldCamera=Camera;SingleCut=true;++cuts;}};
struct Physics{C3DVector facing;C3DVector GetFacingVector()const{check(phase==3);phase=4;return facing;}};
class CThingGameObject{ public:Physics* physics;const Physics* GetPhysicsTC()const{check(phase==2);phase=3;return physics;}};
template<class T>class CCountedPointer{ public:
 T* Data;int* count;
 CCountedPointer(T* p=0):Data(p),count(p?new int(1):0){}
 CCountedPointer(const CCountedPointer& o):Data(o.Data),count(o.count){if(count)++*count;}
 ~CCountedPointer(){if(count&&!--*count){delete Data;delete count;}}
 CCountedPointer& operator=(const CCountedPointer& o){check(!Data);Data=o.Data;count=o.count;if(count)++*count;return *this;}
};
namespace NCameraModes{
class CBase;
class CSubComponentBase{ public:CBase& owner;const CGameCameraManager& manager;CSubComponentBase(CBase& o,const CGameCameraManager& m):owner(o),manager(m){}virtual ~CSubComponentBase(){++destroyed;}};
class CPhysicsTargetModelGoTo{ public:};class CPhysicsForceModelRigid{ public:};class CPhysicsCoordRigid{ public:};
template<class T,class F,class C>struct CPhysicsLeaf:CSubComponentBase{CPhysicsLeaf(CBase& o,const CGameCameraManager& m):CSubComponentBase(o,m){check(phase==0);++constructed;}};
class CBase{ public:
 CGameCameraManager& manager;const CThingGameObject* thing;CCountedPointer<CSubComponentBase> held;int adds,observations;
 CBase(CGameCameraManager& m,const CThingGameObject* t):manager(m),thing(t),adds(0),observations(0){}
 CGameCameraManager& GetCameraManager(){return manager;}
 void AddSubComponent(CCountedPointer<CSubComponentBase> p,bool replace){check(!replace&&phase==0&&p.Data&&&p.Data->owner==this&&&p.Data->manager==&manager);phase=1;held=p;++adds;}
 const CThingGameObject* GetObservedThing()const{check(phase==1);phase=2;++const_cast<CBase*>(this)->observations;return thing;}
};
struct CMoveOnCylinder:CBase{
 C3DVector AxisX,AxisY,AxisZ;float Theta,HeightParam;
 CMoveOnCylinder(CGameCameraManager& m,const CThingGameObject* t):CBase(m,t),AxisX(8,8,8),AxisY(8,8,8),AxisZ(8,8,8),Theta(8),HeightParam(8){}
 void OnInit();
};
}
void NCameraModes::CMoveOnCylinder::OnInit()
{
 AddSubComponent(CCountedPointer<CSubComponentBase>(new CPhysicsLeaf<CPhysicsTargetModelGoTo,CPhysicsForceModelRigid,CPhysicsCoordRigid>(*this,GetCameraManager())),false);
 AxisX=GetObservedThing()->GetPhysicsTC()->GetFacingVector();
 AxisZ=C3DVector(0.0f,0.0f,1.0f);
 AxisY=AxisX.Cross(AxisZ);
 Theta=0.0f;
 HeightParam=0.75f;
 GetCameraManager().SetSingleCut();
}

int main(){
 const float coordinates[]={-2.0f,-0.5f,0.0f,0.5f,2.0f};int cases=0;
 for(int seed=0;seed<8;++seed)for(int x=0;x<5;++x)for(int y=0;y<5;++y)for(int z=0;z<5;++z){
  CGameCameraManager manager;manager.SingleCut=false;manager.cuts=0;
  for(int i=0;i<14;++i){manager.Camera.data[i]=float(seed*16+i)*0.25f;manager.OldCamera.data[i]=-8;}
  Physics physics;physics.facing=C3DVector(coordinates[x],coordinates[y],coordinates[z]);CThingGameObject thing;thing.physics=&physics;
  phase=0;int before=destroyed;
  {
   NCameraModes::CMoveOnCylinder mode(manager,&thing);mode.OnInit();
   check(phase==5&&mode.adds==1&&mode.observations==1&&constructed==cases+1&&destroyed==before);
   check(mode.AxisX.X==coordinates[x]&&mode.AxisX.Y==coordinates[y]&&mode.AxisX.Z==coordinates[z]);
   check(mode.AxisY.X==coordinates[y]&&mode.AxisY.Y==-coordinates[x]&&mode.AxisY.Z==0);
   check(mode.AxisZ.X==0&&mode.AxisZ.Y==0&&mode.AxisZ.Z==1&&mode.Theta==0&&mode.HeightParam==0.75f);
   check(manager.SingleCut&&manager.cuts==1);
   for(int j=0;j<14;++j)check(manager.OldCamera.data[j]==float(seed*16+j)*0.25f&&manager.Camera.data[j]==manager.OldCamera.data[j]);
  }
  check(destroyed==before+1);++cases;
 }
 check(cases==1000&&constructed==1000&&destroyed==1000);std::puts("0085c7d0_TEST PASS");return 0;
}