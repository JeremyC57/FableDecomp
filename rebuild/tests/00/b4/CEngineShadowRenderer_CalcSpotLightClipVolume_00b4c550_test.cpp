#include <assert.h>
#include <stdio.h>
#include <math.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector(){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 const C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
 const float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}
};
struct CMatrix4x4;
static const CMatrix4x4* ExpectedSource;
static const CMatrix4x4* SuppliedInverse;
static long InverseCalls,PlaneCalls;
struct CMatrix4x4 {
 union {struct{float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44;};float Data[4][4];};
 CMatrix4x4(){}
 void SetInverse(const CMatrix4x4& m){assert(&m==ExpectedSource);++InverseCalls;*this=*SuppliedInverse;}
 void PerspectiveTransformPoint(C3DVector& point) {
  float x=point.X*E11+point.Y*E21+point.Z*E31+E41;
  float y=point.X*E12+point.Y*E22+point.Z*E32+E42;
  float z=point.X*E13+point.Y*E23+point.Z*E33+E43;
  float w=point.X*E14+point.Y*E24+point.Z*E34+E44;
  point.X=x/w;point.Y=y/w;point.Z=z/w;
 }
};
struct CPlane {
 C3DVector Normal;float Distance;
 CPlane(){}
 CPlane(const C3DVector&a,const C3DVector&b,const C3DVector&c){Initialise(a,b,c);}
 bool Initialise(const C3DVector&,const C3DVector&,const C3DVector&);
 const C3DVector& GetNormal()const{return Normal;}
};
static C3DVector Args[6][3];
bool CPlane::Initialise(const C3DVector&a,const C3DVector&b,const C3DVector&c){assert(PlaneCalls<6);Args[PlaneCalls][0]=a;Args[PlaneCalls][1]=b;Args[PlaneCalls][2]=c;Normal=C3DVector(100.0f+PlaneCalls,200.0f+PlaneCalls,300.0f+PlaneCalls);Distance=400.0f+PlaneCalls;++PlaneCalls;return true;}
struct C3DBoundingSphere {C3DVector Center;float Radius;C3DBoundingSphere(){}C3DBoundingSphere(const C3DVector&c,float r):Center(c),Radius(r){}};
struct CShadowClipVolume {
 CPlane ClipPlanes[8];long ClipPlaneCount;C3DBoundingSphere BoundingSphere;bool ClipToSphere;
 void Reset(){ClipPlaneCount=0;ClipToSphere=false;}
 void AddPlane(const CPlane&p){assert(ClipPlaneCount<8);ClipPlanes[ClipPlaneCount]=p;++ClipPlaneCount;}
 void SetSphere(const C3DBoundingSphere&s){BoundingSphere=s;ClipToSphere=true;}
};
struct CShadowRendererSceneData{CMatrix4x4 WorldToShadowBufferMatrix;float ShadowFadeStart,ShadowFadeEnd;C3DVector ShadowLightVector;CShadowClipVolume CasterClipVolume,RecieverClipVolume;};
struct CEngineCamera {C3DVector Position;CPlane FrustrumPlanes[4];const CPlane&PeekFrustrumPlane(long n)const{assert(n>=0&&n<4);return FrustrumPlanes[n];}const C3DVector&PeekCameraPos()const{return Position;}};
struct CEngineShadowRenderer{CShadowRendererSceneData SceneData;void CalcSpotLightClipVolume(const struct CEngineLightingManagerSpotlightParams&);};
struct CEngineLightingManagerSpotlightParams{long Marker[16];};
// BEGIN_AUTHORED_BODY
void CEngineShadowRenderer::CalcSpotLightClipVolume(const CEngineLightingManagerSpotlightParams& spot_params)
{
    SceneData.CasterClipVolume.Reset();
    SceneData.RecieverClipVolume.Reset();

    C3DVector corners[8] = {
        C3DVector(0.0f, 0.0f, 0.0f),
        C3DVector(1.0f, 0.0f, 0.0f),
        C3DVector(0.0f, 1.0f, 0.0f),
        C3DVector(1.0f, 1.0f, 0.0f),
        C3DVector(0.0f, 0.0f, 1.0f),
        C3DVector(1.0f, 0.0f, 1.0f),
        C3DVector(0.0f, 1.0f, 1.0f),
        C3DVector(1.0f, 1.0f, 1.0f)
    };
    CMatrix4x4 buffer_to_world;
    buffer_to_world.SetInverse(SceneData.WorldToShadowBufferMatrix);
    for (long n = 0; n < 8; ++n)
        buffer_to_world.PerspectiveTransformPoint(corners[n]);

    SceneData.CasterClipVolume.Reset();
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[2], corners[1]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[4], corners[5], corners[6]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[4], corners[2]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[1], corners[3], corners[5]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[1], corners[4]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[2], corners[6], corners[3]));

    SceneData.RecieverClipVolume = SceneData.CasterClipVolume;
}

// END_AUTHORED_BODY
static bool Near(float a,long double b){long double d=(long double)a-b;if(d<0)d=-d;long double mag=b<0?-b:b;return d<=0.00008L*(1.0L+mag);}
static void VectorNear(const C3DVector&v,const long double p[3]){assert(Near(v.X,p[0])&&Near(v.Y,p[1])&&Near(v.Z,p[2]));}
static void SamePlane(const CPlane&a,const CPlane&b){assert(a.Normal.X==b.Normal.X&&a.Normal.Y==b.Normal.Y&&a.Normal.Z==b.Normal.Z&&a.Distance==b.Distance);}
int main(){
 for(int c=0;c<4096;++c){
  CEngineShadowRenderer r;CEngineLightingManagerSpotlightParams params;CMatrix4x4 inverse;
  for(int i=0;i<16;++i)params.Marker[i]=c*17+i;
  for(int i=0;i<4;++i)for(int j=0;j<4;++j){inverse.Data[i][j]=0.0f;r.SceneData.WorldToShadowBufferMatrix.Data[i][j]=(float)(c+i*4+j+1);}
  inverse.E11=0.5f+(float)(c%7)*0.25f;inverse.E22=0.75f+(float)((c/7)%5)*0.25f;inverse.E33=0.75f+(float)((c/35)%5)*0.25f;
  inverse.E12=(float)((c%3)-1)*0.125f;inverse.E21=(float)(((c/3)%3)-1)*0.125f;inverse.E13=(float)(((c/9)%3)-1)*0.25f;inverse.E23=(float)(((c/27)%3)-1)*0.25f;inverse.E31=(float)(((c/81)%3)-1)*0.125f;inverse.E32=(float)(((c/243)%3)-1)*0.125f;
  inverse.E41=(float)(c%17-8)*0.5f;inverse.E42=(float)((c/17)%13-6)*0.5f;inverse.E43=(float)((c/221)%9-4)*0.25f;
  inverse.E14=(c&1)?0.125f:0.0f;inverse.E24=(c&2)?-0.125f:0.0f;inverse.E34=(c&4)?0.125f:0.0f;inverse.E44=1.0f+(float)((c/3)%3)*0.25f;
  C3DBoundingSphere old_sphere(C3DVector((float)c,2.0f,3.0f),17.0f+(float)(c%13));
  CShadowClipVolume&caster=r.SceneData.CasterClipVolume;CShadowClipVolume&receiver=r.SceneData.RecieverClipVolume;
  caster.ClipPlaneCount=7;caster.ClipToSphere=true;caster.BoundingSphere=old_sphere;receiver.ClipPlaneCount=8;receiver.ClipToSphere=true;receiver.BoundingSphere=C3DBoundingSphere(C3DVector(-8,-9,-10),11);
  for(int i=0;i<8;++i){caster.ClipPlanes[i].Normal=C3DVector(-100.0f-i,-200.0f-i,-300.0f-i);caster.ClipPlanes[i].Distance=-400.0f-i;receiver.ClipPlanes[i].Normal=C3DVector(-1,-2,-3);receiver.ClipPlanes[i].Distance=-4;}
  ExpectedSource=&r.SceneData.WorldToShadowBufferMatrix;SuppliedInverse=&inverse;InverseCalls=PlaneCalls=0;
  r.CalcSpotLightClipVolume(params);
  assert(InverseCalls==1&&PlaneCalls==6&&caster.ClipPlaneCount==6&&receiver.ClipPlaneCount==6&&!caster.ClipToSphere&&!receiver.ClipToSphere);
  long double points[8][3];
  for(int k=0;k<8;++k){long double q[4]={0,0,0,0};const long double v[4]={(long double)(k&1),(long double)((k>>1)&1),(long double)((k>>2)&1),1};for(int j=0;j<4;++j)for(int i=0;i<4;++i)q[j]+=v[i]*(long double)inverse.Data[i][j];for(int j=0;j<3;++j)points[k][j]=q[j]/q[3];}
  const int faces[6][3]={{0,2,1},{4,5,6},{0,4,2},{1,3,5},{0,1,4},{2,6,3}};
  for(int k=0;k<6;++k){for(int i=0;i<3;++i)VectorNear(Args[k][i],points[faces[k][i]]);const CPlane&p=caster.ClipPlanes[k];assert(p.Normal.X==100+k&&p.Normal.Y==200+k&&p.Normal.Z==300+k&&p.Distance==400+k);}
  for(int k=6;k<8;++k){const CPlane&p=caster.ClipPlanes[k];assert(p.Normal.X==-100-k&&p.Normal.Y==-200-k&&p.Normal.Z==-300-k&&p.Distance==-400-k);}
  for(int k=0;k<8;++k)SamePlane(caster.ClipPlanes[k],receiver.ClipPlanes[k]);
  const C3DBoundingSphere&s=receiver.BoundingSphere;assert(s.Center.X==old_sphere.Center.X&&s.Center.Y==2&&s.Center.Z==3&&s.Radius==old_sphere.Radius);assert(caster.BoundingSphere.Center.X==old_sphere.Center.X&&caster.BoundingSphere.Radius==old_sphere.Radius);
  for(int i=0;i<4;++i)for(int j=0;j<4;++j)assert(r.SceneData.WorldToShadowBufferMatrix.Data[i][j]==(float)(c+i*4+j+1));for(int i=0;i<16;++i)assert(params.Marker[i]==c*17+i);
 }
 puts("00b4c550_TEST PASS");return 0;
}