#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
// Synthetic external-table fixture, generated from reciprocal square roots.
// This tests the helper algorithm, not the original engine table contents.
static unsigned long GFastOneOverSqrtTable[128];
inline float GFSqr(float value){return value*value;}
inline float GFFastOneOverSqrt(float n){
 unsigned int bits;memcpy(&bits,&n,4);
 unsigned int seed=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);
 float r;memcpy(&r,&seed,4);r=(3.0f-r*r*n)*r*0.5f;return r;
}
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
const C3DVector operator-(const C3DVector& other)const{return C3DVector((X-other.X),(Y-other.Y),Z-other.Z);}
const C3DVector& operator-=(const C3DVector& v){X-=v.X;Y-=v.Y;Z-=v.Z;return *this;}
const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
const float Dot(const C3DVector& other)const{float dot=(Y*other.Y);dot+=Z*other.Z;dot+=X*other.X;return dot;}
C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GFSqr(X)+GFSqr(Y)+GFSqr(Z));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}};
class CRightHandedSet {protected:C3DVector Up,Forward;public:void SetUp(const C3DVector& v){Up=v;}void SetForward(const C3DVector& v){Forward=v;}const C3DVector& GetUp()const{return Up;}const C3DVector& GetForward()const{return Forward;}void MakePerpendicular();};
void CRightHandedSet::MakePerpendicular()
{
 Forward.Normalise();
 Up-=Forward*Up.Dot(Forward);
 Up.Normalise();
}
int main(){
for(unsigned int j=0;j<128;++j){unsigned int bits=0x3f000000u|(j<<17);float input;memcpy(&input,&bits,4);float reciprocal=(float)(1.0/sqrt((double)input));unsigned int result;memcpy(&result,&reciprocal,4);unsigned int exponent=((0x17c-((bits>>23)&255))>>1)<<23;GFastOneOverSqrtTable[j]=(result-exponent)>>15;}
for(int x=-5;x<=5;++x)for(int y=-4;y<=4;++y)for(int z=-3;z<=3;++z){if(x==0&&y==0&&z==0)continue;C3DVector up(0.3f,1.2f,0.7f),forward((float)x,(float)y,(float)z);CRightHandedSet r;r.SetUp(up);r.SetForward(forward);
float scale=GFFastOneOverSqrt(forward.X*forward.X+forward.Y*forward.Y+forward.Z*forward.Z);forward.X*=scale;forward.Y*=scale;forward.Z*=scale;float dot=up.Y*forward.Y;dot+=up.Z*forward.Z;dot+=up.X*forward.X;up.X-=forward.X*dot;up.Y-=forward.Y*dot;up.Z-=forward.Z*dot;scale=GFFastOneOverSqrt(up.X*up.X+up.Y*up.Y+up.Z*up.Z);up.X*=scale;up.Y*=scale;up.Z*=scale;
r.MakePerpendicular();const C3DVector& a=r.GetUp();const C3DVector& b=r.GetForward();assert(fabs(a.X-up.X)<0.00001&&fabs(a.Y-up.Y)<0.00001&&fabs(a.Z-up.Z)<0.00001);assert(fabs(b.X-forward.X)<0.00001&&fabs(b.Y-forward.Y)<0.00001&&fabs(b.Z-forward.Z)<0.00001);}
puts("00a22e10_TEST PASS");return 0;}