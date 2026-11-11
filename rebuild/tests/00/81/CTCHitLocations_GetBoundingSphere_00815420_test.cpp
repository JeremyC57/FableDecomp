#include <vector>
#include <utility>
#include <cmath>
#include <cstdio>
#include <cstdlib>
static int cfg[8];
static void check_impl(bool b,int line){if(!b){std::printf("FAIL line %d cfg %d %d %d %d %d %d %d %d\n",line,cfg[0],cfg[1],cfg[2],cfg[3],cfg[4],cfg[5],cfg[6],cfg[7]);std::exit(1);}}
#define check(x) check_impl((x),__LINE__)
static const float DELTA_NOTIONAL_ZERO=0.0001f;
class C3DVector { public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}C3DVector operator-(const C3DVector&o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}C3DVector&operator+=(const C3DVector&o){X+=o.X;Y+=o.Y;Z+=o.Z;return *this;}C3DVector&operator/=(float s){float inv=1.0f/s;X*=inv;Y*=inv;Z*=inv;return *this;}float GetAccurateMagnitude()const{return float(std::sqrt(X*X+Y*Y+Z*Z));}};
static bool near(float a,float b){return std::fabs(a-b)<=1.0f/4096.0f;}
static bool equal(const C3DVector&a,const C3DVector&b){return near(a.X,b.X)&&near(a.Y,b.Y)&&near(a.Z,b.Z);}
class CThing{ public:C3DVector pos;const C3DVector&GetPos()const{return pos;}};
static std::vector<int>calls;
struct Primitive{int id,axis;float offset,radius;virtual void GetBoundingSphere(const CThing&o,C3DVector&c,float&r)const{calls.push_back(id);C3DVector v=o.pos;if(axis==0)v.X+=offset;else if(axis==1)v.Y+=offset;else v.Z+=offset;c=v;r=radius;}};
struct Init{Primitive*PPrimitive;};struct Def{Init PhysicalPrimitive;};
namespace NHitLocations{class CHitLocation{ public:CThing*POwnerThing;const Def*PDef;void GetBoundingSphere(C3DVector&c,float&r)const{PDef->PhysicalPrimitive.PPrimitive->GetBoundingSphere(*POwnerThing,c,r);}};}
class CTCHitLocations{ public:std::vector<std::pair<NHitLocations::CHitLocation,long> >HitLocations;CThing*parent;mutable C3DVector BoundingSphereOffsetFromPos;mutable float BoundingSphereRadius;mutable bool CachedBoundingSphere;const CThing&GetParentThing()const{return *parent;}void GetBoundingSphere(C3DVector&,float&,bool)const;};
struct Fixture{CThing owner;Primitive primitives[4];Def defs[4];CTCHitLocations h;C3DVector out;float rad;Fixture(int count,int profile,int radii,int axis,int pos,int cached){owner.pos=C3DVector(float(pos*16),float(pos*32),float(-pos*16));h.parent=&owner;h.BoundingSphereOffsetFromPos=C3DVector(8,16,32);h.BoundingSphereRadius=64;h.CachedBoundingSphere=cached!=0;out=C3DVector(-128,-64,-32);rad=-16;for(int i=0;i<count;++i){const float offsets[]={-16,0,8,32};const float rs[]={0,2,4,8};primitives[i].id=i;primitives[i].axis=axis;primitives[i].offset=offsets[(profile>>(2*i))&3];primitives[i].radius=rs[(radii+i)&3];defs[i].PhysicalPrimitive.PPrimitive=&primitives[i];NHitLocations::CHitLocation hl;hl.POwnerThing=&owner;hl.PDef=&defs[i];h.HitLocations.push_back(std::make_pair(hl,i));}}};
void CTCHitLocations::GetBoundingSphere(C3DVector& centre_pos,float& radius,bool accurate)const
{
 if(HitLocations.empty())return;
 if(!accurate&&CachedBoundingSphere) {
  const C3DVector& pos=GetParentThing().GetPos();
  centre_pos=pos+BoundingSphereOffsetFromPos;
  radius=BoundingSphereRadius;
  return;
 }
  std::vector<std::pair<NHitLocations::CHitLocation,long> >::const_iterator it=HitLocations.begin();
  (it++)->first.GetBoundingSphere(centre_pos,radius);
  for(;it!=HitLocations.end();++it) {
   C3DVector this_centre;
   float this_rad;
   it->first.GetBoundingSphere(this_centre,this_rad);
   C3DVector line_to_this_centre=this_centre-centre_pos;
   float centre_sep=line_to_this_centre.GetAccurateMagnitude();
   if(centre_sep>DELTA_NOTIONAL_ZERO)line_to_this_centre/=centre_sep;
   if(!(this_rad+centre_sep<radius)) {
    float new_radius=(centre_sep+this_rad+radius)/2.0f;
    float offset=new_radius-radius;
    if(offset<0.0f)offset=0.0f;
    centre_pos+=line_to_this_centre*offset;
    radius=new_radius;
   }
  }
  BoundingSphereOffsetFromPos=centre_pos-GetParentThing().GetPos();
  BoundingSphereRadius=radius;
  CachedBoundingSphere=true;
}

static void reference(Fixture&f,C3DVector&centre,float&radius,bool accurate){int n=int(f.h.HitLocations.size());if(!n)return;
 if(!accurate&&f.h.CachedBoundingSphere){C3DVector result;result.X=f.owner.pos.X+f.h.BoundingSphereOffsetFromPos.X;result.Y=f.owner.pos.Y+f.h.BoundingSphereOffsetFromPos.Y;result.Z=f.owner.pos.Z+f.h.BoundingSphereOffsetFromPos.Z;centre=result;radius=f.h.BoundingSphereRadius;return;}
 C3DVector c=f.owner.pos;float *v[3]={&c.X,&c.Y,&c.Z};*v[f.primitives[0].axis]+=f.primitives[0].offset;centre=c;radius=f.primitives[0].radius;calls.push_back(0);
 for(int i=1;i<n;++i){c=f.owner.pos;float *w[3]={&c.X,&c.Y,&c.Z};*w[f.primitives[i].axis]+=f.primitives[i].offset;calls.push_back(i);double dx=c.X-centre.X,dy=c.Y-centre.Y,dz=c.Z-centre.Z;double distance=std::sqrt(dx*dx+dy*dy+dz*dz);float r=f.primitives[i].radius;if(r+distance>=radius){double updated=(distance+r+radius)*0.5;double advance=updated-radius;if(advance<0)advance=0;double factor=distance>DELTA_NOTIONAL_ZERO?advance/distance:advance;centre.X=float(centre.X+dx*factor);centre.Y=float(centre.Y+dy*factor);centre.Z=float(centre.Z+dz*factor);radius=float(updated);}}
 C3DVector offset;offset.X=centre.X-f.owner.pos.X;offset.Y=centre.Y-f.owner.pos.Y;offset.Z=centre.Z-f.owner.pos.Z;f.h.BoundingSphereOffsetFromPos=offset;f.h.BoundingSphereRadius=radius;f.h.CachedBoundingSphere=true;
}
int main(){unsigned long cases=0;for(int n=0;n<5;++n)for(int profile=0;profile<256;++profile)for(int radii=0;radii<4;++radii)for(int axis=0;axis<3;++axis)for(int pos=-1;pos<2;++pos)for(int cached=0;cached<2;++cached)for(int accurate=0;accurate<2;++accurate)for(int alias=0;alias<3;++alias){
 cfg[0]=n;cfg[1]=profile;cfg[2]=radii;cfg[3]=axis;cfg[4]=pos;cfg[5]=cached;cfg[6]=accurate;cfg[7]=alias;
 Fixture a(n,profile,radii,axis,pos,cached),b(n,profile,radii,axis,pos,cached);C3DVector&ac=alias==0?a.out:alias==1?a.owner.pos:a.h.BoundingSphereOffsetFromPos;C3DVector&bc=alias==0?b.out:alias==1?b.owner.pos:b.h.BoundingSphereOffsetFromPos;float&ar=alias==2?a.h.BoundingSphereRadius:a.rad;float&br=alias==2?b.h.BoundingSphereRadius:b.rad;calls.clear();a.h.GetBoundingSphere(ac,ar,accurate!=0);std::vector<int>actual=calls;calls.clear();reference(b,bc,br,accurate!=0);

 check(actual==calls);check(equal(ac,bc)&&near(ar,br));check(equal(a.owner.pos,b.owner.pos)&&equal(a.h.BoundingSphereOffsetFromPos,b.h.BoundingSphereOffsetFromPos));check(near(a.h.BoundingSphereRadius,b.h.BoundingSphereRadius)&&a.h.CachedBoundingSphere==b.h.CachedBoundingSphere);check(a.h.HitLocations.size()==unsigned(n));++cases;
 }check(cases==552960);std::puts("00815420_TEST PASS");return 0;}