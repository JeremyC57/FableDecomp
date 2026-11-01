// CEngine::~CEngine() -- engine destructor: Uninitialise(), then the members in reverse order (the three component/layer/Z-buffer CArrays freed, FrameMemoryPool, the four video-option def pointers, PEngineDef, PEnvironmentDef, PreloadCameras), then ~CIEngine inline and ~CInitBaseClass
// Leaf: ??1CEngine@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B261D0, 238 bytes, thiscall, tail jump to ~CInitBaseClass 0x0099A410.
// Identity (proven): stores vtable 0x012A0F3C first (rtti-vtables.tsv / COL: .?AVCEngine@@ : CIEngine : CInitBaseClass : CBaseClass,
//   CDeviceResetCallback at +0x0C, whose vtable 0x012A0F30 is the second store); the sole caller 0x00B2AB13 is the scalar deleting
//   destructor 0x00B2AB10, slot 0 of 0x012A0F3C. Ego_r.pdb / FableWin.pdb ??1CEngine@@UAE@XZ. The catalog label is right; the landed
//   ??_G 0x00B2AB10 is a `CEngine::ScalarDeletingDestructor` / `Dtor()` stand-in.
// Shape: a user destructor (it stores both vtables) whose body is `Uninitialise();` -- ?Uninitialise@CEngine@@UAEXXZ 0x00B25D80
//   (CEngine vtable slot 85), called directly as VC7.1 does for a virtual called in a destructor. The CIEngine vtable 0x012A0D24
//   store before the tail jump is ~CIEngine's inline user body (its ??_G 0x00B23770 is the same store + ~CInitBaseClass call);
//   ~CSimpleMemoryPool is inline too (??_G 0x00987F30: vtable 0x0129A064, ClearMemoryBlocks, ~CBaseClassNonCopyable).
// Layout: Ego_r.pdb CEngine members at retail offsets (STLport CArray 12 vs Dinkumware 16 bytes; the double-aligned CPreloadStats
//   absorbs the first 4): PreloadCameras +0x3C (CArray<CCamera>, CCamera trivially destroyed), PreloadStats +0x48,
//   PEnvironmentDef +0x118, PEngineDef +0x11C, PEngineVideoOptionsDefs[4] +0x120 (CDefPointer<const CDefClassBase>),
//   FrameMemoryPool +0x130 (CSimpleMemoryPool, 0x1C), ScratchZBuffersD24S8 +0x14C, AnisotropicFilteringLevel +0x158,
//   RenderLayers +0x15C, EngineComponents +0x168; sizeof 0x178 as engine/CEngine.h states.
// Layout: engine/CEngine.h, CIEngine.h and CSimpleMemoryPool.h are flattened records (vptr as data, no bases), so the polymorphic
//   chain is a local model with the PDB member names (NOTES.md "Header debt"); CDefPointer / CDefClassBase are the engine headers.
// Callees: CEngine::Uninitialise 0x00B25D80, free 0x00BFEA14 (STLport allocator<T>::deallocate), CSimpleMemoryPool::ClearMemoryBlocks
//   0x00A5C600 (private: Ego_r / FableWin ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ), ~CBaseClassNonCopyable 0x0099A300,
//   CDefPointer<T>::Release inline (engine/CDefPointer.h: dec [Object+4], slot 1 Delete), ~CInitBaseClass 0x0099A410.
#include "engine/CBaseClass.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: the 12-byte STLport vector
};

class C3DVector { public: float X, Y, Z; };
class C2DExtentsI { public: long X, Y; };
class C2DExtentsF { public: float X, Y; };
class CRightHandedSet { public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};
class CCamera { public:                                     // 0x38, trivially destroyed (no ??1CCamera in either PDB)
    C3DVector       WorldPosition;                          // +0x00
    CRightHandedSet RHSet;                                  // +0x0C
    bool            HeightLocked;                           // +0x24
    bool            ViewVecZLocked;                         // +0x25
    unsigned long   FOVFlags;                               // +0x28
    float           HorizontalFOV;                          // +0x2C
    float           VerticalFOV;                            // +0x30
    float           Zoom;                                   // +0x34
};
enum EEngineVideoOptionQuality { };
enum ESurfaceMultisampleType { };
class CEngineVideoOptions { public:
    C2DExtentsI               ScreenResolution;             // +0x00
    C2DExtentsF               ScreenAspectRatio;            // +0x08
    long                      BitsPerPixel;                 // +0x10
    long                      RefreshRate;                  // +0x14
    bool                      VerticalSync;                 // +0x18
    EEngineVideoOptionQuality LandscapeDetail;              // +0x1C
    EEngineVideoOptionQuality MeshDetail;                   // +0x20
    EEngineVideoOptionQuality TextureDetail;                // +0x24
    EEngineVideoOptionQuality ShadowQuality;                // +0x28
    EEngineVideoOptionQuality EffectsQuality;               // +0x2C
    ESurfaceMultisampleType   Antialiasing;                 // +0x30
};

class CInitBaseClass : public CBaseClass { public:          // RTTI .?AVCInitBaseClass@@ : CBaseClass
    CInitBaseClass();
    virtual ~CInitBaseClass();                              // 0x0099A410
    bool Valid;                                             // +0x04
};
class CIEngine : public CInitBaseClass { public:            // vtable 0x012A0D24
    virtual ~CIEngine() { }
    bool Active;                                            // +0x08
protected:
    CIEngine();
};
class CDeviceResetCallback { public:                        // no RTTI-visible dtor; two virtuals
    CDeviceResetCallback();
protected:
    virtual void OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
enum EAreaStatus { };
struct CArea {
    unsigned long Size;                                     // +0x00
    CArea*        NextFreeArea;                             // +0x04
    EAreaStatus   Status;                                   // +0x08
};
class CSimpleMemoryPool : public CBaseClassNonCopyable { public:   // vtable 0x0129A064
    class CMemoryBlock;
    CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity);
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
private:
    void ClearMemoryBlocks();                               // 0x00A5C600
public:
    CMemoryBlock* FirstMemoryBlock;                         // +0x04
    unsigned long Capacity;                                 // +0x08
    unsigned long GrowthGranularity;                        // +0x0C
    CArea         Root;                                     // +0x10
};
class CSurface;
class CEngineLayerRenderer;
class CEngineComponent;
class CEngine : public CIEngine, public CDeviceResetCallback {   // vtable 0x012A0F3C, CDeviceResetCallback vtable 0x012A0F30 at +0x0C
public:
    class CPreloadStats { public:
        CPreloadStats();
        double LastFullPreloadTime;                         // +0x00
        bool   LastFullPreloadTimedOut;                     // +0x08
        double LastFastPreloadTime;                         // +0x10
        bool   LastFastPreloadTimedOut;                     // +0x18
    };
    CEngine();
    virtual ~CEngine();
    virtual void Uninitialise();                            // 0x00B25D80
protected:
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
public:
    bool          EnableWireFrame;                          // +0x10
    bool          EnableEngine;                             // +0x11
    bool          EnableLocalLights;                        // +0x12
    bool          EnableTextures;                           // +0x13
    CCamera*      CurrentCamera;                            // +0x14
    bool          NewSceneFlag;                             // +0x18
    unsigned long NewSceneLoadFlags;                        // +0x1C
    float         NewSceneTimeOut;                          // +0x20
    bool          CutscenePreloadFlag;                      // +0x24
    float         CutscenePreloadTimeout;                   // +0x28
    float         CutsceneFastPreloadTimeout;               // +0x2C
    float         ActorAdditionalLoadTimeout;               // +0x30
    float         ActorRadius;                              // +0x34
    bool          ActorsLoading;                            // +0x38
    bool          CameraAngleChanged;                       // +0x39
    CArray<CCamera> PreloadCameras;                         // +0x3C
    CPreloadStats PreloadStats;                             // +0x48
    C3DVector     PreloadedCameraPos;                       // +0x68
    C3DVector     PreloadedLookVector;                      // +0x74
    float         StartNewSceneTransitionTime;              // +0x80
    bool          IsInitialised;                            // +0x84
    long          CurrentRenderFrame;                       // +0x88
    long          CurrentWorldFrame;                        // +0x8C
    long          CurrentPausedWorldFrame;                  // +0x90
    double        NonInterpolatedWorldTime;                 // +0x98
    double        NonInterpolatedPausedWorldTime;           // +0xA0
    double        InterpolatedWorldTime;                    // +0xA8
    unsigned long WorldFramesPerSecond;                     // +0xB0
    bool          BlockOnLoad;                              // +0xB4
    unsigned long SceneFilterFlags;                         // +0xB8
    bool          EnableScreenShotMode;                     // +0xBC
    bool          EnablePostDeviceReset;                    // +0xBD
    CEngineVideoOptions VideoOptions;                       // +0xC0
    struct {
        bool   ProcessThisFrame;                            // +0x00
        bool   ProcessingEnabled;                           // +0x01
        float  ProcessTicksBehindAccumilator;               // +0x04
        float  MaxProcessTicksBehind;                       // +0x08
        float  MaxProcessTicksBehindBeforeCatchup;          // +0x0C
        float  MinProcessFrequency;                         // +0x10
        double LastFrameTimeStamp;                          // +0x18
    } OffWorldTickProcessInfo;                              // +0xF8
    CDefPointer<const CDefClassBase> PEnvironmentDef;       // +0x118
    CDefPointer<const CDefClassBase> PEngineDef;            // +0x11C
    CDefPointer<const CDefClassBase> PEngineVideoOptionsDefs[4];   // +0x120
    CSimpleMemoryPool FrameMemoryPool;                      // +0x130
    CArray<CSurface*> ScratchZBuffersD24S8;                 // +0x14C
    long          AnisotropicFilteringLevel;                // +0x158
    CArray<CEngineLayerRenderer*> RenderLayers;             // +0x15C
    CArray<CEngineComponent*> EngineComponents;             // +0x168
};
#include <stddef.h>
FABLE_STATIC_ASSERT(offsetof(CEngine, PEnvironmentDef) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CEngine, EngineComponents) == 0x168);
FABLE_STATIC_ASSERT(sizeof(CEngine) == 0x178);

CEngine::~CEngine()
{
    Uninitialise();
}