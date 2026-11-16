// Standalone real shared lighting/camera/heap objects, original target and controlled external operations.
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
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define private public
#define protected public
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
// 00b47bc0 CEngineLightingManager::UpdateShadowedSpotLights
// Flags: /d2inlT29
// Leaf: ?UpdateShadowedSpotLights@CEngineLightingManager@@AAEXXZ
// Inline-budget: /d2inlT29 -- smallest tested integer threshold retaining the real out-of-line element constructor and expanding original caller helpers; default expands constructor1098B,10..28 differ,29 yields1028B.
// Retail: Fable.exe00b47bc0,1028bytes.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_CRGBFLOATCOLOUR_SCALE
#define FABLE_RETAIL_STLP_TAG_CTORS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
namespace NEngineInternal {extern CEngineCamera* GPCamera;}
CEngineLocalLightElement::CEngineLocalLightElement(const C3DVector& world_pos,float radius,float spot_radius,const CRGBFloatColour& colour,unsigned long channel_mask):WorldPos(world_pos),Radius(radius),RadiusSquared(radius*radius),SpotRadius(spot_radius),Colour(colour),ChannelMask(channel_mask)
{
 AttenuationDistSquared=1.0f/(SpotRadius*SpotRadius)-1.0f/(Radius*Radius);
 AttenuationConstant=(1.0f/(Radius*Radius)/AttenuationDistSquared)*-1.0f;
}

void CEngineLightingManager::UpdateShadowedSpotLights()
{
    if (!ShadowedSpotLightsEnabled) {
        CurrentShadowedSpotLightParams.Active = false;
        return;
    }
    long nearest_index = -1;
    float nearest_distance_squared = 1e30f;
    long second_nearest_index = -1;
    float second_nearest_distance_squared = 1e30f;
    C3DVector cam_pos = NEngineInternal::GPCamera->PeekCameraPos();
    for (long n = 0; n < ShadowedSpotLightList.size(); ++n) {
        float distance_squared=(cam_pos-ShadowedSpotLightList[n]->Position).GetSquaredMagnitude();
        if (distance_squared < nearest_distance_squared) {
            nearest_distance_squared = distance_squared;
            nearest_index = n;
        }
    }
    for (long n = 0; n < ShadowedSpotLightList.size(); ++n) {
        float distance_squared=(cam_pos-ShadowedSpotLightList[n]->Position).GetSquaredMagnitude();
        if (n != nearest_index && distance_squared < second_nearest_distance_squared) {
            second_nearest_distance_squared = distance_squared;
            second_nearest_index = n;
        }
    }
    if (nearest_index >= 0) {
        CManagerShadowedSpotLight* nearest_light = ShadowedSpotLightList[nearest_index];
        CurrentShadowedSpotLightParams.Active = true;
        CurrentShadowedSpotLightParams.Colour = nearest_light->Colour;
        CurrentShadowedSpotLightParams.ConeWidth = nearest_light->ConeRadius * 2.0f;
        float inv_fade_alpha = 0.0f;
        CurrentShadowedSpotLightParams.Radius = nearest_light->Radius;
        CurrentShadowedSpotLightParams.SpotRadius = nearest_light->SpotRadius;
        CurrentShadowedSpotLightParams.Vector = nearest_light->Direction;
        CurrentShadowedSpotLightParams.Pos = nearest_light->Position;
        if (second_nearest_index >= 0) {
            CManagerShadowedSpotLight* second_light=ShadowedSpotLightList[second_nearest_index];
            float dist0=(cam_pos-nearest_light->Position).GetAccurateMagnitude();
            float dist1=(cam_pos-second_light->Position).GetAccurateMagnitude();
            float total=dist0+dist1;
            dist0/=total;dist1/=total;
            float faded_range=0.5f/2.0;
            inv_fade_alpha=(dist0-(0.5-faded_range))/faded_range;
            if (inv_fade_alpha < 0.0f)
                inv_fade_alpha = 0.0f;
            else if (inv_fade_alpha > 1.0f)
                inv_fade_alpha = 1.0f;
            float fade_alpha=1.0f-inv_fade_alpha;
            CurrentShadowedSpotLightParams.Colour*=fade_alpha;
        }
        for (long n = 0; n < ShadowedSpotLightList.size(); ++n) {
            float scale = (n == nearest_index) ? inv_fade_alpha : 1.0f;
            CManagerShadowedSpotLight* spot_light = ShadowedSpotLightList[n];
            CManagerLocalLight* local_light=new CManagerLocalLight(spot_light->Position,spot_light->Radius,spot_light->SpotRadius,spot_light->Colour*scale,0xFFFFFFFF);
            LocalLightList.push_back(local_light);
        }
    } else
        CurrentShadowedSpotLightParams.Active = false;
}

#undef private
#undef protected


CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CEngineComponent::CEngineComponent():GlobalPointerAddress(0),RenderEnabled(true),KeepAlive(0){}CEngineComponent::~CEngineComponent(){}
CEnginePrimitiveRenderUtil::~CEnginePrimitiveRenderUtil(){}
static CArray<CEngineScene*> Scenes;
CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>&s):SceneList(s),Enable(true){}CEnginePrimitiveManager::~CEnginePrimitiveManager(){}
CEngineLightingManager::CEngineLightingManager():CEnginePrimitiveManager(Scenes){}
CEngineCamera::CEngineCamera(bool){}
namespace NEngineInternal {CEngineCamera* GPCamera=0;}
CEngineLightingManager::CManagerShadowedSpotLight::CManagerShadowedSpotLight(const C3DVector&p,const C3DVector&d,float r,float s,float c,const CRGBFloatColour&colour):Position(p),Direction(d),Radius(r),SpotRadius(s),ConeRadius(c),Colour(colour){}
void CEngineLightingManager::CManagerShadowedSpotLight::Delete(){abort();}
void CEngineLightingManager::CManagerLocalLight::Delete(){abort();}
void __fastcall CEngineLightingManager::CManagerLocalLight::operator delete(void*){abort();}
CDynamicObjectHeapBase::CDynamicObjectHeapBase():AllocCount(0),TotalAllocatorBlocks(1),RefCountReleaseEnabled(false),HeapNext(0){}CDynamicObjectHeapBase::~CDynamicObjectHeapBase(){}
template<> CDynamicObjectHeap<72,16>::CDynamicObjectHeap(bool grow):FreeList(0),AllowHeapToGrow(grow){}
template<> CDynamicObjectHeap<72,16>::~CDynamicObjectHeap(){}
template<> void CDynamicObjectHeap<72,16>::Release(){abort();}
static int GrowthCalls,InitialFree;
static CDynamicObjectHeap<72,16>* ExpectedHeap;
template<> void CDynamicObjectHeap<72,16>::CreateMoreBlocks(){
 assert(this==ExpectedHeap&&AllowHeapToGrow&&!FreeList&&GrowthCalls==0);++GrowthCalls;
 for(int n=InitialFree;n<8;++n)StartBlock.Blocks[n*2].NextEntry=n<7?&StartBlock.Blocks[(n+1)*2]:0;
 FreeList=&StartBlock.Blocks[InitialFree*2];
}
CDynamicObjectHeap<72,16> LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight(false);

void CEngineComponent::FrameStart(const CEngineComponentParams&){abort();}
void CEngineComponent::FrameEnd(const CEngineComponentParams&){abort();}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&){abort();}
void CEngineComponent::Render(const CEngineComponentRenderParams&){abort();}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&){abort();}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&){abort();}
void CEngineComponent::WorldFrameUpdate(bool){abort();}
void CEngineComponent::PausedWorldFrameUpdate(){abort();}
CCharString CEngineComponent::GetConsoleEnableFunctionName(){abort();}
void CEngineComponent::OnReleaseDefaultPoolResources(){abort();}
bool CEngineComponent::OnRestoreDefaultPoolResources(){abort();}
void CEnginePrimitiveManager::FrameStart(){abort();}
void CEnginePrimitiveManager::FrameEnd(){abort();}
void CEnginePrimitiveManager::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass){abort();}
void CEnginePrimitiveManager::RenderBoundingVolume(CEngineSubPrimitiveBase& sub){abort();}
ERendered CEnginePrimitiveManager::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim){abort();}
bool CEnginePrimitiveManager::PreloadPrimitive(CEngineInternalPrimitiveBase& prim, const C3DVector& pos, unsigned long flags){abort();}
void CEnginePrimitiveManager::Render2DPrimitive(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type){abort();}
void CEnginePrimitiveManager::RenderNonsortedPrimitives(){abort();}
void CEnginePrimitiveManager::AddStaticShadowData(CEngineInternalPrimitiveBase& prim, NEngineShadowRenderer::CShadowSceneContext& context){abort();}
CEngineInternalPrimitiveBase* CEngineLightingManager::CreateInternalPrimitive(EEnginePrimitiveType type){abort();}
void CEngineLightingManager::GetSupportedTypes(CEnginePrimitiveTypeList& types) const{abort();}
ERendered CEngineLightingManager::RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain){abort();}
unsigned long CEngineLightingManager::PeekSceneFilterFlags() const{abort();}
unsigned long CEngineCamera::PeekSceneFilterFlags()const{abort();}
static bool EqualFloat(float a,float b){return a==b||(a!=a&&b!=b);}
static void EqualVector(const C3DVector&a,const C3DVector&b){assert(EqualFloat(a.X,b.X)&&EqualFloat(a.Y,b.Y)&&EqualFloat(a.Z,b.Z));}
static void EqualColour(const CRGBFloatColour&a,const CRGBFloatColour&b){assert(EqualFloat(a.R,b.R)&&EqualFloat(a.G,b.G)&&EqualFloat(a.B,b.B)&&EqualFloat(a.A,b.A));}
// The mathematical oracle allows four float epsilons for the original stored-float ratio at /Od versus extended evaluation in production.
static bool CloseFloat(float a,float b){return EqualFloat(a,b)||fabs(a-b)<=4*1.1920928955078125e-7*(fabs(a)+fabs(b)+1);}
static void EqualScaledColour(const CRGBFloatColour&a,const CRGBFloatColour&b){assert(CloseFloat(a.R,b.R)&&CloseFloat(a.G,b.G)&&CloseFloat(a.B,b.B)&&CloseFloat(a.A,b.A));}
static CRGBFloatColour Scaled(const CRGBFloatColour&c,float f){CRGBFloatColour out;out.R=c.R*f;out.G=c.G*f;out.B=c.B*f;out.A=c.A*f;return out;}
static void ParamsCanary(CEngineLightingManagerSpotlightParams&p){p.Active=true;p.Pos=C3DVector(101,102,103);p.Vector=C3DVector(104,105,106);p.Radius=107;p.SpotRadius=108;p.ConeWidth=109;p.Colour=CRGBFloatColour(110,111,112,113);}
static void UnchangedParams(const CEngineLightingManagerSpotlightParams&p){assert(!p.Active);EqualVector(p.Pos,C3DVector(101,102,103));EqualVector(p.Vector,C3DVector(104,105,106));assert(p.Radius==107&&p.SpotRadius==108&&p.ConeWidth==109);EqualColour(p.Colour,CRGBFloatColour(110,111,112,113));}
int main(){
 typedef CEngineLightingManager::CManagerLocalLight Local;
 typedef CEngineLightingManager::CManagerShadowedSpotLight Spot;
 typedef CDynamicObjectHeap<72,16> Heap;
 CEngineLightingManager manager;CEngineCamera camera(false);NEngineInternal::GPCamera=&camera;
 CRGBFloatColour black(0,0,0,0);C3DVector zero(0,0,0);
 Spot s0(zero,zero,1,2,3,black),s1(zero,zero,1,2,3,black),s2(zero,zero,1,2,3,black),s3(zero,zero,1,2,3,black),s4(zero,zero,1,2,3,black);Spot* spots[]={&s0,&s1,&s2,&s3,&s4};
 Local existing(zero,2,1,CRGBFloatColour(1,2,3,4),77);
 Heap&heap=LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight;ExpectedHeap=&heap;
 // Leave every alternate real heap entry unavailable. The resulting storage stride also accommodates wider host pointers/longs.
 assert(sizeof(Local)<=2*sizeof(Heap::CObjectEntry));unsigned cases=0;
 float nan_value=(float)HUGE_VAL-(float)HUGE_VAL;
 const float coordinates[15][5]={{1,2,3,4,5},{5,4,3,2,1},{1,1,1,1,1},{0,0,2,4,8},{0,1,2,3,4},{-1,-2,-4,-8,-16},{1,3,5,7,9},{3,1,9,5,7},{0,8,0,8,0},{2,6,10,14,18},{1e16f,1e16f,1e16f,1e16f,1e16f},{0,1e16f,2,1e16f,4},{(float)HUGE_VAL,2,3,4,5},{nan_value,1,nan_value,2,3},{nan_value,nan_value,nan_value,nan_value,nan_value}};
 for(int enabled=0;enabled<2;++enabled)for(int count=0;count<=5;++count)for(int pattern=0;pattern<15;++pattern)for(int seed=0;seed<4;++seed)for(int mode=0;mode<5;++mode)for(int prefix=0;prefix<2;++prefix){
  camera.CameraDesc.WorldPosition=C3DVector(seed*8.0f,seed*4.0f,seed*2.0f);C3DVector old_camera=camera.CameraDesc.WorldPosition;
  manager.ShadowedSpotLightList.clear();manager.LocalLightList.clear();if(prefix)manager.LocalLightList.push_back(&existing);
  for(int n=0;n<count;++n){int k=(n+seed)%count;spots[n]->Position=C3DVector(old_camera.X+coordinates[pattern][k],old_camera.Y,old_camera.Z);spots[n]->Direction=C3DVector(n+1.0f,n+2.0f,n+3.0f);spots[n]->Radius=2.0f+n;spots[n]->SpotRadius=1.0f+n;spots[n]->ConeRadius=.25f*(n+1);spots[n]->Colour=CRGBFloatColour(.25f*(n+1),.5f*(n+1),.75f*(n+1),1.0f);manager.ShadowedSpotLightList.push_back(spots[n]);}
  InitialFree=mode==0?8:mode==1||mode==3?0:1;heap.AllowHeapToGrow=mode>=3;heap.AllocCount=0;GrowthCalls=0;
  for(int n=0;n<8;++n){Heap::CObjectEntry*p=&heap.StartBlock.Blocks[n*2];::new((void*)p)Heap::CObjectEntry;p->NextEntry=n+1<InitialFree?&heap.StartBlock.Blocks[(n+1)*2]:0;}
  heap.FreeList=InitialFree?&heap.StartBlock.Blocks[0]:0;ParamsCanary(manager.CurrentShadowedSpotLightParams);manager.ShadowedSpotLightsEnabled=enabled!=0;
  // Independent two-minimum selection over scalar coordinates; strict comparisons retain the first equal candidate.
  int nearest=-1,second=-1;double best=1e30f,next=1e30f;
  for(int n=0;n<count;++n){double dx=(double)spots[n]->Position.X-old_camera.X;double distance=dx*dx;if(distance<best){best=distance;nearest=n;}}
  for(int n=0;n<count;++n){double dx=(double)spots[n]->Position.X-old_camera.X;double distance=dx*dx;if(n!=nearest&&distance<next){next=distance;second=n;}}
  float fade=0;if(second>=0){double a=sqrt(best),b=sqrt(next);fade=(float)((a/(a+b)-.25)*4);if(fade<0)fade=0;else if(fade>1)fade=1;}
  manager.UpdateShadowedSpotLights();bool active=enabled&&nearest>=0;assert(manager.CurrentShadowedSpotLightParams.Active==active);EqualVector(camera.CameraDesc.WorldPosition,old_camera);assert(manager.ShadowedSpotLightList.size()==(unsigned)count&&manager.ShadowedSpotLightsEnabled==(enabled!=0));
  if(!active)UnchangedParams(manager.CurrentShadowedSpotLightParams);else{const Spot&s=*spots[nearest];const CEngineLightingManagerSpotlightParams&p=manager.CurrentShadowedSpotLightParams;EqualVector(p.Pos,s.Position);EqualVector(p.Vector,s.Direction);assert(p.Radius==s.Radius&&p.SpotRadius==s.SpotRadius&&p.ConeWidth==s.ConeRadius*2);EqualScaledColour(p.Colour,second>=0?Scaled(s.Colour,1.0f-fade):s.Colour);}
  int added=active?count:0,allocated=0;assert(manager.LocalLightList.size()==(unsigned)(prefix+added));if(prefix)assert(manager.LocalLightList[0]==&existing);
  for(int n=0;n<added;++n){Local*p=manager.LocalLightList[prefix+n];bool success=n<InitialFree||heap.AllowHeapToGrow;assert((p!=0)==success);if(p){assert((void*)p==(void*)&heap.StartBlock.Blocks[allocated*2]);++allocated;const CEngineLocalLightElement&e=p->LightElement;EqualVector(e.WorldPos,spots[n]->Position);assert(e.Radius==spots[n]->Radius&&e.SpotRadius==spots[n]->SpotRadius&&e.ChannelMask==0xFFFFFFFFUL&&!p->AlreadyFoundFlag);assert(e.RadiusSquared==e.Radius*e.Radius);EqualScaledColour(e.Colour,Scaled(spots[n]->Colour,n==nearest?fade:1.0f));float attenuation=(float)(1.0f/(e.SpotRadius*e.SpotRadius)-1.0f/(e.Radius*e.Radius));assert(fabs(e.AttenuationDistSquared-attenuation)<1e-6);assert(fabs(e.AttenuationConstant-(-1.0f/(e.Radius*e.Radius)/attenuation))<1e-5);}}
  assert(heap.AllocCount==allocated&&GrowthCalls==((active&&added>InitialFree&&heap.AllowHeapToGrow)?1:0));
  for(int n=0;n<added;++n)if(manager.LocalLightList[prefix+n])manager.LocalLightList[prefix+n]->~Local();
  ++cases;
 }
 assert(cases==7200);puts("00b47bc0_TEST PASS");return 0;
}