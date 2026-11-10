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
class C3DVector{ public:
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 C3DVector& Normalise(){float factor=GFFastOneOverSqrt(X*X+Y*Y+Z*Z);X*=factor;Y*=factor;Z*=factor;return *this;}
 float GetAngleXY()const{float a=(float)std::atan2((double)X,(double)Y)*0.15915494f;if(a<0||a>=1){a=(float)std::fmod((double)a,1.0);if(a<0)a+=1;}return a;}
};
static int phase=0,stabs=0,positions=0;
class CThing{ public:C3DVector pos,stab;const C3DVector& GetPos()const{check(phase==1);phase=2;++positions;return pos;}};
namespace NStealth{static CThing* expectedVictim=0;C3DVector GetStabPosOnThing(CThing& thing){check(&thing==expectedVictim&&phase==0);phase=1;++stabs;return thing.stab;}}
class CTCStealth{ public:CThing* parent;const CThing& GetParentThing()const{return *parent;}C3DVector GetPositionToMoveToForStabOn(CThing&,float&)const;};
C3DVector CTCStealth::GetPositionToMoveToForStabOn(CThing& thing_to_stab,float& new_facing_angle)const
{
 C3DVector sword_stab_pos=NStealth::GetStabPosOnThing(thing_to_stab);
 C3DVector to_stab_pos=sword_stab_pos-GetParentThing().GetPos();
 C3DVector to_stand_pos(to_stab_pos);
 to_stand_pos.Normalise();
 C3DVector standing_stab_pos=sword_stab_pos-to_stand_pos;
 new_facing_angle=to_stand_pos.GetAngleXY();
 return standing_stab_pos;
}

int main(){
 check(sizeof(float)==4&&sizeof(unsigned int)==4);InitTable();
 const float values[]={-4.0f,-1.0f,0.0f,1.0f,4.0f};int cases=0;
 for(int origin=0;origin<8;++origin)for(int x=0;x<5;++x)for(int y=0;y<5;++y)for(int z=0;z<5;++z){
  CThing parent,victim;parent.pos=C3DVector(origin*0.5f,-origin*0.25f,origin*0.125f);
  victim.pos=C3DVector(-16,8,32);victim.stab=C3DVector(parent.pos.X+values[x],parent.pos.Y+values[y],parent.pos.Z+values[z]);
  NStealth::expectedVictim=&victim;CTCStealth stealth;stealth.parent=&parent;phase=0;float angle=-8;
  C3DVector out=stealth.GetPositionToMoveToForStabOn(victim,angle);
  double dx=values[x],dy=values[y],dz=values[z];double distance=std::sqrt(dx*dx+dy*dy+dz*dz);
  double tolerance=1.0/8192.0;if(distance==0)distance=1;
  check(std::fabs(out.X-(victim.stab.X-dx/distance))<tolerance);
  check(std::fabs(out.Y-(victim.stab.Y-dy/distance))<tolerance);
  check(std::fabs(out.Z-(victim.stab.Z-dz/distance))<tolerance);
  double expected=std::atan2(dx,dy)/(2.0*std::acos(-1.0));if(expected<0)expected+=1;
  check(angle>=0&&angle<1&&std::fabs(angle-expected)<1.0/1048576.0);
  check(phase==2&&parent.pos.X==origin*0.5f&&victim.pos.X==-16);++cases;
 }
 check(cases==1000&&stabs==1000&&positions==1000);std::puts("00746ec0_TEST PASS");return 0;
}