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
static C3DVector Args[4][3];
bool CPlane::Initialise(const C3DVector&a,const C3DVector&b,const C3DVector&c){assert(PlaneCalls<4);Args[PlaneCalls][0]=a;Args[PlaneCalls][1]=b;Args[PlaneCalls][2]=c;Normal=C3DVector(100.0f+PlaneCalls,200.0f+PlaneCalls,300.0f+PlaneCalls);Distance=400.0f+PlaneCalls;++PlaneCalls;return true;}
struct C3DBoundingSphere {C3DVector Center;float Radius;C3DBoundingSphere(){}C3DBoundingSphere(const C3DVector&c,float r):Center(c),Radius(r){}};
struct CShadowClipVolume {
 CPlane ClipPlanes[8];long ClipPlaneCount;C3DBoundingSphere BoundingSphere;bool ClipToSphere;
 void Reset(){ClipPlaneCount=0;ClipToSphere=false;}
 void AddPlane(const CPlane&p){assert(ClipPlaneCount<8);ClipPlanes[ClipPlaneCount]=p;++ClipPlaneCount;}
 void SetSphere(const C3DBoundingSphere&s){BoundingSphere=s;ClipToSphere=true;}
};
struct CShadowRendererSceneData{CMatrix4x4 WorldToShadowBufferMatrix;float ShadowFadeStart,ShadowFadeEnd;C3DVector ShadowLightVector;CShadowClipVolume CasterClipVolume,RecieverClipVolume;};
struct CEngineCamera {C3DVector Position;CPlane FrustrumPlanes[4];const CPlane&PeekFrustrumPlane(long n)const{assert(n>=0&&n<4);return FrustrumPlanes[n];}const C3DVector&PeekCameraPos()const{return Position;}};
struct CEngineShadowRenderer{CShadowRendererSceneData SceneData;void CalcClipVolume(const CEngineCamera&);};
// BEGIN_AUTHORED_BODY
void CEngineShadowRenderer::CalcClipVolume(const CEngineCamera& camera)
{
    C3DVector corners[4] = {
        C3DVector(0.0f, 0.0f, 0.0f),
        C3DVector(1.0f, 0.0f, 0.0f),
        C3DVector(1.0f, 1.0f, 0.0f),
        C3DVector(0.0f, 1.0f, 0.0f)
    };
    CMatrix4x4 buffer_to_world;
    buffer_to_world.SetInverse(SceneData.WorldToShadowBufferMatrix);
    for (long n = 0; n < 4; ++n)
        buffer_to_world.PerspectiveTransformPoint(corners[n]);

    SceneData.CasterClipVolume.Reset();
    SceneData.RecieverClipVolume.Reset();


    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[1], corners[0] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[1], corners[2], corners[1] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[2], corners[3], corners[2] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[3], corners[0], corners[3] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));

    // Original donor02d76ba0..02d76bf6 retains these visualization loops;
    // their per-plane/per-corner debug-only body is absent in the retail configuration.
    for (long pl = 0; pl < 4; ++pl)
        for (long cor = 0; cor < 4; ++cor) { }

    CPlane cam_planes[4] = {
        camera.PeekFrustrumPlane(0),
        camera.PeekFrustrumPlane(1),
        camera.PeekFrustrumPlane(2),
        camera.PeekFrustrumPlane(3)
    };
    for (long n = 0; n < 4; ++n)
        if (cam_planes[n].GetNormal().Dot(SceneData.ShadowLightVector) > 0.0f)
            SceneData.CasterClipVolume.AddPlane(cam_planes[n]);

    SceneData.RecieverClipVolume.SetSphere(C3DBoundingSphere(camera.PeekCameraPos(), SceneData.ShadowFadeEnd));
}

// END_AUTHORED_BODY
static bool Near(float a,long double b){long double d=(long double)a-b;if(d<0)d=-d;long double mag=b<0?-b:b;return d<=0.00008L*(1.0L+mag);}
static void VectorNear(const C3DVector&v,const long double p[3]){assert(Near(v.X,p[0])&&Near(v.Y,p[1])&&Near(v.Z,p[2]));}
int main(){
 for(int c=0;c<4096;++c){
  CEngineShadowRenderer r;CEngineCamera camera;CMatrix4x4 inverse;
  for(int i=0;i<4;++i)for(int j=0;j<4;++j){inverse.Data[i][j]=0.0f;r.SceneData.WorldToShadowBufferMatrix.Data[i][j]=(float)(c+i*4+j+1);}
  inverse.E11=0.5f+(float)(c%7)*0.25f;inverse.E22=0.75f+(float)((c/7)%5)*0.25f;inverse.E33=1.0f;
  inverse.E12=(float)((c%3)-1)*0.125f;inverse.E21=(float)(((c/3)%3)-1)*0.125f;inverse.E13=(float)(((c/9)%3)-1)*0.25f;
  inverse.E23=(float)(((c/27)%3)-1)*0.25f;inverse.E41=(float)(c%17-8)*0.5f;inverse.E42=(float)((c/17)%13-6)*0.5f;inverse.E43=(float)((c/221)%9-4)*0.25f;
  inverse.E14=(c&1)?0.125f:0.0f;inverse.E24=(c&2)?-0.125f:0.0f;inverse.E44=1.0f+(float)((c/3)%3)*0.25f;
  r.SceneData.ShadowFadeStart=0.25f+(float)(c%11)*0.5f;r.SceneData.ShadowFadeEnd=1.0f+(float)(c%31)*0.25f;
  C3DVector light((float)(c%7-3)*0.25f,(float)((c/7)%7-3)*0.25f,(float)((c/49)%7-3)*0.25f);r.SceneData.ShadowLightVector=light;
  r.SceneData.CasterClipVolume.ClipPlaneCount=8;r.SceneData.CasterClipVolume.ClipToSphere=true;r.SceneData.RecieverClipVolume.ClipPlaneCount=7;r.SceneData.RecieverClipVolume.ClipToSphere=false;
  camera.Position=C3DVector((float)(c%19-9),(float)((c/19)%13-6),(float)((c/247)%11-5));
  const float normals[4][3]={{1,0,0},{-1,0,0},{0,1,0},{0,0,-1}};
  for(int i=0;i<4;++i){camera.FrustrumPlanes[i].Normal=C3DVector(normals[i][0],normals[i][1],normals[i][2]);camera.FrustrumPlanes[i].Distance=20.0f+i;}
  ExpectedSource=&r.SceneData.WorldToShadowBufferMatrix;SuppliedInverse=&inverse;InverseCalls=PlaneCalls=0;
  r.CalcClipVolume(camera);
  assert(InverseCalls==1&&PlaneCalls==4);
  const int xy[4][2]={{0,0},{1,0},{1,1},{0,1}};long double points[4][3];
  for(int k=0;k<4;++k){long double q[4]={0,0,0,0};const long double v[4]={(long double)xy[k][0],(long double)xy[k][1],0,1};for(int j=0;j<4;++j)for(int i=0;i<4;++i)q[j]+=v[i]*(long double)inverse.Data[i][j];for(int j=0;j<3;++j)points[k][j]=q[j]/q[3];}
  const long double lv[3]={light.X,light.Y,light.Z};
  for(int k=0;k<4;++k){VectorNear(Args[k][0],points[k]);VectorNear(Args[k][1],points[(k+1)%4]);long double extruded[3];for(int j=0;j<3;++j)extruded[j]=points[k][j]+lv[j]*(long double)r.SceneData.ShadowFadeStart;VectorNear(Args[k][2],extruded);const CPlane&p=r.SceneData.CasterClipVolume.ClipPlanes[k];assert(p.Normal.X==100+k&&p.Normal.Y==200+k&&p.Normal.Z==300+k&&p.Distance==400+k);}
  int count=4;for(int i=0;i<4;++i){long double d=0;for(int j=0;j<3;++j)d+=normals[i][j]*lv[j];if(d>0){const CPlane&p=r.SceneData.CasterClipVolume.ClipPlanes[count++];assert(p.Normal.X==normals[i][0]&&p.Normal.Y==normals[i][1]&&p.Normal.Z==normals[i][2]&&p.Distance==20+i);}}
  assert(r.SceneData.CasterClipVolume.ClipPlaneCount==count&&!r.SceneData.CasterClipVolume.ClipToSphere);
  assert(r.SceneData.RecieverClipVolume.ClipPlaneCount==0&&r.SceneData.RecieverClipVolume.ClipToSphere);
  const C3DBoundingSphere&s=r.SceneData.RecieverClipVolume.BoundingSphere;assert(s.Center.X==camera.Position.X&&s.Center.Y==camera.Position.Y&&s.Center.Z==camera.Position.Z&&s.Radius==r.SceneData.ShadowFadeEnd);
  for(int i=0;i<4;++i)for(int j=0;j<4;++j)assert(r.SceneData.WorldToShadowBufferMatrix.Data[i][j]==(float)(c+i*4+j+1));
 }
 puts("00b4bcc0_TEST PASS");return 0;
}