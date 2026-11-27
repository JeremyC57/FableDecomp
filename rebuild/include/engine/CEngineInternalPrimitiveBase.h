// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED)
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CEngineSceneGridPos.h"
class CEngineSceneGridCell;
class CEnginePrimitive;
class CEngineCamera;
class CEnginePrimitiveAnimation;
class CEnginePrimitivePosition;
class CEngineMeshEffect;
class CEngineInternalPrimitiveOcclusionBase;
class C3DVector;
class CMatrix3x4;
class C3DBoundingBox;
class C3DBoundingSphere;
template<class T>class CIVCountedPointer;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#include "engine/CEngineInterpolationInfo.h"
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE
#include "engine/GFFastFloatToLong.h"
#endif
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
 CEngineInternalPrimitiveBase(long,unsigned long);
#ifdef FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
 // Original FableWin methods, expanded at 0x00B92950.
 unsigned long GetRenderLayerMask() const { return RenderLayerMask; }
 void SetAsRendered() { LastFrameRendered = CurrentRenderFrame; }
 void RemoveFromList();                                                        // retail 0x00B8FE60
private:
 static unsigned short CurrentRenderFrame;
public:
 void SetGridCell(CEngineSceneGridCell* cell) { GridCell = cell; }
 void AddToList(CEngineInternalPrimitiveBase** list);
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_NEXT_PASS23
 // Both original PDB public const getter; private original field at38.
 CEngineInternalPrimitiveBase* GetNextPrimitive() const {return NextPrimitive;}
#endif
 #ifdef FABLE_RETAIL_PARTICLE_GROUP_DESTRUCTOR_PASS29
    virtual ~CEngineInternalPrimitiveBase() { }
#else
    virtual ~CEngineInternalPrimitiveBase();
#endif
 virtual void Update(const CEnginePrimitive&,unsigned long);
 virtual unsigned long AddChildPrimitive(const CEnginePrimitive&,unsigned long);
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETCHILDPRIMITIVE_ORDER // NF325: retail vftable 0x012A3D7C slot 7 = (K, CEnginePrimitiveHandle&, const CEnginePrimitive&, K), slot 8 = (K, const CEnginePrimitive&, K); VC7.1 lays an overloaded virtual pair out in reverse declaration order, so the three-argument one is declared first (emitted-vftable probe, NF325 evidence/probe_*.txt)
 virtual void SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
#else
 virtual void SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long);
#endif
 virtual void GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle&);
 virtual void ClearChildPrimitive(unsigned long);
 virtual void ClearAllChildPrimitives();
 virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long);
 virtual CEngineFadeDistance GetFadeDistance()const;
 virtual void PostRender();
 virtual bool GetWorldPosition(C3DVector&);
 virtual bool GetBoundingBoxTransform(CMatrix3x4&);
 virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox&);
 virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere&);
 virtual bool GetBoundingSphereRadiusWorldSpace(float&);
 virtual bool IsInsideFrustum(const CEngineCamera&,bool);
 virtual bool GetObjectTransform(CMatrix3x4&)const;
 virtual bool SetObjectTransform(const CMatrix3x4&);
 virtual void SetAnimation(const CEnginePrimitiveAnimation&);
 virtual void SetPosition(const CEnginePrimitivePosition&);
 virtual void SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&);
 virtual bool GetMeshEffect(CIVCountedPointer<CEngineMeshEffect>&)const;
 virtual CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive();
 virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
 void SetNonPersistent(bool non_persistent){NonPersistent=non_persistent;}
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
 // Overlay opt-in (chunk 2011, row 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects): the static
 // CurrentInterpolationInfo (Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CEngineInternalPrimitiveBase::CurrentInterpolationInfo''
 // 0x01224900; the object at 0x0143B8E0) and FableWin.pdb ?GetCurrentInterpolate@CEngineInternalPrimitiveBase@@QBEMXZ (inline: the
 // row reads `[InterpolationSet*12 + 0x0143B934]` in place, 0x00BF1942..0x00BF1948). No layout change.
#ifdef FABLE_RETAIL_DEBUG_BONE_PASS29
private:
#endif
 static CEngineInterpolationInfo CurrentInterpolationInfo;                    // 0x0143B8E0
#ifdef FABLE_RETAIL_DEBUG_BONE_PASS29
public:
#endif
 float GetCurrentInterpolate()const{return CurrentInterpolationInfo.InterpolationSetList[InterpolationSet].WFInterpolate;}
#endif
 void SetEnable(bool enable){Enable=enable;}
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT
 // Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): FableWin.pdb
 // ?IsPersistent@CEngineInternalPrimitiveBase@@QBE_NXZ (no Ego_r.pdb or Fable.exe copy; 0x00B9A528 `test byte ptr [ebx+34h],2` = NonPersistent).
 bool IsPersistent()const{return !NonPersistent;}
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Original Ego_r/FableWin TPI and module916 call sites; private evidence in00ba115c.
 unsigned char GetAlphaIncludingDistanceFade(float);
#endif

#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE
 // Retail opt-in (agentC40, row 0x00BBDDD0 CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderPrimitive): FableWin.pdb
 // ?GetAlphaIncludingDistanceFade@CEngineInternalPrimitiveBase@@QAEEM@Z with the body of the landed out-of-line copy 0x00BA7B30 (same text),
 // expanded by the row (0x00BBDF95..0x00BBE03B). GFFastFloatToLong is the shared retail-asm helper (engine/GFFastFloatToLong.h).
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE_FORCEINLINE
 // The body holds the retail-asm float-to-int helper, and VC7.1 does not expand a plain inline function that contains `__asm`
 // (the row then calls 0x00BA7B30: 535 bytes); retail expands it (0x00BBE01C `fld` / 0x00BBE024 `fistp [edx]` in the row).
 FABLE_FORCEINLINE
#endif
 unsigned char GetAlphaIncludingDistanceFade(float distance)
 {
  if (Alpha == 0)
   return 0;
  CEngineFadeDistance fade = OverrideFadeDistance ? GetFadeDistance() : FadeDistance;
  if (distance > fade.MaxDrawDistance)
   return 0;
  if (distance < fade.FadeBeginDistance)
   return Alpha;
  {
   int fadeAmount;
   fadeAmount = GFFastFloatToLong((distance - fade.FadeBeginDistance) * 255.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance));
   return (unsigned char)(((255 - fadeAmount) * Alpha) >> 8);
  }
 }
#endif
 unsigned long EnginePrimitiveFlags;
 float AlphaMipBias;
 CEngineFadeDistance FadeDistance;
 unsigned long OcclusionLayerBitMask;
 CEngineSceneGridCell* GridCell;
 CEngineSceneGridPos GridPos;
 unsigned char Alpha,AlphaRef,PreloadStat;
 EEngineInterpolationSet InterpolationSet;
protected:
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 virtual bool WasRendered()const;
 unsigned long RenderLayerMask;
private:
 unsigned char Enable:1;
 unsigned char NonPersistent:1;
 unsigned char OverrideFadeDistance:1;
 unsigned char PostRenderEnable:1;
 unsigned short LastFrameRendered;
 CEngineInternalPrimitiveBase* NextPrimitive;
 CEngineInternalPrimitiveBase** RefPrimitive;
 CEngineInternalPrimitiveBase* NextLayerMask;
 CEngineInternalPrimitiveBase** RefLayerMask;
};
FABLE_STATIC_ASSERT_AT(92, sizeof(CEngineInternalPrimitiveBase)==72);
#elif defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS)
// Retail opt-in (chunk 892, row 0x00BE1C10 CEngineInternalPrimitive2DViewport::Update): the polymorphic class, so a derived internal
// primitive can declare its own virtuals over it. RTTI .?AVCEngineInternalPrimitiveBase@@ : CEnginePersistentPrimitiveBase :
// CIVCountedPointeeBase (ghidra_out/rtti_bases.tsv); the own members in Ego_r.pdb order, names and offsets (the flattened model
// below, typed: CEngineFadeDistance / CEngineSceneGridPos as their PDB members, the bitfield group as its uchar:1 word).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#endif
#include "rebuild_abi.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
class CEngineSceneGridCell;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
#include "engine/EEngineInterpolationSet.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CEngineSceneGridPos.h"
#else
enum EEngineInterpolationSet;
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO
class CEngineInterpolationInfo;
#endif
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    #ifdef FABLE_RETAIL_PARTICLE_GROUP_DESTRUCTOR_PASS29
    virtual ~CEngineInternalPrimitiveBase() { }
#else
    virtual ~CEngineInternalPrimitiveBase();
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
    virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const;
    void SetGridCell(CEngineSceneGridCell* cell){GridCell=cell;}
    void AddToList(CEngineInternalPrimitiveBase** list);
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETGRIDPOS
    // Nested opt-in (hand3c, row 0x00B93B30 CEngineSceneMap::AddPrimitive): FableWin.pdb
    // ?SetGridPos@CEngineInternalPrimitiveBase@@QAEXABVCEngineSceneGridPos@@@Z (0x02E1CDC0: copies the three bytes to +0x24), inline in
    // retail (0x00B93C25..0x00B93C35 `lea edx,[edi+24h] ; mov word ptr [edx],ax ; mov byte ptr [edx+2],al`). No layout change.
    void SetGridPos(const CEngineSceneGridPos& pos){GridPos=pos;}
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO
    // Nested opt-in (chunk 899, row 0x01224900): the static data member Ego_r.pdb / FableWin.pdb name through
    // `dynamic initializer for 'CEngineInternalPrimitiveBase::CurrentInterpolationInfo'' (the 112-byte CEngineInterpolationInfo
    // @ 0x0143B8E0 that CEnginePrimitiveRenderer::FrameStart 0x00B32660 refreshes each frame). Access is not recorded (public).
    static CEngineInterpolationInfo CurrentInterpolationInfo;   // 0x0143B8E0
#endif
    unsigned long                  EnginePrimitiveFlags;           // +0x0c
    float                          AlphaMipBias;                   // +0x10
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
    CEngineFadeDistance FadeDistance;
#else
    float                          FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                          FadeDistance_MaxDrawDistance;   // +0x18 CEngineFadeDistance::MaxDrawDistance
#endif
    unsigned long                  OcclusionLayerBitMask;          // +0x1c
    CEngineSceneGridCell*          GridCell;                       // +0x20
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
    CEngineSceneGridPos GridPos;
#else
    signed char                    GridPos_Level;                  // +0x24 CEngineSceneGridPos::Level
    unsigned char                  GridPos_X;                      // +0x25 CEngineSceneGridPos::X
    unsigned char                  GridPos_Y;                      // +0x26 CEngineSceneGridPos::Y
#endif
    unsigned char                  Alpha;                          // +0x27
    unsigned char                  AlphaRef;                       // +0x28
    unsigned char                  PreloadStat;                    // +0x29
    EEngineInterpolationSet        InterpolationSet;               // +0x2c
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
protected:
#endif
    unsigned long                  RenderLayerMask;                // +0x30
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
private:
    unsigned char Enable:1,NonPersistent:1,OverrideFadeDistance:1;
#endif
    unsigned char                  PostRenderEnable : 1;           // +0x34
    unsigned short                 LastFrameRendered;              // +0x36
    CEngineInternalPrimitiveBase*  NextPrimitive;                  // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;                   // +0x3c
    CEngineInternalPrimitiveBase*  NextLayerMask;                  // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;                   // +0x44
};
FABLE_STATIC_ASSERT_AT(167, sizeof(CEngineInternalPrimitiveBase) == 0x48);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineInternalPrimitiveBase = 0x48.
// Retail evidence: 1 trusted landed function(s); reconciliation 2 conflict(s) in 1 file(s) vs 0 agreeing; PINNED by 3 retyped file(s) despite conflicts.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CEngineSceneGridCell;

#pragma pack(push, 1)
class CEngineInternalPrimitiveBase {
public:
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTRENDERFRAME
    // Opt-in (NF240, 2026-11-22, rows 0x00B32660 CEnginePrimitiveRenderer::FrameStart / 0x00B92C20 CEngineSceneGrid::RenderCellAll):
    // the frame counter @ 0x0143B8D8 is ?CurrentRenderFrame@CEngineInternalPrimitiveBase@@0GA (a private static ushort; Ego_r.pdb,
    // FableWin.pdb, Ego_d.pdb). Its two public accessors, inline in retail: ?SetAsRendered@CEngineInternalPrimitiveBase@@QAEXXZ
    // (FableWin.pdb / Ego_d.pdb; the store to +0x36 the scene renderers expand, as in the PASS27 block above) and
    // ?IncrementFrameCounter@CEngineInternalPrimitiveBase@@SIXXZ (FableWin.pdb / Ego_d.pdb; retail FrameStart 0x00B3271B
    // `inc word ptr [0x0143B8D8]`). Static members only: no layout change.
    void SetAsRendered() { LastFrameRendered = CurrentRenderFrame; }
    static void FABLE_FASTCALL IncrementFrameCounter() { ++CurrentRenderFrame; }
private:
    static unsigned short CurrentRenderFrame;                // 0x0143B8D8
public:
#endif
    void*                         __vftable;                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                 _pad_0x04[0x4];            // +0x04
    unsigned char                 _pad_0x08[0x4];            // +0x08
    unsigned long                 EnginePrimitiveFlags;      // +0x0c
    float                         AlphaMipBias;              // +0x10
    float                         FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                         FadeDistance_MaxDrawDistance; // +0x18 CEngineFadeDistance::MaxDrawDistance
    unsigned long                 OcclusionLayerBitMask;     // +0x1c
    CEngineSceneGridCell*         GridCell;                  // +0x20
    signed char                   GridPos_Level;             // +0x24 CEngineSceneGridPos::Level
    unsigned char                 GridPos_X;                 // +0x25 CEngineSceneGridPos::X
    unsigned char                 GridPos_Y;                 // +0x26 CEngineSceneGridPos::Y
    unsigned char                 Alpha;                     // +0x27
    unsigned char                 AlphaRef;                  // +0x28
    unsigned char                 PreloadStat;               // +0x29
    unsigned char                 _pad_0x2a[0x2];            // +0x2a
    unsigned char                 InterpolationSet[0x4];     // +0x2c EEngineInterpolationSet
    unsigned long                 RenderLayerMask;           // +0x30
    unsigned char                 PostRenderEnable[0x2];     // +0x34 bitfield group: uchar:1
    unsigned short                LastFrameRendered;         // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;             // +0x38
    void*                         RefPrimitive;              // +0x3c CEngineInternalPrimitiveBase * *
    CEngineInternalPrimitiveBase* NextLayerMask;             // +0x40
    void*                         RefLayerMask;              // +0x44 CEngineInternalPrimitiveBase * *
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(209, sizeof(CEngineInternalPrimitiveBase) == 0x48);
FABLE_STATIC_ASSERT_AT(210, offsetof(CEngineInternalPrimitiveBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(211, offsetof(CEngineInternalPrimitiveBase, EnginePrimitiveFlags) == 0xc);
FABLE_STATIC_ASSERT_AT(212, offsetof(CEngineInternalPrimitiveBase, AlphaMipBias) == 0x10);
FABLE_STATIC_ASSERT_AT(213, offsetof(CEngineInternalPrimitiveBase, FadeDistance_FadeBeginDistance) == 0x14);
FABLE_STATIC_ASSERT_AT(214, offsetof(CEngineInternalPrimitiveBase, FadeDistance_MaxDrawDistance) == 0x18);
FABLE_STATIC_ASSERT_AT(215, offsetof(CEngineInternalPrimitiveBase, OcclusionLayerBitMask) == 0x1c);
FABLE_STATIC_ASSERT_AT(216, offsetof(CEngineInternalPrimitiveBase, GridCell) == 0x20);
FABLE_STATIC_ASSERT_AT(217, offsetof(CEngineInternalPrimitiveBase, GridPos_Level) == 0x24);
FABLE_STATIC_ASSERT_AT(218, offsetof(CEngineInternalPrimitiveBase, GridPos_X) == 0x25);
FABLE_STATIC_ASSERT_AT(219, offsetof(CEngineInternalPrimitiveBase, GridPos_Y) == 0x26);
FABLE_STATIC_ASSERT_AT(220, offsetof(CEngineInternalPrimitiveBase, Alpha) == 0x27);
FABLE_STATIC_ASSERT_AT(221, offsetof(CEngineInternalPrimitiveBase, AlphaRef) == 0x28);
FABLE_STATIC_ASSERT_AT(222, offsetof(CEngineInternalPrimitiveBase, PreloadStat) == 0x29);
FABLE_STATIC_ASSERT_AT(223, offsetof(CEngineInternalPrimitiveBase, InterpolationSet) == 0x2c);
FABLE_STATIC_ASSERT_AT(224, offsetof(CEngineInternalPrimitiveBase, RenderLayerMask) == 0x30);
FABLE_STATIC_ASSERT_AT(225, offsetof(CEngineInternalPrimitiveBase, PostRenderEnable) == 0x34);
FABLE_STATIC_ASSERT_AT(226, offsetof(CEngineInternalPrimitiveBase, LastFrameRendered) == 0x36);
FABLE_STATIC_ASSERT_AT(227, offsetof(CEngineInternalPrimitiveBase, NextPrimitive) == 0x38);
FABLE_STATIC_ASSERT_AT(228, offsetof(CEngineInternalPrimitiveBase, RefPrimitive) == 0x3c);
FABLE_STATIC_ASSERT_AT(229, offsetof(CEngineInternalPrimitiveBase, NextLayerMask) == 0x40);
FABLE_STATIC_ASSERT_AT(230, offsetof(CEngineInternalPrimitiveBase, RefLayerMask) == 0x44);

#endif // FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
