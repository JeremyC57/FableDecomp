// 0087bf40 behavior fixture for the TRUE 295-byte extent body (extent/0087bf40.cpp pasted unchanged; agent680).
// Selected target and same-type handle assignment pasted unchanged. Real object lifetimes.
// Reduced unused interfaces and checked external methods do not prove native dispatch ABI.
#include "fable_compiler.h"
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int failures=0,normalises=0,observedCalls=0,posCalls=0,fovCalls=0,deleteCalls=0;
static void Check(bool b){if(!b)++failures;}
class C3DVector{public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator-(const class C2DVector&v)const;C3DVector&AccurateNormalise(){++normalises;float len=(float)sqrt(X*X+Y*Y+Z*Z);Check(len>0.0f);float factor=1.0f/len;X*=factor;Y*=factor;Z*=factor;return*this;}};
class C2DVector{public:float X,Y;C2DVector(){}inline C2DVector(const C3DVector&v);};
inline C2DVector::C2DVector(const C3DVector&v):X(v.X),Y(v.Y){}
C3DVector C3DVector::operator-(const C2DVector&v)const{C3DVector r;r.X=X-v.X;r.Y=Y-v.Y;r.Z=Z;return r;}
class CThingGameObject{public:C3DVector Position;const C3DVector&GetPos()const{++posCalls;Check(observedCalls==1&&normalises==0);return Position;}};
class CDefPointeeBase{public:int refs,deleted;CDefPointeeBase():refs(0),deleted(0){}virtual~CDefPointeeBase(){}void AddRef(){Check(!deleted);++refs;}unsigned long Release(){Check(refs>0);return (unsigned long)--refs;}virtual void Delete();};
class CCameraModeDef:public CDefPointeeBase{};
static CDefPointeeBase*deleteTrigger=0;static void (*deleteAction)()=0;
void CDefPointeeBase::Delete(){Check(refs==0&&!deleted);++deleted;++deleteCalls;if(this==deleteTrigger&&deleteAction){void(*f)()=deleteAction;deleteAction=0;f();}}
template<class T>class CDefPointer{public:CDefPointeeBase*Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer&o):Object(o.Object){if(Object)Object->AddRef();}~CDefPointer(){Release();}void Release(){if(Object){CDefPointeeBase*object=Object;if(object->Release()==0)object->Delete();Object=0;}}CDefPointer&operator=(const CDefPointer&);};
template<> FABLE_FORCEINLINE CDefPointer<const CCameraModeDef>&CDefPointer<const CCameraModeDef>::operator=(const CDefPointer<const CCameraModeDef>&other){if(Object!=other.Object){Release();Object=other.Object;if(Object)Object->AddRef();}return *this;}

namespace NCameraModes{class CBase;}
class CGameCameraManager{public:NCameraModes::CBase*Current;CGameCameraManager():Current(0){}NCameraModes::CBase*GetCameraMode(){return Current;}};
namespace NCameraModes{
class CPhysicsState{public:C3DVector Position;};
class CBase{protected:float DefaultFOV;CPhysicsState LookAt;private:CGameCameraManager&CameraManager;CDefPointer<const CCameraModeDef>PCameraModeDef;CPhysicsState GoTo;float ObservedPositionZOffset;const CThingGameObject*Observed;float FixtureFOV;
public:CBase(CGameCameraManager&m,const CThingGameObject*t):DefaultFOV(0),CameraManager(m),ObservedPositionZOffset(0),Observed(t),FixtureFOV(0){}virtual~CBase(){}virtual float GetDefaultFOV()const{++fovCalls;Check(normalises==2);return FixtureFOV;}const CThingGameObject*GetObservedThing()const{++observedCalls;Check(normalises==0);return Observed;}const C3DVector&GetPosition()const{return GoTo.Position;}const C3DVector&GetObservedPos()const{return LookAt.Position;}float GetObservedPositionZOffset()const{return ObservedPositionZOffset;}void SetObservedPositionZOffset(float f){ObservedPositionZOffset=f;}const CDefPointer<const CCameraModeDef>&GetCameraModeDef()const{return PCameraModeDef;}
protected:CGameCameraManager&GetCameraManager(){return CameraManager;}void SetCameraModeDef(const CDefPointer<const CCameraModeDef>&p){PCameraModeDef=p;}
public:void FixtureGeometry(const C3DVector&look,const C3DVector&go){LookAt.Position=look;GoTo.Position=go;}void FixtureDef(CDefPointeeBase*p){PCameraModeDef.Release();PCameraModeDef.Object=p;if(p)p->AddRef();}void FixtureValues(float f,float z){FixtureFOV=f;ObservedPositionZOffset=z;}float ResultFOV()const{return DefaultFOV;}
};
class CRotateAround:public CBase{private:float StringLengthXY,HeightOffset;C3DVector XAxis,YAxis;float Theta;protected:virtual void OnInit();public:CRotateAround(CGameCameraManager&m,const CThingGameObject*t):CBase(m,t),StringLengthXY(-1),HeightOffset(-1),Theta(9){}void Run(){OnInit();}void CheckResult(float length,float height,float x,float y)const{Check(StringLengthXY==length&&HeightOffset==height&&XAxis.X==x&&XAxis.Y==y&&XAxis.Z==0&&YAxis.X==y&&YAxis.Y==-x&&YAxis.Z==0&&Theta==0);}};
void CRotateAround::OnInit()
{
    C2DVector foc_pos(GetObservedPos());
    float dx = foc_pos.X - GetPosition().X;
    float dy = foc_pos.Y - GetPosition().Y;
    StringLengthXY = (float)sqrt(dx * dx + dy * dy);
    HeightOffset = GetPosition().Z - GetObservedThing()->GetPos().Z;
    C3DVector line = GetPosition() - foc_pos;
    line.Z = 0.0f;
    C3DVector& xAxis = XAxis;
    xAxis = line;
    xAxis.AccurateNormalise();
    line.X = xAxis.Y;
    line.Y = -xAxis.X;
    line.Z = 0.0f;
    YAxis = line;
    YAxis.AccurateNormalise();
    Theta = 0.0f;
    CBase* previous = GetCameraManager().GetCameraMode();
    DefaultFOV = previous->GetDefaultFOV();
    SetObservedPositionZOffset(previous->GetObservedPositionZOffset());
    SetCameraModeDef(previous->GetCameraModeDef());
}
}

static NCameraModes::CBase*changeCamera=0;static CDefPointeeBase*replacement=0;static int changed=0;
static void ChangeSource(){Check(normalises==2&&fovCalls==1);++changed;changeCamera->FixtureDef(replacement);}
int main(){unsigned int cases=0;const float positions[3]={-8.0f,0.0f,16.0f};const float lengths[3]={1.0f,2.0f,4.0f};
for(int direction=0;direction<4;++direction)for(int lengthIndex=0;lengthIndex<3;++lengthIndex)for(int position=0;position<3;++position)for(int height=0;height<2;++height)for(int self=0;self<2;++self)for(int initial=0;initial<3;++initial)for(int previous=0;previous<2;++previous)for(int mutate=0;mutate<2;++mutate){
 CCameraModeDef oldDef,newDef,thirdDef;CGameCameraManager manager;CThingGameObject thing;thing.Position=C3DVector(32,64,height?8.0f:-4.0f);normalises=observedCalls=posCalls=fovCalls=deleteCalls=changed=0;deleteTrigger=0;deleteAction=0;
 {NCameraModes::CBase previousMode(manager,&thing);NCameraModes::CRotateAround mode(manager,&thing);float x=direction==0?1.0f:direction==1?-1.0f:0.0f,y=direction==2?1.0f:direction==3?-1.0f:0.0f,length=lengths[lengthIndex];C3DVector look(positions[position],positions[2-position],4.0f),go(look.X+x*length,look.Y+y*length,height?16.0f:2.0f);mode.FixtureGeometry(look,go);previousMode.FixtureValues(64.0f,2.0f);mode.FixtureValues(32.0f,4.0f);previousMode.FixtureDef(previous?&newDef:0);mode.FixtureDef(initial==1?&oldDef:initial==2?&newDef:0);manager.Current=self?(NCameraModes::CBase*)&mode:&previousMode;
 bool doChange=!self&&initial==1&&mutate;changeCamera=&previousMode;replacement=&thirdDef;if(doChange){deleteTrigger=&oldDef;deleteAction=ChangeSource;}
 CDefPointeeBase*expected=self?mode.GetCameraModeDef().Object:doChange?&thirdDef:previous?&newDef:0;float expectedFov=self?32.0f:64.0f,expectedOffset=self?4.0f:2.0f;
 mode.Run();mode.CheckResult(length,go.Z-thing.Position.Z,x,y);Check(normalises==2&&observedCalls==1&&posCalls==1&&fovCalls==1);Check(mode.ResultFOV()==expectedFov&&mode.GetObservedPositionZOffset()==expectedOffset&&mode.GetCameraModeDef().Object==expected);Check(changed==(doChange?1:0));Check(mode.GetObservedPos().X==look.X&&mode.GetObservedPos().Y==look.Y&&mode.GetPosition().X==go.X&&mode.GetPosition().Y==go.Y&&mode.GetPosition().Z==go.Z);if(expected)Check(expected->refs>=1&&!expected->deleted);deleteTrigger=0;deleteAction=0;
 }Check(oldDef.refs==0&&newDef.refs==0&&thirdDef.refs==0);Check(oldDef.deleted<=1&&newDef.deleted<=1&&thirdDef.deleted<=1);++cases;
}Check(cases==1728);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("0087bf40_TEST PASS");return 0;}