// 00666d40 CThingCreatureBase::GetHeightZBelowAt
// Leaf: ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z
// Flags:
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CThingSearchTools CThingSearchToolsFields
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#define CWorldMap CWorldMapFields
#include "engine/CWorldMap.h"
#undef CWorldMap
class C3DVector{public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;};
class CTCPhysicsBase:public CTCPhysicsBaseFields{protected:using CTCPhysicsBaseFields::Radius;public:float GetRadius()const{return Radius;}};
class CWorldMap:public CWorldMapFields{public:float GetGroundSizeZAt(const C3DVector&)const;float GetThemeSizeZAt(const C3DVector&)const;};
class CThingSearchTools:public CThingSearchToolsFields{public:template<class T,class F,class O>bool ForEachInRoughArea(const C2DBoxI&,const F&,const O&)const;};
// Layout: supplied CThing prefix60 physics pointer and original protected const service accessors.
class CThing:public CThingFields{protected:const CWorldMap&PeekWorldMap()const;const CThingSearchTools&PeekThingSearchTools()const;};
class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};
class CThingCreatureBase:public CThingGameObject{public:float GetHeightZBelowAt(const C3DVector&,bool,bool)const;};
namespace std{template<class A,class R>struct unary_function{};}
class CTCSolidToThing;
class CThingFilter_SolidToThing:public std::unary_function<CThing,bool>{private:const CThing&SourceThing;const CTCSolidToThing*PTCSolidToThing;public:CThingFilter_SolidToThing(const CThing&t):SourceThing(t),PTCSolidToThing(0){}};
class CThingFilter_PhysicsFlagsSolidToThing:public std::unary_function<CThing,bool>{private:const CThing&SourceThing;const CTCPhysicsBase*SourceThingPhysicsTC;public:CThingFilter_PhysicsFlagsSolidToThing(const CThing&t):SourceThing(t),SourceThingPhysicsTC(t.PhysicsTC){}};
class CThingFilter_OverlappingCircle:public std::unary_function<CThing,bool>{private:C3DVector SpherePos;float Radius;public:CThingFilter_OverlappingCircle(const C3DVector&p,float r):SpherePos(p),Radius(r){}};
template<class A,class B,class C>class CFilter_Bind3{protected:const A&F1;const B&F2;const C&F3;public:CFilter_Bind3(const A&a,const B&b,const C&c):F1(a),F2(b),F3(c){}};
template<class A,class B,class C>inline CFilter_Bind3<A,B,C> __fastcall GFFilter_Bind3(const A&a,const B&b,const C&c){return CFilter_Bind3<A,B,C>(a,b,c);}
namespace{class CThingFindHeightZBelowAt{private:const C3DVector&From;const C3DVector&To;mutable float Height;public:CThingFindHeightZBelowAt(const C3DVector&f,const C3DVector&t,float h):From(f),To(t),Height(h){}float GetHeight()const{return Height;}bool operator()(const CThing&)const;};}
float CThingCreatureBase::GetHeightZBelowAt(const C3DVector&pos,bool alternateGround,bool includeThings)const{
 float radius=PhysicsTC->GetRadius();C2DBoxI area;
 area.TLX=(long)(pos.X-radius-1.0f);area.TLY=(long)(pos.Y-radius-1.0f);area.BRX=(long)(pos.X+radius+1.0f);area.BRY=(long)(pos.Y+radius+1.0f);
 float height=alternateGround?PeekWorldMap().GetThemeSizeZAt(pos):PeekWorldMap().GetGroundSizeZAt(pos);
 if(includeThings){C3DVector top(pos.X,pos.Y,pos.Z+radius+1.0f);C3DVector bottom(pos.X,pos.Y,pos.Z+radius-50.0f);CThingFindHeightZBelowAt finder(top,bottom,height);
 PeekThingSearchTools().ForEachInRoughArea<CThingPhysical>(area,GFFilter_Bind3(CThingFilter_SolidToThing(*this),CThingFilter_PhysicsFlagsSolidToThing(*this),CThingFilter_OverlappingCircle(pos,radius)),finder);
 height=finder.GetHeight();}
 return height;
}
