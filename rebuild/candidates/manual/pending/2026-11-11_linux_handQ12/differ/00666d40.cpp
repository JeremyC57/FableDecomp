// 00666d40 CThingCreatureBase::GetHeightZBelowAt
// Grade: DIFFER(346v348) -- regraded as it sits (chunk Q12, header overlay on), not reworked: local model of classes that have or need shared headers -- the migration is the next step. Earlier diagnosis: box, ground queries and filter temporaries match; VC7.1 puts the finder below the top vector (retail: top 0x18, finder 0x24, bottom 0x30) and loads 1.0f before adding it to the CSE'd pos.Z+radius (retail fld st0/fxch/fadd [1.0]) CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool alternateGround, bool includeThings) const -- the height of whatever lies below `pos` for this creature: the world map's ground height there (one of two ground queries), optionally raised by the solid physical things overlapping the creature's circle in that area (a search from radius+1
// Next step: the double z local (this file) only reorders the later stores; the prior's all-float form is equally close and probably the faithful one. Try the real anonymous-namespace finder layout and ForEachInRoughArea body from the CThingCreatureBase TU, or treat frame order plus x87 dup as a lane gap.
// Leaf: ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z
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
 if(includeThings){double z=pos.Z+radius;C3DVector top(pos.X,pos.Y,(float)(z+1.0f));C3DVector bottom(pos.X,pos.Y,(float)(z-50.0f));CThingFindHeightZBelowAt finder(top,bottom,height);
 PeekThingSearchTools().ForEachInRoughArea<CThingPhysical>(area,GFFilter_Bind3(CThingFilter_SolidToThing(*this),CThingFilter_PhysicsFlagsSolidToThing(*this),CThingFilter_OverlappingCircle(pos,radius)),finder);
 height=finder.GetHeight();}
 return height;
}
