// Behavior-only fixture. Target and original filter/finder construction bodies
// are pasted unchanged. World-height and rough-search callees are checked
// boundaries; this fixture does not claim to implement the search algorithm.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int cases,worldCalls,searchCalls,raiseHeight;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
class C3DVector{public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;};
class CTCPhysicsBase{public:float Radius;float GetRadius()const{return Radius;}};
class CWorldMap{public:float Ground,Theme;float GetGroundSizeZAt(const C3DVector&)const;float GetThemeSizeZAt(const C3DVector&)const;};
class CThingSearchTools{public:template<class T,class F,class O>bool ForEachInRoughArea(const C2DBoxI&,const F&,const O&)const;};
static CWorldMap world;static CThingSearchTools search;static const C3DVector*expectedPos;static float expectedHeight,expectedRadius;static const void*expectedThing;
class CThing{public:CTCPhysicsBase*PhysicsTC;protected:const CWorldMap&PeekWorldMap()const{return world;}const CThingSearchTools&PeekThingSearchTools()const{return search;}};class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{public:float GetHeightZBelowAt(const C3DVector&,bool,bool)const;};
float CWorldMap::GetGroundSizeZAt(const C3DVector&p)const{CHECK(&p==expectedPos);++worldCalls;return Ground;}
float CWorldMap::GetThemeSizeZAt(const C3DVector&p)const{CHECK(&p==expectedPos);++worldCalls;return Theme;}
namespace std{template<class A,class R>struct unary_function{};}
class CTCSolidToThing;
class CThingFilter_SolidToThing:public std::unary_function<CThing,bool>{public:const CThing&SourceThing;const CTCSolidToThing*PTCSolidToThing;public:CThingFilter_SolidToThing(const CThing&t):SourceThing(t),PTCSolidToThing(0){}};
class CThingFilter_PhysicsFlagsSolidToThing:public std::unary_function<CThing,bool>{public:const CThing&SourceThing;const CTCPhysicsBase*SourceThingPhysicsTC;public:CThingFilter_PhysicsFlagsSolidToThing(const CThing&t):SourceThing(t),SourceThingPhysicsTC(t.PhysicsTC){}};
class CThingFilter_OverlappingCircle:public std::unary_function<CThing,bool>{public:C3DVector SpherePos;float Radius;public:CThingFilter_OverlappingCircle(const C3DVector&p,float r):SpherePos(p),Radius(r){}};
template<class A,class B,class C>class CFilter_Bind3{public:const A&F1;const B&F2;const C&F3;public:CFilter_Bind3(const A&a,const B&b,const C&c):F1(a),F2(b),F3(c){}};
template<class A,class B,class C>inline CFilter_Bind3<A,B,C> __fastcall GFFilter_Bind3(const A&a,const B&b,const C&c){return CFilter_Bind3<A,B,C>(a,b,c);}
namespace{class CThingFindHeightZBelowAt{public:const C3DVector&From;const C3DVector&To;mutable float Height;public:CThingFindHeightZBelowAt(const C3DVector&f,const C3DVector&t,float h):From(f),To(t),Height(h){}float GetHeight()const{return Height;}bool operator()(const CThing&)const;};}
float CThingCreatureBase::GetHeightZBelowAt(const C3DVector&pos,bool alternateGround,bool includeThings)const{
 float radius=PhysicsTC->GetRadius();C2DBoxI area;
 area.TLX=(long)(pos.X-radius-1.0f);area.TLY=(long)(pos.Y-radius-1.0f);area.BRX=(long)(pos.X+radius+1.0f);area.BRY=(long)(pos.Y+radius+1.0f);
 float height=alternateGround?PeekWorldMap().GetThemeSizeZAt(pos):PeekWorldMap().GetGroundSizeZAt(pos);
 if(includeThings){C3DVector top(pos.X,pos.Y,pos.Z+radius+1.0f);C3DVector bottom(pos.X,pos.Y,pos.Z+radius-50.0f);CThingFindHeightZBelowAt finder(top,bottom,height);
 PeekThingSearchTools().ForEachInRoughArea<CThingPhysical>(area,GFFilter_Bind3(CThingFilter_SolidToThing(*this),CThingFilter_PhysicsFlagsSolidToThing(*this),CThingFilter_OverlappingCircle(pos,radius)),finder);
 height=finder.GetHeight();}
 return height;
}

template<class T,class F,class O>bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI&area,const F&filter,const O&finder)const{
 CHECK(area.TLX==(long)(expectedPos->X-expectedRadius-1.0f)&&area.TLY==(long)(expectedPos->Y-expectedRadius-1.0f)&&area.BRX==(long)(expectedPos->X+expectedRadius+1.0f)&&area.BRY==(long)(expectedPos->Y+expectedRadius+1.0f));
 CHECK(&filter.F1.SourceThing==expectedThing&&!filter.F1.PTCSolidToThing);CHECK(&filter.F2.SourceThing==expectedThing&&filter.F2.SourceThingPhysicsTC==static_cast<const CThing*>(expectedThing)->PhysicsTC);
 CHECK(filter.F3.SpherePos.X==expectedPos->X&&filter.F3.SpherePos.Y==expectedPos->Y&&filter.F3.SpherePos.Z==expectedPos->Z&&filter.F3.Radius==expectedRadius);
 CHECK(finder.From.X==expectedPos->X&&finder.From.Y==expectedPos->Y&&finder.From.Z==expectedPos->Z+expectedRadius+1.0f&&finder.To.X==expectedPos->X&&finder.To.Y==expectedPos->Y&&finder.To.Z==expectedPos->Z+expectedRadius-50.0f&&finder.Height==expectedHeight);
 finder.Height+=raiseHeight?4.0f:0.0f;++searchCalls;return true;
}
int main(){const float coords[]={-8.5f,0.0f,8.5f};const float radii[]={0.0f,0.5f,1.0f,4.0f};
 for(int x=0;x<3;++x)for(int y=0;y<3;++y)for(int z=0;z<3;++z)for(int r=0;r<4;++r)for(int alt=0;alt<2;++alt)for(int things=0;things<2;++things)for(int raised=0;raised<2;++raised){
 CThingCreatureBase creature;CTCPhysicsBase physics;physics.Radius=radii[r];creature.PhysicsTC=&physics;C3DVector pos(coords[x],coords[y],coords[z]);expectedThing=&creature;expectedPos=&pos;expectedRadius=radii[r];world.Ground=-2.0f;world.Theme=6.0f;expectedHeight=alt?world.Theme:world.Ground;raiseHeight=raised;worldCalls=searchCalls=0;
 float result=creature.GetHeightZBelowAt(pos,alt!=0,things!=0);CHECK(result==expectedHeight+(things&&raised?4.0f:0.0f)&&worldCalls==1&&searchCalls==things);CHECK(pos.X==coords[x]&&pos.Y==coords[y]&&pos.Z==coords[z]&&physics.Radius==radii[r]);++cases;
 }
 CHECK(cases==864);puts("00666d40_TEST PASS");return 0;}
