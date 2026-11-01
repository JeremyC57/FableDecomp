// 00863ca0 behavior fixture for recovered full body; supplied assigned extent remains blocked.
// Selected target operations unchanged; raw pointer iterator declaration adapts to the fixture STL iterator.
// Real objects and standard C++03 containers; external search is checked.
// Reduced native declarations exercise behavior without claiming original dispatch ABI or parity.
#include <stdio.h>
#include <vector>
static int failures=0;static void Check(bool b){if(!b)++failures;}
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class CTCPhysicsBase{public:float Radius;mutable int Reads;CTCPhysicsBase(float r):Radius(r),Reads(0){}float GetRadius()const{++Reads;return Radius;}};
class CThingFields{public:CTCPhysicsBase*PhysicsTC;CThingFields(CTCPhysicsBase*p):PhysicsTC(p){}};
class CThing:public CThingFields{public:C3DVector Position;mutable int Reads;int refs,id;CThing(int n,CTCPhysicsBase*p=0):CThingFields(p),Reads(0),refs(0),id(n){}const C3DVector&GetPos()const{++Reads;return Position;}};
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
static int liveHandles=0;
template<class T>class CIntelligentPointer{T*p;public:explicit CIntelligentPointer(T*v):p(v){++liveHandles;if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){++liveHandles;if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;--liveHandles;}CIntelligentPointer&operator=(const CIntelligentPointer&o){if(p)--p->refs;p=o.p;if(p)++p->refs;return*this;}T*Get()const{return p;}};
class CThingFilter_HasPhysicsMesh{};
static CThing*found[3];static unsigned int foundCount;static long ex,ey,er;static int searches;
class CThingSearchTools{public:template<class T,class V,class F>void GetAllThingsInRoughArea(const C2DBoxI&b,const F&,V&v)const{++searches;Check(v.empty());Check(b.TLX==ex-er&&b.TLY==ey-er&&b.BRX==ex+er&&b.BRY==ey+er);for(unsigned int i=0;i<foundCount;++i)v.push_back(found[i]);}};
class CWorld{public:CThingSearchTools Tools;CThingSearchTools&GetThingSearchTools(){return Tools;}};
class CMainGameComponent{static CMainGameComponent*PMainGameComponent;CWorld World;public:CMainGameComponent(){PMainGameComponent=this;}static CMainGameComponent*Get(){return PMainGameComponent;}CWorld&GetWorld(){return World;}};CMainGameComponent*CMainGameComponent::PMainGameComponent=0;
class CThingBodyReorienter{public:bool BodyReorientationDone,ReorientingBody;long ReorientationCount;bool StillCheckingForNewBodyReorientationAngle,HaveFinalReorientationAngle;float FinalReorientationAngleXY;enum ERotationDirection{TestDirection};ERotationDirection RotationDirection;bool HasPossibleIntersectingThings;std::vector<CIntelligentPointer<CThing> >PossibleIntersectingThings;CThing&Body;explicit CThingBodyReorienter(CThing&b):BodyReorientationDone(false),ReorientingBody(true),ReorientationCount(7),StillCheckingForNewBodyReorientationAngle(false),HaveFinalReorientationAngle(true),FinalReorientationAngleXY(2.0f),RotationDirection(TestDirection),HasPossibleIntersectingThings(false),Body(b){}void FillBodyReorientationPossibleIntersecting();};
void CThingBodyReorienter::FillBodyReorientationPossibleIntersecting(){
 HasPossibleIntersectingThings=true;StillCheckingForNewBodyReorientationAngle=true;
 float size=reinterpret_cast<CThingFields&>(Body).PhysicsTC->GetRadius()*2.0f;if(size<1.0f)size=1.0f;
 const C3DVector&position=Body.GetPos();C2DCoordI center((long)position.X,(long)position.Y);long half=(long)size;
 C2DBoxI box(center,half);std::vector<CThing*>things;CThingFilter_HasPhysicsMesh filter;
 CMainGameComponent::Get()->GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<CThing>(box,filter,things);
 for(std::vector<CThing*>::iterator it=things.begin();it!=things.end();++it)PossibleIntersectingThings.push_back(CIntelligentPointer<CThing>(*it));
 if(PossibleIntersectingThings.empty()){BodyReorientationDone=true;StillCheckingForNewBodyReorientationAngle=false;HasPossibleIntersectingThings=false;}
}

int main(){CMainGameComponent game;unsigned int cases=0;float coordinates[8]={-7.5f,-2.0f,-0.5f,0.0f,0.5f,1.0f,3.5f,8.0f};float radii[5]={-2.0f,0.0f,0.25f,0.5f,2.0f};
for(int pos=0;pos<8;++pos)for(int radius=0;radius<5;++radius)for(unsigned int n=0;n<4;++n)for(unsigned int previous=0;previous<4;++previous)for(int duplicate=0;duplicate<2;++duplicate){
 Check(liveHandles==0);CTCPhysicsBase physics(radii[radius]);CThing body(0,&physics),a(1),b(2);body.Position=C3DVector(coordinates[pos],coordinates[7-pos],99.0f);foundCount=n;found[0]=&a;found[1]=duplicate?&a:&b;found[2]=0;ex=(long)body.Position.X;ey=(long)body.Position.Y;er=radius==4?4:1;searches=0;
 {CThingBodyReorienter r(body);for(unsigned int i=0;i<previous;++i)r.PossibleIntersectingThings.push_back(CIntelligentPointer<CThing>(i%2?0:&body));r.FillBodyReorientationPossibleIntersecting();Check(searches==1&&physics.Reads==1&&body.Reads==1);Check(r.PossibleIntersectingThings.size()==previous+n);for(unsigned int i=0;i<previous;++i)Check(r.PossibleIntersectingThings[i].Get()==(i%2?0:&body));for(unsigned int i=0;i<n;++i)Check(r.PossibleIntersectingThings[previous+i].Get()==found[i]);bool empty=previous+n==0;Check(r.BodyReorientationDone==empty&&r.StillCheckingForNewBodyReorientationAngle==!empty&&r.HasPossibleIntersectingThings==!empty);Check(r.ReorientingBody&&r.ReorientationCount==7&&r.HaveFinalReorientationAngle&&r.FinalReorientationAngleXY==2.0f&&r.RotationDirection==CThingBodyReorienter::TestDirection);Check(liveHandles==(int)(previous+n));Check(body.refs==(int)((previous+1)/2));Check(a.refs==(n>0?1:0)+(duplicate&&n>1?1:0));Check(b.refs==(!duplicate&&n>1?1:0));}
 Check(liveHandles==0&&body.refs==0&&a.refs==0&&b.refs==0);++cases;
}Check(cases==1280);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("00863ca0_TEST PASS");return 0;}
