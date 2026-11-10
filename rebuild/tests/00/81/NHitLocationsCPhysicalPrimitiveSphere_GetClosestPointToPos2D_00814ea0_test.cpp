#include <cstdio>
#include <cmath>
// Original game constant0129ba3c; every fixture input below is binary-exact.
static const float DELTA_NOTIONAL_ZERO=0.0001f;
static int gNormaliseCalls=0;
class C3DVector { public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);} C3DVector operator+(const C3DVector& o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);} C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);} bool IsNonZero()const{return Z*Z+Y*Y+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;} C3DVector& AccurateNormalise(){++gNormaliseCalls;float f=1.0f/(float)std::sqrt(X*X+Y*Y+Z*Z);X*=f;Y*=f;Z*=f;return *this;} };
class CTCPhysicsBase { public:float Radius;float GetRadius()const{return Radius;} };
class CThing { public:C3DVector Origin;CTCPhysicsBase Physics;unsigned int Flags;CThing():Flags(0){} const CTCPhysicsBase* GetPhysicsTC()const{return &Physics;}unsigned int GetTypeFlags()const{return Flags;} };
class CThingCreatureBase:public CThing { public:float CombatRadius;float GetCombatRadius()const{return CombatRadius;} };
namespace NHitLocations {class CPhysicalPrimitiveSphere{public:float Radius;explicit CPhysicalPrimitiveSphere(float radius):Radius(radius){} C3DVector GetSphereBase(const CThing& thing)const{return thing.Origin;} float GetSphereRadius(const CThing& thing)const{const CTCPhysicsBase* physics=thing.GetPhysicsTC();float radius=Radius;if(radius<DELTA_NOTIONAL_ZERO){radius=physics->GetRadius();if(thing.GetTypeFlags()&8)radius=static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();}return radius;} virtual C3DVector GetClosestPointToPos2D(const CThing&,const C3DVector&,C3DVector&)const;};}
static bool Close(float a,float b){return std::fabs(a-b)<(1.0f/131072.0f);}
namespace NHitLocations {
C3DVector CPhysicalPrimitiveSphere::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& direction) const
{
    C3DVector centre = GetSphereBase(thing);
    float radius = GetSphereRadius(thing);
    C3DVector offset = pos - centre;
    offset.Z = 0.0f;
    if (offset.IsNonZero())
        offset.AccurateNormalise();
    direction = offset;
    return centre + offset * radius;
}
}

int main(){int ok=1,cases=0;float radii[4]={2.0f,0.0f,-1.0f,8.0f};C3DVector offsets[5]={C3DVector(3,4,8),C3DVector(-4,0,-2),C3DVector(0,0,16),C3DVector(1.0f/8192.0f,0,0),C3DVector(1.0f/16384.0f,0,0)};
for(int creature=0;creature<2;++creature)for(int r=0;r<4;++r)for(int i=0;i<5;++i){CThing plain;CThingCreatureBase living;plain.Origin=C3DVector(1,2,3);plain.Physics.Radius=4;living.Origin=plain.Origin;living.Physics.Radius=4;living.Flags=8;living.CombatRadius=6;const CThing& thing=creature?static_cast<const CThing&>(living):plain;NHitLocations::CPhysicalPrimitiveSphere sphere(radii[r]);C3DVector input=thing.Origin+offsets[i],direction;gNormaliseCalls=0;C3DVector result=sphere.GetClosestPointToPos2D(thing,input,direction);double dx=offsets[i].X,dy=offsets[i].Y,length=std::sqrt(dx*dx+dy*dy);bool normalized=length>DELTA_NOTIONAL_ZERO;double ux=normalized?dx/length:dx,uy=normalized?dy/length:dy;float radius=radii[r]>0?radii[r]:(creature?6.0f:4.0f);ok &= Close(direction.X,(float)ux)&&Close(direction.Y,(float)uy)&&direction.Z==0;ok &= Close(result.X,(float)(1+radius*ux))&&Close(result.Y,(float)(2+radius*uy))&&result.Z==3;ok &= gNormaliseCalls==(normalized?1:0);++cases;}
CThing t;t.Origin=C3DVector(0,0,2);t.Physics.Radius=4;NHitLocations::CPhysicalPrimitiveSphere sphere(2);C3DVector alias(4,0,9);C3DVector result=sphere.GetClosestPointToPos2D(t,alias,alias);ok &= alias.X==1&&alias.Y==0&&alias.Z==0&&result.X==2&&result.Z==2;ok &= cases==40;std::printf(ok?"00814ea0_TEST PASS\n":"00814ea0_TEST FAIL\n");return ok?0:1;}