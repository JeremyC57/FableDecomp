// Full target and both original inline distance members copied verbatim from00bf7bc0.cpp.
// Fixtures model real semantic contracts. long64 is harmless: only small band indices are used.
// LODLoadInfo has nine actual backing elements for the observed lookup-before-bound behavior;
// this extra fixture storage is not a claim about the native eight-element class layout.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "fable_compiler.h"
#ifdef _MSC_VER
#define FABLE_DUAL_PATH_TEST
#else
#define FABLE_PORTABLE
#endif
#include "engine/C3DBoundingBoxSSE.h"
class C3DVector {
public:
 float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 const C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
};
class C3DBoundingBox {
public:
 C3DVector MinCoord,MaxCoord;
 float CalcShortestDistanceToPointSquared(const C3DVector&)const;
 float CalcShortestDistanceToPoint(const C3DVector&)const;
};
class C3DBoundingSphere {public:C3DVector Center;float Radius;};
class CEngineCamera {public:C3DVector Position;const C3DVector& PeekCameraPos()const{return Position;}};
class CEngineLandscapeRenderer {public:float LODZBandsZ[9];float GetLODZBandZ(long band){assert(band>=0&&band<=8);return LODZBandsZ[band];}};
class CLandscapeBackgroundTreeNode;
class CLandscapeBackgroundSubNodeBase {public:virtual bool IsPatch()=0;};
class CLandscapeBackgroundPatch:public CLandscapeBackgroundSubNodeBase {
public:
 unsigned char LODBand;
 bool ReturnsPatch;
 int Effect,Calls;
 CLandscapeBackgroundTreeNode* Node;
 virtual bool IsPatch();
};
class CLandscapeBackgroundTreeNode {
public:
 struct CLODLoadInfo {long FileBlockPos,FileBlockSize,OffsetIntoFileBlock;unsigned char OptimizedBandRemap;};
 CLODLoadInfo LODLoadInfo[9]; // real sentinel backing, safe portable access at8
 CLandscapeBackgroundSubNodeBase* SubNode;
 C3DBoundingBox BoundingBox;
 C3DBoundingSphere CameraUpdateThreshold;
 unsigned char FirstNonSplitBand;
 CLandscapeBackgroundPatch* GetPatch(){return SubNode&&SubNode->IsPatch()?static_cast<CLandscapeBackgroundPatch*>(SubNode):0;}
 CLODLoadInfo& GetLodBandInfo(long band){assert(band>=0&&band<=8);return LODLoadInfo[band];}
 void CalculateCameraUpdateThreshold(long current_lod);
};
bool GEnableSIMD;
namespace NEngineInternal {CEngineCamera* GPCamera;CEngineLandscapeRenderer* GPLandscapeRenderer;}
static CEngineCamera AlternateCamera;
static CEngineLandscapeRenderer AlternateRenderer;
static CLandscapeBackgroundPatch AlternatePatch;
bool CLandscapeBackgroundPatch::IsPatch(){
 ++Calls;
 if(Effect&1)NEngineInternal::GPCamera=&AlternateCamera;
 if(Effect&2)NEngineInternal::GPLandscapeRenderer=&AlternateRenderer;
 if(Effect&4)Node->SubNode=0;
 if(Effect&8)Node->SubNode=&AlternatePatch;
 return ReturnsPatch;
}
inline float C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const
{
    if(GEnableSIMD) {
        float result;
        GF_SSE_BOX_DISTANCE_SQUARED(this, point, result);
        return result;
    }
    C3DVector closest=point;
    if(closest.X<MinCoord.X) closest.X=MinCoord.X;
    else if(closest.X>MaxCoord.X) closest.X=MaxCoord.X;
    if(closest.Y<MinCoord.Y) closest.Y=MinCoord.Y;
    else if(closest.Y>MaxCoord.Y) closest.Y=MaxCoord.Y;
    if(closest.Z<MinCoord.Z) closest.Z=MinCoord.Z;
    else if(closest.Z>MaxCoord.Z) closest.Z=MaxCoord.Z;
    return (point-closest).GetSquaredMagnitude();
}
inline float C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector& point) const
{
    return (float)sqrt(CalcShortestDistanceToPointSquared(point));
}
void CLandscapeBackgroundTreeNode::CalculateCameraUpdateThreshold(long current_lod)
{
    float distance_from_camera=BoundingBox.CalcShortestDistanceToPoint(NEngineInternal::GPCamera->PeekCameraPos());
    float inc_band_distance=1.0e30f;
    float dec_band_distance=1.0e30f;
    CLandscapeBackgroundPatch* patch=GetPatch();
    if(patch) {
        long patch_band=patch->LODBand;
        if(patch_band<8) {
            long next_patch=patch_band+1;
            while(GetLodBandInfo(next_patch).OptimizedBandRemap==patch_band && next_patch<8)
                ++next_patch;
            if(next_patch<8)
                inc_band_distance=NEngineInternal::GPLandscapeRenderer->GetLODZBandZ(next_patch)-distance_from_camera;
        }
        if(patch_band>1)
            dec_band_distance=distance_from_camera-NEngineInternal::GPLandscapeRenderer->GetLODZBandZ(patch_band);
    } else {
        inc_band_distance=NEngineInternal::GPLandscapeRenderer->GetLODZBandZ(FirstNonSplitBand)-distance_from_camera;
    }
    CameraUpdateThreshold.Center=NEngineInternal::GPCamera->PeekCameraPos();
    CameraUpdateThreshold.Radius=inc_band_distance<dec_band_distance?inc_band_distance:dec_band_distance;
}

static unsigned int Bits(float x){unsigned int b;memcpy(&b,&x,4);return b;}
static float FromBits(unsigned int b){float x;memcpy(&x,&b,4);return x;}
static bool Same(float a,float b){return Bits(a)==Bits(b)||((Bits(a)&0x7f800000u)==0x7f800000u&&(Bits(a)&0x7fffffu)&&(Bits(b)&0x7f800000u)==0x7f800000u&&(Bits(b)&0x7fffffu));}
static void CheckKernel(const C3DBoundingBox& box,const C3DVector& point){
 float actual;
 const C3DBoundingBox* boxptr=&box;
 GF_SSE_BOX_DISTANCE_SQUARED(boxptr, point, actual);
 float portable=fable_portable::BoxDistanceSquaredSSE((const float*)&box,(const float*)&point);
 // Multi-NaN payload selection differs between existing x87 portable and SSE paths.
 // Engine coordinates are finite. Preserve exploratory NaN classification, with no
 // source-level NaN payload contract; all non-NaN results are checked bit-for-bit.
 unsigned int ab=Bits(actual),pb=Bits(portable);
 if((ab&0x7f800000u)==0x7f800000u&&(ab&0x7fffffu))
  assert((pb&0x7f800000u)==0x7f800000u&&(pb&0x7fffffu));
 else assert(ab==pb);
}
static float ReferenceDistance(const C3DBoundingBox& box,const C3DVector& point){
 double squared=0.0;
 const float values[3]={point.X,point.Y,point.Z};
 const float lo[3]={box.MinCoord.X,box.MinCoord.Y,box.MinCoord.Z};
 const float hi[3]={box.MaxCoord.X,box.MaxCoord.Y,box.MaxCoord.Z};
 for(int k=0;k<3;++k){double delta=0.0;if(values[k]<lo[k])delta=(double)lo[k]-values[k];else if(values[k]>hi[k])delta=(double)values[k]-hi[k];squared+=delta*delta;}
 return (float)sqrt(squared);
}
int main(){
#ifdef _MSC_VER
 _controlfp(_PC_24|_RC_NEAR,_MCW_PC|_MCW_RC);
#endif
 CEngineCamera camera;CEngineLandscapeRenderer renderer;
 const C3DVector positions[]={C3DVector(0,0,0),C3DVector(-8,0,0),C3DVector(8,0,0),C3DVector(0,8,0),C3DVector(0,0,8),C3DVector(8,8,8),C3DVector(0.25f,-0.5f,0.75f)};
 unsigned int cases=0;
 for(int simd=0;simd<2;++simd)for(int boxcase=0;boxcase<3;++boxcase)for(int cp=0;cp<7;++cp)for(int band=0;band<8;++band)for(int pattern=0;pattern<5;++pattern)for(int mode=0;mode<7;++mode){
  CLandscapeBackgroundTreeNode node;CLandscapeBackgroundPatch patch;
  memset(&node.LODLoadInfo,0,sizeof(node.LODLoadInfo));
  node.BoundingBox.MinCoord=C3DVector((float)(-2-boxcase),-1.0f,0.0f);
  node.BoundingBox.MaxCoord=C3DVector((float)(2+boxcase),1.0f,(float)boxcase);
  camera.Position=positions[cp];AlternateCamera.Position=C3DVector(32,-16,8);
  for(int k=0;k<9;++k){renderer.LODZBandsZ[k]=(float)(k*8);AlternateRenderer.LODZBandsZ[k]=(float)(k*16+4);node.LODLoadInfo[k].OptimizedBandRemap=(unsigned char)(pattern==0?k:pattern==1?band:pattern==2?(k<=band+2?band:k):pattern==3?0:7);}
  patch.LODBand=(unsigned char)band;patch.ReturnsPatch=mode!=1;patch.Effect=mode==3?1:mode==4?2:mode==5?4:mode==6?8:0;patch.Calls=0;patch.Node=&node;
  AlternatePatch.LODBand=(unsigned char)((band+3)%8);AlternatePatch.ReturnsPatch=true;AlternatePatch.Effect=0;AlternatePatch.Calls=0;AlternatePatch.Node=&node;
  node.SubNode=mode==0?0:&patch;node.FirstNonSplitBand=(unsigned char)((band%7)+1);
  node.CameraUpdateThreshold.Center=C3DVector(-100,-100,-100);node.CameraUpdateThreshold.Radius=-100;
  NEngineInternal::GPCamera=&camera;NEngineInternal::GPLandscapeRenderer=&renderer;GEnableSIMD=simd!=0;
  float distance=ReferenceDistance(node.BoundingBox,camera.Position);
  bool ispatch=mode>=2&&mode!=5;
  int observed_band=mode==6?AlternatePatch.LODBand:band;
  CEngineLandscapeRenderer* observed_renderer=mode==4?&AlternateRenderer:&renderer;
  float inc=1.0e30f,dec=1.0e30f;
  if(ispatch){
   int next=observed_band+1;
   while(node.LODLoadInfo[next].OptimizedBandRemap==observed_band&&next<8)++next;
   if(next<8)inc=observed_renderer->LODZBandsZ[next]-distance;
   if(observed_band>1)dec=distance-observed_renderer->LODZBandsZ[observed_band];
  }else inc=observed_renderer->LODZBandsZ[node.FirstNonSplitBand]-distance;
  float expected=inc<dec?inc:dec;
  node.CalculateCameraUpdateThreshold(12345); // unused release argument
  assert(fabs((double)node.CameraUpdateThreshold.Radius-expected)<0.0001||node.CameraUpdateThreshold.Radius==expected);
  C3DVector center=mode==3?AlternateCamera.Position:camera.Position;
  assert(Same(node.CameraUpdateThreshold.Center.X,center.X)&&Same(node.CameraUpdateThreshold.Center.Y,center.Y)&&Same(node.CameraUpdateThreshold.Center.Z,center.Z));
  assert(patch.Calls==(mode==0?0:1));assert(AlternatePatch.Calls==0);
  CheckKernel(node.BoundingBox,camera.Position);++cases;
 }
 assert(cases==11760u);
 // Shared helper versus portable twin, signed zero/NaN/Inf plus generated finite triples.
 C3DBoundingBox box;box.MinCoord=C3DVector(-2,-1,0);box.MaxCoord=C3DVector(2,1,4);
 unsigned int edge[]={0u,0x80000000u,0x7f800000u,0xff800000u,0x7fc12345u,0xffc54321u,0x3f800000u,0xbf800000u};
 for(unsigned int i=0;i<8;++i)for(unsigned int j=0;j<8;++j)for(unsigned int k=0;k<8;++k)CheckKernel(box,C3DVector(FromBits(edge[i]),FromBits(edge[j]),FromBits(edge[k])));
 unsigned int state=0x41312803u;
 for(int i=0;i<20000;++i){float v[3];for(int k=0;k<3;++k){state=state*1664525u+1013904223u;v[k]=(float)((int)(state&65535u)-32768)*0.00390625f;}CheckKernel(box,C3DVector(v[0],v[1],v[2]));}
 puts("00bf7bc0_TEST PASS");return 0;
}