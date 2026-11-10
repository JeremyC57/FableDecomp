#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
const float DELTA_NOTIONAL_ZERO=0.0001f;
inline float __fastcall GFRoot(float n){return (float)sqrt(n);}
inline float __fastcall GFFloatAbs(float n){unsigned int bits;memcpy(&bits,&n,4);bits&=0x7fffffff;memcpy(&n,&bits,4);return n;}
inline bool __fastcall GFIsNonZero(float f){return GFFloatAbs(f)>DELTA_NOTIONAL_ZERO;}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
const C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
const C3DVector& operator-=(const C3DVector& v){X-=v.X;Y-=v.Y;Z-=v.Z;return *this;}
const C3DVector& operator*=(float s){X*=s;Y*=s;Z*=s;return *this;}
const float Dot(const C3DVector& other)const{float dot=X*other.X;dot+=Y*other.Y;dot+=Z*other.Z;return dot;}
float GetMagnitude()const{return GFRoot(X*X+Y*Y+Z*Z);}
bool IsNonZero()const{return X*X+Y*Y+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}};
// Unused-this boundary owner: target reads no owner fields. Production layout is audited separately.
namespace QuadricOptimiserInternals {class COptimisedPrimitive {friend class Test;private:C3DVector MakeOrthogonal(const C3DVector& vect,const C3DVector& normal)const;};class Test {public:static C3DVector run(const C3DVector& v,const C3DVector& n){COptimisedPrimitive p;return p.MakeOrthogonal(v,n);}};}
C3DVector QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal(const C3DVector& vect,const C3DVector& normal)const
{
 C3DVector result=vect;
 float vect_len=vect.GetMagnitude();
 if(GFIsNonZero(vect_len)){
  result-=normal*vect.Dot(normal);
  if(result.IsNonZero())result*=vect_len/result.GetMagnitude();
 }
 return result;
}

int main(){for(int x=-5;x<=5;++x)for(int y=-4;y<=4;++y)for(int z=-3;z<=3;++z){C3DVector v((float)x,(float)y,(float)z);C3DVector n(0.6f,0.8f,0.0f);C3DVector r=QuadricOptimiserInternals::Test::run(v,n);double length=sqrt((double)x*x+(double)y*y+(double)z*z),dot=x*0.6+y*0.8;double a=x-0.6*dot,b=y-0.8*dot,c=z;double projected=sqrt(a*a+b*b+c*c);if(length>0.0001&&projected>0.0001){a*=length/projected;b*=length/projected;c*=length/projected;}else if(length<=0.0001){a=x;b=y;c=z;}assert(fabs(r.X-a)<0.00002&&fabs(r.Y-b)<0.00002&&fabs(r.Z-c)<0.00002);}
C3DVector small(0.00001f,0,0),n(0,0,1);C3DVector r=QuadricOptimiserInternals::Test::run(small,n);assert(r.X==small.X&&r.Y==0&&r.Z==0);
puts("00ac7200_TEST PASS");return 0;}