#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
static void check(bool value){if(!value)std::abort();}
static unsigned int GFastOneOverSqrtTable[128];
static unsigned int Bits(float f){unsigned int b;std::memcpy(&b,&f,4);return b;}
static float Float(unsigned int b){float f;std::memcpy(&f,&b,4);return f;}
static void InitTable(){for(unsigned int i=0;i<128;++i){float r=(float)(1.0/std::sqrt((double)Float((i|0x1f80U)<<17)));GFastOneOverSqrtTable[i]=((Bits(r)+0x2000U)>>15)&255;}GFastOneOverSqrtTable[64]=255;}
static float GFFastOneOverSqrt(float n){unsigned int b=Bits(n);unsigned int index=(b>>17)&127;unsigned int seed=(((0x17cU-((b>>23)&255))>>1)<<23)|(GFastOneOverSqrtTable[index]<<15);float r=Float(seed);return (3.0f-r*r*n)*r*0.5f;}
#include <string>
static std::string trace;
static float GFSqr(float f){return f*f;}
static double GFCos(float f){return std::cos(f*(double)6.2831855f);}
class C3DVector{ public:
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt((GFSqr(X)+GFSqr(Y)+GFSqr(Z)));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
 const float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}
};
class CThing{ public:C3DVector pos;char tag;const C3DVector& GetPos()const{trace+=tag;return pos;}};
class CTCPerceiveThing;
static CTCPerceiveThing* active;
static bool expire;
class CEnvironment{ public:C3DVector wind;const C3DVector& PeekWindDirection()const;};
class CWorld{ public:CEnvironment* environment;const CEnvironment& PeekEnvironment()const{return *environment;}};
static CWorld world;
class CTCPerceiveThing{ public:
 CThing* parent;const CThing* perceived;
 float DistanceFromPerceivedThingSquared,SmellRadius;
 bool Smelt,LineOfSight,DirectAwareness;
 long FramesSinceDirectlyPerceivedThing;int lastCalls;
 float GetSmellRadius()const{return SmellRadius;}
 const CThing* GetPerceivedThing()const{trace+='P';return perceived;}
 const CWorld& PeekWorld()const{trace+='W';return world;}
 CThing& GetParentThing(){return *parent;}
 bool IsThingLineOfSight()const{return LineOfSight;}
 void SetLastAwareInfo(){trace+='A';++lastCalls;check(DirectAwareness&&FramesSinceDirectlyPerceivedThing==0);}
 void SetAsDirectlyAwareOfPerceivedThing(){if(GetPerceivedThing()){DirectAwareness=true;FramesSinceDirectlyPerceivedThing=0;SetLastAwareInfo();}}
 void UpdateSmell();
};
const C3DVector& CEnvironment::PeekWindDirection()const{trace+='V';if(expire)active->perceived=0;return wind;}
void CTCPerceiveThing::UpdateSmell()
{
 float distance_sq=GFSqr(GetSmellRadius());
 Smelt=false;
 if(DistanceFromPerceivedThingSquared<distance_sq) {
  const CThing* pperceived_thing=GetPerceivedThing();
  C3DVector wind_direction=PeekWorld().PeekEnvironment().PeekWindDirection();
  C3DVector between=GetParentThing().GetPos()-pperceived_thing->GetPos();
  between.Normalise();
  float cos_smell_angle_cone=GFCos(70.0f/360.0f);
  if(wind_direction.Dot(between)>cos_smell_angle_cone) {
   Smelt=true;
   if(!IsThingLineOfSight()) SetAsDirectlyAwareOfPerceivedThing();
  }
 }
}

int main(){
 check(sizeof(float)==4&&sizeof(unsigned int)==4);InitTable();
 const C3DVector directions[]={C3DVector(1,0,0),C3DVector(-1,0,0),C3DVector(0,1,0),C3DVector(0,0,1),C3DVector(3,4,0),C3DVector(0,0,0)};
 const C3DVector winds[]={C3DVector(1,0,0),C3DVector(-1,0,0),C3DVector(0,1,0),C3DVector(0,0,1),C3DVector(0.25f,0,0),C3DVector(0.5f,0,0),C3DVector(0,0,0)};
 const float distances[]={-1,0,4,16,17};const float radii[]={0,2,4,-2};int cases=0;
 for(int d=0;d<6;++d)for(int w=0;w<7;++w)for(int ds=0;ds<5;++ds)for(int r=0;r<4;++r)
 for(int flags=0;flags<16;++flags){
  CThing owner,target;owner.tag='O';target.tag='T';target.pos=C3DVector(8,-4,2);owner.pos=C3DVector(8+directions[d].X,-4+directions[d].Y,2+directions[d].Z);
  CEnvironment env;env.wind=winds[w];world.environment=&env;CTCPerceiveThing s;s.parent=&owner;s.perceived=&target;s.DistanceFromPerceivedThingSquared=distances[ds];s.SmellRadius=radii[r];s.Smelt=(flags&8)!=0;s.LineOfSight=(flags&1)!=0;s.DirectAwareness=(flags&2)!=0;s.FramesSinceDirectlyPerceivedThing=32;s.lastCalls=0;expire=(flags&4)!=0;active=&s;
  bool inside=distances[ds]<radii[r]*radii[r];if(!inside&&(flags&8))s.perceived=0;
  trace="";s.UpdateSmell();
  double x=directions[d].X,y=directions[d].Y,z=directions[d].Z;double length=std::sqrt(x*x+y*y+z*z);if(length==0)length=1;
  double dot=(x*winds[w].X+y*winds[w].Y+z*winds[w].Z)/length;
  bool smelt=inside&&dot>std::cos(70.0/360.0*2.0*std::acos(-1.0));
  bool aware=smelt&&!s.LineOfSight&&!expire;
  check(s.Smelt==smelt&&s.DirectAwareness==(aware||((flags&2)!=0))&&s.FramesSinceDirectlyPerceivedThing==(aware?0:32)&&s.lastCalls==(aware?1:0));
  if(!inside)check(trace=="");else{
   check(trace.substr(0,3)=="PWV");check(trace.substr(3,2)=="TO"||trace.substr(3,2)=="OT");
   std::string suffix=trace.substr(5);check(suffix==(smelt&&!s.LineOfSight?(expire?"P":"PA"):""));
   check(s.perceived==(expire?0:&target));
  }
  check(owner.pos.X==8+directions[d].X&&target.pos.X==8&&env.wind.X==winds[w].X);++cases;
 }
 check(cases==13440);std::puts("00744b50_TEST PASS");return 0;
}