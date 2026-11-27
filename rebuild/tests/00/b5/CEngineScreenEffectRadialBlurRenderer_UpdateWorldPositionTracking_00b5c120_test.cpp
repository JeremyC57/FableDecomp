#include <cmath>
#include <cstdio>
#include <cstdlib>
struct C3DVector { float X,Y,Z; };
struct C2DVector { float X,Y; };
struct C2DExtentsI { int X,Y; };
struct Camera {
    float m[4][3],near_z,cx,cy,hw,hh,factor,offset;
    unsigned int transforms,projections;
    void TransformWorldToView(const C3DVector& p,C3DVector& q) {
        ++transforms;
        q.X=m[3][0]+(p.X*m[0][0]+p.Y*m[1][0]+p.Z*m[2][0]);
        q.Y=m[3][1]+(p.X*m[0][1]+p.Y*m[1][1]+p.Z*m[2][1]);
        q.Z=m[3][2]+(p.X*m[0][2]+p.Y*m[1][2]+p.Z*m[2][2]);
    }
    float PeekNearZ() const {return near_z;}
    void TransformViewToScreen(const C3DVector& p,C3DVector& q) {
        ++projections;const float inv=1.0f/p.Z;
        q.X=p.X*inv*hw+cx;q.Y=cy-p.Y*inv*hh;q.Z=factor*inv+offset;
    }
};
struct Display {
    unsigned int calls;
    C2DExtentsI GetRenderTargetDimensions() {C2DExtentsI d={128+(int)calls,256+(int)calls};++calls;return d;}
};
static Camera camera;static Display display;
static Camera* GPCamera=&camera;static Display* GPDisplayManager=&display;
struct CEngineScreenEffectRadialBlurRenderer{
    bool BlurTrack3DPos,AreaTrack3DPos;C3DVector BlurCenterWorldTrackPosition,AreaCenterWorldTrackPosition;C2DVector BlurCenter,AreaCenter;
    bool UpdateWorldPositionTracking();
};
bool CEngineScreenEffectRadialBlurRenderer::UpdateWorldPositionTracking()
{
    if(BlurTrack3DPos) {
        C3DVector view_pos;
        GPCamera->TransformWorldToView(BlurCenterWorldTrackPosition,view_pos);
        if(view_pos.Z<GPCamera->PeekNearZ()) return false;
        C3DVector screen_pos;
        GPCamera->TransformViewToScreen(view_pos,screen_pos);
        BlurCenter.X=screen_pos.X/GPDisplayManager->GetRenderTargetDimensions().X;
        BlurCenter.Y=screen_pos.Y/GPDisplayManager->GetRenderTargetDimensions().Y;
    }
    if(AreaTrack3DPos) {
        C3DVector view_pos;
        GPCamera->TransformWorldToView(AreaCenterWorldTrackPosition,view_pos);
        if(view_pos.Z<GPCamera->PeekNearZ()) return false;
        C3DVector screen_pos;
        GPCamera->TransformViewToScreen(view_pos,screen_pos);
        AreaCenter.X=screen_pos.X/GPDisplayManager->GetRenderTargetDimensions().X;
        AreaCenter.Y=screen_pos.Y/GPDisplayManager->GetRenderTargetDimensions().Y;
    }
    return true;
}

static void require(bool ok){if(!ok)std::exit(1);}
static bool closeEnough(float a,double b){return std::fabs(double(a)-b)<=0.00001*(1+std::fabs(b));}
int main() {
 for(unsigned int mask=0;mask<16384;++mask) {
    CEngineScreenEffectRadialBlurRenderer r;
    r.BlurTrack3DPos=(mask&1)!=0;r.AreaTrack3DPos=(mask&2)!=0;
    camera.transforms=camera.projections=display.calls=0;camera.cx=4;camera.cy=-8;
    camera.hw=16;camera.hh=32;camera.factor=-2;camera.offset=4;camera.near_z=float(1+((mask>>2)&3));
    for(int y=0;y<4;++y)for(int x=0;x<3;++x)camera.m[y][x]=float(((mask>>((x+3*y)%10))&1)?1:-1);
    camera.m[3][2]=float(int((mask>>4)&15)-2);
    C3DVector inputs[2];C2DVector expected[2];bool enabled[2]={r.BlurTrack3DPos,r.AreaTrack3DPos};
    for(int t=0;t<2;++t){
      inputs[t].X=float(int((mask>>(5+t))&7)-3);inputs[t].Y=float(int((mask>>(8+t))&7)-3);inputs[t].Z=float(int((mask>>(10+t))&7)-2);
      expected[t].X=float(-90+t);expected[t].Y=float(90-t);
    }
    r.BlurCenter=expected[0];r.AreaCenter=expected[1];r.BlurCenterWorldTrackPosition=inputs[0];r.AreaCenterWorldTrackPosition=inputs[1];
    unsigned int calls=0,visited=0,projected=0;bool accepted=true;
    for(int t=0;t<2;++t)if(enabled[t]){
      ++visited;double v[3];
      for(int j=0;j<3;++j)v[j]=double(camera.m[3][j])+inputs[t].X*camera.m[0][j]+inputs[t].Y*camera.m[1][j]+inputs[t].Z*camera.m[2][j];
      if(v[2]<camera.near_z){accepted=false;break;}
      ++projected;expected[t].X=float((camera.cx+camera.hw*v[0]/v[2])/(128+calls));++calls;
      expected[t].Y=float((camera.cy-camera.hh*v[1]/v[2])/(256+calls));++calls;
    }
    require(r.UpdateWorldPositionTracking()==accepted && camera.transforms==visited && camera.projections==projected && display.calls==calls);
    require(closeEnough(r.BlurCenter.X,expected[0].X)&&closeEnough(r.BlurCenter.Y,expected[0].Y));
    require(closeEnough(r.AreaCenter.X,expected[1].X)&&closeEnough(r.AreaCenter.Y,expected[1].Y));
    C3DVector probe={2,4,8};camera.TransformViewToScreen(probe,probe);require(closeEnough(probe.Z,3.75));
 }
 std::puts("00b5c120_TEST PASS");return 0;
}