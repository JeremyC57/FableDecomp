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
class CPCMouseLook;
class TestManager {
public: CPCMouseLook* mode; bool compatible;
    CPCMouseLook* GetCameraMode()const{return mode;}
    bool GetCameraMode(CPCMouseLook** result)const{if(!compatible)return false;*result=mode;return true;}
};
struct CSubComponentBase {CPCMouseLook* owner;TestManager* manager;};
struct CPhysicsTargetModelGoTo {};struct CPhysicsForceModelRigid {};struct CPhysicsCoordRigid {};
template<class A,class B,class C> struct CPhysicsLeaf : CSubComponentBase {
    CPhysicsLeaf(CPCMouseLook& o,TestManager& m){owner=&o;manager=&m;}
};
template<class T>struct CCountedPointer {T* data;explicit CCountedPointer(T* ptr):data(ptr){}};
class CPCMouseLook {
public:
    C3DVector AxisX,AxisY,AxisZ;Snapshot StateCurrent;Thing thing;CameraDef def;
    TestManager manager;bool added;int calls;CPCMouseLook():added(false),calls(0){}
    const Thing* GetObservedThing(){++calls;return &thing;}
    TestManager& GetCameraManager(){return manager;}
    const CameraDef* GetCameraModeDef()const{return &def;}
    void AddSubComponent(CCountedPointer<CSubComponentBase> p,bool flag){
        added=p.data && p.data->owner==this && p.data->manager==&manager && !flag;
        delete static_cast<CPhysicsLeaf<CPhysicsTargetModelGoTo,CPhysicsForceModelRigid,CPhysicsCoordRigid>*>(p.data);
    }
    void OnInit();
};
void CPCMouseLook::OnInit()
{
    StateCurrent.ViewPosOffset = C3DVector(0,0,0);
    CPCMouseLook* pold_mode = 0;
    if (!GetCameraManager().GetCameraMode()) {
        C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
        AxisZ = C3DVector(0.0f,0.0f,1.0f);
        AxisX = facing * -1.0f;
        AxisY = AxisX.Cross(AxisZ);
        StateCurrent.AngleXY = 0.0f;
        StateCurrent.AngleUpDown = 0.125f;
        StateCurrent.DollyLength = GetCameraModeDef()->DollyLength;
    } else if (GetCameraManager().GetCameraMode(&pold_mode)) {
        AxisZ = pold_mode->AxisZ;
        AxisX = pold_mode->AxisX;
        AxisY = pold_mode->AxisY;
        StateCurrent.AngleXY = pold_mode->StateCurrent.AngleXY;
        StateCurrent.AngleUpDown = pold_mode->StateCurrent.AngleUpDown;
        StateCurrent.DollyLength = pold_mode->StateCurrent.DollyLength;
    } else {
        C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
        AxisZ = C3DVector(0.0f,0.0f,1.0f);
        AxisX = facing * -1.0f;
        AxisY = AxisX.Cross(AxisZ);
        StateCurrent.AngleXY = 0.0f;
        StateCurrent.AngleUpDown = 0.125f;
        StateCurrent.DollyLength = GetCameraModeDef()->DollyLength;
    }
    AddSubComponent(CCountedPointer<CSubComponentBase>(
        new CPhysicsLeaf<CPhysicsTargetModelGoTo,CPhysicsForceModelRigid,CPhysicsCoordRigid>(*this,GetCameraManager())),false);
}

bool equal(const C3DVector& v,float x,float y,float z){return v.X==x&&v.Y==y&&v.Z==z;}
int main(){
    CPCMouseLook old;old.AxisX=C3DVector(1,2,3);old.AxisY=C3DVector(4,5,6);old.AxisZ=C3DVector(7,8,9);
    old.StateCurrent.AngleXY=0.375f;old.StateCurrent.AngleUpDown=-0.25f;old.StateCurrent.DollyLength=48;
    for(int k=0;k<3;++k){
        CPCMouseLook m;m.manager.mode=k?&old:0;m.manager.compatible=k==2;
        m.StateCurrent.ViewPosOffset=C3DVector(2,4,6);m.thing.body.direction=C3DVector(0,1,0);
        m.def.DollyLength=13; m.OnInit();
        if(!m.added||!equal(m.StateCurrent.ViewPosOffset,0,0,0)||m.calls!=(k==2?0:1))return 1;
        if(k==2){
            if(!equal(m.AxisX,1,2,3)||!equal(m.AxisY,4,5,6)||!equal(m.AxisZ,7,8,9)||
               m.StateCurrent.AngleXY!=0.375f||m.StateCurrent.AngleUpDown!=-0.25f||m.StateCurrent.DollyLength!=48)return 2;
        } else if(!equal(m.AxisX,0,-1,0)||!equal(m.AxisY,-1,0,0)||!equal(m.AxisZ,0,0,1)||
              m.StateCurrent.AngleXY!=0||m.StateCurrent.AngleUpDown!=0.125f||m.StateCurrent.DollyLength!=13)return 3;
    }
    puts("00874790_TEST PASS");return 0;
}