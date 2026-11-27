#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int assertions;
#define FABLE_RETAIL_ASSERT(condition) do { if(!(condition)) exit(5); ++assertions; } while (0)
union SeedWord { unsigned long bits; float value; };
static unsigned long inverseTable[128];
static void fillFixtureTable(){
 for(int i=0;i<128;++i){
  double mantissa=1.0+((i&63)+0.5)/64.0;
  double normalized=(i&64?2.0:1.4142135623730951)/sqrt(mantissa);
  inverseTable[i]=(unsigned long)((normalized-1.0)*256.0+0.5);
 }
}
static float inverseRoot(float n){
 SeedWord input,seed;input.value=n;
 seed.bits=(((0x17c-((input.bits>>23)&255))>>1)<<23)|(inverseTable[(input.bits>>17)&127]<<15);
 float r=seed.value;
 return (3.0f-((r*r)*n))*r*0.5f;
}
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v) const { return C3DVector(X+v.X,Y+v.Y,Z+v.Z); }
 C3DVector operator-(const C3DVector& v) const { return C3DVector(X-v.X,Y-v.Y,Z-v.Z); }
 C3DVector operator*(float c) const { return C3DVector(X*c,Y*c,Z*c); }
 C3DVector operator^(const C3DVector& v) const { return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X); }
 float Dot(const C3DVector& v) const { return X*v.X+Y*v.Y+Z*v.Z; }
 void Normalise(){float r=inverseRoot((X*X+Y*Y+Z*Z));X*=r;Y*=r;Z*=r;}
 const C3DVector& operator*=(float c){X*=c;Y*=c;Z*=c;return *this;}
};
const float DELTA_NOTIONAL_ZERO=0.0001f;
namespace QuadricOptimiserInternals {
class COptimisedPrimitive {public:
 C3DVector FindTextureTangent(const C3DVector&,const C3DVector&,const C3DVector&,float,float,float);
};
}
namespace QuadricOptimiserInternals {

C3DVector COptimisedPrimitive::FindTextureTangent(const C3DVector& pa, const C3DVector& pb, const C3DVector& pc,
                                                  float ta, float tb, float tc)
{
    C3DVector pos[3];
    pos[0] = pa;
    pos[1] = pb;
    pos[2] = pc;
    float tex[3];
    tex[0] = ta;
    tex[1] = tb;
    tex[2] = tc;
    unsigned long hi_index = (tex[0] > tex[1] && tex[0] > tex[2]) ? 0 : (tex[1] > tex[2] ? 1 : 2);
    unsigned long lo_index = (tex[0] < tex[1] && tex[0] < tex[2]) ? 0 : (tex[1] < tex[2] ? 1 : 2);
    if (tex[lo_index] + DELTA_NOTIONAL_ZERO >= tex[hi_index])
        return C3DVector(0.0f, 0.0f, 0.0f);
    unsigned long mid_index = 0;
    for (unsigned long c_index = 0; c_index < 3; ++c_index) {
        if (c_index != hi_index && c_index != lo_index) {
            mid_index = c_index;
            break;
        }
    }
    float mid_t = tex[mid_index];
    float lo_t = tex[lo_index];
    float hi_t = tex[hi_index];
    FABLE_RETAIL_ASSERT(fabs(hi_t - lo_t) > DELTA_NOTIONAL_ZERO);
    float fraction = (mid_t - lo_t) / (hi_t - lo_t);
    C3DVector iso_pos = pos[hi_index] * fraction + pos[lo_index] * (1.0f - fraction);
    C3DVector iso_vector = iso_pos - pos[mid_index];
    iso_vector.Normalise();
    C3DVector face_normal = (pc - pa) ^ (pb - pa);
    face_normal.Normalise();
    C3DVector tangent_vector = iso_vector ^ face_normal;
    tangent_vector.Normalise();
    float mult = (hi_t - lo_t) / tangent_vector.Dot(pos[hi_index] - pos[lo_index]);
    tangent_vector *= mult;
    return tangent_vector;
}

}
int main() {
 fillFixtureTable();
 QuadricOptimiserInternals::COptimisedPrimitive primitive;
 const float tex[]={-3.0f,-0.5f,0.0f,0.00001f,0.0001f,0.25f,2.0f,5.0f};
 for(int side=0;side!=6;++side)for(int shear=-2;shear<=2;++shear){
  const C3DVector p(2,-3,0.75f);
  C3DVector u(0,0,0),v(0,0,0);
  if(side%3==0){u=C3DVector(2,0,0);v=C3DVector((float)shear,1,0);}
  if(side%3==1){u=C3DVector(0,2,0);v=C3DVector(0,(float)shear,1);}
  if(side%3==2){u=C3DVector(0,0,2);v=C3DVector(1,0,(float)shear);}
  if(side>=3){C3DVector temp=u;u=v;v=temp;}
  for(int i=0;i!=8;++i)for(int j=0;j!=8;++j)for(int k=0;k!=8;++k){
   float a=tex[i],b=tex[j],c=tex[k],lo=a,hi=a;
   if(b<lo)lo=b;if(c<lo)lo=c;if(b>hi)hi=b;if(c>hi)hi=c;
   int prior=assertions;
   C3DVector t=primitive.FindTextureTangent(p,p+u,p+v,a,b,c);
   if(lo+DELTA_NOTIONAL_ZERO>=hi){if(t.X||t.Y||t.Z||assertions!=prior)return 2;continue;}
   if(assertions!=prior+1)return 4;
   // The tangent is the in-plane gradient. Both directed edge derivatives are independent checks.
   if(fabs(t.Dot(u)-(b-a))>0.0004 || fabs(t.Dot(v)-(c-a))>0.0004 || fabs(t.Dot(u^v))>0.0004)return 3;
  }
 }
 puts("00ac7300_TEST PASS");return 0;
}