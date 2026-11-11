#include <stdio.h>
#include <stdlib.h>
static void Check(bool b){if(!b)abort();}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CMatrix3x4 {public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 void TransformPoint(C3DVector& p)const{const float x=p.X,y=p.Y,z=p.Z;p.X=E11*x+(E21*y+E31*z)+E41;p.Y=E12*x;p.Y+=E32*z;p.Y+=E22*y;p.Y+=E42;p.Z=E13*x;p.Z+=E33*z;p.Z+=E23*y;p.Z+=E43;}
};
class CEngineCamera {public:float OneOverHalfWindowWidth,OneOverHalfWindowHeight,WindowCenterX,WindowCenterY,HomogenousViewScaleX;CMatrix3x4 InverseViewTransform;
 void TransformDistScreenToViewHorizontal(float z,float& distance)const{const float scaled=OneOverHalfWindowWidth*distance*z;distance=scaled/HomogenousViewScaleX;}
 void TransformScreenXYLinearZToView(C3DVector& point)const{point.X=(point.X-WindowCenterX)*OneOverHalfWindowWidth*point.Z;point.Y=(WindowCenterY-point.Y)*OneOverHalfWindowHeight*point.Z;}
 void TransformViewToWorld(C3DVector& point)const{InverseViewTransform.TransformPoint(point);}
 void TransformScreenXYLinearZToWorld(C3DVector& point)const{TransformScreenXYLinearZToView(point);TransformViewToWorld(point);}
};
enum EEngine2DLayer{LAYER0,LAYER1,LAYER2};
class CEnginePrimitiveRenderer2D {public:int Calls;EEngine2DLayer ExpectedLayer;float Depth;float Layer2DToDistance(EEngine2DLayer layer){Check(layer==ExpectedLayer);++Calls;return Depth;}};
namespace NEngineInternal{CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D;CEngineCamera* GPCamera;}
class C2DBoxF {public:float TLX,TLY,BRX,BRY;float GetWidth()const{return BRX-TLX;}float GetHeight()const{return BRY-TLY;}};
class C3DBoundingSphere {public:C3DVector Center;float Radius;float GetRadius()const{return Radius;}};
class C3DPositionInterpolator {public:C3DVector StartPosition,EndPosition;};
class CEngineInternalPrimitive2DAnimatedMesh {public:float Scale;C3DPositionInterpolator WorldPos;bool Initialised;EEngine2DLayer Layer;C2DBoxF ClipBox;C3DBoundingSphere ObjectBoundingSphere;void Initialise();};
void CEngineInternalPrimitive2DAnimatedMesh::Initialise()
{
    const float view_z=NEngineInternal::GPPrimitiveRenderer2D->Layer2DToDistance(Layer);
    const float half_clip_box_width=ClipBox.GetWidth()*0.5f;
    const float half_clip_box_height=ClipBox.GetHeight()*0.5f;
    const float half_clip_square_width=half_clip_box_width<half_clip_box_height?half_clip_box_width:half_clip_box_height;
    const float one_on_bsphere_radius=1.0f/ObjectBoundingSphere.GetRadius();
    Scale=half_clip_square_width;
    NEngineInternal::GPCamera->TransformDistScreenToViewHorizontal(view_z,Scale);
    Scale*=one_on_bsphere_radius;
    WorldPos.StartPosition=C3DVector(ClipBox.TLX+half_clip_box_width,ClipBox.TLY+half_clip_box_height,view_z);
    NEngineInternal::GPCamera->TransformScreenXYLinearZToWorld(WorldPos.StartPosition);
    C3DVector& end_position=WorldPos.EndPosition;
    end_position=WorldPos.StartPosition;
    Initialised=true;
}

int main(){
 CEngineCamera camera;CEnginePrimitiveRenderer2D renderer;NEngineInternal::GPCamera=&camera;NEngineInternal::GPPrimitiveRenderer2D=&renderer;
 const float sides[]={0.0f,4.0f,8.0f,16.0f};const float powers[]={1.0f,2.0f,4.0f};const float origins[]={-16.0f,0.0f,16.0f};unsigned long cases=0;
 for(int wi=0;wi<4;++wi)for(int hi=0;hi<4;++hi)for(int ri=0;ri<3;++ri)for(int zi=0;zi<3;++zi)for(int xi=0;xi<3;++xi)for(int yi=0;yi<3;++yi)for(int m=0;m<4;++m)for(int inv=0;inv<2;++inv)for(int hs=0;hs<2;++hs){
  CEngineInternalPrimitive2DAnimatedMesh mesh;mesh.Scale=-64;mesh.Initialised=false;mesh.Layer=(EEngine2DLayer)zi;
  mesh.ClipBox.TLX=origins[xi];mesh.ClipBox.TLY=origins[yi];mesh.ClipBox.BRX=origins[xi]+sides[wi];mesh.ClipBox.BRY=origins[yi]+sides[hi];
  mesh.ObjectBoundingSphere.Center=C3DVector(11,22,33);mesh.ObjectBoundingSphere.Radius=powers[ri];
  mesh.WorldPos.StartPosition=C3DVector(-100,-200,-300);mesh.WorldPos.EndPosition=C3DVector(-400,-500,-600);
  renderer.Calls=0;renderer.ExpectedLayer=mesh.Layer;renderer.Depth=powers[zi];
  camera.OneOverHalfWindowWidth=inv?0.25f:0.5f;camera.OneOverHalfWindowHeight=inv?0.5f:0.25f;camera.WindowCenterX=8;camera.WindowCenterY=16;camera.HomogenousViewScaleX=hs?2.0f:1.0f;
  CMatrix3x4& t=camera.InverseViewTransform;
  t.E11=m==0?1.0f:0.0f;t.E12=m==1?1.0f:0.0f;t.E13=m>=2?1.0f:0.0f;
  t.E21=m>=2?1.0f:0.0f;t.E22=m==0?1.0f:0.0f;t.E23=m==1?1.0f:0.0f;
  t.E31=m==1?1.0f:0.0f;t.E32=m>=2?1.0f:0.0f;t.E33=m==0?1.0f:0.0f;
  if(m==3){t.E13=-1;t.E32=-1;}t.E41=4;t.E42=-8;t.E43=16;
  double depth=renderer.Depth;double x=(mesh.ClipBox.TLX+sides[wi]*0.5-camera.WindowCenterX)*camera.OneOverHalfWindowWidth*depth;
  double y=(camera.WindowCenterY-mesh.ClipBox.TLY-sides[hi]*0.5)*camera.OneOverHalfWindowHeight*depth;
  C3DVector expected((float)(x*t.E11+y*t.E21+depth*t.E31+t.E41),(float)(x*t.E12+y*t.E22+depth*t.E32+t.E42),(float)(x*t.E13+y*t.E23+depth*t.E33+t.E43));
  float scale=(float)((sides[wi]<sides[hi]?sides[wi]:sides[hi])*0.5*camera.OneOverHalfWindowWidth*depth/camera.HomogenousViewScaleX/powers[ri]);
  mesh.Initialise();Check(renderer.Calls==1);Check(mesh.Initialised&&mesh.Scale==scale);
  Check(mesh.WorldPos.StartPosition.X==expected.X&&mesh.WorldPos.StartPosition.Y==expected.Y&&mesh.WorldPos.StartPosition.Z==expected.Z);
  Check(mesh.WorldPos.EndPosition.X==expected.X&&mesh.WorldPos.EndPosition.Y==expected.Y&&mesh.WorldPos.EndPosition.Z==expected.Z);
  Check(mesh.ObjectBoundingSphere.Radius==powers[ri]&&mesh.ObjectBoundingSphere.Center.X==11&&mesh.ObjectBoundingSphere.Center.Y==22&&mesh.ObjectBoundingSphere.Center.Z==33);
  Check(mesh.ClipBox.TLX==origins[xi]&&mesh.ClipBox.BRY==origins[yi]+sides[hi]);++cases;
 }
 Check(cases==20736ul);puts("00bcf4f0_TEST PASS");return 0;
}
