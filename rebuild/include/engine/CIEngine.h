#ifndef FABLE_ENGINE_CIENGINE_H
#define FABLE_ENGINE_CIENGINE_H
#ifdef FABLE_RETAIL_CIENGINE
// Retail opt-in (chunk 725): the polymorphic engine interface. RTTI .?AVCIEngine@@ : CInitBaseClass : CBaseClass, vtable
// 0x012A0D24 (implementation CEngine 0x012A0F3C). Slots 0-16 below; the rest are not modelled yet. Names: FableWin.pdb
// CIEngine / CEngine members, placed by the retail vtables (CIEngine's own bodies 0x00B232D0.. and CEngine's overrides,
// catalog-named) and their `ret N`: 2 Initialise 0x00B232D0, 3 Release 0x00B232E0, 4 SetCamera (CEngine 0x00B23B50,
// ret 4), 5 SetWindow (CEngine 0x00B23B80, ret 4; FableWin declares it right after SetCamera), 6
// SetEnvironmentalEffectsAlwaysUpdate 0x00B232F0, 7 SetWorld 0x00B23300, 9 WorldFrameUpdate 0x00B23310, 10
// PausedWorldFrameUpdate 0x00B23320, 14 SetVideoOptions 0x00B23330, 15 PreChangeResolution / 16 PostChangeResolution
// (_purecall here; CEngine 0x00B2AA20 / 0x00B2AA30 = FableWin ?PreChangeResolution@CEngine@@UAE_NXZ /
// ?PostChangeResolution@CEngine@@UAE_NKKK@Z; called by CDisplayEngine 0x00434A10 / 0x00434A20). Slots 8, 11, 12, 13 are
// pure here and their CEngine overrides are unnamed (ret 8 / ret 8 / ret 0xC / ret 8): descriptive `VirtualNN_<CEngine body>`.
// Chunk 731 added slots 17-84 (below) so that CEngine's own first virtual, Uninitialise, is slot 85 (CEngine::Initialise
// 0x00B26360 calls it as `call [eax+154h]`).
// Layout: CInitBaseClass (vptr, Valid +4), Active +8; sizeof 0xC (Ego_r).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CInitBaseClass.h"
class C2DBoxF;
class C2DVector;
class C3DVector;
class CCamera;
class CEngineInit;
class CEngineVideoOptions;
class CIDrawWorld;
class C2DBoxI; class C2DExtentsI; class C3DAnimationInfo; class C3DBoundingBox; class C3DBoundingSphere; class C4DVector;
class CCharString; class CEngineColourFilterSettings; class CEnginePrimitive; class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo; class CEnginePrimitiveHandle; class CEnginePrimitivePosition; class CEngineTextureMorph;
class CGPUScratchBufferTextureManager; class CMovableResourceMemoryPool; class CPixelFormat; class CRGBColour; class CRGBFloatColour;
class CStaticMapGenerationVisibiilityInfo; class CTexture; class CWideString;
template <class T> class CArray;
enum EEnginePrimitiveStatIndex;
namespace std {                                                          // STLport 4.6 (retail std): declarations for the slot signatures
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare, class _Alloc> class map;
template <class _Key, class _Compare, class _Alloc> class set;
}
class CIEngine : public CInitBaseClass {
public:
    virtual ~CIEngine();                                                   //  0 ??_G 0x00B23770
    // slot 1: CInitBaseClass::Validate 0x0099A340 (not declared by CInitBaseClass.h; modelled here at its slot)
    virtual void Validate() const;                                        //  1
    virtual bool Initialise(const CEngineInit&);                          //  2 0x00B232D0
    virtual void Release();                                               //  3 0x00B232E0
    virtual void SetCamera(const CCamera*) = 0;                           //  4
    virtual void SetWindow(const C2DBoxF*) = 0;                           //  5
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool);               //  6 0x00B232F0
    virtual void SetWorld(const CIDrawWorld*);                            //  7 0x00B23300
    virtual void Virtual08_00b27d90() = 0;                                //  8 (CEngine 0x00B27D90, ret 8)
    virtual void WorldFrameUpdate(bool);                                  //  9 0x00B23310
    virtual void PausedWorldFrameUpdate();                                // 10 0x00B23320
    virtual void Virtual11_00b23cf0() = 0;                                // 11 (CEngine 0x00B23CF0, ret 8)
#ifdef FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
    // Nested opt-in (chunk 758, row 0x00B24400): slot 12 is FableWin.pdb ?ConvertScreenPosToWorldPosAtViewDistance@CEngine@@UBE_NMABVC2DVector@@AAVC3DVector@@@Z
    // (labels_rtti_port.tsv; CEngine vtable 0x012A0F3C slot 12, `ret 0Ch`); pure here like the other unnamed slots.
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const = 0;   // 12 (CEngine 0x00B24400)
#else
    virtual void Virtual12_00b24400() = 0;                                // 12 (CEngine 0x00B24400, ret 0xC)
#endif
#ifdef FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
    // Nested opt-in (chunk 763, row 0x00B242D0): slot 13 is FableWin.pdb ?ConvertWorldPosToScreenPos@CEngine@@UBE_NABVC3DVector@@AAVC2DVector@@@Z
    // (CEngine vtable 0x012A0F3C slot 13 = 0x00B242D0, `ret 8`); pure here like the other unnamed slots.
    virtual bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const = 0;   // 13 (CEngine 0x00B242D0)
#else
    virtual void Virtual13_00b242d0() = 0;                                // 13 (CEngine 0x00B242D0, ret 8)
#endif
    virtual bool SetVideoOptions(const CEngineVideoOptions&);             // 14 0x00B23330
    virtual bool PreChangeResolution() = 0;                               // 15 (CEngine 0x00B2AA20)
    virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) = 0;   // 16 (CEngine 0x00B2AA30)
    // Slots 17-84 (chunk 731): Ego_r.pdb / FableWin.pdb ?<Name>@CIEngine@@ spellings in retail slot order (CIEngine's own bodies
    // 0x00B23340..0x00B28B60, CEngine's overrides as catalogued); slot 23 is pure here (CEngine 0x00B23BC0, ?SetPersistentPrimitive@CEngine@@).
    // The SetChildPrimitive pair is declared four-argument first: VC7.1 lays an overloaded virtual pair out in reverse declaration
    // order, and slot 27 is the five-argument form (CEngine 0x00B23C30), slot 28 the four-argument one (0x00B23C50).
    virtual void Refresh();                                               // 17 0x00B23340
    virtual bool CheckPrimitiveVisibility(unsigned __int64);              // 18 0x00B23350
    virtual float GetPrimitiveDrawDistanceFactor() const;                 // 19
    virtual void AddPrimitive(const CEnginePrimitive&);                   // 20
    virtual bool ClipSphere(const C3DBoundingSphere&) const;              // 21
    virtual bool ClipBox(const C3DBoundingBox&) const;                    // 22
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) = 0;   // 23
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&);   // 24
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&);     // 25
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);   // 26
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long);   // 28
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);   // 27
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&);   // 29
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long);   // 30
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&);        // 31
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&,
                                      const CEnginePrimitiveHandle&, long, unsigned long);   // 32
    virtual void CollectPrimitiveStatsForArea(std::map<unsigned long, unsigned long, std::less<unsigned long>,
                                              std::allocator<std::pair<const unsigned long, unsigned long> > >&,
                                              const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long) const;   // 33
    virtual void ProcessPrimitiveStats(const std::map<unsigned long, unsigned long, std::less<unsigned long>,
                                                      std::allocator<std::pair<const unsigned long, unsigned long> > >&,
                                       EEnginePrimitiveStatIndex, unsigned long&, CArray<CCharString>&) const;   // 34
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&) const;   // 35
    virtual unsigned long AddCustomMorphTexture(CTexture&, const CCharString&) const;   // 36
    virtual long GetStaticMapUpdatePassCount();                           // 37
    virtual void StartStaticMapUpdatePass(const CWideString&, long, const CStaticMapGenerationVisibiilityInfo&, const CArray<CCharString>&);   // 38
    virtual bool IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const;   // 39
    virtual void UpdateStaticMap(long, const CCharString&, unsigned long, long);   // 40
    virtual void CloseStaticMapFile();                                    // 41
    virtual void AddMap(long, long);                                      // 42
    virtual void RemoveMap(long);                                         // 43
    virtual void SetMapAsLoaded(long);                                    // 44
    virtual void SetMapAsUnloaded(long);                                  // 45
    virtual void SetMapVisibilityAllMaps(bool);                           // 46
    virtual void SetMapVisibility(long, bool);                            // 47
    virtual void AreaChanged(const C2DBoxI&);                             // 48
    virtual void OnActivate();                                            // 49
    virtual void OnDeactivate();                                          // 50 0x00B23540
    virtual bool IsActive() const;                                        // 51 0x00B23550
    virtual void SetStaticMapFileForUse(const CWideString&);              // 52
    virtual void SetLandscapeDebugTintColour(long, CRGBColour);           // 53
    virtual void ReloadLocalDetailDefs();                                 // 54
    virtual void StartNewScene(unsigned long, float);                     // 55
    virtual void CutscenePreload(bool, bool);                             // 56
    virtual void CutsceneAddCameraAngle(const CCamera&);                  // 57
    virtual void ChangeCameraAngle();                                     // 58
    virtual bool IsPreloaded() const;                                     // 59
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&, bool, float,
                                         const std::set<long, std::less<long>, std::allocator<long> >*);   // 60
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings&);   // 61
    virtual void SetSpotlightShadowMode(bool);                            // 62
    virtual void SetSeaBodyToRender(CCharString);                         // 63
    virtual void UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&);   // 64
    virtual void EnableDebugOverlay(long, bool, bool);                    // 65
    virtual void SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&);   // 66
    virtual void Set2DPrimitiveLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&);   // 67
    virtual void SetRenderTargetTexture(const CTexture*);                 // 68
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&);   // 69
    virtual void FreeTextureFromEngineResourcePools(CTexture*);           // 70
    virtual CMovableResourceMemoryPool* GetLoadingMemoryPool();           // 71
    virtual CMovableResourceMemoryPool* GetAnimationMemoryPool();         // 72
    virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager();   // 73
    virtual bool DeallocateShadowBuffer();                                // 74
    virtual void ReallocateShadowBuffer();                                // 75
    virtual void Enable3DSceneRendering(bool);                            // 76
    virtual bool Is3DSceneRenderingEnabled();                             // 77
    virtual void Enable2DSceneRendering(bool);                            // 78
    virtual bool Is2DSceneRenderingEnabled();                             // 79
    virtual void OverrideRain(float);                                     // 80
    virtual void OverrideSnow(float);                                     // 81
    virtual void RenderColorFilteredQuad(float, float, const C4DVector&, float, float, CTexture*);   // 82
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const;        // 83 0x00B28B40
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const;        // 84 0x00B28B60
    void Uninitialise();                                                  // ?Uninitialise@CIEngine@@QAEXXZ (non-virtual)
    bool Active;                                                          // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CIEngine) == 0xc);
#else
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

#endif // FABLE_RETAIL_CIENGINE
#endif // FABLE_ENGINE_CIENGINE_H
