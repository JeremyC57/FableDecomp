#ifndef FABLE_ENGINE_CENGINE_H
#define FABLE_ENGINE_CENGINE_H
#ifdef FABLE_RETAIL_CENGINE
// Retail branch (chunk 731, 2026-11-03): the polymorphic engine. RTTI .?AVCEngine@@ : CIEngine (+0, : CInitBaseClass : CBaseClass),
// CDeviceResetCallback (+0xC); vtables 0x012A0F3C (CIEngine part; slot 2 Initialise 0x00B26360) and 0x012A0F30 (CDeviceResetCallback
// part: OnPreDeviceReset 0x00B26010, OnPostDeviceReset 0x00B25D00). Members: Ego_r.pdb names in Ego_r order at the retail offsets of
// the generated model below (12-byte STLport CArrays; the bases end at +0x10 = EnableWireFrame); composite members whose types are
// not modelled yet stay opaque byte blocks named as in the PDB (with the generated model's alignment pads in front of them). Retail reads EnableWireFrame +0x10, EnableTextures +0x13 and
// AnisotropicFilteringLevel +0x158 in SetDefaultRenderStates 0x00B24850. Functions: Ego_r.pdb / FableWin.pdb spellings.
#ifndef FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CIEngine.h"
#include "engine/CDeviceResetCallback.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
class CCamera;
class CEngineInit;
class CEngineDef;
class CEnvironmentDef;
class CEngineVideoOptionsDef;
class CEngineLayerRenderer;
#ifdef FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF_TYPED
#include "engine/EEngineVideoOptionQuality.h"
#endif
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
// NF249 opt-in (q12692, CEngine ctor 0x00B260B0 / dtor 0x00B261D0): the seven composite members typed (Ego_r.pdb CEngine member records,
// retail offsets: STLport 12-byte CArrays), the nested CEngine::CPreloadStats, CEngineVideoOptions with its inline ctor, and the retail
// polymorphic CSimpleMemoryPool. Each typed member replaces its opaque line only under this macro (the #else keeps the line verbatim).
#ifndef FABLE_RETAIL_CENGINEVIDEOOPTIONS_CTOR
#define FABLE_RETAIL_CENGINEVIDEOOPTIONS_CTOR
#endif
#ifndef FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#endif
#include "engine/CArray.h"
#include "engine/CCamera.h"
#include "engine/CEngineVideoOptions.h"
#include "engine/CSimpleMemoryPool.h"
class CSurface;
class CEngineComponent;
#endif
class CEngine : public CIEngine, public CDeviceResetCallback {
public:
#ifdef FABLE_RETAIL_ANIMATED_MESH_PRELOAD_PASS30
    float GetActorRadius() const {return ActorRadius;}
    void SetActorsLoading() {ActorsLoading=true;}
#endif
#ifdef FABLE_RETAIL_CENGINE_RENDER_PASS26
    virtual void Render(const CEngineInterpolationInfo& interpolation, unsigned long render_flags);
private:
    void UpdateOffGameTickProcessInfo(bool);
    void LoadNewScene(const CEngineInterpolationInfo&);
    void RenderFrame(const CEngineInterpolationInfo&);
public:
#endif
    virtual bool Initialise(const CEngineInit& init);                    // slot 2 0x00B26360 ?Initialise@CEngine@@UAE_NABVCEngineInit@@@Z
#ifdef FABLE_RETAIL_CENGINE_OVERRIDES
    // NF244 (q12680 step 2): both PDBs make the two CDeviceResetCallback overrides private (?OnPreDeviceReset@CEngine@@EAEXXZ, Ego_r
    // 0x009B0650 / FableWin 0x02C9AA40; ?OnPostDeviceReset@CEngine@@EAE_NXZ, 0x009B0360 / 0x02C9ABC0); the default keeps them public.
private:
#endif
    virtual void OnPreDeviceReset();                                     // 0x00B26010 ?OnPreDeviceReset@CEngine@@UAEXXZ
    virtual bool OnPostDeviceReset();                                    // 0x00B25D00 ?OnPostDeviceReset@CEngine@@UAE_NXZ
#ifdef FABLE_RETAIL_CENGINE_OVERRIDES
public:
#endif
    virtual void Uninitialise();                                         // slot 85 0x00B25D80 ?Uninitialise@CEngine@@UAEXXZ (CEngine's first own virtual)
#ifdef FABLE_RETAIL_CENGINE_SETPERSISTENTPRIMITIVE
    // NF172 opt-in (row 0x00B23BC0): the CIEngine slot-23 override, Ego_r.pdb / FableWin.pdb
    // ?SetPersistentPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z (vtable 0x012A0F3C slot 23). Declaration only:
    // an override.
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);   // 0x00B23BC0
#endif
#ifdef FABLE_RETAIL_CENGINE_GETSCRATCHBUFFERTEXTUREMANAGER
    // Opt-in (NF46 round 7, row 0x00B23F60): the CIEngine slot-73 override, Ego_r.pdb / FableWin.pdb
    // ?GetScratchBufferTextureManager@CEngine@@UAEPAVCGPUScratchBufferTextureManager@@XZ (retail 0x00B23F60). Declaration only: an override.
    virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager();   // 0x00B23F60
#endif
#ifdef FABLE_RETAIL_CENGINE_GETMEMORYPOOLS
    // Opt-in (NF46 round 7, rows 0x00B25C80 / 0x00B25CC0): the CIEngine slot-71 / 72 overrides, Ego_r.pdb / FableWin.pdb
    // ?GetLoadingMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ and ?GetAnimationMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ
    // (retail 0x00B25C80 / 0x00B25CC0). Declarations only: overrides.
    virtual CMovableResourceMemoryPool* GetLoadingMemoryPool();          // 0x00B25C80
    virtual CMovableResourceMemoryPool* GetAnimationMemoryPool();        // 0x00B25CC0
#endif
#ifdef FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
    // Nested opt-in (chunk 763): the slot-13 override, FableWin.pdb ?ConvertWorldPosToScreenPos@CEngine@@UBE_NABVC3DVector@@AAVC2DVector@@@Z 0x00B242D0.
    virtual bool ConvertWorldPosToScreenPos(const C3DVector& worldPos, C2DVector& screenPos) const;
#endif
#ifdef FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
    // Nested opt-in (chunk 758): the slot-12 override, FableWin.pdb ?ConvertScreenPosToWorldPosAtViewDistance@CEngine@@UBE_NMABVC2DVector@@AAVC3DVector@@@Z 0x00B24400.
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos) const;
#endif
#ifdef FABLE_RETAIL_CENGINE_SETSPOTLIGHTSHADOWMODE
    // Nested opt-in (NF42, row 0x00B23C00): the slot-62 CIEngine override, Ego_r.pdb / FableWin.pdb ?SetSpotlightShadowMode@CEngine@@UAEX_N@Z.
    virtual void SetSpotlightShadowMode(bool enable);
#endif
#ifdef FABLE_RETAIL_CENGINE_GETNONINTERPOLATEDWORLDTIME
    // Opt-in (egod e03, row 0x00B81820 CEngineSimulationManager::FrameStart): FableWin.pdb ?GetNonInterpolatedWorldTime@CEngine@@QBENXZ
    // (Ego_d Engine_Simulation_Manager.obj, 20 debug bytes, first used by FrameStart; inline in retail: `fmul qword [GPEngine+98h]`).
    double GetNonInterpolatedWorldTime() const { return NonInterpolatedWorldTime; }
#endif
#ifdef FABLE_RETAIL_CENGINE_OVERRIDES
    // NF244 opt-in (q12680 step 2, for the ctor / dtor rows 0x00B260B0 / 0x00B261D0): the CEngine ctor, the virtual dtor and every
    // CIEngine override CEngine's vtable 0x012A0F3C holds (slots 2-85; Initialise / Uninitialise are declared above), Ego_r.pdb +
    // FableWin.pdb ?<Name>@CEngine@@ spellings (each one in both dumps: hand-back hdr/README.md), retail override address per slot.
    // Declarations only. Members another opt-in already declares are skipped when that opt-in is on. A row that uses this opt-in
    // must also define FABLE_RETAIL_CIENGINE_RENDER (slot 8: the CIEngine default keeps its descriptive placeholder), as the
    // RENDER_PASS26 rows do, and needs CIEngine.h's slot 11-13 PDB declarations (the default since NF244 q12680 step 1; on an older
    // CIEngine.h also define FABLE_RETAIL_CIENGINE_SCREENPOS / _CONVERTSCREENPOS / _CONVERTWORLDPOS), or those three overrides become new
    // slots after Uninitialise (85). Probe: a derived class's CIEngine-part vtable has 86 entries, slots 8 / 11-13 / 85 on these names.
    CEngine();                                                                                           // 0x00B260B0 ??0CEngine@@QAE@XZ
    virtual ~CEngine();                                                                                  //  0 0x00B261D0 ??1CEngine@@UAE@XZ
    virtual void Release();                                                                          //  3 0x00B23990
    virtual void SetCamera(const CCamera*);                                                          //  4 0x00B23B50
    virtual void SetWindow(const C2DBoxF*);                                                          //  5 0x00B23B80
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool);                                          //  6 0x00B23B90
    virtual void SetWorld(const CIDrawWorld*);                                                       //  7 0x00B23E00
#if !defined(FABLE_RETAIL_CENGINE_RENDER_PASS26)
    virtual void Render(const CEngineInterpolationInfo&,unsigned long);                              //  8 0x00B27D90
#endif
    virtual void WorldFrameUpdate(bool);                                                             //  9 0x00B24030
    virtual void PausedWorldFrameUpdate();                                                           // 10 0x00B240C0
    virtual bool ConvertScreenPosToWorldPos(const C2DVector&,C3DVector&)const;                       // 11 0x00B23CF0
#if !defined(FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS)
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector&,C3DVector&)const;   // 12 0x00B24400
#endif
#if !defined(FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS)
    virtual bool ConvertWorldPosToScreenPos(const C3DVector&,C2DVector&)const;                       // 13 0x00B242D0
#endif
    virtual bool SetVideoOptions(const CEngineVideoOptions&);                                        // 14 0x00B24690
    virtual bool PreChangeResolution();                                                              // 15 0x00B2AA20
    virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long);                    // 16 0x00B2AA30
    virtual float GetPrimitiveDrawDistanceFactor()const;                                             // 19 0x00B23D00
    virtual void AddPrimitive(const CEnginePrimitive&);                                              // 20 0x00B23BB0
    virtual bool ClipSphere(const C3DBoundingSphere&)const;                                          // 21 0x00B24150
    virtual bool ClipBox(const C3DBoundingBox&)const;                                                // 22 0x00B242A0
#if !defined(FABLE_RETAIL_CENGINE_SETPERSISTENTPRIMITIVE)
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // 23 0x00B23BC0
#endif
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&,const CEnginePrimitiveAnimation&);    // 24 0x00B23BE0
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle&,const CEnginePrimitivePosition&);      // 25 0x00B23BF0
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // 26 0x00B23C20
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,const CEnginePrimitive&,unsigned long); // 28 0x00B23C50
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long); // 27 0x00B23C30
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&); // 29 0x00B23C40
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle&,unsigned long);                         // 30 0x00B23C60
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&);                                   // 31 0x00B23C70
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long); // 32 0x00B23C80
    virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,const C2DBoxF&,EEnginePrimitiveStatIndex,unsigned long)const; // 33 0x00B23C90
    virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long&,CArray<CCharString>&)const; // 34 0x00B23CA0
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&)const;                     // 35 0x00B23CB0
    virtual unsigned long AddCustomMorphTexture(CTexture&,const CCharString&)const;                  // 36 0x00B23CE0
    virtual long GetStaticMapUpdatePassCount();                                                      // 37 0x00B2AA40
    virtual void StartStaticMapUpdatePass(const CWideString&,long,const CStaticMapGenerationVisibiilityInfo&,const CArray<CCharString>&); // 38 0x00B23D80
    virtual bool IsStaticMapNeedingUpdate(long,const CCharString&,unsigned long)const;               // 39 0x00B23D90
    virtual void UpdateStaticMap(long,const CCharString&,unsigned long,long);                        // 40 0x00B23DA0
    virtual void CloseStaticMapFile();                                                               // 41 0x00B23DB0
    virtual void AddMap(long,long);                                                                  // 42 0x00B23E10
    virtual void RemoveMap(long);                                                                    // 43 0x00B23E20
    virtual void SetMapAsLoaded(long);                                                               // 44 0x00B23E30
    virtual void SetMapAsUnloaded(long);                                                             // 45 0x00B23E60
    virtual void SetMapVisibilityAllMaps(bool);                                                      // 46 0x00B23E50
    virtual void SetMapVisibility(long,bool);                                                        // 47 0x00B23E40
    virtual void AreaChanged(const C2DBoxI&);                                                        // 48 0x00B23DD0
    virtual void OnActivate();                                                                       // 49 0x00B23DE0
    virtual void SetStaticMapFileForUse(const CWideString&);                                         // 52 0x00B23DC0
    virtual void SetLandscapeDebugTintColour(long,CRGBColour);                                       // 53 0x00B23DF0
    virtual void ReloadLocalDetailDefs();                                                            // 54 0x00B23E70
    virtual void StartNewScene(unsigned long,float);                                                 // 55 0x00B239A0
    virtual void CutscenePreload(bool,bool);                                                         // 56 0x00B23A10
    virtual void CutsceneAddCameraAngle(const CCamera&);                                             // 57 0x00B27D50
    virtual void ChangeCameraAngle();                                                                // 58 0x00B23A50
    virtual bool IsPreloaded()const;                                                                 // 59 0x00B23A70
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&,bool,float,const std::set<long,std::less<long>,std::allocator<long> >*);   // 60 0x00B239C0
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings&);                          // 61 0x00B23EC0
#if !defined(FABLE_RETAIL_CENGINE_SETSPOTLIGHTSHADOWMODE)
    virtual void SetSpotlightShadowMode(bool);                                                       // 62 0x00B23C00
#endif
    virtual void SetSeaBodyToRender(CCharString);                                                    // 63 0x00B23F00
    virtual void UpdateDebugOverlay(long,const CRGBColour*,const C2DBoxI&);                          // 64 0x00B23E80
    virtual void EnableDebugOverlay(long,bool,bool);                                                 // 65 0x00B23E90
    virtual void SetGlobalLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&); // 66 0x00B23EA0
    virtual void Set2DPrimitiveLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&); // 67 0x00B23EB0
    virtual void SetRenderTargetTexture(const CTexture*);                                            // 68 0x00B23EF0
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long,const C2DExtentsI&,long,const CPixelFormat&); // 69 0x00B23F40
    virtual void FreeTextureFromEngineResourcePools(CTexture*);                                      // 70 0x00B23F50
#if !defined(FABLE_RETAIL_CENGINE_GETMEMORYPOOLS)
    virtual CMovableResourceMemoryPool* GetLoadingMemoryPool();                                      // 71 0x00B25C80
#endif
#if !defined(FABLE_RETAIL_CENGINE_GETMEMORYPOOLS)
    virtual CMovableResourceMemoryPool* GetAnimationMemoryPool();                                    // 72 0x00B25CC0
#endif
#if !defined(FABLE_RETAIL_CENGINE_GETSCRATCHBUFFERTEXTUREMANAGER)
    virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager();                       // 73 0x00B23F60
#endif
    virtual bool DeallocateShadowBuffer();                                                           // 74 0x00B23F70
    virtual void ReallocateShadowBuffer();                                                           // 75 0x00B23F80
    virtual void Enable3DSceneRendering(bool);                                                       // 76 0x00B23F90
    virtual bool Is3DSceneRenderingEnabled();                                                        // 77 0x00B23FC0
    virtual void Enable2DSceneRendering(bool);                                                       // 78 0x00B23FD0
    virtual bool Is2DSceneRenderingEnabled();                                                        // 79 0x00B24000
    virtual void OverrideRain(float);                                                                // 80 0x00B24010
    virtual void OverrideSnow(float);                                                                // 81 0x00B24020
    virtual void RenderColorFilteredQuad(float,float,const C4DVector&,float,float,CTexture*);        // 82 0x00B25C00
    virtual const CRGBFloatColour PeekGlobalDiffuseColour()const;                                    // 83 0x00B25D20
    virtual const CRGBFloatColour PeekGlobalAmbientColour()const;                                    // 84 0x00B25D50
#endif
private:
    void SetDefaultRenderStates();                                       // 0x00B24850 ?SetDefaultRenderStates@CEngine@@AAEXXZ
    // ??$AddEngineComponent@V<T>@@@CEngine@@AAEXPAV<T>@@AAPAV1@E@Z (Ego_r.pdb; out of line in retail, one COMDAT per component:
    // 0x00B29930 .. 0x00B2A980): publish the component through its global pointer and append it to EngineComponents.
    template <class T> void AddEngineComponent(T* component, T*& global_pointer, unsigned char flag);
    CEngineLayerRenderer& AddRenderLayer(CEngineLayerRenderer* layer);   // 0x00B262C0 ?AddRenderLayer@CEngine@@AAEAAVCEngineLayerRenderer@@PAV2@@Z
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
public:
    // Ego_r.pdb CEngine::CPreloadStats (32 bytes: LastFullPreloadTime +0, LastFullPreloadTimedOut +8, LastFastPreloadTime +0x10,
    // LastFastPreloadTimedOut +0x18); the user-declared ctor is FableWin.pdb / Ego_d.pdb ??0CPreloadStats@CEngine@@QAE@XZ, inline in retail
    // (expanded in the CEngine ctor 0x00B260B0: both times 0.0 from the double constant 0x0122ED70, both flags false).
    class CPreloadStats {
    public:
        CPreloadStats() : LastFullPreloadTime(0.0), LastFullPreloadTimedOut(false), LastFastPreloadTime(0.0), LastFastPreloadTimedOut(false) { }
        double LastFullPreloadTime;
        bool   LastFullPreloadTimedOut;
        double LastFastPreloadTime;
        bool   LastFastPreloadTimedOut;
    };
#endif
public:
    bool             EnableWireFrame;                                    // +0x10
    bool             EnableEngine;                                       // +0x11
    bool             EnableLocalLights;                                  // +0x12
    bool             EnableTextures;                                     // +0x13
    CCamera*         CurrentCamera;                                      // +0x14
    bool             NewSceneFlag;                                       // +0x18
    unsigned long    NewSceneLoadFlags;                                  // +0x1c
    float            NewSceneTimeOut;                                    // +0x20
    bool             CutscenePreloadFlag;                                // +0x24
    float            CutscenePreloadTimeout;                             // +0x28
    float            CutsceneFastPreloadTimeout;                         // +0x2c
    float            ActorAdditionalLoadTimeout;                         // +0x30
    float            ActorRadius;                                        // +0x34
    bool             ActorsLoading;                                      // +0x38
    bool             CameraAngleChanged;                                 // +0x39
    unsigned char    _pad_0x3a[0x2];                                     // +0x3a
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
    CArray<CCamera>  PreloadCameras;                                     // +0x3c
    CPreloadStats    PreloadStats;                                       // +0x48
#else
    unsigned char    PreloadCameras[0xc];                                // +0x3c CArray<CCamera>
    unsigned char    PreloadStats[0x20];                                 // +0x48 CPreloadStats
#endif
    C3DVector        PreloadedCameraPos;                                 // +0x68
    C3DVector        PreloadedLookVector;                                // +0x74
    float            StartNewSceneTransitionTime;                        // +0x80
    bool             IsInitialised;                                      // +0x84
    long             CurrentRenderFrame;                                 // +0x88
    long             CurrentWorldFrame;                                  // +0x8c
    long             CurrentPausedWorldFrame;                            // +0x90
    double           NonInterpolatedWorldTime;                           // +0x98
    double           NonInterpolatedPausedWorldTime;                     // +0xa0
    double           InterpolatedWorldTime;                              // +0xa8
    unsigned long    WorldFramesPerSecond;                               // +0xb0
    bool             BlockOnLoad;                                        // +0xb4
    unsigned long    SceneFilterFlags;                                   // +0xb8
    bool             EnableScreenShotMode;                               // +0xbc
    bool             EnablePostDeviceReset;                              // +0xbd
    unsigned char    _pad_0xbe[0x2];                                     // +0xbe
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
    CEngineVideoOptions VideoOptions;                                    // +0xc0
#else
    unsigned char    VideoOptions[0x34];                                 // +0xc0 CEngineVideoOptions
#endif
    struct {                                                             // +0xf8 <unnamed-type-OffWorldTickProcessInfo> (0x20)
        bool         ProcessThisFrame;                                   //   +0x00
        bool         ProcessingEnabled;                                  //   +0x01
        float        ProcessTicksBehindAccumilator;                      //   +0x04
        float        MaxProcessTicksBehind;                              //   +0x08
        float        MaxProcessTicksBehindBeforeCatchup;                 //   +0x0c
        float        MinProcessFrequency;                                //   +0x10
        double       LastFrameTimeStamp;                                 //   +0x18
    } OffWorldTickProcessInfo;
    CDefPointer<const CEnvironmentDef>        PEnvironmentDef;           // +0x118
    CDefPointer<const CEngineDef>             PEngineDef;                // +0x11c
    CDefPointer<const CEngineVideoOptionsDef> PEngineVideoOptionsDefs[4];   // +0x120
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
    CSimpleMemoryPool FrameMemoryPool;                                   // +0x130
    CArray<CSurface*> ScratchZBuffersD24S8;                              // +0x14c
#else
    unsigned char    FrameMemoryPool[0x1c];                              // +0x130 CSimpleMemoryPool
    unsigned char    ScratchZBuffersD24S8[0xc];                          // +0x14c CArray<CSurface*>
#endif
    long             AnisotropicFilteringLevel;                          // +0x158
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
    CArray<CEngineLayerRenderer*> RenderLayers;                          // +0x15c
    CArray<CEngineComponent*>     EngineComponents;                      // +0x168
#else
    unsigned char    RenderLayers[0xc];                                  // +0x15c CArray<CEngineLayerRenderer*>
    unsigned char    EngineComponents[0xc];                              // +0x168 CArray<CEngineComponent*>
#endif
#ifdef FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF_TYPED
    // NF56 opt-in (row 0x00B244F0, the out-of-line body): Ego_r.pdb 0x009AEB40 / FableWin.pdb 0x02CA1700
    // ?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z on the typed retail members (returns
    // *PEngineVideoOptionsDefs[quality], +0x120). The flattened branch below declares the same member under FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF
    // for its callers. Declaration only: no layout change.
    const CEngineVideoOptionsDef& PeekVideoOptionsDef(EEngineVideoOptionQuality quality);   // 0x00B244F0
#endif
#ifdef FABLE_RETAIL_CENGINE_GETFRAMEMEMORYPOOL
    // Retail opt-in (agentC40, row 0x00BD2D90): FableWin.pdb ?GetFrameMemoryPool@CEngine@@QAEPAVCSimpleMemoryPool@@XZ (inline in retail:
    // `mov ecx,[GPEngine] ; add ecx,130h`, 0x00BD2DB8). No layout change (the member stays the opaque block).
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
    CSimpleMemoryPool* GetFrameMemoryPool() { return &FrameMemoryPool; }
#else
    class CSimpleMemoryPool* GetFrameMemoryPool() { return (class CSimpleMemoryPool*)FrameMemoryPool; }
#endif
#endif
};
FABLE_STATIC_ASSERT_AT(102, sizeof(CEngine) == 0x178);
FABLE_STATIC_ASSERT_AT(103, offsetof(CEngine, EnableWireFrame) == 0x10);
FABLE_STATIC_ASSERT_AT(104, offsetof(CEngine, PreloadedCameraPos) == 0x68);
FABLE_STATIC_ASSERT_AT(105, offsetof(CEngine, NonInterpolatedWorldTime) == 0x98);
FABLE_STATIC_ASSERT_AT(106, offsetof(CEngine, VideoOptions) == 0xc0);
FABLE_STATIC_ASSERT_AT(107, offsetof(CEngine, OffWorldTickProcessInfo) == 0xf8);
FABLE_STATIC_ASSERT_AT(108, offsetof(CEngine, PEngineDef) == 0x11c);
FABLE_STATIC_ASSERT_AT(109, offsetof(CEngine, AnisotropicFilteringLevel) == 0x158);
FABLE_STATIC_ASSERT_AT(110, offsetof(CEngine, EngineComponents) == 0x168);
#ifdef FABLE_RETAIL_CENGINE_TYPED_MEMBERS
FABLE_STATIC_ASSERT_AT(330, sizeof(CEngine::CPreloadStats) == 0x20);
FABLE_STATIC_ASSERT_AT(331, offsetof(CEngine, PreloadCameras) == 0x3c);
FABLE_STATIC_ASSERT_AT(332, offsetof(CEngine, PreloadStats) == 0x48);
FABLE_STATIC_ASSERT_AT(333, sizeof(CEngineVideoOptions) == 0x34);
FABLE_STATIC_ASSERT_AT(334, offsetof(CEngine, FrameMemoryPool) == 0x130);
FABLE_STATIC_ASSERT_AT(335, offsetof(CEngine, ScratchZBuffersD24S8) == 0x14c);
FABLE_STATIC_ASSERT_AT(336, offsetof(CEngine, RenderLayers) == 0x15c);
#endif
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngine = 0x188.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngine = 0x178.
// Retail evidence: 2 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 1 agreeing; PINNED by 8 retyped file(s) despite conflicts.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCamera;
class CDefPointeeBase;
#ifdef FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF
class CEngineVideoOptionsDef;
enum EEngineVideoOptionQuality;                                          // MS forward enum (no enumerator name recovered)
#endif

#pragma pack(push, 1)
class CEngine {
public:
#ifdef FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF
    // Opt-in (wfE7, row 0x00B3CB30): Ego_r.pdb / FableWin.pdb ?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z,
    // out of line in retail 0x00B244F0 (returns *PEngineVideoOptionsDefs[quality]; the landed row at that address is misnamed
    // CTextureManager::GetPoolTextureSize -- NAME_FIXES.tsv). Declaration only.
    const CEngineVideoOptionsDef& PeekVideoOptionsDef(EEngineVideoOptionQuality quality);   // 0x00B244F0
#endif
#ifdef FABLE_RETAIL_CENGINE_SCRATCHZBUFFER
    // Opt-in (row 0x00B8B780): Ego_r.pdb ?GetScratchZBufferD24S8@CEngine@@QAEPAUCSurfaceNative@@K@Z (0x00B27EE0). Declaration only.
    struct CSurfaceNative* GetScratchZBufferD24S8(unsigned long size);   // 0x00B27EE0
#endif
#ifdef FABLE_RETAIL_CENGINE_SCRATCHZBUFFER_PDB
    // Opt-in (agentNF36 2026-11-16, row 0x00B8B780): Ego_r.pdb 0x009B0790 + FableWin.pdb 0x02CA14A0
    // ?GetScratchZBufferD24S8@CEngine@@QAEAAVCSurface@@J@Z (0x00B27EE0): returns CSurface&, takes long. Declaration only.
    class CSurface& GetScratchZBufferD24S8(long size);   // 0x00B27EE0
#endif
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    f;                                      // +0x08 retail-only unsigned char (previous header)
    unsigned char    _pad_0x09[0x3];                         // +0x09
    unsigned char    _pad_0x0c[0x4];                         // +0x0c
    bool             EnableWireFrame;                        // +0x10
    bool             EnableEngine;                           // +0x11
    bool             EnableLocalLights;                      // +0x12
    bool             EnableTextures;                         // +0x13
    CCamera*         CurrentCamera;                          // +0x14
    bool             NewSceneFlag;                           // +0x18
    unsigned char    _pad_0x19[0x3];                         // +0x19
    unsigned long    NewSceneLoadFlags;                      // +0x1c
    float            NewSceneTimeOut;                        // +0x20
    bool             CutscenePreloadFlag;                    // +0x24
    unsigned char    _pad_0x25[0x3];                         // +0x25
    float            CutscenePreloadTimeout;                 // +0x28
    float            CutsceneFastPreloadTimeout;             // +0x2c
    float            ActorAdditionalLoadTimeout;             // +0x30
    float            ActorRadius;                            // +0x34
    bool             ActorsLoading;                          // +0x38
    bool             CameraAngleChanged;                     // +0x39
    unsigned char    _pad_0x3a[0x2];                         // +0x3a
    unsigned char    PreloadCameras[0xc];                    // +0x3c CArray<CCamera>
    unsigned char    PreloadStats[0x20];                     // +0x48 CPreloadStats
    float            PreloadedCameraPos_X;                   // +0x68 C3DVector::X
    float            PreloadedCameraPos_Y;                   // +0x6c C3DVector::Y
    float            PreloadedCameraPos_Z;                   // +0x70 C3DVector::Z
    float            PreloadedLookVector_X;                  // +0x74 C3DVector::X
    float            PreloadedLookVector_Y;                  // +0x78 C3DVector::Y
    float            PreloadedLookVector_Z;                  // +0x7c C3DVector::Z
    float            StartNewSceneTransitionTime;            // +0x80
    bool             IsInitialised;                          // +0x84
    unsigned char    _pad_0x85[0x3];                         // +0x85
    long             CurrentRenderFrame;                     // +0x88
    long             CurrentWorldFrame;                      // +0x8c
    long             CurrentPausedWorldFrame;                // +0x90
    unsigned char    _pad_0x94[0x4];                         // +0x94
    double           NonInterpolatedWorldTime;               // +0x98
    double           NonInterpolatedPausedWorldTime;         // +0xa0
    double           InterpolatedWorldTime;                  // +0xa8
    unsigned long    WorldFramesPerSecond;                   // +0xb0
    bool             BlockOnLoad;                            // +0xb4
    unsigned char    _pad_0xb5[0x3];                         // +0xb5
    unsigned long    SceneFilterFlags;                       // +0xb8
    bool             EnableScreenShotMode;                   // +0xbc
    bool             EnablePostDeviceReset;                  // +0xbd
    unsigned char    _pad_0xbe[0x2];                         // +0xbe
    unsigned char    VideoOptions[0x38];                     // +0xc0 CEngineVideoOptions
    unsigned char    OffWorldTickProcessInfo[0x20];          // +0xf8 <unnamed-type-OffWorldTickProcessInfo>
    CDefPointeeBase* PEnvironmentDef_Object;                 // +0x118 CDefPointer<CDefClassBase_const_>::Object
    CDefPointeeBase* PEngineDef_Object;                      // +0x11c CDefPointer<CDefClassBase_const_>::Object
    unsigned char    PEngineVideoOptionsDefs[0x10];          // +0x120 CDefPointer<CDefClassBase_const_>[4]
    unsigned char    FrameMemoryPool[0x1c];                  // +0x130 CSimpleMemoryPool
    unsigned char    ScratchZBuffersD24S8[0xc];              // +0x14c CArray<CSurface_*>
    long             AnisotropicFilteringLevel;              // +0x158
    unsigned char    RenderLayers[0xc];                      // +0x15c CArray<CEngineLayerRenderer_*>
    unsigned char    EngineComponents[0xc];                  // +0x168 CArray<CEngineComponent_*>
    unsigned char    _pad_0x174[0x4];                        // +0x174
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(189, sizeof(CEngine) == 0x178);
FABLE_STATIC_ASSERT_AT(190, offsetof(CEngine, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(191, offsetof(CEngine, f) == 0x8);
FABLE_STATIC_ASSERT_AT(192, offsetof(CEngine, EnableWireFrame) == 0x10);
FABLE_STATIC_ASSERT_AT(193, offsetof(CEngine, EnableEngine) == 0x11);
FABLE_STATIC_ASSERT_AT(194, offsetof(CEngine, EnableLocalLights) == 0x12);
FABLE_STATIC_ASSERT_AT(195, offsetof(CEngine, EnableTextures) == 0x13);
FABLE_STATIC_ASSERT_AT(196, offsetof(CEngine, CurrentCamera) == 0x14);
FABLE_STATIC_ASSERT_AT(197, offsetof(CEngine, NewSceneFlag) == 0x18);
FABLE_STATIC_ASSERT_AT(198, offsetof(CEngine, NewSceneLoadFlags) == 0x1c);
FABLE_STATIC_ASSERT_AT(199, offsetof(CEngine, NewSceneTimeOut) == 0x20);
FABLE_STATIC_ASSERT_AT(200, offsetof(CEngine, CutscenePreloadFlag) == 0x24);
FABLE_STATIC_ASSERT_AT(201, offsetof(CEngine, CutscenePreloadTimeout) == 0x28);
FABLE_STATIC_ASSERT_AT(202, offsetof(CEngine, CutsceneFastPreloadTimeout) == 0x2c);
FABLE_STATIC_ASSERT_AT(203, offsetof(CEngine, ActorAdditionalLoadTimeout) == 0x30);
FABLE_STATIC_ASSERT_AT(204, offsetof(CEngine, ActorRadius) == 0x34);
FABLE_STATIC_ASSERT_AT(205, offsetof(CEngine, ActorsLoading) == 0x38);
FABLE_STATIC_ASSERT_AT(206, offsetof(CEngine, CameraAngleChanged) == 0x39);
FABLE_STATIC_ASSERT_AT(207, offsetof(CEngine, PreloadCameras) == 0x3c);
FABLE_STATIC_ASSERT_AT(208, offsetof(CEngine, PreloadStats) == 0x48);
FABLE_STATIC_ASSERT_AT(209, offsetof(CEngine, PreloadedCameraPos_X) == 0x68);
FABLE_STATIC_ASSERT_AT(210, offsetof(CEngine, PreloadedCameraPos_Y) == 0x6c);
FABLE_STATIC_ASSERT_AT(211, offsetof(CEngine, PreloadedCameraPos_Z) == 0x70);
FABLE_STATIC_ASSERT_AT(212, offsetof(CEngine, PreloadedLookVector_X) == 0x74);
FABLE_STATIC_ASSERT_AT(213, offsetof(CEngine, PreloadedLookVector_Y) == 0x78);
FABLE_STATIC_ASSERT_AT(214, offsetof(CEngine, PreloadedLookVector_Z) == 0x7c);
FABLE_STATIC_ASSERT_AT(215, offsetof(CEngine, StartNewSceneTransitionTime) == 0x80);
FABLE_STATIC_ASSERT_AT(216, offsetof(CEngine, IsInitialised) == 0x84);
FABLE_STATIC_ASSERT_AT(217, offsetof(CEngine, CurrentRenderFrame) == 0x88);
FABLE_STATIC_ASSERT_AT(218, offsetof(CEngine, CurrentWorldFrame) == 0x8c);
FABLE_STATIC_ASSERT_AT(219, offsetof(CEngine, CurrentPausedWorldFrame) == 0x90);
FABLE_STATIC_ASSERT_AT(220, offsetof(CEngine, NonInterpolatedWorldTime) == 0x98);
FABLE_STATIC_ASSERT_AT(221, offsetof(CEngine, NonInterpolatedPausedWorldTime) == 0xa0);
FABLE_STATIC_ASSERT_AT(222, offsetof(CEngine, InterpolatedWorldTime) == 0xa8);
FABLE_STATIC_ASSERT_AT(223, offsetof(CEngine, WorldFramesPerSecond) == 0xb0);
FABLE_STATIC_ASSERT_AT(224, offsetof(CEngine, BlockOnLoad) == 0xb4);
FABLE_STATIC_ASSERT_AT(225, offsetof(CEngine, SceneFilterFlags) == 0xb8);
FABLE_STATIC_ASSERT_AT(226, offsetof(CEngine, EnableScreenShotMode) == 0xbc);
FABLE_STATIC_ASSERT_AT(227, offsetof(CEngine, EnablePostDeviceReset) == 0xbd);
FABLE_STATIC_ASSERT_AT(228, offsetof(CEngine, VideoOptions) == 0xc0);
FABLE_STATIC_ASSERT_AT(229, offsetof(CEngine, OffWorldTickProcessInfo) == 0xf8);
FABLE_STATIC_ASSERT_AT(230, offsetof(CEngine, PEnvironmentDef_Object) == 0x118);
FABLE_STATIC_ASSERT_AT(231, offsetof(CEngine, PEngineDef_Object) == 0x11c);
FABLE_STATIC_ASSERT_AT(232, offsetof(CEngine, PEngineVideoOptionsDefs) == 0x120);
FABLE_STATIC_ASSERT_AT(233, offsetof(CEngine, FrameMemoryPool) == 0x130);
FABLE_STATIC_ASSERT_AT(234, offsetof(CEngine, ScratchZBuffersD24S8) == 0x14c);
FABLE_STATIC_ASSERT_AT(235, offsetof(CEngine, AnisotropicFilteringLevel) == 0x158);
FABLE_STATIC_ASSERT_AT(236, offsetof(CEngine, RenderLayers) == 0x15c);
FABLE_STATIC_ASSERT_AT(237, offsetof(CEngine, EngineComponents) == 0x168);

#endif // FABLE_RETAIL_CENGINE
#endif // FABLE_ENGINE_CENGINE_H
