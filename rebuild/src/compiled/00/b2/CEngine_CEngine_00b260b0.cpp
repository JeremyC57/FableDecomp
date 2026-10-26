// CEngine native constructor B260B0. Actual engine/pool headers retained; donor188 vs retail178 (allocator B26340 requests178).
// Native CArray is12 (donor16/20), preload stats native48 vs donor50; later -8 until pool.
// Pool retains actual28-byte layout; A5C570 leaves Root.Status+18 uninitialized. Trailing vectors shrink4 each.
// CInitBaseClass constructor99A310 initializes vptr129A7CC/Valid4 and instance counter13BCA18.
// Callback nativeVT122DE50 has genuine pure OnPreDeviceReset/OnPostDeviceReset slots;
// derived callback12A0F30 targetsB26010/B25D00. Primary engineVT12A0F3C.
// Restores genuine CInitBaseClass/CIEngine plus DeviceResetCallback multiple inheritance.
// Complete primary tables: native CIEngine85/CEngine86 slots; signatures from engine_api, order independently audited.
// Available generated headers flatten bases/vptrs; native projections retain exact fields and genuine virtual signatures.
// Leaf: EngineNative
#include "engine/CEngine.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CIEngine.h"

#include <map>
#include <set>
#include "fable_string.h"
class C2DBoxF;
class C2DBoxI;
class C2DExtentsI;
class C2DVector;
class C3DAnimationInfo;
class C3DBoundingBox;
class C3DBoundingSphere;
class C3DVector;
class C4DVector;
class CCamera;
class CEngineColourFilterSettings;
class CEngineInit;
class CEngineInterpolationInfo;
class CEnginePrimitive;
class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;
class CEngineTextureMorph;
class CEngineVideoOptions;
class CGPUScratchBufferTextureManager;
class CIDrawWorld;
class CMovableResourceMemoryPool;
class CPixelFormat;
class CRGBColour;
class CRGBFloatColour;
class CStaticMapGenerationVisibiilityInfo;
class CTexture;
template<class T>class CArray;
enum EEnginePrimitiveStatIndex{NativePrimitiveStatUnspecified=0};
// Native CInitBase constructor declaration binds genuine99A310; no invented constructor dependency.
struct NativeInitBaseClass {
 virtual ~NativeInitBaseClass();
 virtual void Validate()const;
 bool Valid;
 NativeInitBaseClass();
};
struct CIEngineNative:NativeInitBaseClass {
 bool Active;
 CIEngineNative():Active(false){}
 virtual ~CIEngineNative();
 virtual bool Initialise(const CEngineInit&); // native slot2 00b232d0
 virtual void Release(); // native slot3 00b232e0
 virtual void SetCamera(const CCamera*)=0; // native slot4 00bfea08
 virtual void SetWindow(const C2DBoxF*)=0; // native slot5 00bfea08
 virtual void SetEnvironmentalEffectsAlwaysUpdate(bool); // native slot6 00b232f0
 virtual void SetWorld(const CIDrawWorld*); // native slot7 00b23300
 virtual void Render(const CEngineInterpolationInfo&,unsigned long)=0; // native slot8 00bfea08
 virtual void WorldFrameUpdate(bool); // native slot9 00b23310
 virtual void PausedWorldFrameUpdate(); // native slot10 00b23320
 virtual bool ConvertScreenPosToWorldPos(const C2DVector&,C3DVector&)const=0; // native slot11 00bfea08
 virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector&,C3DVector&)const=0; // native slot12 00bfea08
 virtual bool ConvertWorldPosToScreenPos(const C3DVector&,C2DVector&)const=0; // native slot13 00bfea08
 virtual bool SetVideoOptions(const CEngineVideoOptions&); // native slot14 00b23330
 virtual bool PreChangeResolution()=0; // native slot15 00bfea08
 virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long)=0; // native slot16 00bfea08
 virtual void Refresh(); // native slot17 00b23340
 virtual bool CheckPrimitiveVisibility(unsigned __int64); // native slot18 00b23350
 virtual float GetPrimitiveDrawDistanceFactor()const; // native slot19 00b23360
 virtual void AddPrimitive(const CEnginePrimitive&); // native slot20 00b23370
 virtual bool ClipSphere(const C3DBoundingSphere&)const; // native slot21 00b23380
 virtual bool ClipBox(const C3DBoundingBox&)const; // native slot22 00b23390
 virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long)=0; // native slot23 00bfea08
 virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&,const CEnginePrimitiveAnimation&); // native slot24 00b233a0
 virtual void SetPrimitivePosition(CEnginePrimitiveHandle&,const CEnginePrimitivePosition&); // native slot25 00b233b0
 virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // native slot26 00b233c0
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,const CEnginePrimitive&,unsigned long); // native slot28 00b233d0
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // native slot27 00b233e0
 virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&); // native slot29 00b233f0
 virtual void ClearChildPrimitive(CEnginePrimitiveHandle&,unsigned long); // native slot30 00b23400
 virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&); // native slot31 00b23410
 virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long); // native slot32 00b23420
 virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,const C2DBoxF&,EEnginePrimitiveStatIndex,unsigned long)const; // native slot33 00b23430
 virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long&,CArray<CCharString>&)const; // native slot34 00b23440
 virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&)const; // native slot35 00b23450
 virtual unsigned long AddCustomMorphTexture(CTexture&,const CCharString&)const; // native slot36 00b23460
 virtual long GetStaticMapUpdatePassCount(); // native slot37 00b23470
 virtual void StartStaticMapUpdatePass(const CWideString&,long,const CStaticMapGenerationVisibiilityInfo&,const CArray<CCharString>&); // native slot38 00b23480
 virtual bool IsStaticMapNeedingUpdate(long,const CCharString&,unsigned long)const; // native slot39 00b23490
 virtual void UpdateStaticMap(long,const CCharString&,unsigned long,long); // native slot40 00b234a0
 virtual void CloseStaticMapFile(); // native slot41 00b234b0
 virtual void AddMap(long,long); // native slot42 00b234c0
 virtual void RemoveMap(long); // native slot43 00b234d0
 virtual void SetMapAsLoaded(long); // native slot44 00b234e0
 virtual void SetMapAsUnloaded(long); // native slot45 00b234f0
 virtual void SetMapVisibilityAllMaps(bool); // native slot46 00b23500
 virtual void SetMapVisibility(long,bool); // native slot47 00b23510
 virtual void AreaChanged(const C2DBoxI&); // native slot48 00b23520
 virtual void OnActivate(); // native slot49 00b23530
 virtual void OnDeactivate(); // native slot50 00b23540
 virtual bool IsActive()const; // native slot51 00b23550
 virtual void SetStaticMapFileForUse(const CWideString&); // native slot52 00b23560
 virtual void SetLandscapeDebugTintColour(long,CRGBColour); // native slot53 00b23570
 virtual void ReloadLocalDetailDefs(); // native slot54 00b23580
 virtual void StartNewScene(unsigned long,float); // native slot55 00b23590
 virtual void CutscenePreload(bool,bool); // native slot56 00b235a0
 virtual void CutsceneAddCameraAngle(const CCamera&); // native slot57 00b235b0
 virtual void ChangeCameraAngle(); // native slot58 00b235c0
 virtual bool IsPreloaded()const; // native slot59 00b235d0
 virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&,bool,float,const std::set<long>*); // native slot60 00b235e0
 virtual void SetScreenColourFilter(const CEngineColourFilterSettings&); // native slot61 00b235f0
 virtual void SetSpotlightShadowMode(bool); // native slot62 00b23600
 virtual void SetSeaBodyToRender(CCharString); // native slot63 00b23610
 virtual void UpdateDebugOverlay(long,const CRGBColour*,const C2DBoxI&); // native slot64 00b23620
 virtual void EnableDebugOverlay(long,bool,bool); // native slot65 00b23630
 virtual void SetGlobalLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&); // native slot66 00b23640
 virtual void Set2DPrimitiveLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&); // native slot67 00b23650
 virtual void SetRenderTargetTexture(const CTexture*); // native slot68 00b23660
 virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long,const C2DExtentsI&,long,const CPixelFormat&); // native slot69 00b23670
 virtual void FreeTextureFromEngineResourcePools(CTexture*); // native slot70 00b23680
 virtual CMovableResourceMemoryPool* GetLoadingMemoryPool(); // native slot71 00b23690
 virtual CMovableResourceMemoryPool* GetAnimationMemoryPool(); // native slot72 00b236a0
 virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager(); // native slot73 00b236b0
 virtual bool DeallocateShadowBuffer(); // native slot74 00b236c0
 virtual void ReallocateShadowBuffer(); // native slot75 00b236d0
 virtual void Enable3DSceneRendering(bool); // native slot76 00b236e0
 virtual bool Is3DSceneRenderingEnabled(); // native slot77 00b236f0
 virtual void Enable2DSceneRendering(bool); // native slot78 00b23700
 virtual bool Is2DSceneRenderingEnabled(); // native slot79 00b23710
 virtual void OverrideRain(float); // native slot80 00b23720
 virtual void OverrideSnow(float); // native slot81 00b23730
 virtual void RenderColorFilteredQuad(float,float,const C4DVector&,float,float,CTexture*); // native slot82 00b23740
 virtual const CRGBFloatColour PeekGlobalDiffuseColour()const; // native slot83 00b28b40
 virtual const CRGBFloatColour PeekGlobalAmbientColour()const; // native slot84 00b28b60
};
struct NativeDeviceResetCallback {
 virtual void OnPreDeviceReset()=0;
 virtual bool OnPostDeviceReset()=0;
};
extern const double InitialPreloadTime=0.0; // native122ED70 = double0
struct NativeArray {void*Start,*Finish,*Limit;NativeArray():Start(0),Finish(0),Limit(0){}};
struct NativeDefPointer {CDefPointeeBase*Object;NativeDefPointer():Object(0){}};
struct PreloadStatsNative {double LastFullPreloadTime;bool LastFullPreloadTimedOut;double LastFastPreloadTime;bool LastFastPreloadTimedOut;PreloadStatsNative():LastFullPreloadTime(InitialPreloadTime),LastFullPreloadTimedOut(false),LastFastPreloadTime(InitialPreloadTime),LastFastPreloadTimedOut(false){}};
struct SimpleMemoryPoolNative:CSimpleMemoryPool {SimpleMemoryPoolNative(unsigned long,unsigned long);}; // A5C570, field-free API for actual header
struct ScreenResolutionNative {long Width,Height;ScreenResolutionNative():Width(0),Height(0){}};
struct AspectRatioNative {float Width,Height;AspectRatioNative():Width(4),Height(3){}};
struct VideoOptionsNative {ScreenResolutionNative ScreenResolution;AspectRatioNative ScreenAspectRatio;long BitsPerPixel,RefreshRate;bool VerticalSync;long LandscapeDetail,MeshDetail,TextureDetail,ShadowQuality,EffectsQuality,Antialiasing;
 VideoOptionsNative(){VerticalSync=true;LandscapeDetail=1;MeshDetail=1;TextureDetail=1;ShadowQuality=1;EffectsQuality=1;BitsPerPixel=0;RefreshRate=0;Antialiasing=0;}
};
struct EngineNative:CIEngineNative,NativeDeviceResetCallback {
 bool EnableWireFrame,EnableEngine,EnableLocalLights,EnableTextures;CCamera*CurrentCamera;unsigned char SceneState[0x3c-0x18];
 NativeArray PreloadCameras;PreloadStatsNative PreloadStats;
 unsigned char CameraAndFrameState[0xc0-0x68];VideoOptionsNative VideoOptions;unsigned char OffWorldState[0x118-0xf4];NativeDefPointer PEnvironmentDef,PEngineDef,PEngineVideoOptionsDefs[4];SimpleMemoryPoolNative FrameMemoryPool;
 NativeArray ScratchZBuffersD24S8;long AnisotropicFilteringLevel;NativeArray RenderLayers,EngineComponents;
 virtual ~EngineNative();
 virtual bool Initialise(const CEngineInit&); // native slot2 00b26360
 virtual void Release(); // native slot3 00b23990
 virtual void SetCamera(const CCamera*); // native slot4 00b23b50
 virtual void SetWindow(const C2DBoxF*); // native slot5 00b23b80
 virtual void SetEnvironmentalEffectsAlwaysUpdate(bool); // native slot6 00b23b90
 virtual void SetWorld(const CIDrawWorld*); // native slot7 00b23e00
 virtual void Render(const CEngineInterpolationInfo&,unsigned long); // native slot8 00b27d90
 virtual void WorldFrameUpdate(bool); // native slot9 00b24030
 virtual void PausedWorldFrameUpdate(); // native slot10 00b240c0
 virtual bool ConvertScreenPosToWorldPos(const C2DVector&,C3DVector&)const; // native slot11 00b23cf0
 virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector&,C3DVector&)const; // native slot12 00b24400
 virtual bool ConvertWorldPosToScreenPos(const C3DVector&,C2DVector&)const; // native slot13 00b242d0
 virtual bool SetVideoOptions(const CEngineVideoOptions&); // native slot14 00b24690
 virtual bool PreChangeResolution(); // native slot15 00b2aa20
 virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long); // native slot16 00b2aa30
 virtual float GetPrimitiveDrawDistanceFactor()const; // native slot19 00b23d00
 virtual void AddPrimitive(const CEnginePrimitive&); // native slot20 00b23bb0
 virtual bool ClipSphere(const C3DBoundingSphere&)const; // native slot21 00b24150
 virtual bool ClipBox(const C3DBoundingBox&)const; // native slot22 00b242a0
 virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // native slot23 00b23bc0
 virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&,const CEnginePrimitiveAnimation&); // native slot24 00b23be0
 virtual void SetPrimitivePosition(CEnginePrimitiveHandle&,const CEnginePrimitivePosition&); // native slot25 00b23bf0
 virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // native slot26 00b23c20
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,const CEnginePrimitive&,unsigned long); // native slot28 00b23c50
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // native slot27 00b23c30
 virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&); // native slot29 00b23c40
 virtual void ClearChildPrimitive(CEnginePrimitiveHandle&,unsigned long); // native slot30 00b23c60
 virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&); // native slot31 00b23c70
 virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long); // native slot32 00b23c80
 virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,const C2DBoxF&,EEnginePrimitiveStatIndex,unsigned long)const; // native slot33 00b23c90
 virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long&,CArray<CCharString>&)const; // native slot34 00b23ca0
 virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&)const; // native slot35 00b23cb0
 virtual unsigned long AddCustomMorphTexture(CTexture&,const CCharString&)const; // native slot36 00b23ce0
 virtual long GetStaticMapUpdatePassCount(); // native slot37 00b2aa40
 virtual void StartStaticMapUpdatePass(const CWideString&,long,const CStaticMapGenerationVisibiilityInfo&,const CArray<CCharString>&); // native slot38 00b23d80
 virtual bool IsStaticMapNeedingUpdate(long,const CCharString&,unsigned long)const; // native slot39 00b23d90
 virtual void UpdateStaticMap(long,const CCharString&,unsigned long,long); // native slot40 00b23da0
 virtual void CloseStaticMapFile(); // native slot41 00b23db0
 virtual void AddMap(long,long); // native slot42 00b23e10
 virtual void RemoveMap(long); // native slot43 00b23e20
 virtual void SetMapAsLoaded(long); // native slot44 00b23e30
 virtual void SetMapAsUnloaded(long); // native slot45 00b23e60
 virtual void SetMapVisibilityAllMaps(bool); // native slot46 00b23e50
 virtual void SetMapVisibility(long,bool); // native slot47 00b23e40
 virtual void AreaChanged(const C2DBoxI&); // native slot48 00b23dd0
 virtual void OnActivate(); // native slot49 00b23de0
 virtual void SetStaticMapFileForUse(const CWideString&); // native slot52 00b23dc0
 virtual void SetLandscapeDebugTintColour(long,CRGBColour); // native slot53 00b23df0
 virtual void ReloadLocalDetailDefs(); // native slot54 00b23e70
 virtual void StartNewScene(unsigned long,float); // native slot55 00b239a0
 virtual void CutscenePreload(bool,bool); // native slot56 00b23a10
 virtual void CutsceneAddCameraAngle(const CCamera&); // native slot57 00b27d50
 virtual void ChangeCameraAngle(); // native slot58 00b23a50
 virtual bool IsPreloaded()const; // native slot59 00b23a70
 virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&,bool,float,const std::set<long>*); // native slot60 00b239c0
 virtual void SetScreenColourFilter(const CEngineColourFilterSettings&); // native slot61 00b23ec0
 virtual void SetSpotlightShadowMode(bool); // native slot62 00b23c00
 virtual void SetSeaBodyToRender(CCharString); // native slot63 00b23f00
 virtual void UpdateDebugOverlay(long,const CRGBColour*,const C2DBoxI&); // native slot64 00b23e80
 virtual void EnableDebugOverlay(long,bool,bool); // native slot65 00b23e90
 virtual void SetGlobalLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&); // native slot66 00b23ea0
 virtual void Set2DPrimitiveLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&); // native slot67 00b23eb0
 virtual void SetRenderTargetTexture(const CTexture*); // native slot68 00b23ef0
 virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long,const C2DExtentsI&,long,const CPixelFormat&); // native slot69 00b23f40
 virtual void FreeTextureFromEngineResourcePools(CTexture*); // native slot70 00b23f50
 virtual CMovableResourceMemoryPool* GetLoadingMemoryPool(); // native slot71 00b25c80
 virtual CMovableResourceMemoryPool* GetAnimationMemoryPool(); // native slot72 00b25cc0
 virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager(); // native slot73 00b23f60
 virtual bool DeallocateShadowBuffer(); // native slot74 00b23f70
 virtual void ReallocateShadowBuffer(); // native slot75 00b23f80
 virtual void Enable3DSceneRendering(bool); // native slot76 00b23f90
 virtual bool Is3DSceneRenderingEnabled(); // native slot77 00b23fc0
 virtual void Enable2DSceneRendering(bool); // native slot78 00b23fd0
 virtual bool Is2DSceneRenderingEnabled(); // native slot79 00b24000
 virtual void OverrideRain(float); // native slot80 00b24010
 virtual void OverrideSnow(float); // native slot81 00b24020
 virtual void RenderColorFilteredQuad(float,float,const C4DVector&,float,float,CTexture*); // native slot82 00b25c00
 virtual const CRGBFloatColour PeekGlobalDiffuseColour()const; // native slot83 00b25d20
 virtual const CRGBFloatColour PeekGlobalAmbientColour()const; // native slot84 00b25d50
 virtual void Uninitialise(); // native slot85 00b25d80
 virtual void OnPreDeviceReset();
 virtual bool OnPostDeviceReset();
 EngineNative();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NativeInitBaseClass)==8);
FABLE_STATIC_ASSERT(sizeof(CIEngineNative)==sizeof(CIEngine));
FABLE_STATIC_ASSERT(offsetof(CIEngineNative,Active)==offsetof(CIEngine,Active));
FABLE_STATIC_ASSERT(sizeof(SimpleMemoryPoolNative)==28);
FABLE_STATIC_ASSERT(sizeof(EngineNative)==0x178); // natural8-byte double tail alignment; used native payload ends174
FABLE_STATIC_ASSERT(offsetof(EngineNative,PreloadStats)==0x48);
FABLE_STATIC_ASSERT(offsetof(EngineNative,VideoOptions)==0xc0);
FABLE_STATIC_ASSERT(offsetof(EngineNative,FrameMemoryPool)==0x130);
FABLE_STATIC_ASSERT(offsetof(EngineNative,EngineComponents)==0x168);
#endif
EngineNative::EngineNative():CurrentCamera(0),FrameMemoryPool(0,4096){
 CameraAndFrameState[0x84-0x68]=0;
}