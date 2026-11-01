// 00a87260 CSB::ShouldIgnoreAgent
// Leaf: ?ShouldIgnoreAgent@CSB@@QAE_NPBVCSteeringAgent@@00@Z
// Flags:
#include <math.h>
#define CSB CSBFields
#include "engine/CSB.h"
#undef CSB
class C2DVector{public:float X,Y;};class C3DVector{public:float X,Y,Z;};
class CSteeringAgent;
namespace std{template<class T>class allocator;template<class T,class A>class vector;}
class CNavigatorExternalInterface{public:
virtual bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const=0; // slot 0
virtual float GetTerrainDesirabilityAt(const C3DVector&) const=0; // slot 1
virtual float GetTerrainHeightAt(const C3DVector&) const=0; // slot 2
virtual float GetThemeHeightAt(const C3DVector&) const=0; // slot 3
virtual float FindPosZAt(const C3DVector&,bool) const=0; // slot 4
virtual float FindPosZAtIncludingThemes(const C3DVector&,bool) const=0; // slot 5
virtual float PeekNavigatorRadius(bool) const=0; // slot 6
virtual C2DVector PeekNavigatorOrientation() const=0; // slot 7
virtual C2DVector PeekNavigatorPosition2D() const=0; // slot 8
virtual C3DVector PeekNavigatorPosition3D() const=0; // slot 9
virtual float PeekNavigatorMaxSpeed(const C3DVector&) const=0; // slot 10
virtual const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const=0; // slot 12
virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles()=0; // slot 11
virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles()=0; // slot 13
virtual const CSteeringAgent* GetPThingToNotAvoid() const=0; // slot 14
virtual float GetProximityForDirectApproach() const=0; // slot 15
virtual long GetNavigationLevelAt(const C3DVector&) const=0; // slot 16
virtual long GetConstantFPS() const=0; // slot 17
virtual float GetMinPathCost() const=0; // slot 18
virtual float GetMaxPathCost() const=0; // slot 19
virtual bool IsPlayer() const=0; // slot 20
virtual bool IsDead() const=0; // slot 21
virtual float GetForwardNavigationWidthExtension() const=0; // slot 22
virtual ~CNavigatorExternalInterface();
virtual CNavigatorExternalInterface* Clone() const=0; // slot 24
};

// Layout: three retail lists shrink8 bytes each; donor flags+78/+79/+7c become native+60/+61/+64.
class CSteeringAgent{
 const CNavigatorExternalInterface*NavigatorInterface;unsigned char _pad_0x4[0x60-4];bool AvoidDynamicObstacles,AvoidDynamicObstaclesUntilNextWaypoint,HasWanderedDuringThisSection,FailPath,OnDirectApproach;
 public:
 bool IsOnDirectApproach()const{return OnDirectApproach;}
 bool IsPlayer()const{return NavigatorInterface->IsPlayer();}
 bool GetAvoidDynamicObstacles()const{return AvoidDynamicObstacles&&AvoidDynamicObstaclesUntilNextWaypoint;}
 bool GetAvoidPlayer()const{return AvoidDynamicObstacles;}
 C3DVector GetPosition3D()const{return NavigatorInterface->PeekNavigatorPosition3D();}
};
class CSB:public CSBFields{public:bool ShouldIgnoreAgent(const CSteeringAgent*,const CSteeringAgent*,const CSteeringAgent*);};
bool CSB::ShouldIgnoreAgent(const CSteeringAgent*self,const CSteeringAgent*other,const CSteeringAgent*ignore){
 static float min_height_diff=1.5f;
 if(other==self||other==ignore)return true;
 if(self->IsOnDirectApproach()&&!other->IsPlayer())return true;
 if(!self->GetAvoidDynamicObstacles()&&(!self->GetAvoidPlayer()||!other->IsPlayer()))return true;
 C3DVector selfPos=self->GetPosition3D();C3DVector otherPos=other->GetPosition3D();
 if(fabsf(selfPos.Z-otherPos.Z)>min_height_diff)return true;
 return false;
}