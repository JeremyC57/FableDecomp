#include <math.h>
#include <stdio.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#endif
const float DELTA_NOTIONAL_ZERO=0.0001f;
inline float __fastcall GFSqr(float n){return n*n;}
class C3DVector {public:float X,Y,Z;
C3DVector(){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}
const C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
const C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
const C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}
const C3DVector& operator/=(float f){float reciprocal=1.0f/f;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
const float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}
float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
float GetAccurateMagnitude()const{float magnitude=X*X+Y*Y+Z*Z;return (float)sqrt(magnitude);}
};
namespace NPhysicsHelpers {
C3DVector __fastcall GetClosestPointOnFiniteLineToPoint(const C3DVector& start_pos,const C3DVector& end_pos,const C3DVector& test_pos)
{
 C3DVector point_to_use=start_pos;
 C3DVector line=end_pos-start_pos;
 float line_mag=line.GetAccurateMagnitude();
 if(line_mag>DELTA_NOTIONAL_ZERO){
  line/=line_mag;
  C3DVector start_to_thing=test_pos-start_pos;
  point_to_use=start_pos+line*start_to_thing.Dot(line);
  float sqr_dist_start=(point_to_use-start_pos).GetSquaredMagnitude();
  float sqr_dist_end=(point_to_use-end_pos).GetSquaredMagnitude();
  if(!(sqr_dist_start<GFSqr(line_mag)&&sqr_dist_end<GFSqr(line_mag))){
   if(sqr_dist_start<sqr_dist_end)point_to_use=start_pos;
   else point_to_use=end_pos;
  }
 }
 return point_to_use;
}
}

static void check(const C3DVector& a,const C3DVector& b,const C3DVector& p){
 double dx=b.X-a.X,dy=b.Y-a.Y,dz=b.Z-a.Z;double n=dx*dx+dy*dy+dz*dz;
 double t=n>0.00000001?((p.X-a.X)*dx+(p.Y-a.Y)*dy+(p.Z-a.Z)*dz)/n:0;
 if(t<0)t=0;if(t>1)t=1;
 C3DVector q=NPhysicsHelpers::GetClosestPointOnFiniteLineToPoint(a,b,p);
 assert(fabs(q.X-(a.X+t*dx))<0.0001);assert(fabs(q.Y-(a.Y+t*dy))<0.0001);assert(fabs(q.Z-(a.Z+t*dz))<0.0001);
}
int main(){for(int x=-3;x<=3;++x)for(int y=-3;y<=3;++y)for(int z=-3;z<=3;++z)for(int t=-3;t<=3;++t){
 C3DVector a(1,-2,3),b((float)x,(float)y,(float)z),p((float)t,1.5f,-0.75f);check(a,b,p);check(b,a,p);
}check(C3DVector(1,2,3),C3DVector(1,2,3),C3DVector(5,6,7));
check(C3DVector(0,0,0),C3DVector(0.00001f,0,0),C3DVector(1,2,3));
puts("008f5970_TEST PASS");return 0;}