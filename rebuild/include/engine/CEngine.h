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
class CEngine : public CIEngine, public CDeviceResetCallback {
public:
    virtual bool Initialise(const CEngineInit& init);                    // slot 2 0x00B26360 ?Initialise@CEngine@@UAE_NABVCEngineInit@@@Z
    virtual void OnPreDeviceReset();                                     // 0x00B26010 ?OnPreDeviceReset@CEngine@@UAEXXZ
    virtual bool OnPostDeviceReset();                                    // 0x00B25D00 ?OnPostDeviceReset@CEngine@@UAE_NXZ
    virtual void Uninitialise();                                         // slot 85 0x00B25D80 ?Uninitialise@CEngine@@UAEXXZ (CEngine's first own virtual)
#ifdef FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
    // Nested opt-in (chunk 763): the slot-13 override, FableWin.pdb ?ConvertWorldPosToScreenPos@CEngine@@UBE_NABVC3DVector@@AAVC2DVector@@@Z 0x00B242D0.
    virtual bool ConvertWorldPosToScreenPos(const C3DVector& worldPos, C2DVector& screenPos) const;
#endif
#ifdef FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
    // Nested opt-in (chunk 758): the slot-12 override, FableWin.pdb ?ConvertScreenPosToWorldPosAtViewDistance@CEngine@@UBE_NMABVC2DVector@@AAVC3DVector@@@Z 0x00B24400.
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos) const;
#endif
private:
    void SetDefaultRenderStates();                                       // 0x00B24850 ?SetDefaultRenderStates@CEngine@@AAEXXZ
    // ??$AddEngineComponent@V<T>@@@CEngine@@AAEXPAV<T>@@AAPAV1@E@Z (Ego_r.pdb; out of line in retail, one COMDAT per component:
    // 0x00B29930 .. 0x00B2A980): publish the component through its global pointer and append it to EngineComponents.
    template <class T> void AddEngineComponent(T* component, T*& global_pointer, unsigned char flag);
    CEngineLayerRenderer& AddRenderLayer(CEngineLayerRenderer* layer);   // 0x00B262C0 ?AddRenderLayer@CEngine@@AAEAAVCEngineLayerRenderer@@PAV2@@Z
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
    unsigned char    PreloadCameras[0xc];                                // +0x3c CArray<CCamera>
    unsigned char    PreloadStats[0x20];                                 // +0x48 CPreloadStats
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
    unsigned char    VideoOptions[0x34];                                 // +0xc0 CEngineVideoOptions
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
    unsigned char    FrameMemoryPool[0x1c];                              // +0x130 CSimpleMemoryPool
    unsigned char    ScratchZBuffersD24S8[0xc];                          // +0x14c CArray<CSurface*>
    long             AnisotropicFilteringLevel;                          // +0x158
    unsigned char    RenderLayers[0xc];                                  // +0x15c CArray<CEngineLayerRenderer*>
    unsigned char    EngineComponents[0xc];                              // +0x168 CArray<CEngineComponent*>
};
FABLE_STATIC_ASSERT(sizeof(CEngine) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnableWireFrame) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedCameraPos) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngine, NonInterpolatedWorldTime) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CEngine, VideoOptions) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CEngine, OffWorldTickProcessInfo) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CEngine, PEngineDef) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CEngine, AnisotropicFilteringLevel) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CEngine, EngineComponents) == 0x168);
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

#pragma pack(push, 1)
class CEngine {
public:
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

FABLE_STATIC_ASSERT(sizeof(CEngine) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEngine, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngine, f) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnableWireFrame) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnableEngine) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnableLocalLights) == 0x12);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnableTextures) == 0x13);
FABLE_STATIC_ASSERT(offsetof(CEngine, CurrentCamera) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngine, NewSceneFlag) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngine, NewSceneLoadFlags) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngine, NewSceneTimeOut) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngine, CutscenePreloadFlag) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngine, CutscenePreloadTimeout) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngine, CutsceneFastPreloadTimeout) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngine, ActorAdditionalLoadTimeout) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngine, ActorRadius) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngine, ActorsLoading) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngine, CameraAngleChanged) == 0x39);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadCameras) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadStats) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedCameraPos_X) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedCameraPos_Y) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedCameraPos_Z) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedLookVector_X) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedLookVector_Y) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CEngine, PreloadedLookVector_Z) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CEngine, StartNewSceneTransitionTime) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CEngine, IsInitialised) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CEngine, CurrentRenderFrame) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CEngine, CurrentWorldFrame) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CEngine, CurrentPausedWorldFrame) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CEngine, NonInterpolatedWorldTime) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CEngine, NonInterpolatedPausedWorldTime) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CEngine, InterpolatedWorldTime) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CEngine, WorldFramesPerSecond) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CEngine, BlockOnLoad) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CEngine, SceneFilterFlags) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnableScreenShotMode) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CEngine, EnablePostDeviceReset) == 0xbd);
FABLE_STATIC_ASSERT(offsetof(CEngine, VideoOptions) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CEngine, OffWorldTickProcessInfo) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CEngine, PEnvironmentDef_Object) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CEngine, PEngineDef_Object) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CEngine, PEngineVideoOptionsDefs) == 0x120);
FABLE_STATIC_ASSERT(offsetof(CEngine, FrameMemoryPool) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CEngine, ScratchZBuffersD24S8) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CEngine, AnisotropicFilteringLevel) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CEngine, RenderLayers) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CEngine, EngineComponents) == 0x168);

#endif // FABLE_RETAIL_CENGINE
#endif // FABLE_ENGINE_CENGINE_H
