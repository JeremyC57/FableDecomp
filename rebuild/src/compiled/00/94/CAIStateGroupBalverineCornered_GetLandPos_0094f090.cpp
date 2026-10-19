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