#include <math.h>
#include <assert.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CMatrix3x4 {public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;CMatrix3x4(){}
void TransformVector(C3DVector& v)const{const float xx=v.X;const float yy=v.Y;const float zz=v.Z;v.X=xx*E11+yy*E21+zz*E31;v.Y=xx*E12+yy*E22+zz*E32;v.Z=xx*E13+yy*E23+zz*E33;}};
inline float __fastcall GFSqr(float value){return value*value;}
class CQuaternion {
public:
 float X,Y,Z,W;
 CQuaternion(){}
 void SetFromAxisAngle(const C3DVector& axis,float angle);
 void ToMatrix(CMatrix3x4& dest,const C3DVector& translation)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
  float s=(d!=0.0f)?2.0f/d:1.0f;
  float xs=X*s,ys=Y*s,zs=Z*s;
  float wx=W*xs,wy=W*ys,wz=W*zs;
  float xx=X*xs,xy=X*ys,xz=X*zs;
  float yy=Y*ys,yz=Y*zs,zz=Z*zs;
  dest.E11=1.0f-(yy+zz);dest.E12=xy-wz;dest.E13=xz+wy;
  dest.E21=xy+wz;dest.E22=1.0f-(xx+zz);dest.E23=yz-wx;
  dest.E31=xz-wy;dest.E32=yz+wx;dest.E33=1.0f-(xx+yy);
  dest.E41=translation.X;dest.E42=translation.Y;dest.E43=translation.Z;
 }
};
class CRightHandedSet {protected:C3DVector Up,Forward;public:
void SetUp(const C3DVector& v){Up=v;}void SetForward(const C3DVector& v){Forward=v;}
const C3DVector& GetUp()const{return Up;}const C3DVector& GetForward()const{return Forward;}
void Rotate(const C3DVector& rotate_axis,float angle);};
static float nextQ[4];static int calls;
void CQuaternion::SetFromAxisAngle(const C3DVector& axis,float angle){assert(axis.X==3&&axis.Y==4&&axis.Z==5&&angle==0.37f);++calls;X=nextQ[0];Y=nextQ[1];Z=nextQ[2];W=nextQ[3];}
void CRightHandedSet::Rotate(const C3DVector& rotate_axis,float angle)
{
 CQuaternion quaternion;
 quaternion.SetFromAxisAngle(rotate_axis,angle);
 CMatrix3x4 rotation_mat;
 quaternion.ToMatrix(rotation_mat,C3DVector(0.0f,0.0f,0.0f));
 rotation_mat.TransformVector(Up);
 rotation_mat.TransformVector(Forward);
}

static C3DVector expected(const C3DVector& v){double x=nextQ[0],y=nextQ[1],z=nextQ[2],w=nextQ[3];double n=x*x+y*y+z*z+w*w,s=n?2/n:1;
return C3DVector((float)(v.X*(1-s*(y*y+z*z))+v.Y*s*(x*y+w*z)+v.Z*s*(x*z-w*y)),(float)(v.X*s*(x*y-w*z)+v.Y*(1-s*(x*x+z*z))+v.Z*s*(y*z+w*x)),(float)(v.X*s*(x*z+w*y)+v.Y*s*(y*z-w*x)+v.Z*(1-s*(x*x+y*y))));}
static void check(const C3DVector& a,const C3DVector& b){assert(fabs(a.X-b.X)<0.00001&&fabs(a.Y-b.Y)<0.00001&&fabs(a.Z-b.Z)<0.00001);}
int main(){
 C3DVector axis(3,4,5),up(0.7f,0.2f,0.9f),forward(-0.3f,1.1f,0.4f);
 for(int i=0;i<625;++i){int n=i;for(int k=0;k<4;++k){nextQ[k]=(float)(n%5-2)*0.5f;n/=5;}
 CRightHandedSet rhs;rhs.SetUp(up);rhs.SetForward(forward);calls=0;rhs.Rotate(axis,0.37f);assert(calls==1);check(rhs.GetUp(),expected(up));check(rhs.GetForward(),expected(forward));
 CQuaternion q;q.X=nextQ[0];q.Y=nextQ[1];q.Z=nextQ[2];q.W=nextQ[3];CMatrix3x4 m;C3DVector translation(7,8,9);q.ToMatrix(m,translation);assert(m.E41==7&&m.E42==8&&m.E43==9);
 }
 puts("00a22f80_TEST PASS");return 0;}