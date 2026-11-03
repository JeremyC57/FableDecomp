#ifdef FABLE_RETAIL_TEXTURE_CACHE
#ifndef FABLE_ENGINE_CIENGINE_H
#define FABLE_ENGINE_CIENGINE_H
#include "engine/CInitBaseClass.h"
#include "engine/CEnginePrimitive.h"
namespace std {template<class T> struct less;template<class T> class allocator;template<class A,class B> struct pair;template<class K,class V,class C,class A>class map;template<class K,class C,class A>class set;}
template<class T>class CArray;
class CEngineTextureMorph;
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
class CCharString;
class CEngineColourFilterSettings;
class CEngineInit;
class CEngineInterpolationInfo;
class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;
class CEngineVideoOptions;
class CGPUScratchBufferTextureManager;
class CIDrawWorld;
class CMovableResourceMemoryPool;
class CPixelFormat;
class CRGBFloatColour;
class CStaticMapGenerationVisibiilityInfo;
class CTexture;
class CWideString;
class CIEngine:public CInitBaseClass {
public:
    virtual ~CIEngine();
    virtual bool Initialise(const CEngineInit &); // slot 2
    virtual void Release(); // slot 3
    virtual void SetCamera(const CCamera *); // slot 4
    virtual void SetWindow(const C2DBoxF *); // slot 5
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool); // slot 6
    virtual void SetWorld(const CIDrawWorld *); // slot 7
    virtual void Render(const CEngineInterpolationInfo &,unsigned long); // slot 8
    virtual void WorldFrameUpdate(bool); // slot 9
    virtual void PausedWorldFrameUpdate(); // slot 10
    virtual bool ConvertScreenPosToWorldPos(const C2DVector &,C3DVector &) const; // slot 11
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector &,C3DVector &) const; // slot 12
    virtual bool ConvertWorldPosToScreenPos(const C3DVector &,C2DVector &) const; // slot 13
    virtual bool SetVideoOptions(const CEngineVideoOptions &); // slot 14
    virtual bool PreChangeResolution(); // slot 15
    virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long); // slot 16
    virtual void Refresh(); // slot 17
    virtual bool CheckPrimitiveVisibility(unsigned __int64); // slot 18
    virtual float GetPrimitiveDrawDistanceFactor() const; // slot 19
    virtual void AddPrimitive(const CEnginePrimitive &); // slot 20
    virtual bool ClipSphere(const C3DBoundingSphere &) const; // slot 21
    virtual bool ClipBox(const C3DBoundingBox &) const; // slot 22
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle &,const CEnginePrimitive &,unsigned long); // slot 23
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle &,const CEnginePrimitiveAnimation &); // slot 24
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle &,const CEnginePrimitivePosition &); // slot 25
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle &,const CEnginePrimitive &,unsigned long); // slot 26
    virtual void SetChildPrimitive(CEnginePrimitiveHandle &,unsigned long,CEnginePrimitiveHandle &,const CEnginePrimitive &,unsigned long); // slot 27
    virtual void SetChildPrimitive(CEnginePrimitiveHandle &,unsigned long,const CEnginePrimitive &,unsigned long); // slot 28
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle &,unsigned long,CEnginePrimitiveHandle &); // slot 29
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle &,unsigned long); // slot 30
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle &); // slot 31
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle &,const CEnginePrimitive &,const CEnginePrimitiveAttachmentInfo &,const CEnginePrimitiveHandle &,long,unsigned long); // slot 32
    virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > &,const C2DBoxF &,EEnginePrimitiveStatIndex,unsigned long) const; // slot 33
    virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > &,EEnginePrimitiveStatIndex,unsigned long &,CArray<CCharString> &) const; // slot 34
    virtual const C3DAnimationInfo * GetEditorAnimation(const CWideString &) const; // slot 35
    virtual unsigned long AddCustomMorphTexture(CTexture &,const CCharString &) const; // slot 36
    virtual long GetStaticMapUpdatePassCount(); // slot 37
    virtual void StartStaticMapUpdatePass(const CWideString &,long,const CStaticMapGenerationVisibiilityInfo &,const CArray<CCharString> &); // slot 38
    virtual bool IsStaticMapNeedingUpdate(long,const CCharString &,unsigned long) const; // slot 39
    virtual void UpdateStaticMap(long,const CCharString &,unsigned long,long); // slot 40
    virtual void CloseStaticMapFile(); // slot 41
    virtual void AddMap(long,long); // slot 42
    virtual void RemoveMap(long); // slot 43
    virtual void SetMapAsLoaded(long); // slot 44
    virtual void SetMapAsUnloaded(long); // slot 45
    virtual void SetMapVisibilityAllMaps(bool); // slot 46
    virtual void SetMapVisibility(long,bool); // slot 47
    virtual void AreaChanged(const C2DBoxI &); // slot 48
    virtual void OnActivate(); // slot 49
    virtual void OnDeactivate(); // slot 50
    virtual bool IsActive() const; // slot 51
    virtual void SetStaticMapFileForUse(const CWideString &); // slot 52
    virtual void SetLandscapeDebugTintColour(long,CRGBColour); // slot 53
    virtual void ReloadLocalDetailDefs(); // slot 54
    virtual void StartNewScene(unsigned long,float); // slot 55
    virtual void CutscenePreload(bool,bool); // slot 56
    virtual void CutsceneAddCameraAngle(const CCamera &); // slot 57
    virtual void ChangeCameraAngle(); // slot 58
    virtual bool IsPreloaded() const; // slot 59
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph> &,bool,float,const std::set<long,std::less<long>,std::allocator<long> > *); // slot 60
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings &); // slot 61
    virtual void SetSpotlightShadowMode(bool); // slot 62
    virtual void SetSeaBodyToRender(CCharString); // slot 63
    virtual void UpdateDebugOverlay(long,const CRGBColour *,const C2DBoxI &); // slot 64
    virtual void EnableDebugOverlay(long,bool,bool); // slot 65
    virtual void SetGlobalLightingParameters(const C3DVector &,const CRGBFloatColour &,const CRGBFloatColour &,const CRGBFloatColour &); // slot 66
    virtual void Set2DPrimitiveLightingParameters(const C3DVector &,const CRGBFloatColour &,const CRGBFloatColour &,const CRGBFloatColour &); // slot 67
    virtual void SetRenderTargetTexture(const CTexture *); // slot 68
    virtual CTexture * AllocateTextureFromEngineResourcePools(unsigned long,const C2DExtentsI &,long,const CPixelFormat &); // slot 69
    virtual void FreeTextureFromEngineResourcePools(CTexture *); // slot 70
    virtual CMovableResourceMemoryPool * GetLoadingMemoryPool(); // slot 71
    virtual CMovableResourceMemoryPool * GetAnimationMemoryPool(); // slot 72
    virtual CGPUScratchBufferTextureManager * GetScratchBufferTextureManager(); // slot 73
    virtual bool DeallocateShadowBuffer(); // slot 74
    virtual void ReallocateShadowBuffer(); // slot 75
    virtual void Enable3DSceneRendering(bool); // slot 76
    virtual bool Is3DSceneRenderingEnabled(); // slot 77
    virtual void Enable2DSceneRendering(bool); // slot 78
    virtual bool Is2DSceneRenderingEnabled(); // slot 79
    virtual void OverrideRain(float); // slot 80
    virtual void OverrideSnow(float); // slot 81
    virtual void RenderColorFilteredQuad(float,float,const C4DVector &,float,float,CTexture *); // slot 82
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const; // slot 83
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const; // slot 84
protected:
    bool Active;
};
#endif

#else
#ifdef FABLE_RETAIL_CAMERA_DRAW
#ifndef FABLE_ENGINE_CIENGINE_H
#define FABLE_ENGINE_CIENGINE_H
#include "engine/CInitBaseClass.h"
#include "engine/CEnginePrimitive.h"
namespace std {template<class T> struct less;template<class T> class allocator;template<class A,class B> struct pair;template<class K,class V,class C,class A>class map;template<class K,class C,class A>class set;}
template<class T>class CArray;
class CEngineTextureMorph;
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
class CCharString;
class CEngineColourFilterSettings;
class CEngineInit;
class CEngineInterpolationInfo;
class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;
class CEngineVideoOptions;
class CGPUScratchBufferTextureManager;
class CIDrawWorld;
class CMovableResourceMemoryPool;
class CPixelFormat;
class CRGBFloatColour;
class CStaticMapGenerationVisibiilityInfo;
class CTexture;
class CWideString;
class CIEngine:public CInitBaseClass {
public:
    virtual ~CIEngine();
    virtual bool Initialise(const CEngineInit &)=0; // slot 2
    virtual void Release()=0; // slot 3
    virtual void SetCamera(const CCamera *)=0; // slot 4
    virtual void SetWindow(const C2DBoxF *)=0; // slot 5
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool)=0; // slot 6
    virtual void SetWorld(const CIDrawWorld *)=0; // slot 7
    virtual void Render(const CEngineInterpolationInfo &,unsigned long)=0; // slot 8
    virtual void WorldFrameUpdate(bool)=0; // slot 9
    virtual void PausedWorldFrameUpdate()=0; // slot 10
    virtual bool ConvertScreenPosToWorldPos(const C2DVector &,C3DVector &) const=0; // slot 11
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector &,C3DVector &) const=0; // slot 12
    virtual bool ConvertWorldPosToScreenPos(const C3DVector &,C2DVector &) const=0; // slot 13
    virtual bool SetVideoOptions(const CEngineVideoOptions &)=0; // slot 14
    virtual bool PreChangeResolution()=0; // slot 15
    virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long)=0; // slot 16
    virtual void Refresh()=0; // slot 17
    virtual bool CheckPrimitiveVisibility(unsigned __int64)=0; // slot 18
    virtual float GetPrimitiveDrawDistanceFactor() const=0; // slot 19
    virtual void AddPrimitive(const CEnginePrimitive &)=0; // slot 20
    virtual bool ClipSphere(const C3DBoundingSphere &) const=0; // slot 21
    virtual bool ClipBox(const C3DBoundingBox &) const=0; // slot 22
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle &,const CEnginePrimitive &,unsigned long)=0; // slot 23
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle &,const CEnginePrimitiveAnimation &)=0; // slot 24
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle &,const CEnginePrimitivePosition &)=0; // slot 25
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle &,const CEnginePrimitive &,unsigned long)=0; // slot 26
    virtual void SetChildPrimitive(CEnginePrimitiveHandle &,unsigned long,CEnginePrimitiveHandle &,const CEnginePrimitive &,unsigned long)=0; // slot 27
    virtual void SetChildPrimitive(CEnginePrimitiveHandle &,unsigned long,const CEnginePrimitive &,unsigned long)=0; // slot 28
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle &,unsigned long,CEnginePrimitiveHandle &)=0; // slot 29
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle &,unsigned long)=0; // slot 30
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle &)=0; // slot 31
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle &,const CEnginePrimitive &,const CEnginePrimitiveAttachmentInfo &,const CEnginePrimitiveHandle &,long,unsigned long)=0; // slot 32
    virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > &,const C2DBoxF &,EEnginePrimitiveStatIndex,unsigned long) const=0; // slot 33
    virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > &,EEnginePrimitiveStatIndex,unsigned long &,CArray<CCharString> &) const=0; // slot 34
    virtual const C3DAnimationInfo * GetEditorAnimation(const CWideString &) const=0; // slot 35
    virtual unsigned long AddCustomMorphTexture(CTexture &,const CCharString &) const=0; // slot 36
    virtual long GetStaticMapUpdatePassCount()=0; // slot 37
    virtual void StartStaticMapUpdatePass(const CWideString &,long,const CStaticMapGenerationVisibiilityInfo &,const CArray<CCharString> &)=0; // slot 38
    virtual bool IsStaticMapNeedingUpdate(long,const CCharString &,unsigned long) const=0; // slot 39
    virtual void UpdateStaticMap(long,const CCharString &,unsigned long,long)=0; // slot 40
    virtual void CloseStaticMapFile()=0; // slot 41
    virtual void AddMap(long,long)=0; // slot 42
    virtual void RemoveMap(long)=0; // slot 43
    virtual void SetMapAsLoaded(long)=0; // slot 44
    virtual void SetMapAsUnloaded(long)=0; // slot 45
    virtual void SetMapVisibilityAllMaps(bool)=0; // slot 46
    virtual void SetMapVisibility(long,bool)=0; // slot 47
    virtual void AreaChanged(const C2DBoxI &)=0; // slot 48
    virtual void OnActivate()=0; // slot 49
    virtual void OnDeactivate()=0; // slot 50
    virtual bool IsActive() const=0; // slot 51
    virtual void SetStaticMapFileForUse(const CWideString &)=0; // slot 52
    virtual void SetLandscapeDebugTintColour(long,CRGBColour)=0; // slot 53
    virtual void ReloadLocalDetailDefs()=0; // slot 54
    virtual void StartNewScene(unsigned long,float)=0; // slot 55
    virtual void CutscenePreload(bool,bool)=0; // slot 56
    virtual void CutsceneAddCameraAngle(const CCamera &)=0; // slot 57
    virtual void ChangeCameraAngle()=0; // slot 58
    virtual bool IsPreloaded() const=0; // slot 59
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph> &,bool,float,const std::set<long,std::less<long>,std::allocator<long> > *)=0; // slot 60
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings &)=0; // slot 61
    virtual void SetSpotlightShadowMode(bool)=0; // slot 62
    virtual void SetSeaBodyToRender(CCharString)=0; // slot 63
    virtual void UpdateDebugOverlay(long,const CRGBColour *,const C2DBoxI &)=0; // slot 64
    virtual void EnableDebugOverlay(long,bool,bool)=0; // slot 65
    virtual void SetGlobalLightingParameters(const C3DVector &,const CRGBFloatColour &,const CRGBFloatColour &,const CRGBFloatColour &)=0; // slot 66
    virtual void Set2DPrimitiveLightingParameters(const C3DVector &,const CRGBFloatColour &,const CRGBFloatColour &,const CRGBFloatColour &)=0; // slot 67
    virtual void SetRenderTargetTexture(const CTexture *)=0; // slot 68
    virtual CTexture * AllocateTextureFromEngineResourcePools(unsigned long,const C2DExtentsI &,long,const CPixelFormat &)=0; // slot 69
    virtual void FreeTextureFromEngineResourcePools(CTexture *)=0; // slot 70
    virtual CMovableResourceMemoryPool * GetLoadingMemoryPool()=0; // slot 71
    virtual CMovableResourceMemoryPool * GetAnimationMemoryPool()=0; // slot 72
    virtual CGPUScratchBufferTextureManager * GetScratchBufferTextureManager()=0; // slot 73
    virtual bool DeallocateShadowBuffer()=0; // slot 74
    virtual void ReallocateShadowBuffer()=0; // slot 75
    virtual void Enable3DSceneRendering(bool)=0; // slot 76
    virtual bool Is3DSceneRenderingEnabled()=0; // slot 77
    virtual void Enable2DSceneRendering(bool)=0; // slot 78
    virtual bool Is2DSceneRenderingEnabled()=0; // slot 79
    virtual void OverrideRain(float)=0; // slot 80
    virtual void OverrideSnow(float)=0; // slot 81
    virtual void RenderColorFilteredQuad(float,float,const C4DVector &,float,float,CTexture *)=0; // slot 82
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const=0; // slot 83
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const=0; // slot 84
protected:
    bool Active;
};
#endif
#else
#ifndef FABLE_ENGINE_CIENGINE_H
#define FABLE_ENGINE_CIENGINE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CIEngine = 0xc.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CIEngine {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    bool          Active;                                    // +0x08
    unsigned char _pad_0x09[0x3];                            // +0x09
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CIEngine) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CIEngine, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CIEngine, Active) == 0x8);

#endif // FABLE_ENGINE_CIENGINE_H

#endif

#endif
