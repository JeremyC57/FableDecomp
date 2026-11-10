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
const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt((GFSqr(X)+GFSqr(Y)+GFSqr(Z)));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}};
class CRightHandedSet {protected:C3DVector Up,Forward;public:CRightHandedSet(){}void Set(const C3DVector& up,const C3DVector& forward){Up=up;Forward=forward;}const C3DVector& GetUp()const{return Up;}const C3DVector& GetForward()const{return Forward;}};
class CCameraFixture;
class CCamera{friend class CCameraFixture;private:C3DVector WorldPosition;CRightHandedSet RHSet;bool HeightLocked,ViewVecZLocked;unsigned long FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;public:CCamera(const C3DVector&,const C3DVector&,float,bool);};
CCamera::CCamera(const C3DVector& world_position,const C3DVector& target_position,float fov,bool is_2d)
{
 C3DVector temp_up(0.0f,0.0f,1.0f);
 C3DVector up_difference;
 WorldPosition=world_position;
 C3DVector view=target_position-WorldPosition;
 view.Normalise();
 float factor=(temp_up.Dot(view));
 up_difference=view*factor;
 C3DVector up=temp_up-up_difference;
 up.Normalise();
 RHSet.Set(up,view);
 HorizontalFOV=fov;VerticalFOV=0.0f;Zoom=fov;
 FOVFlags=is_2d?4:1;
 HeightLocked=false;ViewVecZLocked=false;
}

class CCameraFixture {public:static void check(const CCamera& c,const C3DVector& p,const C3DVector& target,float f,bool mode){
assert(c.WorldPosition.X==p.X&&c.WorldPosition.Y==p.Y&&c.WorldPosition.Z==p.Z);assert(c.HorizontalFOV==f&&c.Zoom==f&&c.VerticalFOV==0);assert(c.FOVFlags==(mode?4u:1u));assert(!c.HeightLocked&&!c.ViewVecZLocked);
float x=target.X-p.X,y=target.Y-p.Y,z=target.Z-p.Z;float k=GFFastOneOverSqrt(x*x+y*y+z*z);x*=k;y*=k;z*=k;
float ux=-x*z,uy=-y*z,uz=1-z*z;k=GFFastOneOverSqrt(ux*ux+uy*uy+uz*uz);ux*=k;uy*=k;uz*=k;
const C3DVector& u=c.RHSet.GetUp();const C3DVector& v=c.RHSet.GetForward();assert(fabs(v.X-x)<0.00001&&fabs(v.Y-y)<0.00001&&fabs(v.Z-z)<0.00001);assert(fabs(u.X-ux)<0.00001&&fabs(u.Y-uy)<0.00001&&fabs(u.Z-uz)<0.00001);}};

int main(){
for(unsigned int j=0;j<128;++j){unsigned int bits=0x3f000000u|(j<<17);float input;memcpy(&input,&bits,4);float reciprocal=(float)(1.0/sqrt((double)input));unsigned int result;memcpy(&result,&reciprocal,4);unsigned int exponent=((0x17c-((bits>>23)&255))>>1)<<23;GFastOneOverSqrtTable[j]=(result-exponent)>>15;}
for(int x=-5;x<=5;++x)for(int y=-5;y<=5;++y)for(int z=-4;z<=4;++z){if(x==0&&y==0)continue;C3DVector p(2,-3,1),q(2+x,-3+y,1+z);for(int mode=0;mode<2;++mode){CCamera c(p,q,0.27f,mode!=0);CCameraFixture::check(c,p,q,0.27f,mode!=0);}}
puts("00a0bf20_TEST PASS");return 0;}