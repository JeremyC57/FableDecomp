#include <stdio.h>
#include <math.h>
#include <string.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}
 C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
};
static int trace[16],steps;
struct CMatrix3x4 {
 float v[4][3];
 void InitialiseSkewedSymmetric(const C3DVector& a){trace[steps++]=1;memset(v,0,sizeof v);v[0][1]=-a.Z;v[0][2]=a.Y;v[1][0]=a.Z;v[1][2]=-a.X;v[2][0]=-a.Y;v[2][1]=a.X;}
 CMatrix3x4 operator*(float s){CMatrix3x4 r=*this;for(int i=0;i<4;++i)for(int j=0;j<3;++j)r.v[i][j]*=s;return r;}
 CMatrix3x4 operator+(const CMatrix3x4& b){CMatrix3x4 r=*this;for(int i=0;i<4;++i)for(int j=0;j<3;++j)r.v[i][j]+=b.v[i][j];return r;}
 CMatrix3x4 operator*(const CMatrix3x4& b)const{trace[steps++]=2;CMatrix3x4 r;for(int i=0;i<4;++i)for(int j=0;j<3;++j){r.v[i][j]=i==3?b.v[3][j]:0;for(int k=0;k<3;++k)r.v[i][j]+=v[i][k]*b.v[k][j];}return r;}
 C3DVector operator*(const C3DVector& a)const{trace[steps++]=4;return C3DVector(v[0][0]*a.X+v[0][1]*a.Y+v[0][2]*a.Z,v[1][0]*a.X+v[1][1]*a.Y+v[1][2]*a.Z,v[2][0]*a.X+v[2][1]*a.Y+v[2][2]*a.Z);}
 // Deterministic substitute exposes use of the updated orientation by all later tensor operations.
 void Orthonormalise(){trace[steps++]=3;v[2][0]+=0.25f;}
 void Transpose(){for(int i=0;i<3;++i)for(int j=i+1;j<3;++j){float k=v[i][j];v[i][j]=v[j][i];v[j][i]=k;}}
};
struct CRigidBody {struct CRigidBodyConfig {C3DVector CMPosition,CMVelocity,AngularMomentum,CMForce,Torque,AngularVelocity;CMatrix3x4 Orientation,InverseWorldInertiaTensor;};CRigidBodyConfig LastConfig,CurrentConfig;float OneOverMass;CMatrix3x4 InverseBodyInertiaTensor;};
struct CTCPhysicsRigidBody {CRigidBody Body;void Integrate(float);};
void CTCPhysicsRigidBody::Integrate(float delta_time)
{
    CRigidBody::CRigidBodyConfig& source = Body.LastConfig;
    CRigidBody::CRigidBodyConfig& target = Body.CurrentConfig;
    target.CMPosition = source.CMPosition + source.CMVelocity * delta_time;
    CMatrix3x4 skew_symmetric;
    skew_symmetric.InitialiseSkewedSymmetric(source.AngularVelocity);
    skew_symmetric = skew_symmetric * delta_time;
    CMatrix3x4 result = skew_symmetric * source.Orientation;
    target.Orientation = source.Orientation + result;
    target.CMVelocity = source.CMVelocity + source.CMForce * (delta_time * Body.OneOverMass);
    target.AngularMomentum = source.AngularMomentum + source.Torque * delta_time;
    target.Orientation.Orthonormalise();
    CMatrix3x4 inertia_orientation = target.Orientation * Body.InverseBodyInertiaTensor;
    CMatrix3x4 orientation_transpose = target.Orientation;
    orientation_transpose.Transpose();
    target.InverseWorldInertiaTensor = inertia_orientation * orientation_transpose;
    target.AngularVelocity = target.InverseWorldInertiaTensor * target.AngularMomentum;
}

static bool close(float a,float b){return fabs(a-b)<0.00002;}
static bool vector_close(const C3DVector& a,const C3DVector& b){return close(a.X,b.X)&&close(a.Y,b.Y)&&close(a.Z,b.Z);}
int main(){
 for(int frame=-8;frame<=8;++frame)for(int mass=0;mass<5;++mass)for(int sign=-1;sign<=1;++sign)for(int axis=0;axis<3;++axis){
  CTCPhysicsRigidBody w;CRigidBody& b=w.Body;CRigidBody::CRigidBodyConfig& src=b.LastConfig;
  CRigidBody::CRigidBodyConfig& dst=b.CurrentConfig;
  memset(src.InverseWorldInertiaTensor.v,0,sizeof src.InverseWorldInertiaTensor.v);
  float dt=frame*0.0625f;b.OneOverMass=mass*0.25f;
  src.CMPosition=C3DVector(1,2,-3);src.CMVelocity=C3DVector(sign,0.5f,-1);src.CMForce=C3DVector(0.25f,-0.5f,0.75f);
  src.AngularMomentum=C3DVector(1,-2,sign);src.Torque=C3DVector(0,0.75f,0.5f);
  src.AngularVelocity=C3DVector(axis==0?sign:0,axis==1?sign:0,axis==2?sign:0);
  for(int i=0;i<4;++i)for(int j=0;j<3;++j){src.Orientation.v[i][j]=i==j?1:(i*3+j+1)*0.0625f;b.InverseBodyInertiaTensor.v[i][j]=i==j?i+1:0;}
  CRigidBody::CRigidBodyConfig before=src;steps=0;w.Integrate(dt);
  int expected[]={1,2,3,2,2,4};if(steps!=6||memcmp(trace,expected,sizeof expected))return 1;
  if(memcmp(&before,&src,sizeof src))return 2;
  if(!vector_close(dst.CMPosition,C3DVector(1+dt*sign,2+dt*.5f,-3-dt))||!vector_close(dst.CMVelocity,C3DVector(sign+dt*b.OneOverMass*.25f,.5f-dt*b.OneOverMass*.5f,-1+dt*b.OneOverMass*.75f)))return 3;
  float momentum[]={1,-2+.75f*dt,sign+.5f*dt};if(!vector_close(dst.AngularMomentum,C3DVector(momentum[0],momentum[1],momentum[2])))return 4;
  float o[3][3],omega[]={src.AngularVelocity.X,src.AngularVelocity.Y,src.AngularVelocity.Z},vel[]={0,0,0};
  for(int r=0;r<3;++r)for(int c=0;c<3;++c){int j=(r+1)%3,k=(r+2)%3;o[r][c]=src.Orientation.v[r][c]+dt*(omega[j]*src.Orientation.v[k][c]-omega[k]*src.Orientation.v[j][c])+(r==2&&c==0?.25f:0);if(!close(o[r][c],dst.Orientation.v[r][c]))return 5;}
  for(int r=0;r<3;++r)for(int c=0;c<3;++c){float v=0;for(int k=0;k<3;++k)v+=o[r][k]*(k+1)*o[c][k];if(!close(v,dst.InverseWorldInertiaTensor.v[r][c]))return 6;vel[r]+=v*momentum[c];}
  if(!vector_close(dst.AngularVelocity,C3DVector(vel[0],vel[1],vel[2])))return 7;
 }
 puts("00759f80_TEST PASS");return 0;}