#include <cstdio>
#include <cmath>
#include <cstring>
static const float DELTA_NOTIONAL_ZERO=0.0001f; // Original game constant, not a fixture input.
static const unsigned int GFastOneOverSqrtTable[128]={106,103,100,98,95,92,90,87,85,83,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,51,49,47,46,44,42,41,39,38,36,35,33,32,31,29,28,26,25,24,22,21,20,19,17,16,15,14,13,12,10,9,8,7,6,5,4,3,2,1,255,252,248,244,240,237,233,230,226,223,220,217,214,211,208,205,202,199,196,193,191,188,185,183,180,178,176,173,171,168,166,164,162,160,158,155,153,151,149,147,145,143,142,140,138,136,134,133,131,129,127,126,124,122,121,119,118,116,115,113,112,110,109,107};
static float GFSqr(float v){return v*v;}
static float GFFastOneOverSqrt(float n){unsigned int bits;std::memcpy(&bits,&n,4);unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r;std::memcpy(&r,&estimate,4);return (3.0f-r*r*n)*r*0.5f;}
class C2DVector{public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}bool IsNonZero()const{return X*X+Y*Y>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C2DVector ToC2DVector()const{return C2DVector(X,Y);}C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GFSqr(X)+GFSqr(Y)+GFSqr(Z));X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}C3DVector& operator-=(const C3DVector& v){X-=v.X;Y-=v.Y;Z-=v.Z;return *this;}};
class CPhysicsCollideResponse{public:C3DVector CollidePos,CollideNormal;float Friction;};
class CTCPhysicsBase{public:void ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse&,const CTCPhysicsBase&,C3DVector&)const;};
void CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject(const CPhysicsCollideResponse& response_info,const CTCPhysicsBase& collided_with,C3DVector& velocity)const
{
 C3DVector normal(response_info.CollideNormal);
 if(normal.ToC2DVector().IsNonZero()){
  normal.Z=0;
  normal.Normalise();
 }
 float dot=(normal.X*velocity.X+normal.Y*velocity.Y)+normal.Z*velocity.Z;
 velocity-=normal*dot;
}

static bool Check(const C3DVector& n,const C3DVector& before,const C3DVector& result){double x=n.X,y=n.Y,z=n.Z,xy=x*x+y*y;double eps=DELTA_NOTIONAL_ZERO;if(xy>eps*eps){double length=std::sqrt(xy);x/=length;y/=length;z=0;}double dot=x*before.X+y*before.Y+z*before.Z;double tolerance=1.0/1024.0;return std::fabs(result.X-(before.X-x*dot))<tolerance&&std::fabs(result.Y-(before.Y-y*dot))<tolerance&&std::fabs(result.Z-(before.Z-z*dot))<tolerance;}
int main(){int ok=sizeof(unsigned int)==4&&sizeof(float)==4,cases=0;CTCPhysicsBase physics,other;C3DVector normals[8]={C3DVector(1,0,9),C3DVector(0,2,-1),C3DVector(3,4,5),C3DVector(-4,3,2),C3DVector(0,0,1),C3DVector(0,0,2),C3DVector(1.0f/16384,0,1),C3DVector(1.0f/8192,0,1)};for(int i=0;i<8;++i)for(int x=-4;x<=4;++x)for(int y=-4;y<=4;++y)for(int z=-2;z<=2;++z){CPhysicsCollideResponse response;response.CollidePos=C3DVector(2,4,8);response.CollideNormal=normals[i];response.Friction=0.5f;C3DVector before((float)x,(float)y,(float)z),velocity=before;physics.ImpulseRespondToCollisonWithImpassableObject(response,other,velocity);ok &= Check(normals[i],before,velocity)&&response.CollideNormal.X==normals[i].X&&response.CollideNormal.Y==normals[i].Y&&response.CollideNormal.Z==normals[i].Z&&response.Friction==0.5f;++cases;}
for(int i=0;i<8;++i){CPhysicsCollideResponse response;response.CollideNormal=normals[i];physics.ImpulseRespondToCollisonWithImpassableObject(response,other,response.CollideNormal);ok &= Check(normals[i],normals[i],response.CollideNormal);++cases;}ok &= cases==3248;std::printf(ok?"006afaf0_TEST PASS\n":"006afaf0_TEST FAIL\n");return ok?0:1;}