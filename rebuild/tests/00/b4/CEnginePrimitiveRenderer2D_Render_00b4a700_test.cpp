// Standalone original shared objects, target body and controlled external operations.
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
enum _D3DRENDERSTATETYPE{};enum _D3DTEXTURESTAGESTATETYPE{};enum _D3DSAMPLERSTATETYPE{};
enum EIndexBufferFormat{};enum EIndexBufferPool{};enum EShaderBankNo{};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_COMPLETE_ENUM_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_LIFETIME_PASS23
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PORTABLE_NEW_PASS23
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define private public
#define protected public
#include "engine/CEngineRenderEnums.h"
#include "engine/EPrimitiveType.h"
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
// 00b4a700 CEnginePrimitiveRenderer2D::Render
// Flags: none
// Draw 2D primitives with temporary camera and lighting state.
// Leaf: ?Render@CEnginePrimitiveRenderer2D@@UAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe0x00B4A700,926bytes,thiscall ret4.
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_RENDER_PASS23
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALDIFFUSE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_GLOBAL_LIGHTING_PASS23
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_CGRAPHICDATABANK_DELAYS_PASS23
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RENDERTYPE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_NEXT_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/EGraphicBankNo.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineComponentRenderParams.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineSubPrimitiveChain.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CDisplayManager.h"
namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;
extern CEngineCamera* GPCamera;
extern CEngineLightingManager* GPLightingManager;
extern CDisplayManager* GPDisplayManager;
extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;
extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;
}
using namespace NEngineInternal;
void CEnginePrimitiveRenderer2D::Render(const CEngineComponentRenderParams& params)
{
 CRenderStateScopedCaptureBlock block;
 CScopedShadersEnable shaders_enabled(true,true);
 CGraphicDataBank& graphic_bank=*GPResourceManager->GetGraphicBank(MAIN_GRAPHIC_BANK_NO);
 unsigned long old_texture_resize_delay=graphic_bank.GetReduceSizeFrameDelay();
 unsigned long old_texture_unload_delay=graphic_bank.GetUnloadFrameDelay();
 graphic_bank.SetReduceSizeFrameDelay(1);
 graphic_bank.SetUnloadFrameDelay(1);
 CEngineCameraDesc old_camera_desc(GPCamera->PeekCameraState());
 CEngineInternalPrimitiveBase* current_prim;
 CEngineCameraDesc new_camera_desc;
 new_camera_desc.HorizontalFOV=FOV2D;
 new_camera_desc.StateValid=true;
 new_camera_desc.NearZPlane=.1f;
 new_camera_desc.FarZPlane=1;
 new_camera_desc.ZBufferStart=0;
 new_camera_desc.ZBufferEnd=.1f;
 new_camera_desc.Use2DFOV=false;
 new_camera_desc.VerticalFOV=0;
 new_camera_desc.WorldPosition.X=0;
 new_camera_desc.WorldPosition.Y=0;
 new_camera_desc.WorldPosition.Z=0;
 new_camera_desc.RotationMatrix.SetIdentity();
 new_camera_desc.RotationMatrix.E22=-1;
 C2DExtentsI render_target_extents=GPDisplayManager->GetRenderTargetDimensions();
 new_camera_desc.Window=C2DBoxF(0,0,(float)render_target_extents.X,(float)render_target_extents.Y);
 GPCamera->SetCameraState(new_camera_desc,false,true);
 C3DVector old_light_vector=GPLightingManager->PeekDiffuseVector();
 CRGBFloatColour old_ambient,old_backlight,old_diffuse;
 old_ambient=GPLightingManager->PeekGlobalAmbientColour();
 old_diffuse=GPLightingManager->PeekGlobalDiffuseColour();
 old_backlight=GPLightingManager->PeekGlobalBacklightColour();
 GPLightingManager->SetGlobalLightingParameters(LightVector,AmbientLightColour,DiffuseLightColour,BackLightColour);
 if(params.RenderLayer==RENDER_LAYER_EFFECTS){
  current_prim=First2DDisplacementPrim;
  while(current_prim){
   CEnginePrimitiveManager* prim_manager=GPPrimitiveTypeInfo->GetPrimitiveManager(current_prim->GetType());
   CEngineSubPrimitiveChain subprim_chain;
   prim_manager->Render2DPrimitive(*current_prim,subprim_chain,SUBPRIM_TYPE_2D);
   current_prim=current_prim->GetNextPrimitive();
  }
 }else{
  current_prim=First2DPrim;
  while(current_prim){
   CEnginePrimitiveManager* prim_manager=GPPrimitiveTypeInfo->GetPrimitiveManager(current_prim->GetType());
   CEngineSubPrimitiveChain subprim_chain;
   prim_manager->Render2DPrimitive(*current_prim,subprim_chain,SUBPRIM_TYPE_2D);
   current_prim=current_prim->GetNextPrimitive();
  }
  GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_2D,true);
 }
 GPLightingManager->SetGlobalLightingParameters(old_light_vector,old_ambient,old_diffuse,old_backlight);
 GPCamera->SetCameraState(old_camera_desc,false,true);
 graphic_bank.SetReduceSizeFrameDelay(old_texture_resize_delay);
 graphic_bank.SetUnloadFrameDelay(old_texture_unload_delay);
}

#include "engine/CSystemManager.h"
#include "engine/CEngineInit.h"
#include "engine/CGraphicInfo.h"
#include "engine/CDateAndTime.h"
#include "engine/CEngineSubPrimitiveBase.h"
#undef private
#undef protected


CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){assert(!PStringData);}
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CSystemManager::CSystemManager(){}CSystemManager::~CSystemManager(){}
CBusyResourceManager::CBusyResourceManager():PendingFrame(0),FinishedFrame(0),D3DDevice(0){}CBusyResourceManager::~CBusyResourceManager(){assert(FreeQueoryObjects.empty()&&PendingQueories.empty());}
CSurface::~CSurface(){assert(!PD3DSurface);}
void CTexture::Uninitialise(){assert(!PD3DTexture);}
namespace boost {template<> scoped_ptr<CRenderManager>::~scoped_ptr(){assert(!ptr);}template<> scoped_ptr<CVsyncCallbackTimer>::~scoped_ptr(){assert(!ptr);}}
CDisplayManager::CDisplayManager(CSystemManager&s):SystemManager(s){}CDisplayManager::~CDisplayManager(){}
CRenderStateManager::CDeviceState::CDeviceState(){}CRenderStateManager::CStateInfo::CStateInfo(){}
CRenderStateManager::CRenderStateManager(unsigned long f):InitFlags(f),StateStackSize(0),CurrentBookmarkMask(1){}CRenderStateManager::~CRenderStateManager(){}
CVertexShaderConstantLayout::CVertexShaderConstantLayout(){}CVertexShaderConstantLayout::~CVertexShaderConstantLayout(){}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long c,unsigned long g):FirstMemoryBlock(0),Capacity(c),GrowthGranularity(g){Root.Size=0;Root.NextFreeArea=0;Root.Status=STATUS_FREE;}
void CSimpleMemoryPool::ClearMemoryBlocks(){assert(!FirstMemoryBlock);}
CPixelShader::CPixelShader():Data(0){}CPixelShaderData::~CPixelShaderData(){abort();}
CShaderRenderManager::CShaderRenderManager():LightPool(0,0),VertexShadersEnabled(false),PixelShadersEnabled(false){}
CShaderRenderManager::~CShaderRenderManager(){}
CShaderRenderManager CShaderRenderManager::SingletonInstance;
CRenderStateManager* CRenderStateCaptureBlock::StateManager=0;
CEngineComponent::CEngineComponent():GlobalPointerAddress(0),RenderEnabled(true),KeepAlive(0){}CEngineComponent::~CEngineComponent(){}
CEnginePrimitiveRenderUtil::~CEnginePrimitiveRenderUtil(){}
static CArray<CEngineScene*> FixtureScenes;
CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>&s):SceneList(s),Enable(true){}CEnginePrimitiveManager::~CEnginePrimitiveManager(){}
CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D():First2DPrim(0),First2DDisplacementPrim(0),FOV2D(0){}CEnginePrimitiveRenderer2D::~CEnginePrimitiveRenderer2D(){}
CEngineLightingManager::CEngineLightingManager():CEnginePrimitiveManager(FixtureScenes){}
CEngineCamera::CEngineCamera(bool){}
CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer(){}
CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo(){}
CEngineMemoryInit::CEngineMemoryInit(){}
CEngineInit::CEngineInit(){}CEngineInit::~CEngineInit(){}
CEngineResourceManager::CEngineResourceManager(const CEngineInit&,const CEngineMemoryInit&){}
CEngineResourceManager::~CEngineResourceManager(){}
CBankFile::~CBankFile(){}CBankFileAsync::~CBankFileAsync(){}
CResourceBank::CResourceBank(){}CResourceBank::~CResourceBank(){}
CResource::~CResource(){}CResourceList::~CResourceList(){}
CGraphicDataBank::CGraphicDataBank():Initialised(false),UnloadFrameDelay(0),ReduceSizeFrameDelay(0){}CGraphicDataBank::~CGraphicDataBank(){}
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long t):Type(t){}CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long t,unsigned long):CEnginePersistentPrimitiveBase(t),NextPrimitive(0){}CEngineInternalPrimitiveBase::~CEngineInternalPrimitiveBase(){}
CEngineSubPrimitiveBase::~CEngineSubPrimitiveBase(){}
#ifdef _MSC_VER
void* __fastcall CEngineSubPrimitiveBase::operator new(unsigned int){abort();}
#else
void* CEngineSubPrimitiveBase::operator new(size_t){abort();}
#endif
void __fastcall CEngineSubPrimitiveBase::operator delete(void*){abort();}
namespace NEngineInternal {CEngineResourceManager* GPResourceManager=0;CEngineCamera* GPCamera=0;CEngineLightingManager* GPLightingManager=0;CDisplayManager* GPDisplayManager=0;CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer=0;CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo=0;}

CCharString CShaderBase::GetDebugName() const{abort();}
CCharString CPixelShader::GetDebugName() const{abort();}
unsigned long CPixelShader::CountReferences(){abort();}
CCharString CVertexShaderConstantLayout::PeekName() const{abort();}
bool CResource::UnloadResource(){abort();}
unsigned long CResource::GetUnloadableMemory() const{abort();}
bool CResource::IsResourceLoaded() const{abort();}
void CResource::OnResourceLoaded(){abort();}
bool CResource::CanAbortResourceLoad(long) const{abort();}
void CResource::OnResourceLoadAborted(){abort();}
unsigned long CResource::GetDebugMemoryInUse() const{abort();}
void CResource::ResetDebugMemoryInUse(){abort();}
bool CResourceList::HandleFailedAllocation(unsigned long){abort();}
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long){abort();}
void CBankFile::OpenReadWrite(const CCharString&,unsigned long,unsigned long){abort();}
bool CBankFile::OpenFileReadOnly(const CWideString&,unsigned long){abort();}
void CBankFile::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long){abort();}
void CBankFile::Close(){abort();}
void CBankFile::AddEntryMultipleFiles(const CCharString&,unsigned long,const CArray<CCharString>&,const CCountedPointer<CBankStateBlock>){abort();}
void CBankFile::Update(){abort();}
unsigned long CBankFile::AdjustUpdateEntries(){abort();}
void CBankFile::NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&){abort();}
void CBankFile::NotifyBankOpened(){abort();}
void CBankFile::NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&){abort();}
void CBankFile::NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&){abort();}
void CBankFile::NotifyEntryRemoved(unsigned long){abort();}
void CBankFile::NotifyPostEntriesLoaded(){abort();}
void CBankFile::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&){abort();}
bool CBankFile::IsDiskFile(unsigned long,unsigned long){abort();}
CDateAndTime CBankFile::GetLastModifiedTime(const CCharString&){abort();}
CCharString CBankFile::GetEnumerationType(unsigned long) const{abort();}
bool CBankFileAsync::OpenReadOnly(const CCharString&,unsigned long){abort();}
void CBankFileAsync::OpenReadWrite(const CCharString&,unsigned long,unsigned long){abort();}
bool CBankFileAsync::OpenFileReadOnly(const CWideString&,unsigned long){abort();}
void CBankFileAsync::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long){abort();}
void CBankFileAsync::Close(){abort();}
unsigned long CGraphicDataBank::GetNoFramesInGraphic(unsigned long) const{abort();}
unsigned long CGraphicDataBank::GetGraphicFrameWidth(unsigned long, unsigned long) const{abort();}
unsigned long CGraphicDataBank::GetGraphicFrameHeight(unsigned long, unsigned long) const{abort();}
unsigned long CGraphicDataBank::GetGraphicMaxWidth(unsigned long) const{abort();}
unsigned long CGraphicDataBank::GetGraphicMaxHeight(unsigned long) const{abort();}
bool CGraphicDataBank::IsUsingSurfaces() const{abort();}
void CGraphicDataBank::Close(){abort();}
unsigned long CGraphicDataBank::GetDataFormatVersion(){abort();}
void CGraphicDataBank::NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&){abort();}
void CGraphicDataBank::NotifyBankOpened(){abort();}
void CGraphicDataBank::NotifyEntryUpdated(unsigned long, unsigned long, const CArray<char>&){abort();}
void CGraphicDataBank::NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&){abort();}
void CGraphicDataBank::NotifyEntryRemoved(unsigned long){abort();}
void CGraphicDataBank::PrepareDataForInclusion(unsigned long, const CArray<CCharString>&, unsigned long, const CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&){abort();}
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
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&){abort();}
bool CEnginePersistentPrimitiveBase::WasRendered() const{abort();}
void CEngineInternalPrimitiveBase::Update(const CEnginePrimitive&,unsigned long){abort();}
unsigned long CEngineInternalPrimitiveBase::AddChildPrimitive(const CEnginePrimitive&,unsigned long){abort();}
void CEngineInternalPrimitiveBase::SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long){abort();}
void CEngineInternalPrimitiveBase::SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long){abort();}
void CEngineInternalPrimitiveBase::GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle&){abort();}
void CEngineInternalPrimitiveBase::ClearChildPrimitive(unsigned long){abort();}
void CEngineInternalPrimitiveBase::ClearAllChildPrimitives(){abort();}
CEnginePersistentPrimitiveBase* CEngineInternalPrimitiveBase::GetChildPrimitive(unsigned long){abort();}
CEngineFadeDistance CEngineInternalPrimitiveBase::GetFadeDistance()const{abort();}
void CEngineInternalPrimitiveBase::PostRender(){abort();}
bool CEngineInternalPrimitiveBase::GetWorldPosition(C3DVector&){abort();}
bool CEngineInternalPrimitiveBase::GetBoundingBoxTransform(CMatrix3x4&){abort();}
bool CEngineInternalPrimitiveBase::GetBoundingBoxWorldSpace(C3DBoundingBox&){abort();}
bool CEngineInternalPrimitiveBase::GetBoundingSphereWorldSpace(C3DBoundingSphere&){abort();}
bool CEngineInternalPrimitiveBase::GetBoundingSphereRadiusWorldSpace(float&){abort();}
bool CEngineInternalPrimitiveBase::IsInsideFrustum(const CEngineCamera&,bool){abort();}
bool CEngineInternalPrimitiveBase::GetObjectTransform(CMatrix3x4&)const{abort();}
bool CEngineInternalPrimitiveBase::SetObjectTransform(const CMatrix3x4&){abort();}
void CEngineInternalPrimitiveBase::SetAnimation(const CEnginePrimitiveAnimation&){abort();}
void CEngineInternalPrimitiveBase::SetPosition(const CEnginePrimitivePosition&){abort();}
void CEngineInternalPrimitiveBase::SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&){abort();}
bool CEngineInternalPrimitiveBase::GetMeshEffect(CIVCountedPointer<CEngineMeshEffect>&)const{abort();}
CEngineInternalPrimitiveOcclusionBase* CEngineInternalPrimitiveBase::GetAsOcclusionPrimitive(){abort();}
void CEngineInternalPrimitiveBase::CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const{abort();}
void CEngineInternalPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&){abort();}
bool CEngineInternalPrimitiveBase::WasRendered()const{abort();}
CEngineInternalPrimitiveBase* CEngineLightingManager::CreateInternalPrimitive(EEnginePrimitiveType type){abort();}
void CEngineLightingManager::GetSupportedTypes(CEnginePrimitiveTypeList& types) const{abort();}
ERendered CEngineLightingManager::RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain){abort();}
unsigned long CEngineLightingManager::PeekSceneFilterFlags() const{abort();}
unsigned long CEngineCamera::PeekSceneFilterFlags()const{abort();}
unsigned long CEngineSubPrimitiveRenderer::PeekSceneFilterFlags()const{abort();}
void CEngineResourceManager::ProcessLoading(){abort();}
unsigned long CEngineResourceManager::PeekSceneFilterFlags() const{abort();}
unsigned long CEnginePrimitiveTypeInfo::PeekSceneFilterFlags() const{abort();}
unsigned long CEnginePrimitiveRenderer2D::PeekSceneFilterFlags() const{abort();}
CShaderConstantRange::CShaderConstantRange():Offset(0),Count(0){}CShaderConstantRange::~CShaderConstantRange(){}
CFailedAllocationHandler::~CFailedAllocationHandler(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}
CEngineFadeDistance::CEngineFadeDistance():FadeBeginDistance(0),MaxDrawDistance(0){}
CEngineSceneGridPos::CEngineSceneGridPos():Level(0),X(0),Y(0){}
static CEnginePrimitiveRenderer2D* ExpectedRenderer;
static CGraphicDataBank* ExpectedBank;static CGraphicDataBank* OtherBank;
static CRenderStateManager* ExpectedState;static CEngineSubPrimitiveBase* Marker;
static CEngineInternalPrimitiveBase* ExpectedPrimitives[8];static int ExpectedPrimitiveCount,PrimitiveCalls;
static unsigned long OldReduce,OldUnload;static float ExpectedFOV;static C2DExtentsI Dimensions;
static CEngineCameraDesc OldCamera;static C3DVector OldLight;static CRGBFloatColour OldAmbient,OldDiffuse,OldBack;
static bool InitialVertex,InitialPixel,ClobberVertex,ClobberPixel,Overlay;
static bool Active;static int LightCalls,CameraCalls,ReduceCalls,UnloadCalls,FlushCalls;
static long StackDepth;static unsigned long StackMask;static int Trace[64],TraceSize;
static void Event(int code){assert(Active&&TraceSize<64);Trace[TraceSize++]=code;}
static void CaptureActive(){assert(CRenderStateCaptureBlock::StateManager==ExpectedState);assert(ExpectedState->StateStackSize==StackDepth+1&&ExpectedState->CurrentBookmarkMask==StackMask*2);assert(ExpectedState->StateStack[StackDepth].State==&ExpectedState->BookmarkDummyState);}
static void Colour(CRGBFloatColour&c,float r,float g,float b,float a){c.R=r;c.G=g;c.B=b;c.A=a;}
static void EqualColour(const CRGBFloatColour&a,const CRGBFloatColour&b){assert(a.R==b.R&&a.G==b.G&&a.B==b.B&&a.A==b.A);}
static void EqualVector(const C3DVector&a,const C3DVector&b){assert(a.X==b.X&&a.Y==b.Y&&a.Z==b.Z);}
static void EqualCamera(const CEngineCameraDesc&a,const CEngineCameraDesc&b){
 assert(a.Window.TLX==b.Window.TLX&&a.Window.TLY==b.Window.TLY&&a.Window.BRX==b.Window.BRX&&a.Window.BRY==b.Window.BRY);
 assert(a.RotationMatrix.E11==b.RotationMatrix.E11&&a.RotationMatrix.E12==b.RotationMatrix.E12&&a.RotationMatrix.E13==b.RotationMatrix.E13&&a.RotationMatrix.E21==b.RotationMatrix.E21&&a.RotationMatrix.E22==b.RotationMatrix.E22&&a.RotationMatrix.E23==b.RotationMatrix.E23&&a.RotationMatrix.E31==b.RotationMatrix.E31&&a.RotationMatrix.E32==b.RotationMatrix.E32&&a.RotationMatrix.E33==b.RotationMatrix.E33&&a.RotationMatrix.E41==b.RotationMatrix.E41&&a.RotationMatrix.E42==b.RotationMatrix.E42&&a.RotationMatrix.E43==b.RotationMatrix.E43);
 EqualVector(a.WorldPosition,b.WorldPosition);assert(a.HorizontalFOV==b.HorizontalFOV&&a.VerticalFOV==b.VerticalFOV&&a.Use2DFOV==b.Use2DFOV&&a.NearZPlane==b.NearZPlane&&a.FarZPlane==b.FarZPlane&&a.ZBufferStart==b.ZBufferStart&&a.ZBufferEnd==b.ZBufferEnd&&a.StateValid==b.StateValid);
}
static void NewCamera(const CEngineCameraDesc&d){
 assert(d.HorizontalFOV==ExpectedFOV&&d.StateValid&&d.NearZPlane==.1f&&d.FarZPlane==1&&d.ZBufferStart==0&&d.ZBufferEnd==.1f&&!d.Use2DFOV&&d.VerticalFOV==0);
 assert(d.WorldPosition.X==0&&d.WorldPosition.Y==0&&d.WorldPosition.Z==0);
 assert(d.Window.TLX==0&&d.Window.TLY==0&&d.Window.BRX==(float)Dimensions.X&&d.Window.BRY==(float)Dimensions.Y);
 assert(d.RotationMatrix.E11==1&&d.RotationMatrix.E22==-1&&d.RotationMatrix.E33==1&&d.RotationMatrix.E12==0&&d.RotationMatrix.E13==0&&d.RotationMatrix.E21==0&&d.RotationMatrix.E23==0&&d.RotationMatrix.E31==0&&d.RotationMatrix.E32==0&&d.RotationMatrix.E41==0&&d.RotationMatrix.E42==0&&d.RotationMatrix.E43==0);
}
static void RestoredScene(){EqualCamera(GPCamera->CameraDesc,OldCamera);EqualVector(GPLightingManager->DiffuseVector,OldLight);EqualColour(GPLightingManager->ThemeChannels[0].AmbientColour,OldAmbient);EqualColour(GPLightingManager->ThemeChannels[0].DiffuseColour,OldDiffuse);EqualColour(GPLightingManager->ThemeChannels[0].BacklightColour,OldBack);}
static void FinalRenderAction(){
 CShaderRenderManager::SingletonInstance.VertexShadersEnabled=ClobberVertex;CShaderRenderManager::SingletonInstance.PixelShadersEnabled=ClobberPixel;
 GPCamera->CameraDesc.HorizontalFOV=-123.0f;GPLightingManager->DiffuseVector.X=-456.0f;GPLightingManager->ThemeChannels[0].AmbientColour.G=-789.0f;GPLightingManager->ThemeChannels[0].DiffuseColour.B=-987.0f;GPLightingManager->ThemeChannels[0].BacklightColour.A=-654.0f;
 // The target caches the original graphic bank by reference and restores that bank.
 GPResourceManager->GraphicBanks[0]=OtherBank;
}
void CShaderRenderManager::EnableVertexShaders(){assert(this==&SingletonInstance&&!VertexShadersEnabled);CaptureActive();Event(10);VertexShadersEnabled=true;}
void CShaderRenderManager::DisableVertexShaders(){assert(this==&SingletonInstance&&VertexShadersEnabled);CaptureActive();Event(11);VertexShadersEnabled=false;}
void CShaderRenderManager::EnablePixelShaders(){assert(this==&SingletonInstance&&!PixelShadersEnabled);CaptureActive();Event(12);PixelShadersEnabled=true;}
void CShaderRenderManager::DisablePixelShaders(){assert(this==&SingletonInstance&&PixelShadersEnabled);CaptureActive();Event(13);PixelShadersEnabled=false;}
void CGraphicDataBank::SetReduceSizeFrameDelay(unsigned long n){assert(this==ExpectedBank);CaptureActive();++ReduceCalls;assert(n==(ReduceCalls==1?1:OldReduce));if(ReduceCalls==2)RestoredScene();Event(ReduceCalls==1?1:8);ReduceSizeFrameDelay=n;}
void CGraphicDataBank::SetUnloadFrameDelay(unsigned long n){assert(this==ExpectedBank);CaptureActive();++UnloadCalls;assert(n==(UnloadCalls==1?1:OldUnload));Event(UnloadCalls==1?2:9);UnloadFrameDelay=n;}
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){assert(this==GPDisplayManager);CaptureActive();assert(ExpectedBank->ReduceSizeFrameDelay==1&&ExpectedBank->UnloadFrameDelay==1);return Dimensions;}
void CEngineCamera::SetCameraState(const CEngineCameraDesc&d,bool a,bool b){assert(this==GPCamera&&!a&&b&&&d!=&CameraDesc);CaptureActive();++CameraCalls;if(CameraCalls==1){NewCamera(d);Event(3);}else{assert(CameraCalls==2);EqualCamera(d,OldCamera);EqualVector(GPLightingManager->DiffuseVector,OldLight);Event(7);}CameraDesc=d;}
void CEngineLightingManager::SetGlobalLightingParameters(const C3DVector&v,const CRGBFloatColour&a,const CRGBFloatColour&d,const CRGBFloatColour&b){
 assert(this==GPLightingManager);CaptureActive();++LightCalls;
 if(LightCalls==1){assert(&v==&ExpectedRenderer->LightVector&&&a==&ExpectedRenderer->AmbientLightColour&&&d==&ExpectedRenderer->DiffuseLightColour&&&b==&ExpectedRenderer->BackLightColour);NewCamera(GPCamera->CameraDesc);Event(4);}
 else{assert(LightCalls==2);assert(&v!=&DiffuseVector&&&a!=&ThemeChannels[0].AmbientColour&&&d!=&ThemeChannels[0].DiffuseColour&&&b!=&ThemeChannels[0].BacklightColour);EqualVector(v,OldLight);EqualColour(a,OldAmbient);EqualColour(d,OldDiffuse);EqualColour(b,OldBack);Event(6);}
 DiffuseVector=v;ThemeChannels[0].AmbientColour=a;ThemeChannels[0].DiffuseColour=d;ThemeChannels[0].BacklightColour=b;
}
void CEngineSubPrimitiveRenderer::RenderType(ESubPrimitiveType type,bool enabled){assert(this==GPSubPrimitiveRenderer&&type==SUBPRIM_TYPE_2D&&enabled&&!Overlay&&PrimitiveCalls==ExpectedPrimitiveCount);CaptureActive();++FlushCalls;Event(5);FinalRenderAction();}
void CRenderStateManager::RestoreCaptureBlock(){assert(this==ExpectedState);CaptureActive();RestoredScene();assert(ExpectedBank->ReduceSizeFrameDelay==OldReduce&&ExpectedBank->UnloadFrameDelay==OldUnload);assert(OtherBank->ReduceSizeFrameDelay==222&&OtherBank->UnloadFrameDelay==333);assert(CShaderRenderManager::SingletonInstance.VertexShadersEnabled==InitialVertex&&CShaderRenderManager::SingletonInstance.PixelShadersEnabled==InitialPixel);Event(14);--StateStackSize;CurrentBookmarkMask>>=1;}
class FixtureManager:public CEnginePrimitiveManager {
 int Identity;
public:
 FixtureManager(int id):CEnginePrimitiveManager(FixtureScenes),Identity(id){}
 CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType){abort();}
 void GetSupportedTypes(CEnginePrimitiveTypeList&)const{abort();}
 ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&){abort();}
 void Render2DPrimitive(CEngineInternalPrimitiveBase&p,CEngineSubPrimitiveChain&chain,ESubPrimitiveType type){
  assert(PrimitiveCalls<ExpectedPrimitiveCount&&&p==ExpectedPrimitives[PrimitiveCalls]&&p.GetType()==Identity&&type==SUBPRIM_TYPE_2D);assert(chain.LastOpaque==0&&chain.LastAlpha==0);CaptureActive();NewCamera(GPCamera->CameraDesc);EqualVector(GPLightingManager->DiffuseVector,ExpectedRenderer->LightVector);EqualColour(GPLightingManager->ThemeChannels[0].AmbientColour,ExpectedRenderer->AmbientLightColour);EqualColour(GPLightingManager->ThemeChannels[0].DiffuseColour,ExpectedRenderer->DiffuseLightColour);EqualColour(GPLightingManager->ThemeChannels[0].BacklightColour,ExpectedRenderer->BackLightColour);assert(ExpectedBank->ReduceSizeFrameDelay==1&&ExpectedBank->UnloadFrameDelay==1);
  Event(100+Identity);chain.LastOpaque=Marker;chain.LastAlpha=Marker;++PrimitiveCalls;if(Overlay&&PrimitiveCalls==ExpectedPrimitiveCount)FinalRenderAction();
 }
};
static void FillCamera(CEngineCameraDesc&d,float n){
 d.Window=C2DBoxF(n,n+1,n+2,n+3);d.RotationMatrix.E11=n+1;d.RotationMatrix.E12=n+2;d.RotationMatrix.E13=n+3;d.RotationMatrix.E21=n+4;d.RotationMatrix.E22=n+5;d.RotationMatrix.E23=n+6;d.RotationMatrix.E31=n+7;d.RotationMatrix.E32=n+8;d.RotationMatrix.E33=n+9;d.RotationMatrix.E41=n+10;d.RotationMatrix.E42=n+11;d.RotationMatrix.E43=n+12;
 d.WorldPosition=C3DVector(n+1,n+2,n+3);d.HorizontalFOV=n+.25f;d.VerticalFOV=n+.5f;d.Use2DFOV=true;d.NearZPlane=n+1;d.FarZPlane=n+100;d.ZBufferStart=.25f;d.ZBufferEnd=.75f;d.StateValid=true;
}
int main(){
 CSystemManager system;CDisplayManager display(system);CEngineInit engine_init;CEngineResourceManager resources(engine_init,engine_init.MemoryInit);CGraphicDataBank bank,other_bank;CRenderStateManager states(0);CEngineCamera camera(false);CEngineLightingManager lighting;CEnginePrimitiveRenderer2D renderer;CEnginePrimitiveTypeInfo types;CEngineSubPrimitiveRenderer subrenderer;CEngineSubPrimitiveBase marker(0);
 FixtureManager m0(0),m1(1),m2(2),m3(3);CEngineInternalPrimitiveBase p0(0,0),p1(1,0),p2(2,0),p3(3,0),p4(3,0),p5(2,0),p6(1,0),p7(0,0);CEngineInternalPrimitiveBase* primitives[]={&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7};
 CEngineLightingManager::CThemeChannel channel;Colour(channel.AmbientColour,0,0,0,0);Colour(channel.DiffuseColour,0,0,0,0);Colour(channel.BacklightColour,0,0,0,0);Colour(channel.ReflectionColour,0,0,0,0);Colour(channel.FogColour,0,0,0,0);channel.FogStartZ=channel.FogEndZ=channel.ShadowFactor=channel.FadedShadowFactor=0;channel.FogTransform.X=channel.FogTransform.Y=channel.FogTransform.Z=channel.FogTransform.W=0;lighting.ThemeChannels.push_back(channel);
 for(int i=0;i<47;++i)types.PrimitiveManagers[i]=0;types.PrimitiveManagers[0]=&m0;types.PrimitiveManagers[1]=&m1;types.PrimitiveManagers[2]=&m2;types.PrimitiveManagers[3]=&m3;
 GPDisplayManager=&display;GPResourceManager=&resources;GPCamera=&camera;GPLightingManager=&lighting;GPPrimitiveTypeInfo=&types;GPSubPrimitiveRenderer=&subrenderer;CRenderStateCaptureBlock::StateManager=&states;ExpectedRenderer=&renderer;ExpectedBank=&bank;OtherBank=&other_bank;ExpectedState=&states;Marker=&marker;unsigned cases=0;
 const long layers[]={0,RENDER_LAYER_EFFECTS,RENDER_LAYER_EFFECTS|1};
 for(int shader=0;shader<4;++shader)for(int clobber=0;clobber<4;++clobber)for(int normal=0;normal<=4;++normal)for(int effects=0;effects<=4;++effects)for(int layer=0;layer<3;++layer)for(int seed=0;seed<8;++seed){
  Active=false;TraceSize=PrimitiveCalls=LightCalls=CameraCalls=ReduceCalls=UnloadCalls=FlushCalls=0;InitialVertex=(shader&1)!=0;InitialPixel=(shader&2)!=0;ClobberVertex=(clobber&1)!=0;ClobberPixel=(clobber&2)!=0;Overlay=layer==1;
  CShaderRenderManager::SingletonInstance.VertexShadersEnabled=InitialVertex;CShaderRenderManager::SingletonInstance.PixelShadersEnabled=InitialPixel;
  StackDepth=seed%3;StackMask=1UL<<(seed%4);states.StateStackSize=StackDepth;states.CurrentBookmarkMask=StackMask;resources.GraphicBanks[0]=&bank;OldReduce=seed*7;OldUnload=seed*13+2;bank.ReduceSizeFrameDelay=OldReduce;bank.UnloadFrameDelay=OldUnload;other_bank.ReduceSizeFrameDelay=222;other_bank.UnloadFrameDelay=333;
  Dimensions.X=640+seed*32;Dimensions.Y=480+seed*16;ExpectedFOV=renderer.FOV2D=.5f+seed*.125f;FillCamera(camera.CameraDesc,(float)seed);OldCamera=camera.CameraDesc;OldLight=C3DVector(seed+3.0f,seed+4.0f,seed+5.0f);lighting.DiffuseVector=OldLight;Colour(OldAmbient,seed+.25f,seed+.5f,seed+.75f,seed+1.0f);Colour(OldDiffuse,seed+2.0f,seed+3.0f,seed+4.0f,seed+5.0f);Colour(OldBack,seed+6.0f,seed+7.0f,seed+8.0f,seed+9.0f);lighting.ThemeChannels[0].AmbientColour=OldAmbient;lighting.ThemeChannels[0].DiffuseColour=OldDiffuse;lighting.ThemeChannels[0].BacklightColour=OldBack;
  renderer.LightVector=C3DVector(seed+10.0f,seed+11.0f,seed+12.0f);Colour(renderer.AmbientLightColour,1,2,3,4);Colour(renderer.DiffuseLightColour,5,6,7,8);Colour(renderer.BackLightColour,9,10,11,12);
  for(int n=0;n<8;++n)primitives[n]->NextPrimitive=0;for(int n=0;n<normal-1;++n)primitives[n]->NextPrimitive=primitives[n+1];for(int n=0;n<effects-1;++n)primitives[n+4]->NextPrimitive=primitives[n+5];renderer.First2DPrim=normal?primitives[0]:0;renderer.First2DDisplacementPrim=effects?primitives[4]:0;ExpectedPrimitiveCount=Overlay?effects:normal;for(int n=0;n<ExpectedPrimitiveCount;++n)ExpectedPrimitives[n]=primitives[n+(Overlay?4:0)];
  CEngineComponentRenderParams params;params.Params=0;params.RenderLayer=layers[layer];params.RenderLayerFlags=123;Active=true;renderer.Render(params);Active=false;
  assert(PrimitiveCalls==ExpectedPrimitiveCount&&CameraCalls==2&&LightCalls==2&&ReduceCalls==2&&UnloadCalls==2&&FlushCalls==(Overlay?0:1));assert(states.StateStackSize==StackDepth&&states.CurrentBookmarkMask==StackMask);RestoredScene();assert(params.RenderLayer==layers[layer]&&params.RenderLayerFlags==123);
  int expected[64],count=0;if(!InitialVertex)expected[count++]=10;if(!InitialPixel)expected[count++]=12;expected[count++]=1;expected[count++]=2;expected[count++]=3;expected[count++]=4;for(int n=0;n<ExpectedPrimitiveCount;++n)expected[count++]=100+ExpectedPrimitives[n]->GetType();if(!Overlay)expected[count++]=5;expected[count++]=6;expected[count++]=7;expected[count++]=8;expected[count++]=9;bool cv=Overlay&&effects==0?true:ClobberVertex,cp=Overlay&&effects==0?true:ClobberPixel;if(cv!=InitialVertex)expected[count++]=InitialVertex?10:11;if(cp!=InitialPixel)expected[count++]=InitialPixel?12:13;expected[count++]=14;assert(TraceSize==count);for(int n=0;n<count;++n)assert(Trace[n]==expected[n]);++cases;
 }
 assert(cases==9600);puts("00b4a700_TEST PASS");return 0;
}