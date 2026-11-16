// 00aeeba0 NParticleEngine::CParticleEmitter::StartFrame standalone behavior test
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CRANDOM
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define private public
#define protected public
// 00aeeba0 NParticleEngine::CParticleEmitter::StartFrame
// Leaf: ?StartFrame@CParticleEmitter@NParticleEngine@@QAEXMABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /d2inlT262
// Inline-budget: native expands the inverse; ordinary helper at T261 gives462B, T262 gives890B. Existing grouped cross preserves the native scheduling region.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CPARTICLEEMITTER_STARTFRAME_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PLAIN_INLINE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_ADD_SET
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "engine/CParticleEmitter.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CAEnvironmentInteraction.h"
namespace NParticleEngine {
CAEnvironmentInteraction& __fastcall GetEnvironmentInteraction();
void CParticleEmitter::StartFrame(const float interpolate,const CEnginePrimitivePrepareInfo& info){
 if(IsSleeping())return;
 if(IsWaitingToSleep()&&GetNoParticles()==0&&GetNoDecals()==0){
  ClearAllPrimitives(&info.Engine);Sleeping=true;return;
 }
 C3DVector current_pos=PositionInterpolator.GetInterpolated(interpolate);
 Transform.Set(OrientationInterpolator.GetInterpolated(interpolate),current_pos);
 InverseTransform.SetInverseNoTranslate(Transform);
 if(Is2DEmitter())DistanceToCam=0.0f;
 else{
  C3DVector camera_pos;
  GetEnvironmentInteraction().GetCameraPos(camera_pos);
  DistanceToCam=(camera_pos-current_pos).GetAccurateMagnitude();
 }
}
}

#include "engine/CIEngine.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBFloatColour.h"
#undef private
#undef protected

CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CIEngine::~CIEngine(){}
CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){assert(!PStringData);}
C3DOrientationInterpolator::C3DOrientationInterpolator(){}
C3DPositionInterpolator::C3DPositionInterpolator(){}C3DPositionInterpolator::~C3DPositionInterpolator(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
CInterpolationInfo::CInterpolationInfo(){memset(this,0,sizeof(*this));}
CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(const CInterpolationInfo&i,CIEngine&e,bool edit,const CCamera&c):Interpolate(i),Engine(e),Edit2DEngine(edit),Camera(c){}
namespace NParticleEngine {
template<class K,class V>CParticleUpdateParameters<K,V>::~CParticleUpdateParameters(){assert(VectorParams.empty()&&UpdateParams.empty());}
CParticleEmitter::CParticleEmitter(){}CParticleEmitter::~CParticleEmitter(){}
CAEnvironmentInteraction::~CAEnvironmentInteraction(){}
bool CAEnvironmentInteraction::IsWindActive()const{abort();}
C3DVector CAEnvironmentInteraction::GetGravityVector()const{abort();}
C3DVector CAEnvironmentInteraction::GetVectorParam(const CCharString&)const{abort();}
}
void CBaseObject::Validate() const{abort();}
void CIEngine::Validate() const{abort();}
bool CIEngine::Initialise(const CEngineInit&){abort();}
void CIEngine::Release(){abort();}
void CIEngine::SetEnvironmentalEffectsAlwaysUpdate(bool){abort();}
void CIEngine::SetWorld(const CIDrawWorld*){abort();}
void CIEngine::WorldFrameUpdate(bool){abort();}
void CIEngine::PausedWorldFrameUpdate(){abort();}
bool CIEngine::SetVideoOptions(const CEngineVideoOptions&){abort();}
void CIEngine::Refresh(){abort();}
bool CIEngine::CheckPrimitiveVisibility(unsigned long long){abort();}
float CIEngine::GetPrimitiveDrawDistanceFactor() const{abort();}
void CIEngine::AddPrimitive(const CEnginePrimitive&){abort();}
bool CIEngine::ClipSphere(const C3DBoundingSphere&) const{abort();}
bool CIEngine::ClipBox(const C3DBoundingBox&) const{abort();}
void CIEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&){abort();}
void CIEngine::SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&){abort();}
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long){abort();}
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long){abort();}
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long){abort();}
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&){abort();}
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long){abort();}
void CIEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle&){abort();}
void CIEngine::SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long){abort();}
void CIEngine::CollectPrimitiveStatsForArea(std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long) const{abort();}
void CIEngine::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, EEnginePrimitiveStatIndex, unsigned long&, CArray<CCharString>&) const{abort();}
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&) const{abort();}
unsigned long CIEngine::AddCustomMorphTexture(CTexture&, const CCharString&) const{abort();}
long CIEngine::GetStaticMapUpdatePassCount(){abort();}
void CIEngine::StartStaticMapUpdatePass(const CWideString&, long, const CStaticMapGenerationVisibiilityInfo&, const CArray<CCharString>&){abort();}
bool CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const{abort();}
void CIEngine::UpdateStaticMap(long, const CCharString&, unsigned long, long){abort();}
void CIEngine::CloseStaticMapFile(){abort();}
void CIEngine::AddMap(long, long){abort();}
void CIEngine::RemoveMap(long){abort();}
void CIEngine::SetMapAsLoaded(long){abort();}
void CIEngine::SetMapAsUnloaded(long){abort();}
void CIEngine::SetMapVisibilityAllMaps(bool){abort();}
void CIEngine::SetMapVisibility(long, bool){abort();}
void CIEngine::AreaChanged(const C2DBoxI&){abort();}
void CIEngine::OnActivate(){abort();}
void CIEngine::OnDeactivate(){abort();}
bool CIEngine::IsActive() const{abort();}
void CIEngine::SetStaticMapFileForUse(const CWideString&){abort();}
void CIEngine::SetLandscapeDebugTintColour(long, CRGBColour){abort();}
void CIEngine::ReloadLocalDetailDefs(){abort();}
void CIEngine::StartNewScene(unsigned long, float){abort();}
void CIEngine::CutscenePreload(bool, bool){abort();}
void CIEngine::CutsceneAddCameraAngle(const CCamera&){abort();}
void CIEngine::ChangeCameraAngle(){abort();}
bool CIEngine::IsPreloaded() const{abort();}
void CIEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&, bool, float, const std::set<long, std::less<long>, std::allocator<long> >*){abort();}
void CIEngine::SetScreenColourFilter(const CEngineColourFilterSettings&){abort();}
void CIEngine::SetSpotlightShadowMode(bool){abort();}
void CIEngine::SetSeaBodyToRender(CCharString){abort();}
void CIEngine::UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&){abort();}
void CIEngine::EnableDebugOverlay(long, bool, bool){abort();}
void CIEngine::SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&){abort();}
void CIEngine::Set2DPrimitiveLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&){abort();}
void CIEngine::SetRenderTargetTexture(const CTexture*){abort();}
CTexture* CIEngine::AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&){abort();}
void CIEngine::FreeTextureFromEngineResourcePools(CTexture*){abort();}
CMovableResourceMemoryPool* CIEngine::GetLoadingMemoryPool(){abort();}
CMovableResourceMemoryPool* CIEngine::GetAnimationMemoryPool(){abort();}
CGPUScratchBufferTextureManager* CIEngine::GetScratchBufferTextureManager(){abort();}
bool CIEngine::DeallocateShadowBuffer(){abort();}
void CIEngine::ReallocateShadowBuffer(){abort();}
void CIEngine::Enable3DSceneRendering(bool){abort();}
bool CIEngine::Is3DSceneRenderingEnabled(){abort();}
void CIEngine::Enable2DSceneRendering(bool){abort();}
bool CIEngine::Is2DSceneRenderingEnabled(){abort();}
void CIEngine::OverrideRain(float){abort();}
void CIEngine::OverrideSnow(float){abort();}
void CIEngine::RenderColorFilteredQuad(float, float, const C4DVector&, float, float, CTexture*){abort();}
const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const{abort();}
const CRGBFloatColour CIEngine::PeekGlobalAmbientColour() const{abort();}
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&){abort();}
bool CEnginePersistentPrimitiveBase::WasRendered() const{abort();}
class FixtureEngine:public CIEngine{public:
virtual void SetCamera(const CCamera*) {abort();}
virtual void SetWindow(const C2DBoxF*) {abort();}
virtual void Virtual08_00b27d90() {abort();}
virtual void Virtual11_00b23cf0() {abort();}
virtual void Virtual12_00b24400() {abort();}
virtual void Virtual13_00b242d0() {abort();}
virtual bool PreChangeResolution() {abort();}
virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) {abort();}
virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) {abort();}
};
using namespace NParticleEngine;
static CParticleEmitter* ExpectedEmitter;static CIEngine* ExpectedEngine;static bool Active;
static long Particles,Decals;static int ParticleCalls,DecalCalls,ClearCalls,OrientationCalls,EnvironmentCalls,CameraCalls;
static float ExpectedTime;static C3DVector CameraPosition;static CRightHandedSet OutputOrientation;
long CParticleEmitter::GetNoParticles()const{assert(Active&&this==ExpectedEmitter);++ParticleCalls;return Particles;}
long CParticleEmitter::GetNoDecals()const{assert(Active&&this==ExpectedEmitter&&ParticleCalls==1&&Particles==0);++DecalCalls;return Decals;}
void CParticleEmitter::ClearAllPrimitives(CIEngine*e){assert(Active&&this==ExpectedEmitter&&e==ExpectedEngine&&Particles==0&&Decals==0&&ParticleCalls==1&&DecalCalls==1);++ClearCalls;}
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet&end,float t)const{assert(Active&&this==&ExpectedEmitter->OrientationInterpolator.StartOrientation&&&end==&ExpectedEmitter->OrientationInterpolator.EndOrientation&&t==ExpectedTime);++OrientationCalls;return OutputOrientation;}
class FixtureEnvironment:public CAEnvironmentInteraction{
public:
 bool IsParticleCollidingAt(const C3DVector&,const C3DVector&,const bool,C3DVector&,C3DVector&,bool&)const{abort();}
 C3DVector GetWindVelocity()const{abort();}
 void GetCameraPos(C3DVector&v)const{assert(Active&&EnvironmentCalls==1&&OrientationCalls==1&&ExpectedEmitter&&!ExpectedEmitter->Emitter2D);++CameraCalls;v=CameraPosition;}
};
static FixtureEnvironment Environment;
CAEnvironmentInteraction& __fastcall NParticleEngine::GetEnvironmentInteraction(){assert(Active);++EnvironmentCalls;return Environment;}
static bool Near(float actual,double expected){return fabs((double)actual-expected)<=2e-5*(1+fabs(expected));}
static void InitMatrix(CMatrix3x4&m,float x){m.Set(x,x+1,x+2,x+3,x+4,x+5,x+6,x+7,x+8,x+9,x+10,x+11);}
static void CheckInverse(const CMatrix3x4&m,const CMatrix3x4&inv){
 const float a[]={m.E11,m.E12,m.E13,m.E21,m.E22,m.E23,m.E31,m.E32,m.E33};const float b[]={inv.E11,inv.E12,inv.E13,inv.E21,inv.E22,inv.E23,inv.E31,inv.E32,inv.E33};
 for(int i=0;i<3;++i)for(int j=0;j<3;++j){double sum=0;for(int k=0;k<3;++k)sum+=(double)a[i*3+k]*b[k*3+j];assert(fabs(sum-(i==j?1.0:0.0))<3e-5);}
 assert(inv.E41==0&&inv.E42==0&&inv.E43==0);
}
int main(){
 FixtureEngine engine;engine.Active=false;CCamera camera;camera.WorldPosition=C3DVector(1,2,3);camera.RHSet.Set(C3DVector(0,0,1),C3DVector(0,1,0));camera.HeightLocked=false;camera.ViewVecZLocked=false;camera.FOVFlags=0;camera.HorizontalFOV=1;camera.VerticalFOV=1;camera.Zoom=1;CInterpolationInfo interpolation;CEnginePrimitivePrepareInfo info(interpolation,engine,false,camera);CParticleEmitter emitter;ExpectedEmitter=&emitter;ExpectedEngine=&engine;
 unsigned cases=0;const float times[]={-.5f,0,.25f,1,1.5f};const long counts[]={0,1,-1};
 for(int seed=0;seed<8;++seed)for(int t=0;t<5;++t)for(int flags=0;flags<8;++flags)for(int p=0;p<3;++p)for(int d=0;d<3;++d){
  Active=false;ParticleCalls=DecalCalls=ClearCalls=OrientationCalls=EnvironmentCalls=CameraCalls=0;Particles=counts[p];Decals=counts[d];ExpectedTime=times[t];
  emitter.Priority=seed%8;emitter.IsScreenDisplacement=1;emitter.ReadZBuffer=0;emitter.Dead=1;emitter.ParticleCreationDisabled=0;emitter.ContinuousEmitter=1;emitter.Initialised=1;emitter.WaitingToSleep=(flags&2)!=0;emitter.Sleeping=(flags&1)!=0;emitter.Emitter2D=(flags&4)!=0;emitter.Emitter2DLayer=seed;
  emitter.PreWaterEffect=1;emitter.WaterEffect=0;emitter.ZBufferWriteable=1;emitter.Visible=0;emitter.AlwaysUpdate=1;emitter.DieIfOffsceen=0;emitter.OffscreenUpdate=1;emitter.ClipEffectToWeatherMask=0;emitter.EnableDithering=1;emitter.CalcBoundingSphereOnceOnly=0;emitter.BoundingSphereValid=1;emitter.ClearedParticles=0;emitter.ClearedDecals=1;
  emitter.OldPos=C3DVector((float)seed+1,(float)seed+2,(float)seed+3);emitter.Velocity=C3DVector((float)seed+4,(float)seed+5,(float)seed+6);
  emitter.PositionInterpolator.StartPosition=C3DVector(seed*.25f,-seed*.5f,seed*.125f);emitter.PositionInterpolator.EndPosition=C3DVector(seed*.25f+2,-seed*.5f-4,seed*.125f+1);
  emitter.OrientationInterpolator.StartOrientation.Set(C3DVector(0,0,1),C3DVector(0,1,0));emitter.OrientationInterpolator.EndOrientation.Set(C3DVector(1,0,1),C3DVector(0,2,0));
  OutputOrientation.Set(C3DVector(.125f*seed,0,1+.125f*seed),C3DVector(0,1+.25f*seed,.125f));CameraPosition=C3DVector((float)seed+8,(float)seed-12,(float)seed+16);
  InitMatrix(emitter.Transform,100);InitMatrix(emitter.InverseTransform,200);InitMatrix(emitter.EndTransform,300);emitter.DistanceToCam=-100;emitter.MaxSpawnDistance=10;emitter.MaxDrawDistance=20;emitter.FadeOutStart=30;emitter.FadeInStart=40;emitter.FadeInEnd=50;
  emitter.BSphere.Center=C3DVector(7,8,9);emitter.BSphere.Radius=10;
  unsigned char before[sizeof(emitter)];memcpy(before,&emitter,sizeof(emitter));bool old_sleep=emitter.Sleeping;
  bool waiting=emitter.WaitingToSleep,twod=emitter.Emitter2D;CMatrix3x4 old_transform=emitter.Transform,old_inverse=emitter.InverseTransform;float old_distance=emitter.DistanceToCam;
  Active=true;emitter.StartFrame(ExpectedTime,info);Active=false;
  bool clear=!old_sleep&&waiting&&Particles==0&&Decals==0,work=!old_sleep&&!clear;
  assert(ParticleCalls==(!old_sleep&&waiting?1:0));assert(DecalCalls==(!old_sleep&&waiting&&Particles==0?1:0));assert(ClearCalls==(clear?1:0));assert(OrientationCalls==(work?1:0));assert(CameraCalls==(work&&!twod?1:0)&&EnvironmentCalls==CameraCalls);assert(emitter.Sleeping==(old_sleep||clear));
  if(work){
   C3DVector expected(seed*.25f+2*ExpectedTime,-seed*.5f-4*ExpectedTime,seed*.125f+ExpectedTime);const C3DVector&u=OutputOrientation.GetUp();const C3DVector&f=OutputOrientation.GetForward();C3DVector right(f.Y*u.Z-f.Z*u.Y,f.Z*u.X-f.X*u.Z,f.X*u.Y-f.Y*u.X);
   const CMatrix3x4&tm=emitter.Transform;const float m[]={tm.E11,tm.E12,tm.E13,tm.E21,tm.E22,tm.E23,tm.E31,tm.E32,tm.E33,tm.E41,tm.E42,tm.E43};const float expected_matrix[]={right.X,right.Y,right.Z,f.X,f.Y,f.Z,u.X,u.Y,u.Z,expected.X,expected.Y,expected.Z};for(int n=0;n<12;++n)assert(Near(m[n],expected_matrix[n]));CheckInverse(emitter.Transform,emitter.InverseTransform);
   double dx=(double)CameraPosition.X-expected.X,dy=(double)CameraPosition.Y-expected.Y,dz=(double)CameraPosition.Z-expected.Z;assert(Near(emitter.DistanceToCam,twod?0:sqrt(dx*dx+dy*dy+dz*dz)));
  }else{assert(memcmp(&emitter.Transform,&old_transform,sizeof(old_transform))==0&&memcmp(&emitter.InverseTransform,&old_inverse,sizeof(old_inverse))==0&&emitter.DistanceToCam==old_distance);}
  emitter.Sleeping=old_sleep;emitter.Transform=old_transform;emitter.InverseTransform=old_inverse;emitter.DistanceToCam=old_distance;assert(memcmp(before,&emitter,sizeof(emitter))==0);assert(&info.Engine==&engine&&!info.Edit2DEngine);++cases;
 }
 assert(cases==2880);puts("00aeeba0_TEST PASS");return 0;
}