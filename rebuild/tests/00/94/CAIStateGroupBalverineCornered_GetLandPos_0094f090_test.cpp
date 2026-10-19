#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define FABLE_ENGINE_CAISTATEGROUP_BALVERINECORNERED_H
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
struct CThingAICreature;
struct CAIBrain { CThingAICreature* Creature; };
struct CAIStateGroupBase { CAIBrain* PBrain; };
struct CTCPhysicsBase { float Radius; };
#endif
// CAIStateGroup_BalverineCornered::GetLandPos(CThing*) -- land beyond the target by 1.2 times the combined radii, at terrain height.
// Grade: RELOCATION_MATCH -- corrected no-argument world-map getter and subsequent position-taking height-query signatures reproduce all 223 bytes.
#include "engine/CAIStateGroup_BalverineCornered.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
struct C3DVector {
 float X,Y,Z;
 C3DVector() {}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z) {}
 C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z) {}
 C3DVector operator+(const C3DVector& b) const { return C3DVector(X+b.X,Y+b.Y,Z+b.Z); }
 C3DVector operator*(float factor) const { return C3DVector(X*factor,Y*factor,Z*factor); }
 void Normalise(); // 00A14440, called with no arguments; propagated GetScaled signature disagrees.
};
struct CGeometryThing {
 unsigned char _beforePhysics[0x60]; CTCPhysicsBase* PhysicsTC;
 const C3DVector& GetPosition() const; // 004C73D0
};
struct CWorldMapHeightMethods { float GetHeightZBelowPos(const C3DVector&); }; // 0063E480; position argument is pushed before the preceding no-argument virtual getter.
struct CWorldHeightView {
 virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
 virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
 virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
 virtual void v12(); virtual CWorldMapHeightMethods& PeekWorldMap() const; // slot13, CWorld RTTI vtable012390f0 ->004AE8C0, no stack arguments
};
struct CBalverineLandMethods : CAIStateGroupBase {
 CWorldHeightView* GetPWorld(); // 008FCFC0
 C3DVector GetLandPos(CGeometryThing*);
};
C3DVector CBalverineLandMethods::GetLandPos(CGeometryThing* target) {
 float radius=(((CGeometryThing*)PBrain->Creature)->PhysicsTC->Radius+target->PhysicsTC->Radius)*1.2f;
 C3DVector targetPos=target->GetPosition();
 const C3DVector& mine=((CGeometryThing*)PBrain->Creature)->GetPosition();
 C3DVector direction(targetPos.X-mine.X,targetPos.Y-mine.Y,0.0f);
 direction.Normalise();
 C3DVector result=targetPos+direction*radius;
 result.Z=GetPWorld()->PeekWorldMap().GetHeightZBelowPos(result);
 return result;
}

static CGeometryThing* expectedMe;
static CGeometryThing* expectedTarget;
static CBalverineLandMethods* expectedState;
static CWorldHeightView* expectedWorld;
static CWorldMapHeightMethods* expectedMap;
static C3DVector myPosition,targetPosition,sampledPosition;
static float groundHeight;
static int events[8], eventCount,errors;
static void event(int n) { if(eventCount<8)events[eventCount++]=n;else ++errors; }
const C3DVector& CGeometryThing::GetPosition() const {
 if(this==expectedTarget) {event(1);return targetPosition;}
 if(this!=expectedMe)++errors;event(2);return myPosition;
}
void C3DVector::Normalise() {
 event(3);if(Z!=0 || X!=targetPosition.X-myPosition.X || Y!=targetPosition.Y-myPosition.Y)++errors;
 float inverse=(float)(1.0/std::sqrt(X*X+Y*Y+Z*Z));X*=inverse;Y*=inverse;Z*=inverse;
}
CWorldHeightView* CBalverineLandMethods::GetPWorld() {event(4);if(this!=expectedState)++errors;return expectedWorld;}
void CWorldHeightView::v00() {} void CWorldHeightView::v01() {} void CWorldHeightView::v02() {}
void CWorldHeightView::v03() {} void CWorldHeightView::v04() {} void CWorldHeightView::v05() {}
void CWorldHeightView::v06() {} void CWorldHeightView::v07() {} void CWorldHeightView::v08() {}
void CWorldHeightView::v09() {} void CWorldHeightView::v10() {} void CWorldHeightView::v11() {}
void CWorldHeightView::v12() {}
CWorldMapHeightMethods& CWorldHeightView::PeekWorldMap() const {event(5);if(this!=expectedWorld)++errors;return *expectedMap;}
float CWorldMapHeightMethods::GetHeightZBelowPos(const C3DVector& position) {event(6);if(this!=expectedMap)++errors;sampledPosition=position;return groundHeight;}
static bool near(float a,float b) { return std::fabs(a-b)<.0001f; }
static bool run(float tx,float ty,float tz,float mx,float my,float mz,float r1,float r2,float height,float x,float y) {
 CGeometryThing me,target;CTCPhysicsBase p1,p2;CAIBrain brain;CBalverineLandMethods state;CWorldHeightView world;CWorldMapHeightMethods map;
 me.PhysicsTC=&p1;target.PhysicsTC=&p2;p1.Radius=r1;p2.Radius=r2;brain.Creature=(CThingAICreature*)&me;state.PBrain=&brain;
 expectedMe=&me;expectedTarget=&target;expectedState=&state;expectedWorld=&world;expectedMap=&map;
 myPosition=C3DVector(mx,my,mz);targetPosition=C3DVector(tx,ty,tz);groundHeight=height;
 eventCount=errors=0;C3DVector result=state.GetLandPos(&target);
 if(errors || eventCount!=6 || !near(result.X,x)||!near(result.Y,y)||result.Z!=height || !near(sampledPosition.X,x)||!near(sampledPosition.Y,y)||sampledPosition.Z!=tz) return false;
 for(int i=0;i<6;++i)if(events[i]!=i+1)return false;
 return targetPosition.X==tx&&targetPosition.Y==ty&&targetPosition.Z==tz&&myPosition.X==mx&&myPosition.Y==my&&myPosition.Z==mz;
}
int main() {
 if(!run(3,4,10,0,0,-7,.5f,.75f,42,3.9f,5.2f) ||
    !run(5,2,99,1,2,3,0,0,-10,5,2) ||
    !run(1,6,-20,1,1,0,1,1,7,1,8.4f) ||
    !run(-2,-2,4,1,2,100,.5f,.75f,0,-2.9f,-3.2f)) {std::printf("0094f090_TEST FAIL\n");return 1;}
 std::printf("0094f090_TEST PASS\n");return 0;
}